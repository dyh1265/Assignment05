#include "triangle.h"
#include <rt/intersection.h>
#include <rt/bbox.h>
#include <core/vector.h>
#include <core/assert.h>

namespace rt {
    
    Triangle::Triangle(
                       Point vertices[3],
                       CoordMapper* texMapper,
                       Material* material): v1(vertices[0]), v2(vertices[1]), v3(vertices[2]), Solid(texMapper, material) {
        this->n = cross((v2 - v1),(v3 - v1)).normalize();
        this->span1 = v2 - v1;
        this->span2 = v3 - v1;
        this->dSpan1Span2 = dot(span1, span2);
        this->span1_lensqr = span1.lensqr();
        this->span2_lensqr = span2.lensqr();
        this->cram_denom = this->span1_lensqr * this->span2_lensqr
        - this->dSpan1Span2 * this->dSpan1Span2;
    }
    
    Triangle::Triangle(
                       const Point& v1,
                       const Point& v2,
                       const Point& v3,
                       CoordMapper* texMapper,
                       Material* material): v1(v1), v2(v2), v3(v3), Solid(texMapper, material)  {
        this->n = cross((v2 - v1),(v3 - v1)).normalize();
        this->span1 = v2 - v1;
        this->span2 = v3 - v1;
        this->dSpan1Span2 = dot(span1, span2);
        this->span1_lensqr = span1.lensqr();
        this->span2_lensqr = span2.lensqr();
        this->cram_denom = this->span1_lensqr * this->span2_lensqr
        - this->dSpan1Span2 * this->dSpan1Span2;
    }
    
    
    Intersection Triangle::intersect(const Ray& ray, float previousBestDistance) const {
        
        /* Test first if ray hits the plane in which the triangle is embedded */
        float t = dot(this->n, (this->v1 - ray.o)) / dot(ray.d, this->n);
        if (t < 0 || t > previousBestDistance) {
            return Intersection();
        }
        
        /* Use Cramer's rule to compute barycentric coordinates */
        /* Source: http://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates */
        Point p = ray.o + t * ray.d;
        Vector vec = p - v1;
        float dVecSpan1 = dot(vec, this->span1);
        float dVecSpan2 = dot(vec, this->span2);
        
        float b1 = (this->span2_lensqr * dVecSpan1 - this->dSpan1Span2 * dVecSpan2) / this->cram_denom;
        if (b1 > 1 || b1 < 0) {
            return Intersection();
        }
        
        float b2 = (this->span1_lensqr * dVecSpan2 - this->dSpan1Span2 * dVecSpan1) / this->cram_denom;
        if (b2 > 1 || b2 < 0) {
            return Intersection();
        }
        
        float b3 = 1.0 - b1 - b2;
        if (b3 > 1 || b3 < 0) {
            return Intersection();
        }
        
        /* If normal points downward wrt to camera, revert normal */
        float n_dir = dot(ray.d, this->n);
        
        if (n_dir > 0) {
            return Intersection(t, ray, this, Vector(0,0,0) - this->n, Point(b1, b2, b3));		
        }
        
        return Intersection(t, ray, this, this->n, Point(b1, b2, b3));
    }
    
    Point Triangle::sample() const {
        NOT_IMPLEMENTED;
    }
    float Triangle::getArea() const{
        float parallArea = cross(this->span1, this->span2).length();
        return parallArea/2.f ;
    }
    BBox Triangle::getBounds() const{
       return Union(BBox(v1, v2), v3);
    }
    // Simple center of gravity
    Point Triangle::get_midpoint() const{
        return Point((v1.x+v2.x+v3.x)/3.f, (v1.y+v2.y+v3.y)/3.f,(v1.z+v2.z+v3.z)/3.f);
        
    }
}
