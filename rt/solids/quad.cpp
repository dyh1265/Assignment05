#include "quad.h"
#include "triangle.h"
#include <rt/intersection.h>
#include <rt/bbox.h>
#include <core/assert.h>
#include <core/point.h>
#include <core/vector.h>
namespace rt {
    
    Quad::Quad(
               const Point& v1,
               const Vector& span1,
               const Vector& span2,
               CoordMapper* texMapper,
               Material* material): v1(v1), span1(span1), span2(span2), Solid(texMapper, material) {
        
        this->normal = cross(span1, span2).normalize();
    }
    
    
    /* 	Parallelogram consists of two triangles.
     If one triangle gets hit, return
     intersection of that hit */
    Intersection Quad::intersect(const Ray& ray, float previousBestDistance) const {
        
        /* Test intersection of plane */
        float t = dot(this->normal, (this->v1 - ray.o)) / dot(ray.d, this->normal);
        if (t < 0 || t > previousBestDistance) {
            return Intersection::failure();
        }
        
        /* Compute vertices */
        Point p1 = this->v1 + span1;
        Point p2 = this->v1 + span2;
        Point p3 = p1 + span2;
        
        /* Compute triangles */
        Triangle lower(this->v1, p1, p2, this->texMapper, this->material);
        Triangle upper(p1, p2, p3, this->texMapper, this->material);
        
        /* Intersect ray with both triangles */
        Intersection lower_tr = lower.intersect(ray, previousBestDistance);
        Intersection upper_tr = upper.intersect(ray, previousBestDistance);
        
        if (lower_tr.distance > 0 && lower_tr.distance < previousBestDistance) {
            lower_tr.solid = this;
            return lower_tr;
        }
        else if (upper_tr.distance > 0 && upper_tr.distance < previousBestDistance) {
            upper_tr.solid = this;
            return upper_tr;
        }
        
        return Intersection::failure();
    }
    
    float Quad::getArea() const {
        return cross(this->span1, this->span2).length();
    }
    BBox Quad::getBounds() const{
       	
        Point p1 = this->v1 + span1;
        Point p2 = this->v1 + span2;
        Point p3 = p1 + span2;
        
        BBox box = BBox(this->v1, p1);
        box.extend(p2);
        box.extend(p3);
        
        return box;
    }
    Point Quad::sample() const{
        NOT_IMPLEMENTED;
        return Point();
    }
    Point Quad:: get_midpoint() const{
        NOT_IMPLEMENTED;
    }
}
