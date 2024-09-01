// kdb-cpp-common - File Utilities
// Copyright (c) 2024 Jaskirat Rajasansir

#include "kx-c-lib/c/c/k.h"

#include <string>
#include "errno.h"

#include <sys/stat.h>
#include <sys/statvfs.h>
#include <sys/sysinfo.h>
#include <sys/sysmacros.h>
#include <sys/utsname.h>

#include "include/Common.hpp"
#include "include/Time.hpp"

namespace kdbfile {

// https://man7.org/linux/man-pages/man2/stat.2.html
K getFileInfo(std::string file) {
    file = kdbcommon::ensureCppPath(file);

    struct stat fileInfo;

    int result = stat(file.c_str(), &fileInfo);

    if(result != 0)
        return kdbcommon::error(errno, "Failed to get file info");

    K fileKeys = ktn(KS, 13);
    kS(fileKeys)[0] = ss((char*) "file.path");
    kS(fileKeys)[1] = ss((char*) "file.device.major");
    kS(fileKeys)[2] = ss((char*) "file.device.minor");
    kS(fileKeys)[3] = ss((char*) "file.inode");
    kS(fileKeys)[4] = ss((char*) "file.mode");
    kS(fileKeys)[5] = ss((char*) "file.hardlinks");
    kS(fileKeys)[6] = ss((char*) "file.uid");
    kS(fileKeys)[7] = ss((char*) "file.gid");
    kS(fileKeys)[8] = ss((char*) "file.totalSizeBytes");
    kS(fileKeys)[9] = ss((char*) "file.totalBlocks");
    kS(fileKeys)[10] = ss((char*) "file.lastAccessTime");
    kS(fileKeys)[11] = ss((char*) "file.lastModifyTime");
    kS(fileKeys)[12] = ss((char*) "file.lastStatusChangeTime");

    K lastAccess = kdbtime::timespecToTimestamp(fileInfo.st_atim);
    K lastModify = kdbtime::timespecToTimestamp(fileInfo.st_mtim);
    K lastStatus = kdbtime::timespecToTimestamp(fileInfo.st_ctim);

    K fileVals = knk(13,
                        ks((char*) file.c_str()),
                        ki(major(fileInfo.st_dev)),
                        ki(minor(fileInfo.st_dev)),
                        kj(fileInfo.st_ino),
                        ki(fileInfo.st_mode),
                        ki(fileInfo.st_nlink),
                        ki(fileInfo.st_uid),
                        ki(fileInfo.st_gid),
                        kj(fileInfo.st_size),
                        kj(fileInfo.st_blocks),
                        lastAccess,
                        lastModify,
                        lastStatus);

    return xD(fileKeys, fileVals);
}

// https://man7.org/linux/man-pages/man3/statvfs.3.html
K getFileSystemInfo(std::string filesystemStr) {
    filesystemStr = kdbcommon::ensureCppPath(filesystemStr);

    struct statvfs filesystemInfo;

    int result = statvfs(filesystemStr.c_str(), &filesystemInfo);

    if(result != 0)
        return kdbcommon::error(errno, "Failed to get filesystem info");

    K filesystemKeys = ktn(KS, 5);
    kS(filesystemKeys)[0] = ss((char*) "filesystem.path");
    kS(filesystemKeys)[1] = ss((char*) "filesystem.blockSizeBytes");
    kS(filesystemKeys)[2] = ss((char*) "filesystem.totalBytes");
    kS(filesystemKeys)[3] = ss((char*) "filesystem.freeBytes");
    kS(filesystemKeys)[4] = ss((char*) "filesystem.availableBytes");

    K filesystemVals = knk(5,
                            ks((char*) filesystemStr.c_str()),
                            kj(filesystemInfo.f_bsize),
                            kj(filesystemInfo.f_blocks * filesystemInfo.f_bsize),
                            kj(filesystemInfo.f_bfree * filesystemInfo.f_bsize),
                            kj(filesystemInfo.f_bavail * filesystemInfo.f_bsize));

    return xD(filesystemKeys, filesystemVals);
}

}   // namespace kdbfile


extern "C" K _cpp_filesystem_getFileInfo(K file) {
    if(file->t != -KS)
        return kdbcommon::error(EINVAL, "Type for file path must be symbol");

    std::string fileStr = std::string(file->s);

    if(fileStr.empty())
        return kdbcommon::error(EINVAL, "No file path specified");

    return kdbfile::getFileInfo(fileStr);
}

extern "C" K _cpp_filesystem_info(K filesystem) {
    if(filesystem->t != -KS)
        return kdbcommon::error(EINVAL, "Type for filesystem info must be symbol");

    std::string filesystemStr = std::string(filesystem->s);

    if(filesystemStr.empty())
        return kdbcommon::error(EINVAL, "No filesystem path specified");

    return kdbfile::getFileSystemInfo(filesystemStr);
}
