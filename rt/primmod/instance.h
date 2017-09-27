#ifndef CG1RAYTRACER_PRIMMOD_INSTANCE_HEADER
#define CG1RAYTRACER_PRIMMOD_INSTANCE_HEADER

#include <rt/primitive.h>
#include <core/matrix.h>
namespace rt {

class Instance : public Primitive {

public:
    Matrix transMatr = Matrix::identity();
    Matrix transMatrInv = Matrix::identity();
    Instance(Primitive* content);
    Primitive* content() const;

    void reset(); //reset transformation back to identity
    void translate(const Vector& t);
    void rotate(const Vector& axis, float angle);
    void scale(float scale);
    void scale(const Vector& scale);
    
    virtual Point get_midpoint() const;
    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance=FLT_MAX) const;
    virtual void setMaterial(Material* m);
    virtual void setCoordMapper(CoordMapper* cm);
private:
    Primitive* primP;

};

}

#endif
