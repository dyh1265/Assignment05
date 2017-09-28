//
//  dummy.cpp
//  cgdoc
//
//  Created by Андрей Сиразитдинов on 27/09/2017.
//  Copyright © 2017 Андрей Сиразитдинов. All rights reserved.
//

#include "dummy.h"
#include <rt/materials/material.h>
#include <core/color.h>
#include <core/assert.h>
#include <cmath>
namespace rt {
    
    RGBColor DummyMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
        float cosTheta = fabs(dot(normal,inDir));
        return RGBColor::rep(cosTheta);
    }
    
    RGBColor DummyMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const{
        
        return RGBColor::rep(0.f);
    }
    
    Material::SampleReflectance DummyMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const{
        NOT_IMPLEMENTED;
    }
}
