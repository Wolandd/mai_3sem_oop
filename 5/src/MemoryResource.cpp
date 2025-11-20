#include "../include/MemoryResource.h"
#include <new>
#include <iostream>
#include <stdexcept>

bool ENABLE_MEMORY_LOG = true;

void* ManagedMemoryResource::do_allocate(size_t bytes, size_t alignment) {
    auto block_key = std::make_pair(bytes, alignment);

    auto block_iterator = available_blocks.find(block_key);
    if (block_iterator != available_blocks.end() && !block_iterator->second.empty()) {
        void* memory_ptr = block_iterator->second.back();
        block_iterator->second.pop_back();

        active_blocks[memory_ptr] = {bytes, alignment};

        if (ENABLE_MEMORY_LOG)
            std::cout << "[MR] reuse: " << memory_ptr << " (" << bytes << ")\n";

        return memory_ptr;
    }

    void* memory_ptr = ::operator new(bytes, std::align_val_t(alignment));
    active_blocks[memory_ptr] = {bytes, alignment};

    if (ENABLE_MEMORY_LOG)
        std::cout << "[MR] new:   " << memory_ptr << " (" << bytes << ")\n";

    return memory_ptr;
}

void ManagedMemoryResource::do_deallocate(void* ptr, size_t bytes, size_t alignment) {
    
    auto block_iterator = active_blocks.find(ptr);
    if (block_iterator == active_blocks.end())
        throw std::logic_error("deallocate on non-allocated ptr");

    auto block_info = block_iterator->second;
    active_blocks.erase(block_iterator);

    available_blocks[{block_info.size, block_info.alignment}].push_back(ptr);

    if (ENABLE_MEMORY_LOG)
        std::cout << "[MR] free:  " << ptr << "\n";
}

bool ManagedMemoryResource::do_is_equal(const std::pmr::memory_resource& other) const noexcept {
    return this == &other;
}

ManagedMemoryResource::~ManagedMemoryResource() {
    std::cout << "[MR] CLEANUP\n";

    for (auto& block_pair : active_blocks) {
        ::operator delete(block_pair.first, std::align_val_t(block_pair.second.alignment));
        std::cout << "  delete active " << block_pair.first << "\n";
    }

    for (auto& block_map_entry : available_blocks) {
        for (void* memory_ptr : block_map_entry.second) {
            ::operator delete(memory_ptr, std::align_val_t(block_map_entry.first.second));
            std::cout << "  delete free   " << memory_ptr << "\n";
        }
    }
}