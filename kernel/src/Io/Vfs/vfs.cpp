//
// Created by Piotr on 13.08.2024.
//

#include <cstring>
#include "vfs.hpp"

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