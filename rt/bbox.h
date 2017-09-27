#ifndef CG1RAYTRACER_BBOX_HEADER
#define CG1RAYTRACER_BBOX_HEADER

#include <utility>
#include <core/point.h>
#include <core/vector.h>
#include <rt/intersection.h>
namespace rt {

class Ray;

class BBox {
public:
    Point pMin, pMax;
   // Point centr;
    BBox() {
        pMin = Point( FLT_MAX, FLT_MAX, FLT_MAX);
        pMax = Point(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    }
    BBox(const Point& p1, const Point& p2) {
        pMin = Point(min(p1.x, p2.x), min(p1.y, p2.y), min(p1.z, p2.z));
        pMax = Point(max(p1.x, p2.x), max(p1.y, p2.y), max(p1.z, p2.z));
        //centr = Point(0.5f * (pMin.x + pMax.x),
        //                    0.5f * (pMin.y + pMax.y),
        //                    0.5f * (pMin.z + pMax.z));
  
    }
    static BBox empty();
    static BBox full();
    
    void extend(const Point& point);
    void extend(const BBox& bbox);
    int longestAxis() const;
    Vector diagonal() const { return pMax - pMin; }
    float getSplit (int axis) const;
    std::pair<float,float> intersect(const Ray& ray) const;
    std::pair<int,float> findGreatestDimensionAndMiddleLocation() const;
    float SurfaceArea() const;
    bool isUnbound();
    float Volume() const;
    bool Inside(const Point &pt) const;
    
    
    bool Overlaps(const BBox &b) const;
    void Expand(float delta);
    
    const Point &operator[](int i) const;
    
    Point &operator[](int i);
    
    Vector Offset(const Point &p) const;
    bool isEmpty();
    Point get_midpoint();
};
     bool isInside(BBox first, BBox second);
     BBox Union(const BBox &a, const BBox &b);
    BBox Union(const BBox &b, const Point &p);
}


#endif
