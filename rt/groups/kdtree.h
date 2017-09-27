#ifndef CG1RAYTRACER_GROUPS_KDTREE_HEADER
#define CG1RAYTRACER_GROUPS_KDTREE_HEADER

#include <vector>
#include <rt/groups/group.h>
#include <rt/bbox.h>
#include "KDNode.h"
#include <queue>
#include <stack>
const int maxDepth = 120;
namespace rt {
 
    
    class MyStack
    {
    public:
        KDNode* node;
        float t_near;
        float t_far;
        MyStack() {}
        MyStack(KDNode * node, float t_near, float t_far)
        :node(node), t_near(t_near), t_far(t_far) {}
    };
    
    class KDTree : public Group {
    public:
        KDTree();
        
        virtual BBox getBounds() const;
        BBox bbox;
        virtual Intersection intersect(const Ray& ray, float previousBestDistance=FLT_MAX) const;
        virtual void rebuildIndex();
        virtual ~KDTree();
        virtual void add(Primitive* p);
        virtual void setMaterial(Material* m);
        virtual void setCoordMapper(CoordMapper* cm);
        KDNode*  buildKDTree(Primitives prims, int  depth, float  prevminSAH);
        Intersection IntersectAllPrimitivesInLeaf(const Ray& ray, KDNode* node) const;
        bool operator<(Primitive* p1) ;
        private:
        Primitives unsortedList;
        KDNode* root;
        std::stack<MyStack> node_stack;
        std::vector<float> area;
    };
    
    
    
}

#endif
