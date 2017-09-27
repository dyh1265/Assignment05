#include "KDNode.h"
#include <rt/bbox.h>
namespace rt
{
    KDNode::KDNode()
    {
        bbox = BBox::empty();
        left = NULL;
        right = NULL;
    }
    
    void KDNode::extendBox(const BBox& inputBox)
    {
        bbox.extend(inputBox);
    }
  
}
