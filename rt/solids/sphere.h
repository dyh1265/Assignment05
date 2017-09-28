#ifndef CG1RAYTRACER_SOLIDS_SPHERE_HEADER
#define CG1RAYTRACER_SOLIDS_SPHERE_HEADER

#include <rt/solids/solid.h>
#include <core/point.h>

namespace rt {

class Sphere : public Solid {
public:
    Point center;
    float radius;
    float radsqr;
    CoordMapper *texMapper;
    Sphere() {}
    Sphere(const Point& center, float radius, CoordMapper* texMapper, Material* material);

    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance=FLT_MAX) const;
	virtual Point sample() const;
    virtual float getArea() const;
    virtual Point get_midpoint() const;
    virtual void setMaterial(Material* m);
};

}


#endif
