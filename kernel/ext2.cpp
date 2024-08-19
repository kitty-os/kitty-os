//
// Created by Piotr on 16.08.2024.
//

#include <string>
#include <vector>

#include "structures.hpp"
#include "ext2.hpp"

constexpr size_t SUPERBLOCK_OFFSET = 1024;
constexpr size_t SUPERBLOCK_SIZE = 1024;

std::vector<std::string> SplitPath(const std::string &path)
{
    std::vector<std::string> components;
    std::string part;
    for (char c : path)
    {
        if (c == '/') {
            if (!part.empty())
            {
                components.push_back(part);
                part.clear();
            }
        }
        else
        {
            part += c;
        }
    }
    if (!part.empty())
    {
        components.push_back(part);
    }
    return components;
}

Ext2::Ext2(StorageInterface &storage_interface) : storage_interface(storage_interface)
{
    return;
}

Ext2::~Ext2()
{
    return;
}

std::pair <Filesystem::Status, Filesystem::FileUID> Ext2::Open(const char *path, Filesystem::OpenMode mode, Filesystem::UserUID as_user)
{
    return std::make_pair(UNIMPLEMENTED, 0);
}

Filesystem::Status Ext2::Close(Filesystem::FileUID uid)
{
    return UNIMPLEMENTED;
}

Filesystem::Status Ext2::Read(Filesystem::FileUID uid, void *buf, size_t size)
{
    return UNIMPLEMENTED;
}

Filesystem::Status Ext2::Write(Filesystem::FileUID uid, void *buf, size_t size)
{
    return UNIMPLEMENTED;
}

Filesystem::Status Ext2::CreateFile(const char *path, Filesystem::UserUID as_user, Filesystem::FilePermission permissions)
{
    return UNIMPLEMENTED;
}

Filesystem::Status Ext2::CreateDirectory(const char *path, Filesystem::UserUID as_user, Filesystem::FilePermission permissions)
{
    return UNIMPLEMENTED;
}

Filesystem::Status Ext2::GetFileInformation(const char *path, Filesystem::UserUID as_user)
{
    return UNIMPLEMENTED;
}

std::pair<Filesystem::Status, bool> Ext2::FileExists(const char *path, Filesystem::UserUID as_user)
{
    // Check if the path is valid (non-null and non-empty)
    if (!path || *path == '\0')
    {
        return {Filesystem::Status::FAILURE, false};
    }

    // Split the path into components
    std::vector<std::string> path_components = SplitPath(path);
    if (path_components.empty())
    {
        return {Filesystem::Status::FAILURE, false};
    }

    // Start from the root directory inode
    uint32_t current_inode = ROOT_INODE;

    // Traverse through the path components
    for (size_t i = 0; i < path_components.size(); ++i)
    {
        const std::string &component = path_components[i];
        bool is_last_component = (i == path_components.size() - 1);

        // Read the directory entries for the current inode
        std::vector<DirectoryEntry> entries;
        auto status = ReadDirectory(current_inode, entries);
        if (status != Filesystem::Status::SUCCESS)
        {
            return {status, false};
        }

        // Search for the component in the directory entries
        bool found = false;
        for (const auto &entry : entries)
        {
            if (entry.name == component)
            {
                found = true;
                current_inode = entry.inode;
                break;
            }
        }

        // If not found, the file doesn't exist
        if (!found)
        {
            return {Filesystem::Status::SUCCESS, false};
        }

        // If this is the last component, check if it's a file (not a directory)
        if (is_last_component)
        {
            Inode inode;
            status = ReadInode(current_inode, inode);
            if (status != Filesystem::Status::SUCCESS)
            {
                return {status, false};
            }

            // Check if the inode represents a regular file
            bool is_file = (inode.mode & INODE_TYPE_MASK) == INODE_TYPE_FILE;
            return {Filesystem::Status::SUCCESS, is_file};
        }
    }

    // If we reached here, the path refers to a directory, not a file
    return {Filesystem::Status::SUCCESS, false};
}

std::pair<Filesystem::Status, bool> Ext2::DirectoryExists(const char *path, Filesystem::UserUID as_user)
{
    return std::make_pair(UNIMPLEMENTED, false);
}

Filesystem::Status Ext2::ListDirectory(const char *path, Filesystem::UserUID as_user, std::vector <std::string> &entries)
{
    return UNIMPLEMENTED;
}

std::pair<Filesystem::Status, bool>
Ext2::HasPermission(const char *path, Filesystem::UserUID user, Filesystem::FilePermission permission)
{
    return std::make_pair(UNIMPLEMENTED, false);
}

std::pair<Filesystem::Status, bool> Ext2::SetPermission(const char *path, Filesystem::UserUID user, Filesystem::FilePermission permission)
{
    return std::make_pair(UNIMPLEMENTED, false);
}

std::pair<Filesystem::Status, bool> Ext2::ClearPermission(const char *path, Filesystem::UserUID user, Filesystem::FilePermission permission)
{
    return std::make_pair(UNIMPLEMENTED, false);
}

bool Ext2::IsFilesystem(StorageInterface &si)
{
    Superblock sb;
    auto status = si.Read(&sb, SUPERBLOCK_OFFSET, sizeof(Superblock));
    if (status != StorageInterface::Status::SUCCESS) {
        return false;
    }

    // Ext2 filesystem magic number is 0xEF53
    constexpr uint16_t EXT2_MAGIC = 0xEF53;

    return sb.magic == EXT2_MAGIC;
}

Filesystem::Status Ext2::DeleteFile(const char *path, Filesystem::UserUID as_user)
{
    return UNIMPLEMENTED;
}

Filesystem::Status Ext2::DeleteDirectory(const char *path, Filesystem::UserUID as_user)
{
    return UNIMPLEMENTED;
}