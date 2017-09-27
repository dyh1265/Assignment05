#include <core/point.h>
#include <core/vector.h>
#include <cmath>
#include "float4.h"
#include "assert.h"
namespace rt {
    Vector::Vector(const Float4& f4){
        Assert(f4.w == 0 );
        x = f4.x / f4.w;
        y = f4.y / f4.w;
        z = f4.z / f4.w;
    }
    Vector Vector::operator+(const Vector& b) const
    { return Vector(this->x + b.x, this->y + b.y, this->z + b.z);}
    
    Vector Vector::operator-(const Vector& b) const
    { return Vector(this->x - b.x, this->y - b.y, this->z - b.z);}
    
    Vector Vector::normalize() const
    {return *this / this->length();}
    
    Vector Vector::operator-() const
    {return Vector(0 - this->x, 0 - this->y, 0 - this->z);}
    
    float Vector::lensqr() const
    { return sqr(this->x) + sqr(this->y) + sqr(this->z);}

    float Vector::length() const
    { return sqrt(sqr(this->x) + sqr(this->y) + sqr(this->z));}
    
    
    bool Vector::operator!=(const Vector& b) const
    {
        if ( this->x == b.x ) { return false; }
        if ( this->y == b.y ) { return false; }
        if ( this->z == b.z ) { return false; }
        return true;
    }
    
    bool Vector::operator==(const Vector& b) const
    {
        if ( this->x != b.x ) { return false; }
        if ( this->y != b.y ) { return false; }
        if ( this->z != b.z ) { return false; }
        return true;
    }
    
    Vector operator*(float scalar, const Vector& b)
    {return Vector(scalar * b.x, scalar * b.y, scalar * b.z);}
    
    Vector operator*(const Vector& a, float scalar)
    {return Vector(scalar * a.x, scalar * a.y, scalar * a.z);}
    
    Vector operator/(const Vector& a, float scalar)
    {return Vector(a.x / scalar, a.y / scalar, a.z / scalar);}
    
    Vector operator/( float scalar,const Vector& a)
    {return Vector(scalar /a.x, scalar/a.y  , scalar/a.z);}
    
    Vector cross(const Vector& a, const Vector& b)
    {return Vector(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);}
    
    float dot(const Vector& a, const Vector& b)
    {return (a.x * b.x + a.y * b.y + a.z * b.z);}
    
    Vector min(const Vector& a, const Vector& b)
    {return Vector(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));}
    
    Vector max(const Vector& a, const Vector& b)
    {return Vector(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));}
    
    Point operator+(const Point& a, const Vector& b)
    { return Point(a.x + b.x, a.y + b.y, a.z + b.z);}
    
    Point operator+(const Vector& a, const Point& b)
    { return Point(a.x + b.x, a.y + b.y, a.z + b.z);}
    
    Point operator-(const Point& a, const Vector& b)
    { return Point(a.x - b.x, a.y - b.y, a.z - b.z);}
    
    /* Orthogonalize vector A with respect to vector B using Gram-Schmidt */
    Vector orthogonalize( const Vector&a, const Vector& b ) {
        Vector c = a;
        if (dot(a, b) != 0) {
            c = a - (dot(a, b) / b.lensqr()) * b;
        }
        
        return c;
    }
    
}
