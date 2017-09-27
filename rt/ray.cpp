#include "ray.h"
#include <core/vector.h>
#include <core/point.h>
namespace rt {
    Ray::Ray(const Point& o, const Vector& d):o(o),d(d){
        inv_dir = Vector(1/d.x, 1/d.y, 1/d.z);
        sign[0] = (inv_dir.x < 0);
        sign[1] = (inv_dir.y < 0);
        sign[2] = (inv_dir.z < 0);
        invdir=inv_dir;
    }
    
    Point Ray::getPoint(float distance) const{
        Point p;
        Vector w = distance * this->d;
        p = this->o + w;
        return p;
       
    }
    
}
