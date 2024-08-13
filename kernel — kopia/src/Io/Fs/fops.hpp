#pragma once

#include <Ke/status.hpp>
#include <functional>
#include <wchar.h>
#include <cstddef>

#define FS_SUCCESS             CREATE_ERROR_CODE(VFS_ERROR_BASE, SEVERITY_ERROR, 0);
#define FS_FAILED_TO_OPEN      CREATE_ERROR_CODE(VFS_ERROR_BASE, SEVERITY_ERROR, 1);
#define FS_FAILED_TO_READ      CREATE_ERROR_CODE(VFS_ERROR_BASE, SEVERITY_ERROR, 2);
#define FS_FAILED_TO_WRITE     CREATE_ERROR_CODE(VFS_ERROR_BASE, SEVERITY_ERROR, 4);
#define FS_FAILED_TO_EXOPEN    CREATE_ERROR_CODE(VFS_ERROR_BASE, SEVERITY_ERROR, 5); // note: failed to exclusively open the file. it may exist already.
#define FS_FAILED_TO_CLOSE     CREATE_ERROR_CODE(VFS_ERROR_BASE, SEVERITY_ERROR, 6);

enum FileOpenMode
{
    READ = 1 << 0,          // We will read from the file
    WRITE = 1 << 1,         // We will write to the file.
    APPEND = 1 << 2,        // Data will be appended to the file.
    EXCLUSIVE = 1 << 3,     // File mustn't exist in first place.
};

using FileFileDescriptor = uint64_t;

using OpenFileFileOp = std::function<KSTATUS(
        FileFileDescriptor f_Fd,
        const wchar_t *f_Path,
        FileOpenMode f_Mode
)>;

using CloseFileFileOp = std::function<KSTATUS(
        FileFileDescriptor f_Fd
)>;

using ReadFileFileOp = std::function<KSTATUS(
        FileFileDescriptor f_Fd,
        char *f_Buf,
        size_t f_Len,
        size_t f_Off
)>;

using WriteFileFileOp = std::function<KSTATUS(
        FileFileDescriptor f_Fd,
        char *f_Buf,
        size_t f_Len,
        size_t f_Off
)>;

struct FileSystem
{
    OpenFileFileOp open;
    CloseFileFileOp close;

    ReadFileFileOp read;
    WriteFileFileOp write;
};