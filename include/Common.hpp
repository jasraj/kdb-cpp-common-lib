// kdb-cpp-common - Common Functions - Header
// Copyright (c) 2024 Jaskirat Rajasansir

#include <string>

namespace kdbcommon {

K error(int code, std::string description);

std::string ensureCppPath(std::string path);

}
