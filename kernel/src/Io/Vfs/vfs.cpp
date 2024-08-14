//
// Created by Piotr on 13.08.2024.
//

#include <cstring>
#include "vfs.hpp"

VFS::Status VFS::MountFilesystem(const char *path, std::shared_ptr <Filesystem> filesystem)
{
    if (!IsValidMountPath(path))
        return Status::FAILURE;

    filesystems[path] = std::move(filesystem);

    return VFS::Status::SUCCESS;
}

bool VFS::IsValidMountPath(const char *path)
{
    if (path == nullptr || strlen(path) == 0)
    {
        return false;
    }

    if (!(isalpha(path[0]) && path[1] == ':' && path[2] == '\\'))
    {
        return false;
    }

    for (const auto& [mountPath, filesystem] : filesystems)
    {
        if (strncmp(mountPath, path, strlen(mountPath)) == 0)
        {
            return false; // Path overlaps with an existing mount
        }
    }

    return true;
}

std::pair<Filesystem::Status, Filesystem::FileUID> VFS::Open(const char* path, Filesystem::OpenMode mode, Filesystem::UserUID as_user)
{
    // Validate input path
    if (path == nullptr || strlen(path) == 0)
    {
        return {Filesystem::Status::FAILURE, 0};
    }

    for (const auto& [mount_path, filesystem] : filesystems)
    {
        if (strncmp(path, mount_path, strlen(mount_path)) == 0)
        {
            std::string relative_path = path + strlen(mount_path);

            auto [fs_status, file_uid] = filesystem->Open(relative_path.c_str(), mode, as_user);

            if (fs_status == Filesystem::Status::SUCCESS)
            {
                auto fid = AllocateFID();

                fid_translation_map[fid] = std::make_pair(filesystem, fid);

                return { Filesystem::Status::SUCCESS, fid };
            }
            else
            {
                return {Filesystem::Status::FAILURE, 0 };
            }

        }
    }

    // Path not found in any mounted filesystems
    return { Filesystem::Status::FAILURE, 0 };
}


uint64_t VFS::AllocateFID()
{
    return last_fid++;
}