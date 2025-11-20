#include <iostream>
#include "include/MemoryResource.h"
#include "include/DynamicArray.h"
#include "include/Person.h"

template <typename Container>
void display(const Container& container) {
    for (auto& element : container) std::cout << element << "\n";
}

int main() {
    ManagedMemoryResource memory;

    std::cout << "\n=== Demo int ===\n";
    DynamicArray<int> int_array(&memory);
    int_array.append(10);
    int_array.append(20);
    int_array.append(30);
    display(int_array);

    std::cout << "\nIterate with iterator explicitly:\n";
    for (auto iterator = int_array.begin(); iterator != int_array.end(); ++iterator)
        std::cout << *iterator << "\n";

    std::cout << "\n=== Demo Person ===\n";
    DynamicArray<Person> person_array(&memory);
    person_array.append(Person("Alice", 20));
    person_array.append(Person("Bob", 30));
    display(person_array);

    std::cout << "\n=== Test erase ===\n";
    int_array.erase(1);
    display(int_array);

    std::cout << "\n=== Program end ===\n";
    return 0;
}
