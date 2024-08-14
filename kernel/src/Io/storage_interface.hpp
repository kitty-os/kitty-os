#pragma once

#include <cstdint>
#include <cstddef>

class StorageInterface
{
public:
    enum class Status
    {
        SUCCESS, FAILURE, OUT_OF_BOUNDS, DEVICE_ERROR, NOT_INITIALIZED
    };

    virtual uint64_t GetUUID() = 0;

    virtual const char* GetLastError() = 0;
    virtual size_t GetCapacity() = 0;
    virtual bool IsDeviceHealthy() = 0;

    virtual Status Read(void* dest, size_t off, size_t len) = 0;
    virtual Status Write(void* dest, size_t off, size_t len) = 0;
};