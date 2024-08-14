#pragma once

#include <Io/storage_interface.hpp>
#include <cstdint>
#include <cstddef>

class MemoryStorageInterface : public StorageInterface
{
public:
    MemoryStorageInterface(uint64_t uuid, void* data, size_t size);

    uint64_t GetUUID() override;

    const char * GetLastError() override;

    size_t GetCapacity() override;

    bool IsDeviceHealthy() override;

    Status Read(void *dest, size_t off, size_t len) override;
    Status Write(void *dest, size_t off, size_t len) override;
private:
    void* data;
    size_t size;
    uint64_t uuid;
    Status status = StorageInterface::Status::SUCCESS;
};