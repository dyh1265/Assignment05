#include "color.h"
#include <cmath>
#include "scalar.h"
namespace rt {
    
    RGBColor RGBColor::operator+(const RGBColor& c) const
    {return RGBColor(this->r + c.r,this->g + c.g,this->b + c.b);}
    
    
    RGBColor RGBColor::operator-(const RGBColor& c) const
    {return RGBColor(this->r - c.r,this->g - c.g,this->b - c.b);}
    
    RGBColor RGBColor::operator*(const RGBColor& c) const
    {return RGBColor(this->r * c.r,this->g * c.g,this->b * c.b);}
    
    bool RGBColor::operator==(const RGBColor& b) const{
    if ( this->r != b.r ) { return false; }
    if ( this->g != b.g ) { return false; }
    if ( this->b != b.b ) { return false; }
    return true;
    }
 
    bool RGBColor::operator!=(const RGBColor& b) const{
        if ( this->r == b.r ) { return false; }
        if ( this->g == b.g ) { return false; }
        if ( this->b == b.b ) { return false; }
        return true;
    }
    
    RGBColor RGBColor::clamp() const{
    RGBColor o;
    o.r = this->r > 1.0f ? 1.0f : (this->r < 0.0f ? 0.0f : this->r);
    o.g = this->g > 1.0f ? 1.0f : (this->g < 0.0f ? 0.0f : this->g);
    o.b = this->b > 1.0f ? 1.0f : (this->b < 0.0f ? 0.0f : this->b);
    
    return o;
    }
    
    RGBColor operator*(float scalar, const RGBColor& b)
    {return RGBColor(scalar * b.r, scalar * b.g, scalar * b.b);}
    
    RGBColor operator*(const RGBColor& a, float scalar)
     {return RGBColor(scalar * a.r, scalar * a.g, scalar * a.b);}
    
    RGBColor operator/(const RGBColor& a, float scalar)
    {return RGBColor(a.r/scalar, a.g / scalar , a.b / scalar);}
    
    
}
