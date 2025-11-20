#ifndef MEMORYRESOURCE_H
#define MEMORYRESOURCE_H

#include <memory_resource>
#include <map>
#include <vector>
#include <cstddef>
#include <iostream>

extern bool ENABLE_MEMORY_LOG;

class ManagedMemoryResource : public std::pmr::memory_resource {
public:
    ManagedMemoryResource() = default;
    ~ManagedMemoryResource();

protected:
    void* do_allocate(size_t bytes, size_t alignment) override;
    void  do_deallocate(void* ptr, size_t bytes, size_t alignment) override;
    bool  do_is_equal(const std::pmr::memory_resource& other) const noexcept override;

private:
    struct MemoryBlock {
        size_t size;
        size_t alignment;
    };

    std::map<void*, MemoryBlock> active_blocks;
    std::map<std::pair<size_t,size_t>, std::vector<void*>> available_blocks;
};

#endif
