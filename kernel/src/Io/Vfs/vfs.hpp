#pragma once

#include <unordered_map>
#include <Io/Fs/fs.hpp>
#include <memory>

class VFS
{
public:
    enum class Status
    {
        SUCCESS, FAILURE
    };

    std::pair<Filesystem::Status, Filesystem::FileUID> Open(const char* path, Filesystem::OpenMode mode, Filesystem::UserUID  as_user);

    Status MountFilesystem(const char* path, std::shared_ptr<Filesystem> filesystem);
private:
    uint64_t last_fid = 1; // last file id

    uint64_t AllocateFID();

    bool IsValidMountPath(const char* path);

    // An unordered map of filesystems.
    // Example:
    // NTFS -> C:\
    // FAT32 -> D:\
    // exFAT -> X:\
    // Or, if we run out of partition characters we can use paths.
    // EXT4 -> C:\partitions\nvme0
    std::unordered_map<const char*, std::shared_ptr<Filesystem>> filesystems;
    // Updated declaration of translation_table
    std::unordered_map<Filesystem::FileUID, std::pair<std::shared_ptr<Filesystem>, Filesystem::FileUID>> fid_translation_map;
};