#include <core/point.h>
#include <core/vector.h>
#include <rt/intersection.h>
#include "infiniteplane.h"
#include <core/vector.h>
#include <rt/bbox.h>
#include <core/assert.h>
namespace rt{
    
InfinitePlane::InfinitePlane(const Point& origin,
                                 const Vector& normal,
                                 CoordMapper* texMapper,
                                Material* material): origin(origin), normal(normal.normalize()), Solid(texMapper,material){}
        
Intersection InfinitePlane::intersect(const Ray& ray,float previousBestDistance) const
    {
        /* Test first if ray hits the plane in which the disc is embedded 
         t = (<a,n> - <o,n>)/<d,n>
         */
        float t = dot(this->normal, (this->origin - ray.o)) / dot(ray.d, this->normal);
        if (t < 0 || t > previousBestDistance) {
            return Intersection::failure();
        }
        
        /* If normal points downward */
        float n_dir = dot(ray.d, this->normal);
        
        Point local = ray.getPoint(t);
        
        if (n_dir > 0) {
            return Intersection(t, ray, this, Vector(0,0,0) - this->normal, local);
        }
        
        return Intersection(t, ray, this, this->normal, local);
    }
    
    BBox InfinitePlane::getBounds() const{
        if (this->normal.x>0) return BBox(Point(0,-FLT_MAX+epsilon,-FLT_MAX+epsilon),Point(0,FLT_MAX-epsilon,FLT_MAX-epsilon));
        if (this->normal.y>0) return BBox(Point(-FLT_MAX+epsilon,0,-FLT_MAX+epsilon),Point(FLT_MAX-epsilon,0,FLT_MAX-epsilon));
        return BBox(Point(-FLT_MAX+epsilon,-FLT_MAX+epsilon,0),Point(FLT_MAX-epsilon,FLT_MAX-epsilon,0));
    }
    Point InfinitePlane::sample() const{
        NOT_IMPLEMENTED;
        return Point();
    }
    float InfinitePlane::getArea() const{
        return FLT_MAX;
    }
    Point InfinitePlane::get_midpoint() const{
        NOT_IMPLEMENTED;
    }
}
