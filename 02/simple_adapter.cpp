#include <cmath>
#include <iostream>
#include <numbers>

struct CartesianPoint {
    float x;
    float y;
};

inline double distance(const CartesianPoint& p1, const CartesianPoint& p2) noexcept {
    return std::sqrt(std::pow(p2.x - p1.x, 2) + std::pow(p2.y - p1.y, 2));
}

struct PolarPoint {
    float r;
    float phi;

    PolarPoint(float r, float phi) noexcept
        : r(r)
        , phi(phi) {
    }
};

struct PointAdapter : CartesianPoint {
    explicit PointAdapter(const PolarPoint& p) noexcept
        : CartesianPoint{p.r * cos(p.phi), p.r * sin(p.phi)} {
    }
};

int main() {
    std::cout << "Working with cartesian coordinates" << std::endl;
    std::cout << distance(CartesianPoint{12.23, 34.43}, CartesianPoint{56.78, 90.12}) << std::endl;

    std::cout << "Working with polar coordinates" << std::endl;
    PolarPoint polar_point_1(42.0, std::numbers::pi);
    PolarPoint polar_point_2(42.0, std::numbers::pi / 2);
    PointAdapter adapted_point_1(polar_point_1);
    PointAdapter adapted_point_2(polar_point_2);
    std::cout << distance(adapted_point_1, adapted_point_2) << std::endl;

    return 0;
}
