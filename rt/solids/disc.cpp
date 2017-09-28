#include <rt/intersection.h>
#include <rt/bbox.h>
#include <core/assert.h>
#include "disc.h"
#include <core/point.h>
namespace rt {
    Disc::Disc(const Point& center, const Vector& normal, float radius, CoordMapper* texMapper, Material* material):center(center), normal(normal), radius(radius),Solid(texMapper,material){
        this->radsqr = radius * radius;
    }
    Intersection Disc::intersect( const Ray& ray, float previousBestDistance ) const {
        
        /* Test first if ray hits the plane in which the disc is embedded */
        float t = dot(this->normal, (this->center - ray.o)) / dot(ray.d, this->normal);
        if (t < 0 || t > previousBestDistance) {
            return Intersection::failure();
        }
        
        /* Test if ray hits within the radius of the circle */
        float distsqr = ((ray.o + t * ray.d) - this->center).lensqr();
        
        //TODO what if < 1
        if (distsqr > this->radsqr) {
            return Intersection::failure();
        }
        Point local = ray.getPoint(t) - Vector(this->center.x,
                                               this->center.y, this->center.z);	
        
        return Intersection(t, ray, this, this->normal, local);
        
    }
    float Disc::getArea() const {
        return 3.14f * this->radius * this->radius;
    }
    void Disc::setMaterial(Material* m){
        this->material = m;
    }
    Point Disc::sample() const{
        NOT_IMPLEMENTED;
        return Point();
    }
    BBox Disc::getBounds() const {
      return BBox(Point(center.x - radius,center.y -radius,center.z - radius), Point (center.x + radius, center.y + radius, center.z + radius));
    }
    Point  Disc::get_midpoint() const{
        return center;
    }
}
