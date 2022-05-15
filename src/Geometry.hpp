#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP


struct Point2D
{
    int X, Y;
};

struct Point2DF
{
    float X, Y;
};

struct Dimensions2D
{
    int W, H;
    bool operator==(const Dimensions2D& rhs) const;
    bool operator!=(const Dimensions2D& rhs) const;
};

struct Dimensions2DF
{
    float W, H;
};

struct Rectangle
{
    int X, Y, W, H;
};

struct RectangleF
{
    float X, Y, W, H;
    bool Overlaps(const RectangleF& other)  const;
    bool OverlapsX(const RectangleF& other) const;
    bool OverlapsY(const RectangleF& other) const;
};

#endif // GEOMETRY_HPP
