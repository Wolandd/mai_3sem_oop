#include "../include/Triangle.h"
#include "../include/Hexagon.h"
#include "../include/Octagon.h"
#include <gtest/gtest.h>
#include <sstream>
#include <cmath>
#include <vector>
#include <iomanip>

const double EPS = 1e-6;

std::vector<std::pair<double, double>> create_triangle_vertices(double center_x, double center_y, double side_length) {
    const double radius = side_length / std::sqrt(3.0);
    const double start_angle = M_PI / 2.0;
    std::vector<std::pair<double, double>> vertex_list(3);
    for (int vertex_idx = 0; vertex_idx < 3; ++vertex_idx) {
        double current_angle = start_angle + 2.0 * M_PI * vertex_idx / 3.0;
        vertex_list[vertex_idx] = {center_x + radius * std::cos(current_angle), center_y + radius * std::sin(current_angle)};
    }
    return vertex_list;
}

std::vector<std::pair<double, double>> create_hexagon_vertices(double center_x, double center_y, double side_length) {
    std::vector<std::pair<double, double>> vertex_list(6);
    for (int vertex_idx = 0; vertex_idx < 6; ++vertex_idx) {
        double current_angle = 2.0 * M_PI * vertex_idx / 6.0;
        vertex_list[vertex_idx] = {center_x + side_length * std::cos(current_angle), center_y + side_length * std::sin(current_angle)};
    }
    return vertex_list;
}

std::vector<std::pair<double, double>> create_octagon_vertices(double center_x, double center_y, double side_length) {
    const double radius = side_length / (2.0 * std::sin(M_PI / 8.0));
    std::vector<std::pair<double, double>> vertex_list(8);
    for (int vertex_idx = 0; vertex_idx < 8; ++vertex_idx) {
        double current_angle = 2.0 * M_PI * vertex_idx / 8.0;
        vertex_list[vertex_idx] = {center_x + radius * std::cos(current_angle), center_y + radius * std::sin(current_angle)};
    }
    return vertex_list;
}

TEST(TriangleTest, AreaAndCenter) {
    auto vertex_list = create_triangle_vertices(1.0, 2.0, 2.0);
    Triangle triangle(vertex_list);
    EXPECT_NEAR(static_cast<double>(triangle), std::sqrt(3.0), EPS);
    auto center_point = triangle.get_center();
    EXPECT_NEAR(center_point.first, 1.0, EPS);
    EXPECT_NEAR(center_point.second, 2.0, EPS);
}

TEST(HexagonTest, AreaAndCenter) {
    auto vertex_list = create_hexagon_vertices(0.0, 0.0, 1.0);
    Hexagon hexagon(vertex_list);
    double expected_area = (3.0 * std::sqrt(3.0)) / 2.0;
    EXPECT_NEAR(static_cast<double>(hexagon), expected_area, EPS);
    auto center_point = hexagon.get_center();
    EXPECT_NEAR(center_point.first, 0.0, EPS);
    EXPECT_NEAR(center_point.second, 0.0, EPS);
}

TEST(OctagonTest, AreaAndCenter) {
    auto vertex_list = create_octagon_vertices(-1.0, 3.0, 0.5);
    Octagon octagon(vertex_list);
    double expected_area = 2.0 * (1.0 + std::sqrt(2.0)) * 0.25;
    EXPECT_NEAR(static_cast<double>(octagon), expected_area, EPS);
    auto center_point = octagon.get_center();
    EXPECT_NEAR(center_point.first, -1.0, EPS);
    EXPECT_NEAR(center_point.second, 3.0, EPS);
}

TEST(TriangleTest, Equality) {
    auto v1 = create_triangle_vertices(0, 0, 1);
    Triangle triangle1(v1);
    Triangle triangle2(v1);

    auto v3 = create_triangle_vertices(0, 0, 1.0001);
    Triangle triangle3(v3);

    auto v4 = create_triangle_vertices(0.1, 0, 1);
    Triangle triangle4(v4);

    EXPECT_TRUE(triangle1 == triangle2);
    EXPECT_FALSE(triangle1 == triangle3);
    EXPECT_FALSE(triangle1 == triangle4);
}

TEST(FigureTest, DifferentTypesNotEqual) {
    Triangle triangle(create_triangle_vertices(0, 0, 1));
    Hexagon hexagon(create_hexagon_vertices(0, 0, 1));
    EXPECT_FALSE(triangle == hexagon);
}

