
#ifndef KDNode_h
#define KDNode_h
#include <vector>
#include <rt/groups/group.h>
#include <rt/bbox.h>

namespace rt{
class KDNode{
    
    public:
        std::vector<Primitive*> primitives;
        KDNode();
        BBox bbox;
        void extendBox(const BBox& inputBox);
    
        KDNode* left = nullptr;
        KDNode* right = nullptr;
        void add(Primitive* p);
        bool isLeaf;
        float split;
        int axis;
};

}

#endif /* KDNode_h */
