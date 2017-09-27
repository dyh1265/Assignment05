#include "bbox.h"
#include <utility>
#include <core/point.h>
#include <core/vector.h>
#include <core/assert.h>
#include <cmath>
#include <cfloat>
#include <rt/solids/aabox.h>
#include <rt/intersection.h>


namespace rt{
  
   // Point BBox:: get_midpoint(){
   //     return this->centr;
   // }
    BBox BBox::empty(){
    BBox box;
    box.pMin = Point(FLT_MAX, FLT_MAX, FLT_MAX);
    box.pMax = Point(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    return box;
}

    Point& BBox::operator[](int i) {
        Assert(i>=0 && i<=1);
        return (&pMin)[i];
    }
    const Point &BBox::operator[](int i) const{
        Assert(i>=0 && i<=1);
        return (&pMin)[i];
    }

    
BBox BBox::full() {
    BBox box;
    box.pMin = Point(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    box.pMax = Point(FLT_MAX, FLT_MAX, FLT_MAX);
    return box;
}

    
    void BBox::extend(const Point& point){
        this->pMin.x = this->pMin.x < point.x ? this->pMin.x : point.x;
        this->pMin.y = this->pMin.y < point.y ? this->pMin.y : point.y;
        this->pMin.z = this->pMin.z < point.z ? this->pMin.z : point.z;
        
        this->pMax.x = this->pMax.x > point.x ? this->pMax.x : point.x;
        this->pMax.y = this->pMax.y> point.y ? this->pMax.y : point.y;
        this->pMax.z = this->pMax.z> point.z ? this->pMax.z : point.z;
    }
    void BBox::extend(const BBox& bbox){
        this->pMin = min(bbox.pMin, this->pMin);
        this->pMax = max(bbox.pMax, this->pMax);
    }
    bool BBox::isUnbound(){
        return this->pMax.x > 1e5|| this->pMax.y > 1e5||this->pMax.z > 1e5
        || this->pMin.x <-1e5||this->pMin.y <-1e5||this->pMin.z <-1e5;
    }
    bool BBox::isEmpty(){
        return  this->pMin.x > this->pMax.x ||
        this->pMin.y > this->pMax.y ||
        this->pMin.z > this->pMax.z;
    }
    std::pair<float,float> BBox::intersect(const Ray& ray) const{
        float tmin, tmax, tymin, tymax, tzmin, tzmax;
        Vector invdir = 1.f/ray.d;
        
        if (invdir.x >= 0.f) {
            tmin = (pMin.x - ray.o.x) * invdir.x;
            tmax = (pMax.x - ray.o.x) * invdir.x;
        }
        else {
            tmin = (pMax.x - ray.o.x) * invdir.x;
            tmax = (pMin.x - ray.o.x) * invdir.x;
        }
        
        if (invdir.y >= 0.f) {
            tymin = (pMin.y - ray.o.y) * invdir.y;
            tymax = (pMax.y - ray.o.y) * invdir.y;
        }
        else {
            tymin = (pMax.y - ray.o.y) * invdir.y;
            tymax = (pMin.y - ray.o.y) * invdir.y;
        }
        if (tymin > tmin)
            tmin = tymin;
        if (tymax < tmax)
            tmax = tymax;
        
        if (invdir.z >= 0.f) {
            tzmin = (pMin.z - ray.o.z) * invdir.z;
            tzmax = (pMax.z - ray.o.z) * invdir.z;
        }
        else {
            tzmin = (pMax.z - ray.o.z) * invdir.z;
            tzmax = (pMin.z - ray.o.z) * invdir.z;
        }
        
        if (tzmin > tmin)
            tmin = tzmin;
        if (tzmax < tmax)
            tmax = tzmax;
        
        
//        float xmin, xmax, ymin, ymax, zmin, zmax;
//        
//        /* Retrieve min/max values for ray origin */
//        if ((pMin.x - ray.o.x) * (pMin.x - ray.o.x) <
//            (pMax.x - ray.o.x) * (pMax.x - ray.o.x)) {
//            xmin = pMin.x;
//            xmax = pMax.x;
//        }
//        else {
//            xmin = pMax.x;
//            xmax = pMin.x;
//        }
//        if ((pMin.y - ray.o.y) * (pMin.y - ray.o.y) <
//            (pMax.y - ray.o.y) * (pMax.y - ray.o.y)) {
//            ymin = pMin.y;
//            ymax = pMax.y;
//        }
//        else {
//            ymin = pMax.y;
//            ymax = pMin.y;
//        }
//        if ((pMin.z - ray.o.z) * (pMin.z - ray.o.z) <
//            (pMax.z - ray.o.z) * (pMax.z - ray.o.z)) {
//            zmin = pMin.z;
//            zmax = pMax.z;
//        }
//        else {
//            zmin = pMax.z;
//            zmax = pMin.z;
//        }
//        /* Compute intersections with slabs */
//        float txnear = (xmin - ray.o.x) * invdir.x;
//        float tynear = (ymin - ray.o.y) * invdir.y;
//        float tmin = txnear > tynear ? txnear : tynear;
//        float tznear = (zmin - ray.o.z) * invdir.z;
//        tmin = tznear > tmin ? tznear : tmin;
//        
//        float txfar = (xmax - ray.o.x)  * invdir.x;
//        float tyfar = (ymax - ray.o.y) * invdir.y;
//        float tmax = txfar < tyfar ? txfar : tyfar;
//        float tzfar = (zmax - ray.o.z) * invdir.z;
//        tmax = tzfar < tmax ? tzfar : tmax;
        
        return std::make_pair(tmin, tmax);
    }
        
    int BBox::longestAxis() const {
        Vector diag = this->pMax-this->pMin;
        if (diag.x > diag.y && diag.x > diag.z) {
            return 0;
        }
        else if (diag.y > diag.z) {
            return 1;
        }
        else {
            return 2;
        }
    }
  
    
    float BBox::getSplit(int axis) const {
        if (axis == 0) {
            return 0.5f * (pMin.x + pMax.x);
        }
        else if (axis == 1) {
            return 0.5f * (pMin.y + pMax.y);
        }
        else {
            return 0.5f * (pMin.z + pMax.z);
        }
    }
    
    
    float BBox::SurfaceArea() const {
        Vector d = this->pMax - this->pMin;
        return 2.f * (d.x * d.y + d.x * d.z + d.y * d.z);
    }
    
    
    float BBox::Volume() const {
        Vector d = this->pMax - this->pMin;
        return d.x * d.y * d.z;
    }
    
    
    bool BBox::Inside(const Point &pt) const {
        return (pt.x >= pMin.x && pt.x <= pMax.x &&
                pt.y >= pMin.y && pt.y <= pMax.y &&
                pt.z >= pMin.z && pt.z <= pMax.z);
    }
    
    
    BBox Union(const BBox &b, const Point &p) {
        BBox ret = b;
        ret.pMin.x = min(b.pMin.x, p.x);
        ret.pMin.y = min(b.pMin.y, p.y);
        ret.pMin.z = min(b.pMin.z, p.z);
        
        ret.pMax.x = max(b.pMax.x, p.x);
        ret.pMax.y = max(b.pMax.y, p.y);
        ret.pMax.z = max(b.pMax.z, p.z);
        return ret;
    }
    
    BBox Union(const BBox &b, const BBox &a) {
        BBox ret;
        ret.pMin.x = min(b.pMin.x, a.pMin.x);
        ret.pMin.y = min(b.pMin.y, a.pMin.y);
        ret.pMin.z = min(b.pMin.z, a.pMin.z);
        ret.pMax.x = max(b.pMax.x, a.pMax.x);
        ret.pMax.y = max(b.pMax.y, a.pMax.y);
        ret.pMax.z = max(b.pMax.z, a.pMax.z);
        return ret;
    }
    
    
    bool BBox::Overlaps(const BBox &b) const {
        bool x = (pMax.x >= b.pMin.x) && (pMin.x <= b.pMax.x);
        bool y = (pMax.y >= b.pMin.y) && (pMin.y <= b.pMax.y);
        bool z = (pMax.z >= b.pMin.z) && (pMin.z <= b.pMax.z);
        return (x && y && z);
    }
    
    
    void BBox::Expand(float delta) {
        pMin = pMin - Vector(delta, delta, delta);
        pMax = pMax + Vector(delta, delta, delta);
    }
    
    
    Vector BBox::Offset(const Point &p) const {
        return Vector((p.x - pMin.x) / (pMax.x - pMin.x),
                      (p.y - pMin.y) / (pMax.y - pMin.y),
                      (p.z - pMin.z) / (pMax.z - pMin.z));
    }
    
    std::pair<int,float> BBox::findGreatestDimensionAndMiddleLocation() const{
        Vector diag = this->pMax-this->pMin;
        
        if (diag.x >=diag.y && diag.x >=diag.z) {
            return std::make_pair(0,(pMax.x + pMin.x)/2.f);
        }
        else if (diag.y >= diag.z  ) {
            return std::make_pair(1,(pMin.y + pMax.y)/2.f);
        }
        else {
            return std::make_pair(2,(pMin.z + pMax.z)/2.f);
       
        }
    }
    
    bool isInside(BBox first, BBox second){
        return  first.pMin.x >= second.pMin.x &&
                first.pMin.y >= second.pMin.y &&
                first.pMin.z >= second.pMin.z &&
        
                first.pMax.x <= second.pMax.x &&
                first.pMax.y <= second.pMax.y &&
                first.pMax.z <= second.pMax.z;
        
    }
    
}
