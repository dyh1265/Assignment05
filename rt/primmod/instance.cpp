#include <rt/primmod/instance.h>
#include <core/assert.h>
#include <core/matrix.h>
#include <core/float4.h>
#include <core/point.h>
#include <rt/bbox.h>
#include <rt/intersection.h>
namespace rt {
    Instance::Instance(Primitive* content):primP(content){}
    
    Primitive* Instance::content() const{
        return this->primP;
    }
    
    void Instance::reset(){
        this->transMatr = Matrix::identity();
        this->transMatrInv = Matrix::identity();
    }
    
    void Instance::translate(const Vector& t){
        Matrix id = Matrix::identity();
        id.r4 = Float4(t.x,t.y,t.z,1);
        id = id.transpose();
        this->transMatr = product(id,this->transMatr);
        this->transMatrInv = this->transMatr.invert();
    }
    
    void Instance::scale(float scale){
        Matrix id = Matrix::identity();
        id = id * scale;
        this->transMatr = product(id,this->transMatr);
        this->transMatrInv = this->transMatr.invert();
    }
    void Instance::scale(const Vector& scale){
        Matrix c = Matrix(Float4(scale.x,0,0,0), Float4(0,scale.y,0,0), Float4(0,0,scale.z,0), Float4(0,0,0,1));
        this->transMatr = product(c,this->transMatr);
        this->transMatrInv = this->transMatr.invert();
    }
    void Instance::rotate(const Vector& axis, float angle){
        Vector u = axis.normalize();
        Matrix m;
        m[0][0] = cos(angle) + u.x * u.x * (1-cos(angle));
        m[0][1] = u.x * u.y * (1-cos(angle)) - u.z * sin(angle);
        m[0][2] =  u.x * u.z * (1-cos(angle)) + u.y * sin(angle);
        m[0][3] = 0;
        
        m[1][0] = u.x * u.y * (1-cos(angle)) + u.z * sin(angle);
        m[1][1] = cos(angle) + u.y * u.y * (1-cos(angle));
        m[1][2] = u.y * u.z * (1-cos(angle)) - u.x * sin(angle);
        m[1][3] = 0;
        
        m[2][0] =  u.x * u.z * (1-cos(angle)) - u.y * sin(angle);
        m[2][1] = u.z * u.y * (1-cos(angle)) + u.x*sin(angle);
        m[2][2] = cos(angle) + u.z * u.z * (1-cos(angle));
        m[2][3] = 0;
        
        m.r4 = Float4(0,0,0,1);
        
        this->transMatr = product(m, this->transMatr);
        this->transMatrInv = this->transMatr.invert();
    }
    BBox Instance::getBounds() const{
        /* Get bounding box of referenced object */
        BBox bb = this->content()->getBounds();
        
        /* Transform bounding box */
        Point tmp_min = this->transMatr * bb.pMin;
        Point tmp_max = this->transMatr * bb.pMax;
        return BBox(tmp_min,tmp_max);
    }
    
    Intersection Instance::intersect(const Ray& ray, float previousBestDistance) const{
     /*Ray transformation to hit parent object*/
        Intersection inter;
        Ray transfRay;
        transfRay.o = this->transMatrInv * ray.o;
        transfRay.d = (this->transMatrInv * ray.d).normalize();
    /*Intersect ray with parent object*/
        if (previousBestDistance < FLT_MAX){
            Point hitPobj = ray.getPoint(previousBestDistance);
            Point trhitPobj = this->transMatrInv * hitPobj;
            float dist = (trhitPobj- transfRay.o).length();
            inter = this->content()->intersect(ray,dist);
        }
        else {
            inter = this->content()->intersect(transfRay,previousBestDistance);
        }
        if (inter.distance < 0) {
             return Intersection::failure();
        }
    /* Transform hit point by transformation matrix */
        Point hitPoint = transfRay.getPoint(inter.distance);
        hitPoint = this->transMatr * hitPoint;
        inter.distance = (hitPoint - ray.o).length();
    /* In case transformed value is < 0 */
        if (inter.distance < 0 || inter.distance > previousBestDistance - epsilon) {
            return Intersection::failure();
        }
    /* Transform normal by transpose of the inverse */
        inter.n = (this->transMatrInv.transpose() * inter.n).normalize();
        inter.uv = this->transMatr * inter.uv;
        inter.ray = ray;
        
        return inter;
    }
    
    void Instance::setMaterial(Material* m){
        NOT_IMPLEMENTED;
    }
    
    void Instance::setCoordMapper(CoordMapper* cm){
        NOT_IMPLEMENTED;
    }
    Point Instance::get_midpoint() const {
        NOT_IMPLEMENTED;
    }
    
}

