
#include "simplegroup.h"
#include <rt/intersection.h>
#include <core/assert.h>
#include <rt/bbox.h>
namespace rt {
    void SimpleGroup::add(Primitive* p)
    {
        this->groupElements.push_back(p);
        this->indices++;
    }
    Intersection SimpleGroup::intersect( const Ray& ray, float previousBestDistance ) const {
        
        Intersection newBestIntersect;
        newBestIntersect.distance = previousBestDistance;
        
        /* 	Iterate naively over all members in the group beginning with the
         first one that has been added */
        for (int i = 0; i < this->indices; i++) {
            Intersection tempIntersect = this->groupElements[i]->intersect(ray, previousBestDistance);
            
            /* Catch failed intersection */
            if (tempIntersect.distance < 0) {
                tempIntersect.distance = previousBestDistance;
            }
            
            /* 	If current intersection candidate is closer than closest
             distance in cache */
            if (tempIntersect.distance < newBestIntersect.distance) {
                newBestIntersect = tempIntersect;
            }
        }
        /* If no intersect, account for floating point inaccuracy */
        if (newBestIntersect.distance >= previousBestDistance - 10e-5) {
            return Intersection::failure();
        }
        
        return newBestIntersect;
    }
    
    void SimpleGroup::setMaterial(Material* m) {
    }
    
    BBox SimpleGroup::getBounds() const{
        
        BBox box = this->groupElements[0]->getBounds();
        
        for (int i = 1; i < this->indices; i++) {
            BBox current = this->groupElements[i]->getBounds();
            box.extend(current);
        }
        return box;
    }
    
    void SimpleGroup::rebuildIndex(){}
    void SimpleGroup::setCoordMapper(CoordMapper* cm) {NOT_IMPLEMENTED;}
    Point SimpleGroup::get_midpoint() const{
        NOT_IMPLEMENTED;
    }
    
        

    
    
    
}
