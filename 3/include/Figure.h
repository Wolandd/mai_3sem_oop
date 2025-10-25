#ifndef FIGURE_H
#define FIGURE_H

#include <iostream>

class Figure {
public:
    virtual ~Figure() = default;

    virtual std::pair<double, double> get_center() const = 0;

    virtual operator double() const = 0;

    virtual bool operator==(const Figure& rhs) const = 0;

    virtual Figure& operator=(const Figure& rhs) = 0;
    virtual Figure& operator=(Figure&& rhs) noexcept = 0;

    friend std::ostream& operator<<(std::ostream& stream, const Figure& figure);
    friend std::istream& operator>>(std::istream& stream, Figure& figure);

protected:
    virtual void output(std::ostream& stream) const = 0;
    virtual void input(std::istream& stream) = 0;
};

inline std::ostream& operator<<(std::ostream& stream, const Figure& figure) {
    figure.output(stream);
    return stream;
}

inline std::istream& operator>>(std::istream& stream, Figure& figure) {
    figure.input(stream);
    return stream;
}


#endif