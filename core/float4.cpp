#include "float4.h"
#include "point.h"
#include "vector.h"
#include "assert.h"
namespace rt{
    Float4::Float4(float x, float y, float z, float w):x(x),y(y),z(z),w(w){};
    
    Float4::Float4(const Point& p){
        this->x=p.x;
        this->y=p.y;
        this->z=p.z;
        this->w=1;
    }
    
    Float4::Float4(const Vector& v){
        this->x=v.x;
        this->y=v.y;
        this->z=v.z;
        this->w=0;
    }
    
    float& Float4::operator[](int idx){
        Assert(idx>=0 && idx<=3);
        return (&x)[idx];
    }
    
    float Float4::operator[](int idx) const{
        Assert(idx>=0 && idx<=3);
        return (&x)[idx];
    }
    
    Float4 Float4::operator+(const Float4& b) const{
        return Float4(this->x+b.x, this->y+b.y, this->z+b.z, this->w+b.w);
    }
    
    Float4 Float4::operator-(const Float4& b) const{
        return Float4(this->x-b.x, this->y-b.y, this->z-b.z, this->w-b.w);
    }
    
    Float4 Float4::operator*(const Float4& b) const{
        return Float4(x*b.x, y*b.y, z*b.z, w*b.w);
    }
    
    Float4 Float4::operator/(const Float4& b) const{
       return Float4(this->x/b.x, this->y/b.y, this->z/b.z, this->w/b.w);
    }
    
    Float4 Float4::operator-() const{
        return Float4(-this->x, -this->y ,-this->z, -this->w);
    }
    
    bool Float4::operator==(const Float4& b) const{
        if ( this->x != b.x ) { return false; }
        if ( this->y != b.y ) { return false; }
        if ( this->z != b.z ) { return false; }
        if ( this->w != b.w ) { return false; }
        return true;
    }
    bool Float4::operator!=(const Float4& b) const{
        if ( this->x == b.x ) { return false; }
        if ( this->y == b.y ) { return false; }
        if ( this->z == b.z ) { return false; }
        if ( this->w == b.w ) { return false; }
        return true;
    }
    
    Float4 operator*(float scalar, const Float4& b){
        return Float4(scalar*b.x, scalar*b.y, scalar*b.z, scalar*b.w);
    }
    Float4 operator*(const Float4& a, float scalar){
               return Float4(a.x*scalar, a.y*scalar, a.z*scalar, a.w*scalar);
    }
    Float4 operator/(const Float4& a, float scalar){
        return Float4(a.x/scalar, a.y/scalar, a.z/scalar, a.w/scalar);
    }
    
    float dot(const Float4& a, const Float4& b){
        return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
    }
    
    Float4 min(const Float4& a, const Float4& b){
        Float4 c;
        a.x<b.x?c.x=a.x:b.x;
        a.y<b.y?c.y=a.y:b.y;
        a.z<b.z?c.z=a.z:b.z;
        a.w<b.w?c.w=a.w:b.w;
        return c;
    }

    Float4 max(const Float4& a, const Float4& b){
        Float4 c;
        a.x>b.x?c.x=a.x:b.x;
        a.y>b.y?c.y=a.y:b.y;
        a.z>b.z?c.z=a.z:b.z;
        a.w>b.w?c.w=a.w:b.w;
        return c;
    }
}
