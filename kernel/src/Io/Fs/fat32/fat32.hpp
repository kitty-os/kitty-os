#pragma once

#include <Io/Fs/fs.hpp>

class Fat32 : public Filesystem
{
public:
    Fat32(StorageInterface& si);
    bool IsFilesystem(StorageInterface &si) override;

    std::pair<Status, FileUID> Open(const char *path, Filesystem::OpenMode mode, Filesystem::UserUID as_user) override;
    Status Close(Filesystem::FileUID uid) override;

    Status Read(Filesystem::FileUID uid, void *buf, size_t size) override;
    Status Write(Filesystem::FileUID uid, void *buf, size_t size) override;

    Status CreateFile(const char *path, Filesystem::UserUID as_user, Filesystem::FilePermission permissions) override;
    Status DeleteFile(const char *path, Filesystem::UserUID as_user) override;

    Status GetFileInformation(const char *path, Filesystem::UserUID as_user) override;

    Status CreateDirectory(const char *path, Filesystem::UserUID as_user, Filesystem::FilePermission permissions) override;
    Status DeleteDirectory(const char *path, Filesystem::UserUID as_user) override;
    Status ListDirectory(const char *path, Filesystem::UserUID as_user, std::vector<std::string> &entries) override;

    std::pair<Status, bool> HasPermission(const char *path, Filesystem::UserUID user, Filesystem::FilePermission permission) override;
    Status SetPermission(const char *path, Filesystem::UserUID user, Filesystem::FilePermission permission) override;
    Status ClearPermission(const char *path, Filesystem::UserUID user, Filesystem::FilePermission permission) override;
private:
    StorageInterface& si;
};