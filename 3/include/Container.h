#ifndef CONTAINER_H
#define CONTAINER_H

#include "Figure.h"
#include <algorithm>
#include <stdexcept>

class Container {
public:
    Container() = default;

    void append(Figure* figure) {
        if (count == max_capacity) {
            expand(max_capacity == 0 ? 1 : max_capacity * 2);
        }
        elements[count++] = figure;
    }

    void erase(size_t idx) {
        if (idx >= count) throw std::out_of_range("Index out of range");
        delete elements[idx];
        std::move(elements + idx + 1, elements + count, elements + idx);
        --count;
    }

    Figure* operator[](size_t idx) {
        return elements[idx];
    }

    const Figure* operator[](size_t idx) const {
        return elements[idx];
    }

    size_t length() const {
        return count;
    }

    bool is_empty() const {
        return count == 0;
    }

    void reset() {
        for (size_t i = 0; i < count; ++i) {
            delete elements[i];
        }
        delete[] elements;
        elements = nullptr;
        count = max_capacity = 0;
    }

    ~Container() {
        reset();
    }

private:
    void expand(size_t new_capacity) {
        Figure** new_elements = new Figure*[new_capacity];
        std::copy(elements, elements + count, new_elements);
        delete[] elements;
        elements = new_elements;
        max_capacity = new_capacity;
    }

    Figure** elements = nullptr;
    size_t count = 0;
    size_t max_capacity = 0;
};

#endif

