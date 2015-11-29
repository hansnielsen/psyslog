#include "psyslog.h"

namespace {
    UDP udp;
    IPAddress syslog_remote;
    uint16_t syslog_port;

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

void syslog_printf(int level, const char *tag, const char *msg, ...) {
    if (syslog_remote == (uint32_t)0) {
        ERROR("syslog not configured yet");
        return;
    }

    unsigned int len = SYSLOG_MESSAGE_LEN;
    char buf[len];

    int written = start_syslog_msg(buf, len - written, level, tag);

    va_list args;
    va_start(args, msg);
    written += vsnprintf(buf + written, len - written, msg, args);

    udp.sendPacket(buf, written, syslog_remote, syslog_port);
}

void syslog_debugf(int level, int line, const char *func, const char *file, const char *msg, ...) {
    if (syslog_remote == (uint32_t)0) {
        ERROR("syslog not configured yet");
        return;
    }

    unsigned int len = SYSLOG_MESSAGE_LEN;
    char buf[len];

    file = file ? strrchr(file, '/') + 1 : "particle";
    int written = start_syslog_msg(buf, len, level, file);

    written += snprintf(buf + written, len - written, "%d [%010u] %s -- ", line, (unsigned)HAL_Timer_Get_Milli_Seconds(), func);

    va_list args;
    va_start(args, msg);
    written += vsnprintf(buf + written, len - written, msg, args);

    udp.sendPacket(buf, written, syslog_remote, syslog_port);
}

void syslog_initialize(String host, uint16_t port=514) {
    syslog_remote = WiFi.resolve(host);
    syslog_port = port;

    if (syslog_remote == (uint32_t)0) {
        ERROR("provided IP is not valid");
    }
    if (syslog_port == 0) {
        syslog_port = 514;
    }

    udp.begin(SYSLOG_SOURCE_PORT);

    syslog_printf(LEVEL_INFO, "psyslog", "Particle syslog starting");
    INFO("psyslog starting");
}
