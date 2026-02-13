// kdb-cpp-common - Time Manipulation Functions
// Copyright (c) 2024 Jaskirat Rajasansir

#include "kx-c-lib/c/c/k.h"

#include <time.h>
#include "include/Time.hpp"

namespace kdbtime {


K timespecToTimestamp(timespec tTime) {
    long timespecNs = (tTime.tv_sec * kdbtime::NANOSECONDS_IN_1_SEC) + tTime.tv_nsec;
    long kdbNs = timespecNs - kdbtime::TIMESTAMP_OFFSET;

    return ktj(-KP, kdbNs);
}

}   // namespace kdbtime
