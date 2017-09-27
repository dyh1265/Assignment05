
#include "casting.h"
#include <rt/intersection.h>
#include <rt/world.h>
#include <core/color.h>
#include <core/vector.h>
#include <core/assert.h>
namespace rt
{
    RGBColor RayCastingIntegrator::getRadiance(const Ray& ray) const{
        float col = 0.f;
        
        /* Intersect ray with world */
        Intersection rayIntersect = this->world->scene->intersect(ray);
        
        /* If ray is not hit behind the camera or ray shoots to infinity */
        if (rayIntersect.distance > epsilon) {
            col = std::abs(dot(ray.d, rayIntersect.n));
        }
        
        /* Create greyvalue RGB vector */
        RGBColor newCol(col, col, col);
        
        return newCol.clamp();
        
    };
    
}
