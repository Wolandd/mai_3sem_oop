#ifndef OCTAGON_H
#define OCTAGON_H

#include "Figure.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <vector>

class Octagon : public Figure {
public:
    static constexpr size_t VERTEX_COUNT = 8;

    Octagon() : points(VERTEX_COUNT, std::make_pair(0.0, 0.0)) {}

    Octagon(const std::vector<std::pair<double, double>>& points_list) {
        if (points_list.size() != VERTEX_COUNT) {
            throw std::invalid_argument("Octagon must have exactly 8 vertices");
        }
        points = points_list;
    }

    Octagon(const Octagon& src) : points(src.points) {}
    Octagon(Octagon&& src) noexcept : points(std::move(src.points)) {}

    Figure& operator=(const Figure& rhs) override {
        if (this == &rhs) return *this;
        const Octagon* octagon = dynamic_cast<const Octagon*>(&rhs);
        if (!octagon) {
            throw std::invalid_argument("Cannot assign non-Octagon to Octagon");
        }
        points = octagon->points;
        return *this;
    }

    Figure& operator=(Figure&& rhs) noexcept override {
        Octagon* octagon = dynamic_cast<Octagon*>(&rhs);
        if (!octagon) {
            return *this;
        }
        points = std::move(octagon->points);
        return *this;
    }

    bool operator==(const Figure& rhs) const override {
        const Octagon* octagon = dynamic_cast<const Octagon*>(&rhs);
        if (!octagon) return false;
        const double tolerance = 1e-9;
        for (size_t idx = 0; idx < VERTEX_COUNT; ++idx) {
            if (std::abs(points[idx].first - octagon->points[idx].first) >= tolerance ||
                std::abs(points[idx].second - octagon->points[idx].second) >= tolerance)
                return false;
        }
        return true;
    }

    std::pair<double, double> get_center() const override {
        double center_x = 0.0, center_y = 0.0;
        for (const auto& point : points) {
            center_x += point.first;
            center_y += point.second;
        }
        return {center_x / VERTEX_COUNT, center_y / VERTEX_COUNT};
    }

    operator double() const override {
        double result = 0.0;
        for (size_t idx = 0; idx < VERTEX_COUNT; ++idx) {
            size_t next_idx = (idx + 1) % VERTEX_COUNT;
            result += points[idx].first * points[next_idx].second;
            result -= points[next_idx].first * points[idx].second;
        }
        return std::abs(result) / 2.0;
    }

protected:
    void output(std::ostream& stream) const override {
        stream << std::fixed << std::setprecision(6);
        for (size_t idx = 0; idx < VERTEX_COUNT; ++idx) {
            stream << "(" << points[idx].first << ", " << points[idx].second << ")";
            if (idx < VERTEX_COUNT - 1) stream << " ";
        }
    }

    void input(std::istream& stream) override {
        points.resize(VERTEX_COUNT);
        for (size_t idx = 0; idx < VERTEX_COUNT; ++idx) {
            char left_paren, separator, right_paren;
            if (!(stream >> left_paren >> points[idx].first >> separator >> points[idx].second >> right_paren) ||
                left_paren != '(' || right_paren != ')' || separator != ',') {
                stream.setstate(std::ios::failbit);
                return;
            }
        }
    }

private:
    std::vector<std::pair<double, double>> points;
};

#endif