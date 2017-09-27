#ifndef CG1RAYTRACER_CAMERAS_PERSPECTIVE_HEADER
#define CG1RAYTRACER_CAMERAS_PERSPECTIVE_HEADER

#include <rt/cameras/camera.h>
#include <core/vector.h>
#include <core/point.h>

namespace rt {

class PerspectiveCamera : public Camera {
   
public:
//    Vector forward_axis;
//    Vector right_axis;
//    Vector up_axis;
//    float direction;
//    float aspect_ratio;
//    Point m_center;
    Point center;
    Vector forward;
    Vector up;
    Vector imgX;
    Vector imgY;
    float tan_theta;
    float tan_phi;
    float verticalOpeningAngle;
    float horizontalOpeningAngle;
    float aspectRatio;
    
    PerspectiveCamera(
        const Point& center,
        const Vector& forward,
        const Vector& up,
        float verticalOpeningAngle,
        float horizonalOpeningAngle
        );

    virtual Ray getPrimaryRay(float x, float y) const;
    virtual void setCameraGeometry(int width, int height);
};

}


#endif
