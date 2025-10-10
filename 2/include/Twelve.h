#pragma once
#include <iostream>
#include <string>
#include <initializer_list>
#include <algorithm>

#ifndef TWELVE_H
#define TWELVE_H

class Twelve
{
    public : 
        Twelve();
        Twelve(const size_t & n, unsigned char t = 0);
        Twelve(const std::initializer_list< unsigned char> &t);
        Twelve(const std::string &t);

        Twelve(const Twelve& other);
        Twelve(Twelve&& other) noexcept;  
        virtual ~Twelve() noexcept;  

        Twelve add(const Twelve& other) const;
        Twelve minus(const Twelve& other) const;
        Twelve clone() const;

        bool   is_equal(const Twelve& other) const;
        bool   is_greater(const Twelve& other) const;
        bool   is_less(const Twelve& other) const;
        std::ostream&   print(std::ostream& os);
        int to_decimal() const;

    private:
          size_t length;
          unsigned char *digits;    
};

#endif