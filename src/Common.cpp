// kdb-cpp-common - Common Functions
// Copyright (c) 2024 Jaskirat Rajasansir

#include "kx-c-lib/c/c/k.h"

#include <string>
#include <errno.h>

namespace kdbcommon {

const std::string errorPrefix = std::string("[kdb-cpp-common] ");

K error(int code, std::string description) {
    std::string errString = std::string(errorPrefix).append(description);

    if(code == 0)
        code = EINVAL;

    // Set 'errno' so 'orr' can report it as part of the error back to kdb+
    errno = code;

    return orr((char*) errString.c_str());
}

std::string ensureCppPath(std::string path) {
    if(path.at(0) != ':')
        return path;

    path.erase(0, 1);
    return path;
}

}   // namespace kdbcommon
