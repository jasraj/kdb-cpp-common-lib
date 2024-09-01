// kdb-cpp-common - Time Manipulation Functions
// Copyright (c) 2024 Jaskirat Rajasansir

#include "kx-c-lib/c/c/k.h"

#include <time.h>

namespace kdbtime {

// Offset between Linux epoch (1970.01.01 00:00) and kdb+ epoch (2000.01.01 00:00)
const long TIMESTAMP_OFFSET = 946684800000000000;

const long NANOSECONDS_IN_1_SEC = 1000000000;


K timespecToTimestamp(timespec tTime) {
    long timespecNs = (tTime.tv_sec * kdbtime::NANOSECONDS_IN_1_SEC) + tTime.tv_nsec;
    long kdbNs = timespecNs - kdbtime::TIMESTAMP_OFFSET;

    return ktj(-KP, kdbNs);
}

}   // namespace kdbtime
