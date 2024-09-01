// kdb-cpp-common - Process Introspection
// Copyright (c) 2024 Jaskirat Rajasansir

#include "kx-c-lib/c/c/k.h"

#include <unistd.h>

namespace kdbprocess {

}   // namespace kdbprocess


// https://man7.org/linux/man-pages/man2/gettid.2.html
//  @returns (Integer) The current thread ID. This will equal the PID if executed on the main thread
extern "C" K _cpp_process_getThreadId(K nullArg) {
    return ki(gettid());
}

// https://man7.org/linux/man-pages/man3/getppid.3p.html
//  @returns (Integer) The parent PID that forked this process or 1 if the parent no longer exists
extern "C" K _cpp_process_getParentPid(K nullArg) {
    return ki(getppid());
}
