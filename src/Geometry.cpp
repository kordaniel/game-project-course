#include "Geometry.hpp"


bool
Dimensions2D::operator==(const Dimensions2D& rhs) const
{
    return W == rhs.W && H == rhs.H;
}

bool
Dimensions2D::operator!=(const Dimensions2D& rhs) const
{
    return ! (*this == rhs);
}

bool
RectangleF::Overlaps(const RectangleF& other) const
{
    return OverlapsX(other) && OverlapsY(other);
}

bool
RectangleF::OverlapsX(const RectangleF& other) const
{
    if (other.X + other.W < this->X || other.X > this->X + this->W) {
        return false;
    }
    return true;
}

bool
RectangleF::OverlapsY(const RectangleF& other) const
{
    if (other.Y + other.H < this->Y || other.Y > this->Y + this->H) {
        return false;
    }
    return true;
}
