# psyslog

## About

This library provides a UDP syslog client for logging data from Particle apps.

It has been tested on the Photon in combination with rsyslog. Other devices and syslog daemons may work! Feel free to contact me via email (hans@stackallocated.com) if you have issues with untested devices.

## Usage

Add the following to the top of your application:
```c
#define SYSLOG_DEBUG
#include "psyslog/psyslog.h"
```

Then, set up the remote log endpoint:
```c
syslog_initialize("logs.example.com", 514);
```

To actually log data, use the following macros:
```c
LOGE("This is an error message!");
LOGI("Informational messages are less important);
```

By default, the library will log using the device's MAC address as its hostname. If you want to use the Particle device ID, add `#define SYSLOG_USE_DEVICEID` before the include.

## syslog servers

An example [rsyslog](http://www.rsyslog.com/) [configuration file](doc/rsyslog.conf) is included. It will automatically separate logs by hostname and store them in `/var/log/particle/`.
