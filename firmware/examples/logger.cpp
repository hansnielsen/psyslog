#define SYSLOG_DEBUG
//#define SYSLOG_USE_DEVICEID
#include "psyslog/psyslog.h"

void setup() {
    waitUntil(WiFi.ready);

    syslog_initialize("192.168.1.1", 514);

    LOGC("critical!");
    LOGE("error!");
    LOGW("warn!");
    LOGI("info!");
    LOGD("debug!");
}

void loop() {
    LOGI("loop!");

    delay(10000);
}
