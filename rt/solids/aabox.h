#ifndef CG1RAYTRACER_SOLIDS_BOX_HEADER
#define CG1RAYTRACER_SOLIDS_BOX_HEADER

#include "solid.h"
#include <rt/bbox.h>

namespace rt {

class AABox : public Solid {
public:
    Point corner1;
    Point corner2;
    CoordMapper* texMapper;
    Material* material;
    Point centr;
    AABox() {}
    AABox(const Point& corner1, const Point& corner2, CoordMapper* texMapper, Material* material);

    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance=FLT_MAX) const;
    virtual Point sample() const;
    virtual float getArea() const;
    virtual Point get_midpoint() const;
    virtual void setMaterial(Material* m);
};

}

#endif
