# Shared Library for Common Functionality in kdb+

This repository provides a shared library that provides access to some common functionality that can be useful when running kdb-based systems on Linux.

*NOTE*: This repository incorporates (and supercedes) [kdb-base64-lib](https://github.com/jasraj/kdb-base64-lib) and [kdb-systemd-lib](https://github.com/jasraj/kdb-systemd-lib).

## Summary

| Source File | Function Prefix | Optional |  Description |
| ----------- | --------------- | -------- | ----------- |
| `Base64.cpp`  | `_cpp_b64`        | No  | High performance Base64 encoding and decoding |
| `File.cpp`    | `_cpp_filesystem` | No  | Detailed file and filesystem queries |
| `Process.cpp` | `_cpp_process`    | No  | Process and thread information |
| `System.cpp`  | `_cpp_system`     | No  |  System-level information |
| `Systemd.cpp` | `_cpp_systemd`    | Yes |  Systemd integration for process reporting |

## Compiling

To compile, use `cmake`:

```bash
# Create build in a local 'build' folder within the repo
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

### Optional Components

All the components marked as optional in the table above will be excluded (`OFF`) by default. The sections below describe how to enable them and the additional library requirements.

#### systemd

To include the systemd functionality within the shared library, ensure you have the development files for systemd installed:

* Ubuntu: `libsystemd-dev`
* CentOS: `systemd-devel`

The configuration flag to enable this is `BUILD_SYSTEMD_COMPONENT`. Example:

```bash
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_SYSTEMD_COMPONENT=ON ..
```

## Installing

The generated shared library will be in the `build` folder:

```bash
> ls -l *.so*
lrwxr-xr-x. 1 root root    22 Feb 12 15:30 libkdb-cpp-common.so -> libkdb-cpp-common.so.1
lrwxr-xr-x. 1 root root    26 Feb 12 15:30 libkdb-cpp-common.so.1 -> libkdb-cpp-common.so.1.0.0
-rwxr-xr-x. 1 root root 79928 Feb 12 15:30 libkdb-cpp-common.so.1.0.0
```

and one of the following options should be used for installation:

1. Manually move the shared library into any folder specified on `$LD_LIBRARY_PATH`
1. Install the library into `/usr/lib` with `cmake --install .` (will require `root` user)
1. Generate DEB and RPM files with `cpack .`

