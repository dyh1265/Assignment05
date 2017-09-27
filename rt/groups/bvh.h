#ifndef CG1RAYTRACER_GROUPS_BVH_HEADER
#define CG1RAYTRACER_GROUPS_BVH_HEADER

#include <vector>
#include <rt/groups/group.h>
#include <rt/bbox.h>
//#include "bvhNode.h"
#include <queue>
#include <stack>
#include <core/memory.h>
namespace rt {
    
    class BVHBuildNode {
    public:
        BVHBuildNode() { children[0] = children[1] = NULL; }
        void InitLeaf(int first, int n, const BBox &b) {
            firstPrimOffset = first;
            nPrimitives = n;
            bounds = b;
        }
        void InitInterior(int axis, BVHBuildNode *c0, BVHBuildNode *c1) {
            children[0] = c0;
            children[1] = c1;
            bounds = Union(c0->bounds, c1->bounds);
            splitAxis = axis;
            nPrimitives = 0;
        }
        BBox bounds;
        BVHBuildNode *children[2];
        int splitAxis, firstPrimOffset, nPrimitives;
    };
    class LinearBVHNode {
    public:
        BBox bounds;
        union {
            int primitivesOffset; // leaf
            int secondChildOffset; // interior
        };
        int nPrimitives; // 0 -> interior node
        int axis; // interior node: xyz
        int pad[2]; // ensure 32 byte total size
        
    };
    
class BVH : public Group {
    
    struct BVHPrimitiveInfo {
        int primitiveNumber;
        Point centroid;
        BBox bounds;
        
        BVHPrimitiveInfo(int pn, const BBox &b)
        : primitiveNumber(pn), bounds(b) {
        centroid = Point(.5f * (b.pMin.x+ b.pMax.x), .5f *(b.pMin.y+b.pMax.y), .5f *(b.pMin.z+b.pMax.z));
        }
       
    };
        
    struct ComparePoints {
        ComparePoints(int d) { dim = d; }
        int dim;
        bool operator()(const BVHPrimitiveInfo &a,
                        const BVHPrimitiveInfo &b) const {
            return a.centroid[dim] < b.centroid[dim];
        }
    };
    
    struct BucketInfo {
        BucketInfo() { count = 0; }
        int count;
        BBox bounds;
    };
    
    struct CompareToBucket {
        CompareToBucket(int split, int num, int d, const BBox &b)
        : centroidBounds(b)
        { splitBucket = split; nBuckets = num; dim = d; }
        bool operator()(const BVHPrimitiveInfo &p) const{
            int b = nBuckets * ((p.centroid[dim] - centroidBounds.pMin[dim]) / (centroidBounds.pMax[dim] - centroidBounds.pMin[dim]));
            if (b == nBuckets) b = nBuckets-1;
            return b <= splitBucket;
        }
        int splitBucket, nBuckets, dim;
        const BBox &centroidBounds;
    };
   
public:
    BVH();
    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance=FLT_MAX) const;
    virtual void rebuildIndex();
	virtual ~BVH();
    virtual void add(Primitive* p);
    virtual void setMaterial(Material* m);
    virtual void setCoordMapper(CoordMapper* cm);
    void buildBVH(int maxPrimsInNode);
    
//    static inline bool IntersectP(const BBox &bounds, const Ray &ray,const Vector &invDir, const int dirIsNeg[3]);

private:
    BVHBuildNode* recursiveBuild(MemoryArena &buildArena, std::vector<BVHPrimitiveInfo> &buildData, int start, int end, int *totalNodes, std::vector<Primitive*> &orderedPrims);
    int flattenBVHTree(BVHBuildNode *node, int *offset);
    static inline bool IntersectP(const BBox &bounds, const Ray &ray,
                                  const Vector &invDir, const int dirIsNeg[3]);

    int maxPrimsInNode;
    LinearBVHNode *nodes = nullptr;
    Primitives primitives;
};

}

#endif
