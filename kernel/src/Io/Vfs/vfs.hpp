#pragma once

#include <unordered_map>
#include <Io/Fs/fs.hpp>
#include <memory>

class VFS
{
public:

private:
    bool IsValidMountPath(const char* path);

    // An unordered map of filesystems.
    // Example:
    // NTFS -> C:\
    // FAT32 -> D:\
    // exFAT -> X:\
    // Or, if we run out of partition characters we can use paths.
    // EXT4 -> C:\partitions\nvme0
    std::unordered_map<const char*, std::unique_ptr<Filesystem>> filesystems;
};