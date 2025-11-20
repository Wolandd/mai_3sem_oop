#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

#include <memory_resource>
#include <cstddef>
#include <utility>
#include <stdexcept>
#include <type_traits>

template <typename T>
class DynamicArray {
private:
    std::pmr::polymorphic_allocator<T> mem_alloc;

    T* buffer = nullptr;
    size_t count = 0;
    size_t max_capacity = 0;

public:

    class IteratorType {
    public:
        using value_type = T;
        using reference = T&;
        using pointer = T*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        IteratorType(pointer ptr = nullptr) : current(ptr) {}

        reference operator*() const { return *current; }
        pointer operator->() const { return current; }

        IteratorType& operator++() { ++current; return *this; }
        IteratorType operator++(int) { IteratorType temp=*this; ++current; return temp; }

        bool operator==(const IteratorType& other) const { return current == other.current; }
        bool operator!=(const IteratorType& other) const { return current != other.current; }

    private:
        pointer current;
    };


    class ConstIteratorType {
    public:
        using value_type = T;
        using reference = const T&;
        using pointer = const T*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        ConstIteratorType(pointer ptr = nullptr) : current(ptr) {}

        reference operator*() const { return *current; }
        pointer operator->() const { return current; }

        ConstIteratorType& operator++() { ++current; return *this; }
        ConstIteratorType operator++(int) { ConstIteratorType temp=*this; ++current; return temp; }

        bool operator==(const ConstIteratorType& other) const { return current == other.current; }
        bool operator!=(const ConstIteratorType& other) const { return current != other.current; }

    private:
        pointer current;
    };

    explicit DynamicArray(std::pmr::memory_resource* mr = std::pmr::get_default_resource())
        : mem_alloc(mr) {}

    ~DynamicArray() {
        clear();
        if (buffer) mem_alloc.deallocate(buffer, max_capacity);
    }

    DynamicArray(const DynamicArray&) = delete;
    DynamicArray& operator=(const DynamicArray&) = delete;

    DynamicArray(DynamicArray&& other) noexcept 
        : mem_alloc(other.mem_alloc.resource())  
        , buffer(other.buffer)
        , count(other.count)
        , max_capacity(other.max_capacity) 
    {
        other.buffer = nullptr;
        other.count = other.max_capacity = 0;
    }


    DynamicArray& operator=(DynamicArray&& other) noexcept {
        if (this != &other) {
            clear();
            if (buffer) mem_alloc.deallocate(buffer, max_capacity);
            buffer = other.buffer;
            count = other.count;
            max_capacity = other.max_capacity;
            
            other.buffer = nullptr;
            other.count = other.max_capacity = 0;
        }
        return *this;
    }

    void append(const T& value) {
        grow_if_needed();
        mem_alloc.construct(buffer + count, value);
        ++count;
    }

    void append(T&& value) {
        grow_if_needed();
        mem_alloc.construct(buffer + count, std::move(value));
        ++count;
    }

    template<typename... Args>
    void emplace(Args&&... args) {
        grow_if_needed();
        mem_alloc.construct(buffer + count, std::forward<Args>(args)...);
        ++count;
    }

    size_t size() const { return count; }
    size_t capacity() const { return max_capacity; }

    T& operator[](size_t index) {
        return buffer[index];
    }
    
    const T& operator[](size_t index) const { 
        return buffer[index]; 
    }

    T& at(size_t index) {
        if (index >= count) throw std::out_of_range("DynamicArray index out of range");
        return buffer[index];
    }
    
    const T& at(size_t index) const {
        if (index >= count) throw std::out_of_range("DynamicArray index out of range");
        return buffer[index];
    }

    IteratorType begin() { return IteratorType(buffer); }
    IteratorType end()   { return IteratorType(buffer + count); }

    ConstIteratorType begin() const { return ConstIteratorType(buffer); }
    ConstIteratorType end()   const { return ConstIteratorType(buffer + count); }

    ConstIteratorType cbegin() const { return ConstIteratorType(buffer); }
    ConstIteratorType cend()   const { return ConstIteratorType(buffer + count); }

    bool empty() const { return count == 0; }

    void clear() {
        for (size_t i = 0; i < count; ++i)
            mem_alloc.destroy(buffer + i);
        count = 0;
    }


    void erase(size_t index) {
        if (index >= count) throw std::out_of_range("erase index out of range");
        
        mem_alloc.destroy(buffer + index);
        
        for (size_t i = index; i + 1 < count; ++i) {
            buffer[i] = std::move(buffer[i+1]);
        }
        
        if (count > 0) {
            mem_alloc.destroy(buffer + count - 1);
        }
        --count;
    }

    void reserve(size_t new_capacity) {
        if (new_capacity <= max_capacity) return;
        
        T* new_buffer = mem_alloc.allocate(new_capacity);

        for (size_t i = 0; i < count; ++i) {
            mem_alloc.construct(new_buffer + i, std::move(buffer[i]));
            mem_alloc.destroy(buffer + i);
        }

        if (buffer) mem_alloc.deallocate(buffer, max_capacity);

        buffer = new_buffer;
        max_capacity = new_capacity;
    }

private:
    void grow_if_needed() {
        if (count < max_capacity) return;
        size_t new_cap = (max_capacity == 0) ? 2 : (max_capacity * 2);
        reserve(new_cap);
    }
};

#endif