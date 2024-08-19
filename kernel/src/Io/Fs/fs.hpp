#pragma once

#include <Io/storage_interface.hpp>
#include <utility>
#include <cstdint>
#include <cstddef>
#include <vector>
#include <string>
#include <vector>
#include <ctime>

class Filesystem
{
public:
    enum Status
    {
        SUCCESS,
        FAILURE,
        UNIMPLEMENTED,
        FILE_NOT_FOUND,
        DIRECTORY_NOT_FOUND,
        READ_ERROR,
        INVALID_PATH
    };

    enum class OpenMode
    {
        READ = 1 << 0,
        WRITE = 1 << 1,
        APPEND = 1 << 2,
        EXCLUSIVE = 1 << 3  // Note: File must not exist in first place.
    };

    enum class FilePermission
    {
        READ = 1 << 0,
        WRITE = 1 << 1,
        EXECUTE = 1 << 2
    };

    struct FileInfo
    {
        const char* name;
        size_t size;
        time_t creation_time;
        time_t modification_time;
        time_t last_accessed_time;
    };

    using FileUID = uint64_t;
    using UserUID = uint64_t;

    virtual ~Filesystem() = default;

    virtual bool IsFilesystem(StorageInterface& si) = 0;

    virtual std::pair<Status, FileUID> Open(const char* path, OpenMode mode, UserUID as_user) = 0;
    virtual Status Close(FileUID uid)  = 0;

    virtual Status Read(FileUID uid, void* buf, size_t size)  = 0;
    virtual Status Write(FileUID uid, void* buf, size_t size) = 0;

    virtual Status CreateFile(const char* path, UserUID as_user, FilePermission permissions) = 0;
    virtual Status DeleteFile(const char* path, UserUID as_user) = 0;
    virtual Status GetFileInformation(const char* path, UserUID as_user) = 0;
    virtual std::pair<Status, bool> FileExists(const char* path, UserUID as_user) = 0;

    virtual std::pair<Status, bool> DirectoryExists(const char* path, UserUID as_user) = 0;
    virtual Status CreateDirectory(const char* path, UserUID as_user, FilePermission permissions) = 0;
    virtual Status DeleteDirectory(const char* path, UserUID as_user)  = 0;
    virtual Status ListDirectory(const char* path, UserUID as_user, std::vector<std::string>& entries) = 0;

    virtual std::pair<Status, bool> HasPermission(const char* path, UserUID user, FilePermission permission) = 0;
    virtual std::pair<Status, bool> SetPermission(const char* path, UserUID user, FilePermission permission) = 0;
    virtual std::pair<Status, bool> ClearPermission(const char* path, UserUID user, FilePermission permission) = 0;
};