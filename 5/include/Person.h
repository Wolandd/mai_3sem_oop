#ifndef PERSON_H
#define PERSON_H

#include <string>
#include <iostream>

struct Person {
    std::string full_name;
    int years;

    Person() = default;
    Person(std::string n, int a) : full_name(std::move(n)), years(a) {}
};

inline std::ostream& operator<<(std::ostream& os, const Person& p) {
    return os << p.full_name << " (" << p.years << ")";
}

#endif
