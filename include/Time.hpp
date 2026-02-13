// kdb-cpp-common - Time Manipulation Functions - Header
// Copyright (c) 2024 Jaskirat Rajasansir

namespace kdbtime {

// Offset between Linux epoch (1970.01.01 00:00) and kdb+ epoch (2000.01.01 00:00)
static const long TIMESTAMP_OFFSET = 946684800000000000;

static const long NANOSECONDS_IN_1_SEC = 1000000000;

static const long NANOSECONDS_IN_1_MSEC = 1000000;

K timespecToTimestamp(timespec tTime);


}   // namespace kdbtime
