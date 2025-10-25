#include "../include/Container.h"
#include "../include/Triangle.h"
#include "../include/Hexagon.h"
#include "../include/Octagon.h"
#include <iostream>
#include <string>
#include <memory>
#include <limits>

int main() {
    Container collection;
    std::string figure_type;

    std::cout << "Input format:\n";
    std::cout << "  Triangle (x1, y1) (x2, y2) (x3, y3)\n";
    std::cout << "  Hexagon (x1, y1) ... (x6, y6)\n";
    std::cout << "  Octagon (x1, y1) ... (x8, y8)\n";
    std::cout << "Enter figures (one per line). EOF (Ctrl+D/Ctrl+Z) to finish.\n";

    while (std::cin >> figure_type) {
        std::unique_ptr<Figure> current_figure = nullptr;

        if (figure_type == "Triangle") {
            current_figure = std::make_unique<Triangle>();
        } else if (figure_type == "Hexagon") {
            current_figure = std::make_unique<Hexagon>();
        } else if (figure_type == "Octagon") {
            current_figure = std::make_unique<Octagon>();
        } else {
            std::cerr << "Unknown figure type: " << figure_type << "\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        if (!(std::cin >> *current_figure)) {
            std::cerr << "Failed to read " << figure_type << ". Invalid vertex format.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        collection.append(current_figure.release());
    }

    if (collection.is_empty()) {
        std::cout << "No valid figures entered.\n";
        return 0;
    }

    for (size_t idx = 0; idx < collection.length(); ++idx) {
        auto center_point = collection[idx]->get_center();
        std::cout << "Figure " << idx << ":\n";
        std::cout << "  Center: (" << center_point.first << ", " << center_point.second << ")\n";
        std::cout << "  Area: " << static_cast<double>(*collection[idx]) << "\n";
        std::cout << "  Vertices: " << *collection[idx] << "\n\n";
    }

    double total_area = 0.0;
    for (size_t idx = 0; idx < collection.length(); ++idx) {
        total_area += static_cast<double>(*collection[idx]);
    }
    std::cout << "Total area: " << total_area << "\n";

    collection.erase(collection.length() - 1);
    std::cout << "Removed last figure. New total area: ";
    total_area = 0.0;
    for (size_t idx = 0; idx < collection.length(); ++idx) {
        total_area += static_cast<double>(*collection[idx]);
    }
    std::cout << total_area << "\n";

    return 0;
}