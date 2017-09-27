#ifndef CG1RAYTRACER_SOLIDS_STRIANGLE_HEADER
#define CG1RAYTRACER_SOLIDS_STRIANGLE_HEADER

#include <rt/solids/solid.h>
#include <rt/solids/triangle.h>

namespace rt {

class SmoothTriangle : public Triangle {
public:
    SmoothTriangle() {}
    SmoothTriangle(Point vertices[3], Vector normals[3], CoordMapper* texMapper, Material* material);
    SmoothTriangle(const Point& v1, const Point& v2, const Point& v3, 
        const Vector& n1, const Vector& n2, const Vector& n3, 
        CoordMapper* texMapper, Material* material);

    virtual Intersection intersect(const Ray& ray, float previousBestDistance=FLT_MAX) const;
     virtual Point get_midpoint() const;
private:
    Point v1;
    Point v2;
    Point v3;
    Vector n1;
    Vector n2;
    Vector n3;
    CoordMapper *texMapper;
    Material *material;
    
    /* Used for computation of barycentric coordinates */
    Vector n;
    Vector span1;
    Vector span2;
    float span1_lensqr;
    float span2_lensqr;
    float dSpan1Span2;
    float cram_denom;
};

}

#endif
