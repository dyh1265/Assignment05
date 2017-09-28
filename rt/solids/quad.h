#ifndef CG1RAYTRACER_SOLIDS_QUAD_HEADER
#define CG1RAYTRACER_SOLIDS_QUAD_HEADER

#include <rt/solids/solid.h>
#include <core/point.h>

namespace rt {

class Quad : public Solid {
public:
    Point v1;
    Vector span1;
    Vector span2;
    Vector normal;
    CoordMapper *texMapper;
    Material *material;
    Quad() {}
    Quad(const Point& v1, const Vector& span1, const Vector& span2, CoordMapper* texMapper, Material* material);

    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance=FLT_MAX) const;
	virtual Point sample() const;
    virtual float getArea() const;
    virtual Point get_midpoint() const;
    virtual void setMaterial(Material* m);
};

}

#endif
