//
// Created by Piotr on 13.08.2024.
//

#include <Rtl/string.hpp>
#include "msi.hpp"

MemoryStorageInterface::MemoryStorageInterface(uint64_t uuid, void *data, size_t size)
{
    this->data = data;
    this->size = size;
    this->uuid = uuid;
}

uint64_t MemoryStorageInterface::GetUUID()
{
    return this->uuid;
}

const char *MemoryStorageInterface::GetLastError()
{
    switch (status)
    {
        case StorageInterface::Status::SUCCESS:
            return "Success";
        case StorageInterface::Status::FAILURE:
            return "Failure";
        case StorageInterface::Status::OUT_OF_BOUNDS:
            return "Out of bounds";
        case StorageInterface::Status::DEVICE_ERROR:
            return "Device error";
        case StorageInterface::Status::NOT_INITIALIZED:
            return "Not initialized";
        default:
            return "Unknown error";
    }
}

size_t MemoryStorageInterface::GetCapacity()
{
    return size;
}

MemoryStorageInterface::Status MemoryStorageInterface::Read(void *dest, size_t off, size_t len)
{
    if (off + len > size)
    {
        status = StorageInterface::Status::OUT_OF_BOUNDS;
        return StorageInterface::Status::OUT_OF_BOUNDS;
    }
    memcpy(dest, (void*)((uintptr_t)data + off), len);

    return StorageInterface::Status::SUCCESS;
}

MemoryStorageInterface::Status MemoryStorageInterface::Write(void *dest, size_t off, size_t len)
{
    if (off + len > size)
    {
        status = StorageInterface::Status::OUT_OF_BOUNDS;
        return StorageInterface::Status::OUT_OF_BOUNDS;
    }

    memcpy((void*)((uintptr_t)data + off), dest, len);

    return StorageInterface::Status::SUCCESS;
}

bool MemoryStorageInterface::IsDeviceHealthy()
{
    return true;
}