#include <rt/cameras/camera.h>
#include <core/vector.h>
#include <core/point.h>
#include <rt/cameras/orthographic.h>
#include <rt/ray.h>

namespace rt {
    OrthographicCamera::OrthographicCamera( const Point& center, const Vector& forward, const Vector& up, float scaleX, float scaleY ):center(center), forward(forward), up(up), scaleX(scaleX),scaleY(scaleY) {
        //Orhogonalization
        this->imgY = orthogonalize(this->up, this->forward);
        
        // Normalization
        this->forward = this->forward.normalize();
        this->imgY = this->imgY.normalize();
        
        //Create vectors x in the image plane
        this->imgX = cross(this->forward, this->imgY);
    }
    
    Ray OrthographicCamera::getPrimaryRay( float x, float y ) const {
        Point o = this->center + this->imgX * x * this->scaleX - this->imgY * y * scaleY; //The minus due to inverted y coordinates
        Ray r(o, this->forward);
        return r;
    }
    
//
//   
//   OrthographicCamera::OrthographicCamera(
//                       const Point& center,
//                       const Vector& forward,
//                       const Vector& up,
//                       float scaleX,
//                       float scaleY
//                              ): m_center(center){
//   
//        
//        Vector forward_axis = forward.normalize();
//        Vector right_axis = cross(forward_axis,up).normalize();
//        Vector up_axis = -cross(right_axis,forward_axis).normalize();
//        xright_axis = scaleX * right_axis;
//        yup_axis = scaleY * up_axis ;
//       
//    };
//    
//    Ray  OrthographicCamera::getPrimaryRay(float x, float y) const{
//        Ray ret;
//        ret.o = m_center + x * xright_axis + y * yup_axis ;
//        ret.d = forward_axis;
//        return ret;
//        
//    };
}
