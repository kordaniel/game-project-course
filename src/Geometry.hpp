#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP


struct Point2D
{
    int X, Y;
};

struct Dimensions2D
{
    int W, H;
    bool operator==(const Dimensions2D& rhs) const;
    bool operator!=(const Dimensions2D& rhs) const;
};

#endif // GEOMETRY_HPP
