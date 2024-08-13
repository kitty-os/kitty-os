#pragma once

#include <Io/Fs/fops.hpp>
#include <unordered_map>

class VFS
{
public:
    KSTATUS Open(

            );
private:
    std::unordered_map<FileSystem, const wchar_t*> mount_points;
};