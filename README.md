# Shared Library for Common Functionality in kdb+

This repository provides a shared library that provides access to some common functionality that can be useful when running kdb-based systems on Linux.

*NOTE*: This repository incorporates (and supercedes) [kdb-base64-lib](https://github.com/jasraj/kdb-base64-lib).

## Summary

| Source File | Function Prefix | Description |
| ----------- | --------------- | ----------- |
| `Base64.cpp` | `_cpp_b64`     | High performance Base64 encoding and decoding |
| `File.cpp`   | `_cpp_filesystem` | Detailed file and filesystem queries |
| `Process.cpp` | `_cpp_process` | Process and thread information |
| `System.cpp` | `_cpp_system` | System-level information |

## Compiling

To compile, use `cmake`:

```
# Create build in a local 'build' folder within the repo
mkdir build
cd build
cmake ..
cmake --build .
```
