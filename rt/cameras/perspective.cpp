#include <rt/cameras/camera.h>
#include <core/vector.h>
#include <core/point.h>
#include <rt/cameras/perspective.h>
#include <rt/ray.h>
namespace rt {
    PerspectiveCamera::PerspectiveCamera( const Point& center, const Vector& forward, const Vector& up, float verticalOpeningAngle, float horizontalOpeningAngle ):center(center), forward(forward), up(up),verticalOpeningAngle(verticalOpeningAngle), horizontalOpeningAngle(horizontalOpeningAngle) {
        
        this->tan_theta = tan(this->verticalOpeningAngle / 2.0f);
        this->tan_phi = tan(horizontalOpeningAngle / 2.0f);
        this->aspectRatio =  this->tan_theta/this->tan_phi;
        
        /* Orthogonalize vectors */
        this->forward = this->forward.normalize();
        this->imgX = cross(-this->up, this->forward).normalize();
        this->imgY = cross(this->forward, this->imgX).normalize();
    }
    
    /*  Find pixel center on the image plane and then compute vector d
     from the origin to pixel center. */
    Ray PerspectiveCamera::getPrimaryRay( float x, float y ) const {
        
        Vector d = this->forward + this->imgX * this->tan_theta * this->aspectRatio * x + this->imgY * this->tan_theta * y;
        d = d.normalize();
        Ray r (this->center, d);
        return r;
    }


    void PerspectiveCamera::setCameraGeometry( int width, int height ) {
        this->aspectRatio = (float) width / height;
    }
}
//}
//    PerspectiveCamera::PerspectiveCamera(
//                          const Point& center,
//                          const Vector& forward,
//                          const Vector& up,
//                          float verticalOpeningAngle,
//                          float horizonalOpeningAngle): m_center(center)
//    
//    {
//        forward_axis = forward.normalize();
//        right_axis = cross(forward_axis,up).normalize();
//        up_axis = -cross(right_axis,forward_axis).normalize();
//        
//        float aspect_ratio = tanf(horizonalOpeningAngle/2.f)/tanf(verticalOpeningAngle/2.f);
//        
//        direction = 1/tanf(horizonalOpeningAngle/2.f);
//    };
//    
//    Ray PerspectiveCamera:: getPrimaryRay(float x, float y) const{
//        Vector dir = x * right_axis + direction * forward_axis + aspect_ratio * up_axis * y;
//        dir = dir.normalize();
//        Ray rec;
//        rec.d = dir;
//        rec.o = m_center;
//        return rec;
//    };
//   
    
    
  
//}
