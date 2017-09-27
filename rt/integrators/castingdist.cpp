#include "castingdist.h"
#include <rt/world.h>
#include <rt/intersection.h>
#include <core/color.h>
#include <core/vector.h>
#include <cmath>
namespace  rt{
    RayCastingDistIntegrator::RayCastingDistIntegrator(World* world, const RGBColor& nearColor, float nearDist, const RGBColor& farColor, float farDist): nearColor(nearColor),nearDist(nearDist),farColor(farColor),farDist(farDist)
    {
        this->world = world;
    }
    
    RGBColor RayCastingDistIntegrator::getRadiance(const Ray& ray) const {
        
        float col = 0;
        RGBColor rgb;
        Intersection rayIntersect = this->world->scene->intersect(ray);
        if (rayIntersect.distance > epsilon){
            col = -dot(ray.d, rayIntersect.n);
            /* Linearly interpolate colours */
            rgb = this->nearColor + (rayIntersect.distance - this->nearDist)
            * ((farColor - nearColor)/(farDist - nearDist));
        }
        RGBColor resColor = RGBColor(col, col, col) * rgb;
        
        return resColor.clamp();
        
    }
}
