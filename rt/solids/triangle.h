#ifndef CG1RAYTRACER_SOLIDS_TRIANGLE_HEADER
#define CG1RAYTRACER_SOLIDS_TRIANGLE_HEADER

#include <rt/solids/solid.h>
#include <core/point.h>

namespace rt {

class Triangle : public Solid {
public:
    Point v1;
    Point v2;
    Point v3;
    Vector n;
    Point centr;
    /* Used for computation of barycentric coordinates */
    Vector span1;
    Vector span2;
    float span1_lensqr;
    float span2_lensqr;
    float dSpan1Span2;
    float cram_denom;
    
    CoordMapper *texMapper;
    Material *material;
    
    Triangle() {}
    Triangle(Point vertices[3], CoordMapper* texMapper, Material* material);
    Triangle(const Point& v1, const Point& v2, const Point& v3, CoordMapper* texMapper, Material* material);

    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance=FLT_MAX) const;
	virtual Point sample() const;
    virtual float getArea() const;
    virtual Point get_midpoint() const;
};

}

#endif
