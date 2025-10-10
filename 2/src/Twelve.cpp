#include "../include/Twelve.h"

std::string convert_to_base12(const int x){
    std::string result;
    int num = x;
    while (num){
        if (num % 12 > 9){
            result += ((num % 12) - 10 + 'A');
        }
        else{
            result += ((num % 12) + '0');
        }
            num /= 12;
    }
    std::reverse(result.begin(), result.end());
    return result;
} 


int Twelve::to_decimal() const{
    int result = 0;
    for (int i = 0; i < length; i++){
        if (isdigit(digits[i])){
            result = result * 12 + (digits[i] - '0');
        }
        else{
            result = result * 12 + (digits[i] - 'A' + 10);
        }
    }
    return result;
}
Twelve::Twelve() : length(1)
{
    digits = new unsigned char[1];
    digits[0] = '0';
}

Twelve::Twelve(const size_t &n, unsigned char t)
{
    if (n == 0){
        throw std::invalid_argument("Размер должен быть положительным!");
    }
    
    if (!(t == '0' || t == '1' || t == '2' || t == '3' || t == '4' || t == '5' || t == '6' || t == '7' || t == '8' || t == '9' || t == 'A' || t == 'B' || t == 'a' || t == 'b')) {
        throw std::invalid_argument("Не явл. числом в 12-ричной СС!");
    }

    digits = new unsigned char[n];
    for (size_t i=0; i < n; i++){
        digits[i] = t;
    }
    length = n;
}

Twelve::Twelve(const std::initializer_list<unsigned char> &t) : length(t.size())
{
    if (length == 0){
        throw std::invalid_argument("Размер должен быть положительным!");
    }
    digits = new unsigned char[t.size()];
    size_t i{0};


    for (auto &c : t){
        if (!(c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' || c == 'A' || c == 'B' || c == 'a' || c == 'b')) {
            throw std::invalid_argument("Не явл. числом в 12-ричной СС!");
        }
        digits[i++] = c;
    }
}

Twelve::Twelve(const std::string &t)
{
    digits = new unsigned char[t.size()];
    length  = t.size();

    for(size_t i{0}; i < length; ++i){
        unsigned char c = t[i];
        if (!(c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' || c == 'A' || c == 'B' || c == 'a' || c == 'b')) {
            delete[] digits;
            throw std::invalid_argument("Не явл. числом в 12-ричной СС!");
        }
         digits[i] = c;
    }
}

Twelve::Twelve(const Twelve& other) : length(other.length) {
    digits = new unsigned char[length];
    for (size_t i = 0; i < length; i++) {
        digits[i] = other.digits[i];
    }
}

Twelve::Twelve(Twelve&& other) noexcept : length(other.length), digits(other.digits) {
    other.length = 0;
    other.digits = nullptr;
}

std::ostream &Twelve::print(std::ostream &os)
{
    for (size_t i = 0; i < length; ++i)
        os << digits[i];
    return os;
}

Twelve Twelve::add(const Twelve& other) const{
    int a = other.to_decimal();
    int b = (*this).to_decimal();
    return Twelve(convert_to_base12(a+b));
}

Twelve Twelve::clone() const{
    std::string str;
    for (size_t i = 0; i < length; i++) {
        str += digits[i];
    }
    return Twelve(str);
}

Twelve Twelve::minus(const Twelve& other) const{
    int a = (*this).to_decimal();
    int b = other.to_decimal();
    if (b > a) {
        throw std::underflow_error("Разость будет отрицательной, но по условию мы рассм. только беззнаковые числа");
    }
    return Twelve(convert_to_base12(abs(a-b)));
}

// Twelve Twelve::minus(const Twelve& other) const{
//     int a = (*this).to_decimal();
//     int b = other.to_decimal();
//     return Twelve(convert_to_base12(abs(a-b)));
// }

bool Twelve::is_equal(const Twelve& other) const{
    if (length != other.length) return false;
        for (size_t i = 0; i < length; i++) {
            if (digits[i] != other.digits[i]) return false;
        }
    return true;
}

bool Twelve::is_greater(const Twelve& other) const{
    return (*this).to_decimal() > other.to_decimal();
}

bool Twelve::is_less(const Twelve& other) const{
    return (*this).to_decimal() < other.to_decimal();
}

// Twelve& add_assign(const Twelve& other){
//     std::string new_num = convert_to_base12((*this).to_decimal() + other.to_decimal());
//     (*this).length = new_num.size();
//     for (int i=0; i <= (*this).length ; i++){
//         (*this).digits[i] = new_num[i];
//     }
//     return *this;
// }

// Twelve& minus_assign(const Twelve& other){
//     std::string new_num = convert_to_base12((*this).to_decimal() - other.to_decimal());
//     (*this).length = new_num.size();
//     for (int i=0; i <= (*this).length ; i++){
//         (*this).digits[i] = new_num[i];
//     }
//     return *this;
 // }

Twelve::~Twelve() noexcept
{
    if (length > 0)
    {
        length = 0;
        delete[] digits;
        digits = nullptr;
    }
}
