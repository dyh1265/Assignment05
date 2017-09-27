#ifndef CG1RAYTRACER_CAMERAS_ORTHOGRAPHIC_HEADER
#define CG1RAYTRACER_CAMERAS_ORTHOGRAPHIC_HEADER

#include <rt/cameras/camera.h>
#include <core/vector.h>
#include <core/point.h>

namespace rt {

class OrthographicCamera : public Camera {
   
   
public:
//    Vector xright_axis;
//    Vector yup_axis;
//    Point m_center;
//    Vector forward_axis;
    
    Point center;
    Vector forward;
    Vector up;
    Vector imgX;
    Vector imgY;
    float scaleX;
    float scaleY;
    OrthographicCamera(
        const Point& center,
        const Vector& forward,
        const Vector& up,
        float scaleX,
        float scaleY
        ); 

    virtual Ray getPrimaryRay(float x, float y) const;
};

}


#endif
