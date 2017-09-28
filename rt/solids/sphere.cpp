#include <core/assert.h>
#include <core/point.h>
#include "sphere.h"
#include <rt/bbox.h>
#include <rt/intersection.h>
#include <rt/primitive.h>
#include <cmath>

namespace rt {
    
    Sphere::Sphere(
                   const Point& center,
                   float radius,
                   CoordMapper* texMapper,
                   Material* material): center(center), radius(radius), Solid(texMapper, material), radsqr(radius * radius){
    }

    Intersection Sphere::intersect( const Ray& ray, float previousBestDistance ) const {
        
        Vector oc = ray.o - this->center;
        float pDivBy2 = dot(ray.d, oc);
        float q = oc.lensqr() - this->radsqr;
        
        /* If ray does not intersect sphere */
        if (pDivBy2 * pDivBy2 - q < 0) {
            return Intersection::failure();
        }
        
        float root = sqrt(pDivBy2 * pDivBy2 - q);
        float t1 = root - pDivBy2;
        float t2 = -pDivBy2 - root;
        
        if ((t1 < t2 && t1 < previousBestDistance && t1 > 0) || (t1 > 0.0f && t2 < 0.0f && t1 < previousBestDistance)) {
            Vector normal = (ray.o + t1 * ray.d - this->center).normalize();
            Point local = ray.getPoint(t1) - Vector(this->center.x,
                                                    this->center.y, this->center.z);
            return Intersection(t1, ray, this, normal, local);
        }
        if ((t2 < t1 && t2 < previousBestDistance && t2 > 0) || (t2 > 0.0f && t1 < 0.0f && t2 < previousBestDistance)) {
            Vector normal = (ray.o + t2 * ray.d - this->center).normalize();
            Point local = ray.getPoint(t2) - Vector(this->center.x, 
                                                    this->center.y, this->center.z);
            return Intersection(t2, ray, this, normal, local);
        }
        
        return Intersection::failure();
    }
    
    float Sphere::getArea() const {
        return this->radius * this->radius * M_PI * 4;
    }
    void Sphere::setMaterial(Material* m){
        this->material = m;
    }
    Point Sphere::sample() const{
        NOT_IMPLEMENTED;
        return Point();
    }
    BBox Sphere::getBounds() const{
        return BBox(Point(center.x - radius,center.y -radius,center.z - radius), Point (center.x + radius, center.y + radius, center.z + radius));
    }
    Point  Sphere::get_midpoint() const{
        return  this->center;
    }

    
}
