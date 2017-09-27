#include <core/point.h>
#include "renderer.h"
#include <core/image.h>
#include <core/color.h>
#include <core/julia.h>
#include <rt/cameras/camera.h>
#include <rt/cameras/perspective.h>
#include <rt/cameras/orthographic.h>
#include <rt/ray.h>
#include <core/scalar.h>
#include <core/assert.h>
#include <rt/integrators/integrator.h>
#include <rt/integrators/casting.h>

typedef unsigned int uint;
rt::RGBColor a1computeColor(uint x, uint y, uint width, uint height);
rt::RGBColor a2computeColor(const rt::Ray& r);
namespace rt
{
    
    Renderer::Renderer( Camera* cam, Integrator* integrator ):cam(cam),integrator(integrator) {}
    
    void Renderer::test_render1(Image& img)
    {
        uint height_ = img.height();
        uint width_ = img.width();
        
        for(uint i = 0; i < width_; i++)
           for(uint j = 0; j < height_; j++)
           {
               img(i,j) = a1computeColor(i, j, width_, height_);
            }
    };
    void Renderer::test_render2( Image& img ) {
        
        uint height = img.height();
        uint width = img.width();
        Ray r;
        
        for (int j = 0; j < height; j++) {
            float ny = ((float)j + 0.5) / height;
            float cny = (ny * 2 - 1);
            for (int i = 0; i < width; i++) {
                float nx = ((float)i + 0.5) / width;
                float cnx = (nx * 2 - 1);
                r = this->cam->getPrimaryRay(cnx, cny);
                img(i,j) = a2computeColor(r);
                
            }
        }
    }
    void Renderer::setSamples(uint samples){
        NOT_IMPLEMENTED;
    }
 
    
    void Renderer::render( Image& img ) {
        
        uint height = img.height();
        uint width = img.width();
        Ray r = Ray();
        RGBColor color;        
        this->cam->setCameraGeometry(width, height);
        for (int j = 0; j < height; j++) {
            float ny = ((float)j + 0.5) / height;
            float cny = (ny * 2 - 1);
            for (int i = 0; i < width; i++) {
                float nx = ((float)i + 0.5) / width;
                float cnx = (nx * 2 - 1);
                    r = this->cam->getPrimaryRay(cnx, cny);
                    img(i,j) = this->integrator->getRadiance(r).clamp();
            
            }
           
        }
         
    }

  
    
}
