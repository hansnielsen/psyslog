#include <application.h>

#ifndef SYSLOG_SOURCE_PORT
#  define SYSLOG_SOURCE_PORT 60381 // randomly chosen
#endif
#define SYSLOG_MESSAGE_LEN 480

#ifndef SYSLOG_FACILITY
#  define SYSLOG_FACILITY 1 // user facility
#endif

// by default, use the MAC address as the hostname
#ifndef SYSLOG_USE_DEVICEID
#  define SYSLOG_USE_MAC_ADDR
#endif

void syslog_initialize(String host, uint16_t port);
void syslog_debugf(int level, int line, const char *func, const char *file, const char *msg, ...);
void syslog_printf(int level, const char *tag, const char *msg, ...);

enum {
    LEVEL_EMERGENCY = 0,
    LEVEL_ALERT,
    LEVEL_CRITICAL,
    LEVEL_ERROR,
    LEVEL_WARNING,
    LEVEL_NOTICE,
    LEVEL_INFO,
    LEVEL_DEBUG,
};

#if defined(DEBUG_BUILD) || defined(SYSLOG_DEBUG)
#define LOGA(fmt, ...) do{syslog_debugf(LEVEL_ALERT,__LINE__, __PRETTY_FUNCTION__,__FILE__,fmt,##__VA_ARGS__);}while(0)
#define LOGC(fmt, ...) do{syslog_debugf(LEVEL_CRITICAL,__LINE__, __PRETTY_FUNCTION__,__FILE__,fmt,##__VA_ARGS__);}while(0)
#define LOGE(fmt, ...) do{syslog_debugf(LEVEL_ERROR,__LINE__, __PRETTY_FUNCTION__,__FILE__,fmt,##__VA_ARGS__);}while(0)
#define LOGW(fmt, ...) do{syslog_debugf(LEVEL_WARNING,__LINE__, __PRETTY_FUNCTION__,__FILE__,fmt,##__VA_ARGS__);}while(0)
#define LOGN(fmt, ...) do{syslog_debugf(LEVEL_NOTICE,__LINE__, __PRETTY_FUNCTION__,__FILE__,fmt,##__VA_ARGS__);}while(0)
#define LOGI(fmt, ...) do{syslog_debugf(LEVEL_INFO,__LINE__, __PRETTY_FUNCTION__,__FILE__,fmt,##__VA_ARGS__);}while(0)
#define LOGD(fmt, ...) do{syslog_debugf(LEVEL_DEBUG,__LINE__, __PRETTY_FUNCTION__,__FILE__,fmt,##__VA_ARGS__);}while(0)
#else
#define LOGA(fmt, ...) 
#define LOGC(fmt, ...) 
#define LOGE(fmt, ...) 
#define LOGW(fmt, ...) 
#define LOGN(fmt, ...) 
#define LOGI(fmt, ...) 
#define LOGD(fmt, ...) 
#endif
