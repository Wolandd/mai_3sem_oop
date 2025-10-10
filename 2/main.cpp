#include <iostream>
#include "include/Twelve.h"

int main(){
    std::string input;
    unsigned char digit;
    size_t count;
    std::cout << "Введите число в 12-ной СС:\n";
    std::cin >> input;
    std::cout << "Введите цифру в 12-ной СС и количество её повторений:\n";
    std::cin >> count >> digit;
    
    try{
        Twelve num1(input);
        Twelve num2(count, digit);

        std::cout << "Числа успешно созданы!" << std::endl;
        std::cout << "Демонстрация ариф. операций:" << std::endl;

        std::cout << "Сумма: ";
        (num1.add(num2)).print(std::cout) << std::endl;

        try{
            std::cout << "Разность: ";
            (num1.minus(num2)).print(std::cout) << std::endl;
        }
        catch (const std::underflow_error& e){
            std::cout << "Ошибка при вычитании: " << e.what() << std::endl;    
        }

        std::cout << "Копирование (cоздадим копию первого числа): ";
        (num1.clone()).print(std::cout) << std::endl;

        std::cout << "Демонстрация операций сравнения:" << std::endl;

        std::cout << "Первое > Второе ?: "  <<(num1.is_greater(num2)) << std::endl;
        std::cout << "Первое == Второе ?: " <<(num1.is_equal(num2)) << std::endl;
        std::cout << "Первое < Второе ?: " <<(num1.is_less(num2)) << std::endl;

    }
    catch (const std::invalid_argument& e){
        std::cout << "Ошибка при создании чисел: " << e.what() << std::endl;
    }
    
    
    // std::cout << "Плюс равно: " << std::endl;
    // num1 += num2;
    // num1.print(std::cout) << std::endl;

    // std::cout << "Минус равно 5: " << std::endl;
    // num1 -= Twelve('5');
    // num1.print(std::cout) << std::endl;
    // return 0;
}