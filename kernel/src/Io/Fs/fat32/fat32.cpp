//
// Created by Piotr on 14.08.2024.
//

#include <Io/Fs/fat32/structures.hpp>
#include <cstring>
#include "fat32.hpp"

Fat32::Fat32(StorageInterface &si) : si(si)
{
}

bool Fat32::IsFilesystem(StorageInterface &si)
{
    uint8_t buffer[512];

    si.Read(&buffer, 0, 512);

    FAT32Header* header = (FAT32Header*)&buffer;

    if (header->bootSignature2 != 0xAA55)
    {
        return false;
    }

    if (std::memcmp(header->fileSystemType, "FAT32   ", 8) != 0)
    {
        return false;
    }

    if (header->FATSize16 != 0 || header->FATSize32 == 0)
    {
        return false;
    }

    return true;
}

std::pair <Filesystem::Status, Filesystem::FileUID> Fat32::Open(const char *path, Filesystem::OpenMode mode, Filesystem::UserUID as_user)
{
    return std::make_pair(SUCCESS, 69);
}

Filesystem::Status Fat32::Close(Filesystem::FileUID uid)
{
    return UNIMPLEMENTED;
}

Filesystem::Status Fat32::Read(Filesystem::FileUID uid, void *buf, size_t size)
{
    return UNIMPLEMENTED;
}

Filesystem::Status Fat32::Write(Filesystem::FileUID uid, void *buf, size_t size)
{
    return UNIMPLEMENTED;
}

Filesystem::Status Fat32::CreateFile(const char *path, Filesystem::UserUID as_user, Filesystem::FilePermission permissions)
{
    return UNIMPLEMENTED;
}

Filesystem::Status Fat32::CreateDirectory(const char *path, Filesystem::UserUID as_user, Filesystem::FilePermission permissions)
{
    return UNIMPLEMENTED;
}

Filesystem::Status Fat32::DeleteDirectory(const char *path, Filesystem::UserUID as_user)
{
    return UNIMPLEMENTED;
}

Filesystem::Status Fat32::GetFileInformation(const char *path, Filesystem::UserUID as_user)
{
    return UNIMPLEMENTED;
}

Filesystem::Status Fat32::ListDirectory(const char *path, Filesystem::UserUID as_user, std::vector <std::string> &entries)
{
    return UNIMPLEMENTED;
}

std::pair<Filesystem::Status, bool>
Fat32::HasPermission(const char *path, Filesystem::UserUID user, Filesystem::FilePermission permission)
{
    return std::make_pair(UNIMPLEMENTED, false);
}

Filesystem::Status Fat32::SetPermission(const char *path, Filesystem::UserUID user, Filesystem::FilePermission permission)
{
    return UNIMPLEMENTED;
}

Filesystem::Status Fat32::ClearPermission(const char *path, Filesystem::UserUID user, Filesystem::FilePermission permission)
{
    return UNIMPLEMENTED;
}

Filesystem::Status Fat32::DeleteFile(const char *path, Filesystem::UserUID as_user)
{
    return UNIMPLEMENTED;
}