#include <core/point.h>
#include <core/vector.h>
#include <cmath>
#include <core/assert.h>
#include "float4.h"
namespace rt {
    Point::Point(const Float4& f4){
        Assert(f4.w == 1 );
        x = f4.x / f4.w;
        y = f4.y / f4.w;
        z = f4.z / f4.w;
    }
    Vector Point::operator-( const Point& b ) const
    {
        Vector c;
        c.x = this->x - b.x;
        c.y = this->y - b.y;
        c.z = this->z - b.z;
        return c;
    }
    
    Vector Point::operator+( const Point& b ) const
    {
        Vector c;
        c.x = this->x + b.x;
        c.y = this->y + b.y;
        c.z = this->z + b.z;
        return c;
    }

    bool Point::operator==(const Point& b) const
    {
        if ( this->x != b.x ) { return false; }
        if ( this->y != b.y ) { return false; }
        if ( this->z != b.z ) { return false; }
        return true;
    }
    bool Point::operator!=(const Point& b) const
    {
        if ( this->x == b.x ) { return false; }
        if ( this->y == b.y ) { return false; }
        if ( this->z == b.z ) { return false; }
        return true;
    }
    
    Point operator*(float scalar, const Point& b)
    {return Point(scalar * b.x, scalar * b.y, scalar * b.z);}
    
    Point operator*(const Point& a, float scalar)
    {return Point(scalar * a.x, scalar *a.y, scalar * a.z);}
    
    Point min(const Point& a, const Point& b)
    {return Point(a.x < b.x ? a.x : b.x,
                   a.y < b.y ? a.y : b.y,
                   a.z < b.z ? a.z : b.z);}
    
    Point max(const Point& a, const Point& b)
    {return Point(a.x > b.x ? a.x : b.x,
                  a.y > b.y ? a.y : b.y,
                  a.z > b.z ? a.z : b.z);}
    
    Point operator+(Point& a, const Point& b){
        return Point(a.x+b.x, a.y+b.y, a.z+b.z);
    }
    float Point::operator[](int i) const {
        Assert(i >= 0 && i <= 2);
        return (&x)[i];
    }
   Point Point::operator += (const Point& a) {
        x += a.x; y += a.y; z += a.z;
        return *this;
    }
    
}
