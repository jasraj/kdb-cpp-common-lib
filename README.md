# Shared Library for Common Functionality in kdb+

This repository provides a shared library that provides access to some common functionality that can be useful when running kdb-based systems on Linux.

*NOTE*: This repository incorporates (and supercedes) [kdb-base64-lib](https://github.com/jasraj/kdb-base64-lib) and [kdb-systemd-lib](https://github.com/jasraj/kdb-systemd-lib).

## Summary

| Source File | Function Prefix | Optional |  Description |
| ----------- | --------------- | -------- | ----------- |
| `Base64.cpp`  | `_cpp_b64`        | No | High performance Base64 encoding and decoding |
| `File.cpp`    | `_cpp_filesystem` | No | Detailed file and filesystem queries |
| `Process.cpp` | `_cpp_process`    | No | Process and thread information |
| `System.cpp`  | `_cpp_system`     | No |  System-level information |
| `Systemd.cpp` | `_cpp_systemd`    | Yes (`BUILD_SYSTEMD_COMPONENT`) |  Systemd integration for process reporting |

## Compiling

To compile, use `cmake`:

```bash
# Create build in a local 'build' folder within the repo
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

To include any optional components, specify `-D*option*=ON`. Example:

```bash
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SYSTEMD_COMPONENT=ON ..
```
