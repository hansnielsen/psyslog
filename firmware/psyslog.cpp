#include "psyslog.h"

namespace {
    UDP udp;
    String syslog_host;
    IPAddress syslog_remote;
    uint16_t syslog_port;
    bool syslog_use_tcp;
    TCPClient tcp;

    int start_syslog_msg(char *buf, int len, int level, const char *tag) {
#ifdef SYSLOG_USE_DEVICEID
        String tmp = System.deviceID();
        unsigned char hostname[tmp.length()];
        tmp.getBytes(hostname, sizeof(hostname));
#else
        byte mac[6];
        WiFi.macAddress(mac);
        char hostname[13];
        snprintf(hostname, sizeof(hostname), "%02x%02x%02x%02x%02x%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
#endif

        return snprintf(buf, len, "<%u>%s %s: ", (8*SYSLOG_FACILITY) + level, hostname, tag);
    }
}

bool syslog_check_ready() {
    if (syslog_port == (uint32_t)0) {
        ERROR("syslog not configured yet");
        return false;
    }
    if (syslog_remote == (uint32_t)0) {
        syslog_remote = WiFi.resolve(syslog_host);
        if (syslog_remote == (uint32_t)0) {
            ERROR("name resolution error");
            return false;
        }
    }
    return true;
}

void syslog_transmit(const char* buf, int written) {
    if (syslog_use_tcp) {
        if (!tcp.connected()) tcp.connect(syslog_remote, syslog_port);
        tcp.print(String(written) + " " + buf);
    } else {
        udp.sendPacket(buf, written, syslog_remote, syslog_port);
    }
}

void syslog_printf(int level, const char *tag, const char *msg, ...) {
    if (!syslog_check_ready()) return;

    unsigned int len = SYSLOG_MESSAGE_LEN;
    char buf[len];

    int written = start_syslog_msg(buf, len - written, level, tag);

    va_list args;
    va_start(args, msg);
    written += vsnprintf(buf + written, len - written, msg, args);

    syslog_transmit(buf, written);
}

void syslog_debugf(int level, int line, const char *func, const char *file, const char *msg, ...) {
    if (!syslog_check_ready()) return;

    unsigned int len = SYSLOG_MESSAGE_LEN;
    char buf[len];

    file = file ? strrchr(file, '/') + 1 : "particle";
    int written = start_syslog_msg(buf, len, level, file);

    written += snprintf(buf + written, len - written, "%d [%010u] %s -- ", line, (unsigned)HAL_Timer_Get_Milli_Seconds(), func);

    va_list args;
    va_start(args, msg);
    written += vsnprintf(buf + written, len - written, msg, args);

    syslog_transmit(buf, written);
}

void syslog_initialize(String host, uint16_t port=514, bool use_tcp) {

    syslog_host = host;
    syslog_remote = WiFi.resolve(host);
    syslog_port = port;
    syslog_use_tcp = use_tcp;

    if (syslog_remote == (uint32_t)0) {
        ERROR("provided IP is not valid");
    }
    if (syslog_port == 0) {
        syslog_port = 514;
    }

    if (syslog_use_tcp)
        tcp.connect(host, syslog_port);
    else
        udp.begin(SYSLOG_SOURCE_PORT);

    syslog_printf(LEVEL_INFO, "psyslog", "Particle syslog starting");
    INFO("psyslog starting");
}
