#include <iostream>
#include "include/gcd.h"

int main()
{
    int a, b;
    std::cin >> a >> b;
    std::cout << gcd::gcd(a, b) << std::endl;
}