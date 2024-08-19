#pragma once

#include <Io/Fs/fs.hpp>

class Ext2 : public Filesystem
{
public:
    Ext2(StorageInterface& storage_interface);
    ~Ext2();

    bool IsFilesystem(StorageInterface &si) override;

    std::pair<Status, FileUID> Open(const char *path, Filesystem::OpenMode mode, Filesystem::UserUID as_user) override;
    Status Close(Filesystem::FileUID uid) override;

    Status Read(Filesystem::FileUID uid, void *buf, size_t size) override;
    Status Write(Filesystem::FileUID uid, void *buf, size_t size) override;

    Status CreateFile(const char *path, Filesystem::UserUID as_user, Filesystem::FilePermission permissions) override;
    Status CreateDirectory(const char *path, Filesystem::UserUID as_user, Filesystem::FilePermission permissions) override;
    Status GetFileInformation(const char *path, Filesystem::UserUID as_user) override;

    std::pair<Status, bool> FileExists(const char *path, Filesystem::UserUID as_user) override;
    std::pair<Status, bool> DirectoryExists(const char *path, Filesystem::UserUID as_user) override;
    Status ListDirectory(const char *path, Filesystem::UserUID as_user, std::vector<std::string> &entries) override;

    std::pair<Status, bool> HasPermission(const char *path, Filesystem::UserUID user, Filesystem::FilePermission permission) override;
    std::pair<Status, bool> SetPermission(const char *path, Filesystem::UserUID user, Filesystem::FilePermission permission) override;
    std::pair<Status, bool> ClearPermission(const char *path, Filesystem::UserUID user, Filesystem::FilePermission permission) override;

    Status DeleteFile(const char *path, Filesystem::UserUID as_user) override;
    Status DeleteDirectory(const char *path, Filesystem::UserUID as_user) override;
private:
    StorageInterface& storage_interface;
};