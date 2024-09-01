// kdb-cpp-common - System Information
// Copyright (c) 2024 Jaskirat Rajasansir

#include "kx-c-lib/c/c/k.h"

#include "errno.h"

#include "include/Common.hpp"

#include <sys/sysinfo.h>
#include <sys/utsname.h>


namespace kdbsystem {

// https://man7.org/linux/man-pages/man2/sysinfo.2.html
K getSysinfo() {
    struct sysinfo sysInfo;

    int infoResult = sysinfo(&sysInfo);

    if(infoResult != 0)
        return kdbcommon::error(errno, "Failed to query system information");

    // Memory unit in bytes. Used to give memory values in bytes, not blocks
    int ramUnit = sysInfo.mem_unit;

    // Convert long load factors into doubles, so they match 'uptime' / 'htop' etc
    float loadFactor = 1.f / (1 << SI_LOAD_SHIFT);

    K infoKeys = ktn(KS, 11);
    kS(infoKeys)[0] = ss((char*) "system.uptimeSecs");
    kS(infoKeys)[1] = ss((char*) "system.totalProcesses");
    kS(infoKeys)[2] = ss((char*) "system.loadAvg1m");
    kS(infoKeys)[3] = ss((char*) "system.loadAvg5m");
    kS(infoKeys)[4] = ss((char*) "system.loadAvg15m");
    kS(infoKeys)[5] = ss((char*) "mem.totalBytes");
    kS(infoKeys)[6] = ss((char*) "mem.freeBytes");
    kS(infoKeys)[7] = ss((char*) "mem.sharedBytes");
    kS(infoKeys)[8] = ss((char*) "mem.bufferBytes");
    kS(infoKeys)[9] = ss((char*) "mem.virtualTotalBytes");
    kS(infoKeys)[10] = ss((char*) "mem.virtualFreeBytes");

    K infoValues = knk(11,
                        kj(sysInfo.uptime),
                        kj(sysInfo.procs),
                        kf(sysInfo.loads[0] * loadFactor),
                        kf(sysInfo.loads[1] * loadFactor),
                        kf(sysInfo.loads[2] * loadFactor),
                        kj(sysInfo.totalram * ramUnit),
                        kj(sysInfo.freeram * ramUnit),
                        kj(sysInfo.sharedram * ramUnit),
                        kj(sysInfo.bufferram * ramUnit),
                        kj(sysInfo.totalswap * ramUnit),
                        kj(sysInfo.freeswap * ramUnit));

    return xD(infoKeys, infoValues);
}

}   // namespace kdbsystem


extern "C" K _cpp_system_info(K nullArg) {
    return kdbsystem::getSysinfo();
}

// https://man7.org/linux/man-pages/man2/uname.2.html
extern "C" K _cpp_system_kernel(K nullArg) {
    struct utsname kernelInfo;

    int kInfoResult = uname(&kernelInfo);

    if(kInfoResult != 0)
        return kdbcommon::error(errno, "Failed to query kernel information");

    K kInfoKeys = ktn(KS, 5);
    kS(kInfoKeys)[0] = ss((char*) "kernel.name");
    kS(kInfoKeys)[1] = ss((char*) "kernel.nodeName");
    kS(kInfoKeys)[2] = ss((char*) "kernel.release");
    kS(kInfoKeys)[3] = ss((char*) "kernel.version");
    kS(kInfoKeys)[4] = ss((char*) "kernel.hardwareType");

    K kInfoValues = knk(5,
                        kp(kernelInfo.sysname),
                        kp(kernelInfo.nodename),
                        kp(kernelInfo.release),
                        kp(kernelInfo.version),
                        kp(kernelInfo.machine));

    return xD(kInfoKeys, kInfoValues);
}


