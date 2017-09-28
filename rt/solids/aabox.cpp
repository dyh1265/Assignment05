#include <rt/solids/aabox.h>
#include <rt/intersection.h>
#include <rt/bbox.h>
#include <core/assert.h>
#include <rt/ray.h>
#include <core/point.h>
namespace  rt{
    AABox::AABox(const Point& corner1, const Point& corner2, CoordMapper* texMapper, Material* material):corner1(corner1), corner2(corner2), Solid(texMapper,material){}
    
    Intersection AABox::intersect(const Ray& ray,float previousBestDistance) const{
        
        Vector invdir =Vector(1.f/ray.d.x,1.f/ray.d.y,1.f/ray.d.z);
        
        /* Source: http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter3.htm */
        float xmin, xmax, ymin, ymax, zmin, zmax;
        float sign;
        
        /* Retrieve min/max values for ray origin */
        if ((corner1.x - ray.o.x) * (corner1.x - ray.o.x) <
            (corner2.x - ray.o.x) * (corner2.x - ray.o.x)) {
            xmin = corner1.x;
            xmax = corner2.x;
        }
        else {
            xmin = corner2.x;
            xmax = corner1.x;
        }
        if ((corner1.y - ray.o.y) * (corner1.y - ray.o.y) <
            (corner2.y - ray.o.y) * (corner2.y - ray.o.y)) {
            ymin = corner1.y;
            ymax = corner2.y;
        }
        else {
            ymin = corner2.y;
            ymax = corner1.y;
        }
        if ((corner1.z - ray.o.z) * (corner1.z - ray.o.z) <
            (corner2.z - ray.o.z) * (corner2.z - ray.o.z)) {
            zmin = corner1.z;
            zmax = corner2.z;
        }
        else {
            zmin = corner2.z;
            zmax = corner1.z;
        }
        /* Compute intersections with slabs */
        float txnear = (xmin - ray.o.x) * invdir.x;
        float tynear = (ymin - ray.o.y) * invdir.y;
        float tmin = txnear > tynear ? txnear : tynear;
        float tznear = (zmin - ray.o.z) * invdir.z;
        tmin = tznear > tmin ? tznear : tmin;
        
        float txfar = (xmax - ray.o.x)  * invdir.x;
        float tyfar = (ymax - ray.o.y) * invdir.y;
        float tmax = txfar < tyfar ? txfar : tyfar;
        float tzfar = (zmax - ray.o.z) * invdir.z;
        tmax = tzfar < tmax ? tzfar : tmax;
        
        if (tmax < tmin || tmin > previousBestDistance || tmin < 0) {
            return Intersection::failure();
        }
        
        /* Account for direction of ray */
        if (tmin + 1e-5 > txnear && tmin - 1e-5 < txnear) {
            sign = ray.d.x < 0. ? 1 : -1;
            return Intersection(tmin, ray, this, Vector(sign,0,0), Point());
        }
        if (tmin + 1e-5 > tynear && tmin - 1e-5 < tynear) {
            sign = ray.d.y < 0. ? 1 : -1;
            return Intersection(tmin, ray, this, Vector(0,sign,0), Point());
        }
        if (tmin + 1e-5 > tznear && tmin - 1e-5 < tznear) {
            sign = ray.d.z < 0. ? 1. : -1.;
            return Intersection(tmin, ray, this, Vector(0,0,sign), Point());
        }
        return Intersection::failure();
        
    }
    BBox AABox::getBounds() const{
        return BBox(this->corner1,this->corner2);
    }
    Point AABox::sample() const{
        NOT_IMPLEMENTED;
        return Point();
    }
    float AABox::getArea() const {
        Vector d = this->corner2 - this->corner1;
        return 2.f * (d.x * d.y + d.x * d.z + d.y * d.z);
    }
    Point AABox::get_midpoint() const {
     return Point(0.5 * (corner1.x + corner2.x),
                                      0.5 * (corner1.y + corner2.y),
                                      0.5 * (corner1.z + corner2.z));
    }
    void AABox::setMaterial(Material* m){
        this->material = m;
    }
}
