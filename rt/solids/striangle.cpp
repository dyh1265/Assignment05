#include <rt/solids/striangle.h>
#include <rt/intersection.h>
#include <core/assert.h>

namespace rt {
    
    SmoothTriangle::SmoothTriangle(
                                   Point vertices[3],
                                   Vector normals[3],
                                   CoordMapper* texMapper,
                                   Material* material ):texMapper(texMapper),material(material) {
        this->v1 = vertices[0];
        this->v2 = vertices[1];
        this->v3 = vertices[2];
        this->n1 = normals[0];
        this->n2 = normals[1];
        this->n3 = normals[2];
        
        this->n = cross((this->v2 - this->v1),(this->v3 - this->v1)).normalize();
        this->span1 = this->v2 - this->v1;
        this->span2 = this->v3 - this->v1;
        this->dSpan1Span2 = dot(span1, span2);
        this->span1_lensqr = span1.lensqr();
        this->span2_lensqr = span2.lensqr();
        this->cram_denom = this->span1_lensqr * this->span2_lensqr
        - this->dSpan1Span2 * this->dSpan1Span2;
    }
    
    SmoothTriangle::SmoothTriangle(
                                   const Point& v1,
                                   const Point& v2,
                                   const Point& v3,
                                   const Vector& n1,
                                   const Vector& n2,
                                   const Vector& n3,
                                   CoordMapper* texMapper,
                                   Material* material):v1(v1),v2(v2),v3(v3), n1(n1),n2(n2),n3(n3),texMapper(texMapper), material(material) {

        
        this->n = cross((v2 - v1),(v3 - v1)).normalize();
        this->span1 = this->v2 - this->v1;
        this->span2 = this->v3 - this->v1;
        this->dSpan1Span2 = dot(span1, span2);
        this->span1_lensqr = span1.lensqr();
        this->span2_lensqr = span2.lensqr();
        this->cram_denom = this->span1_lensqr * this->span2_lensqr
        - this->dSpan1Span2 * this->dSpan1Span2;
        
    }
    
    Intersection SmoothTriangle::intersect(const Ray& ray, float previousBestDistance) const {
        
        /* Test first if ray hits the plane in which the triangle is embedded */
        float t = dot(this->n, (this->v1 - ray.o)) / dot(ray.d, this->n);
        if (t < 0 || t > previousBestDistance) {
            return Intersection::failure();
        }
        
        /* Use Cramer's rule to compute barycentric coordinates */
        /* Source: http://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates */
        Point p = ray.o + t * ray.d;
        Vector vec = p - this->v1;
        float dVecSpan1 = dot(vec, this->span1);
        float dVecSpan2 = dot(vec, this->span2);
        
        float b1 = (this->span2_lensqr * dVecSpan1 - this->dSpan1Span2 * dVecSpan2) / this->cram_denom;
        if (b1 > 1.0f || b1 < 0.0f) {
            return Intersection::failure();
        }
        
        float b2 = (this->span1_lensqr * dVecSpan2 - this->dSpan1Span2 * dVecSpan1) / this->cram_denom;
        if (b2 > 1.0f || b2 < 0.0f) {
            return Intersection::failure();
        }
        
        float b3 = 1.0f - b1 - b2;
        if (b3 > 1.0f || b3 < 0.0f) {
            return Intersection::failure();
        }
        
        /* If normal points downward wrt to camera, revert normal */
        float n_dir = dot(ray.d, this->n);
        
        /* Now interpolate normal */
        Vector interp_n = (b3 * this->n1 + b1 * this->n2 + b2 * this->n3).normalize();
        
        if (n_dir > 0) {
            return Intersection(t, ray, this, Vector(0,0,0) - interp_n, Point(b1, b2, b3));		
        }
        
        return Intersection(t, ray, this, interp_n, Point(b1, b2, b3));
    }
    Point SmoothTriangle:: get_midpoint() const{
        NOT_IMPLEMENTED;
    }
    
    
    
}