TEST(TriangleTest, IOStream) {
    auto vertex_list = create_triangle_vertices(0, 0, 2);
    std::ostringstream input_builder;
    for (size_t idx = 0; idx < vertex_list.size(); ++idx) {
        input_builder << "(" << vertex_list[idx].first << ", " << vertex_list[idx].second << ")";
        if (idx < vertex_list.size() - 1) input_builder << " ";
    }
    std::istringstream input_stream(input_builder.str());

    Triangle triangle;
    input_stream >> triangle;

    EXPECT_TRUE(input_stream.good());
    EXPECT_NEAR(triangle.get_center().first, 0.0, EPS);
    EXPECT_NEAR(triangle.get_center().second, 0.0, EPS);
    EXPECT_NEAR(static_cast<double>(triangle), std::sqrt(3.0), EPS);

    std::ostringstream output_stream;
    output_stream << triangle;
    std::string output_string = output_stream.str();
    EXPECT_FALSE(output_string.empty());
    EXPECT_NE(output_string.find('('), std::string::npos);
}

TEST(HexagonTest, IOStream) {
    auto vertex_list = create_hexagon_vertices(1, 1, 1);
    std::ostringstream input_builder;
    input_builder << std::fixed << std::setprecision(6);
    for (size_t idx = 0; idx < vertex_list.size(); ++idx) {
        input_builder << "(" << vertex_list[idx].first << ", " << vertex_list[idx].second << ")";
        if (idx < vertex_list.size() - 1) input_builder << " ";
    }
    std::istringstream input_stream(input_builder.str());

    Hexagon hexagon;
    input_stream >> hexagon;
    ASSERT_TRUE(input_stream.good() || input_stream.eof()) << "Input failed: " << input_builder.str();

    auto center_point = hexagon.get_center();
    EXPECT_NEAR(center_point.first, 1.0, EPS);
    EXPECT_NEAR(center_point.second, 1.0, EPS);

    std::ostringstream output_stream;
    output_stream << hexagon;
    std::string output_string = output_stream.str();
    size_t paren_count = 0;
    for (char ch : output_string) if (ch == '(') ++paren_count;
    EXPECT_EQ(paren_count, 6);
}

TEST(OctagonTest, IOStream) {
    auto vertex_list = create_octagon_vertices(0, 0, 1);
    std::ostringstream input_builder;
    for (size_t idx = 0; idx < vertex_list.size(); ++idx) {
        input_builder << "(" << vertex_list[idx].first << ", " << vertex_list[idx].second << ")";
        if (idx < vertex_list.size() - 1) input_builder << " ";
    }
    std::istringstream input_stream(input_builder.str());

    Octagon octagon;
    input_stream >> octagon;
    EXPECT_TRUE(input_stream.good());

    std::ostringstream output_stream;
    output_stream << octagon;
    std::string output_string = output_stream.str();
    size_t paren_count = 0;
    for (char ch : output_string) if (ch == '(') ++paren_count;
    EXPECT_EQ(paren_count, 8);
}

TEST(TriangleTest, Copy) {
    Triangle triangle1(create_triangle_vertices(1, 2, 3));
    Triangle triangle2 = triangle1;
    EXPECT_TRUE(triangle1 == triangle2);
}

TEST(HexagonTest, Move) {
    Hexagon hexagon1(create_hexagon_vertices(1, 1, 2));
    double original_area = static_cast<double>(hexagon1);
    Hexagon hexagon2 = std::move(hexagon1);
    EXPECT_NEAR(static_cast<double>(hexagon2), original_area, EPS);
}

TEST(FigureTest, Assignment) {
    Triangle triangle1(create_triangle_vertices(0, 0, 1));
    Triangle triangle2;

    Figure& figure_ref1 = triangle1;
    Figure& figure_ref2 = triangle2;

    figure_ref2 = figure_ref1;
    EXPECT_TRUE(triangle1 == triangle2);
}

TEST(FigureTest, AssignmentDifferentTypeThrows) {
    Triangle triangle;
    Hexagon hexagon(create_hexagon_vertices(0, 0, 1));
    EXPECT_THROW(triangle = hexagon, std::invalid_argument);
}

TEST(TriangleTest, InvalidVertexCountThrows) {
    std::vector<std::pair<double, double>> invalid_vertices = {{0,0}, {1,1}};
    EXPECT_THROW(Triangle triangle(invalid_vertices), std::invalid_argument);
}