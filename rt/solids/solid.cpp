#include "solid.h"
#include <core/assert.h>
namespace rt {
    
    Solid::Solid(CoordMapper* texMapper, Material* material):material(material),texMapper(texMapper) {}
    Point Solid::get_midpoint() const {
      return Point();
    }
}
