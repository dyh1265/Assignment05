
#include "intersection.h"
namespace rt {
    Intersection:: Intersection(float distance, const Ray& ray, const Solid* solid, const Vector& normal, const Point& uv):distance(distance),ray(ray),solid(solid),n(normal),uv(uv){}
    
    Point Intersection::hitPoint() const{
        return ray.o + distance * ray.d;
    }
    Vector Intersection::normal() const {
        return this->n;
    }
    Point Intersection::local() const {
        return this->uv;
    }
    Intersection::operator bool() {
        if (this->distance == -9) {
            return false;
        }
        return true;
    }
     Intersection Intersection::failure() {
     return Intersection();
     }
}
