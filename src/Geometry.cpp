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
