// kdb Integration with systemd Nofication Mechanism
// Copyright (c) 2018 Jaskirat Rajasansir

#include "kx-c-lib/c/c/k.h"

#include <string>
#include <iostream>

#include <unistd.h>
#include "errno.h"

#include "include/Common.hpp"

#include <systemd/sd-daemon.h>


// Definitions of strings that are sent to systemd are defined at
//      https://www.freedesktop.org/software/systemd/man/sd_notify.html


namespace kdbsystemd {

// systemd string to signal process is ready
const std::string isReady = std::string("READY=1");

// systemd string to signal process is stopping
const std::string isStopping = std::string("STOPPING=1");

// systemd string to signal process is heartbeating
const std::string isWatchdog = std::string("WATCHDOG=1");

// systemd string prefix to publish information to systemd
const std::string isStatusPrefix = std::string("STATUS=");

// Multiplier to convert between milliseconds and nanoseconds
const uint64_t msToNano = 1000000;


// Notifies that the current process is ready
//  @see sd_pid_notify
//  @see getpid
//  @see isReady
void notifyReady() {
    sd_pid_notify(getpid(), 0, isReady.c_str());
}

// Notifies that the current process is stopping
void notifyStopping() {
    sd_pid_notify(getpid(), 0, isStopping.c_str());
}

// Queries the interval that systemd is expecting heartbeat messages (watchdog's) to be sent from the process
//  @return The interval in milliseconds
//  @see sd_watchdog_enabled
int notifyGetIntervalMs() {
    uint64_t sdInterval;

    int sdRes = sd_watchdog_enabled(0, &sdInterval);

    if(sdRes == 0)
        return 0;

    return sdInterval / 1000;
}

// Sends a heartbeat to systemd
void notifyWatchdog() {
    sd_pid_notify(getpid(), 0, isWatchdog.c_str());
}

// Sends a generic status string to systemd as an update of its internal state
//  @param status A status
//  @see isStatusPrefix
//  @see sd_pid_notify
//  @see getpid
void notifyStatus(std::string status) {
    std::string statusPrefix = std::string(isStatusPrefix);
    sd_pid_notify(getpid(), 0, statusPrefix.append(status).c_str());
}

}   // namespace kdbsystemd


extern "C" K _cpp_systemd_sendReady(K nullArg) {
    kdbsystemd::notifyReady();
    return kb(1);
}

extern "C" K _cpp_systemd_sendStopping(K nullArg) {
    kdbsystemd::notifyStopping();
    return kb(1);
}

extern "C" K _cpp_systemd_getInterval(K nullArg) {
    int interval = kdbsystemd::notifyGetIntervalMs();
    return ktj(-KN, interval * kdbsystemd::msToNano);
}

extern "C" K _cpp_systemd_sendWatchdog(K nullArg) {
    if(kdbsystemd::notifyGetIntervalMs() == 0)
        return kdbcommon::error(EINVAL, "systemd watchdog is not enabled. No heartbeating required");

    kdbsystemd::notifyWatchdog();
    return kb(1);
}

extern "C" K _cpp_systemd_sendStatus(K status) {
    std::string statusStr;

    if(status->t == -KS)
        statusStr = std::string(status->s);
    else if(status->t == KC)
        statusStr = std::string((const char*) kC(status), status->n);
    else
        return kdbcommon::error(EINVAL, "Incorrect type for status. Must be symbol or string");

    if(statusStr.empty())
        return kdbcommon::error(EINVAL, "No status specified");

    kdbsystemd::notifyStatus(statusStr);
    return kb(1);
}
