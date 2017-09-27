#include <rt/groups/kdtree.h>
#include <vector>
#include <rt/groups/group.h>
#include <rt/bbox.h>
#include <rt/intersection.h>
#include <cfloat>
#include <core/assert.h>
#include <rt/groups/bvh.h>
#include <stack>
//#include "bvhNode.h"
#include <core/memory.h>
#include <iostream>
// ref: Pharr physically based renderring
namespace rt {
    
    /***********************************************************************/
    /* Add primitives to stack */
    void BVH::add(Primitive *p){
        primitives.push_back(p);
    }
    
    /***********************************************************************/
    /*Build tree*/
    
    BVHBuildNode* BVH::recursiveBuild(MemoryArena &buildArena, std::vector<BVHPrimitiveInfo> &buildData, int start, int end, int *totalNodes, std::vector<Primitive*> &orderedPrims){
        
        /*Allocate memory*/
        (*totalNodes)++;
        BVHBuildNode *node = buildArena.Alloc<BVHBuildNode>();
        int dim;
        
        /*Compute bounds of all primitives in BVH node*/
        BBox bbox;
        for (int i = start; i < end; ++i){
            bbox = Union(bbox, buildData[i].bounds);
        }
        int nPrimitives = end - start;
        
        /*If number of primitives less then max number of primitives in node,
        create a leaf*/
        if (nPrimitives <= maxPrimsInNode){
            int firstPrimOffset = orderedPrims.size();
            for (int i = start; i < end; i++) {
                int primNum = buildData[i].primitiveNumber;
                orderedPrims.push_back(primitives[primNum]);
            }
            node->InitLeaf(firstPrimOffset, nPrimitives, bbox);
            return node;
            
        } else
        {
            /*Compute bound of primitive centroids, choose split dimension dim*/
            BBox centroidBounds;
            for (int i = start; i < end; ++i){
                centroidBounds = Union(centroidBounds, buildData[i].centroid);
            }
            dim = centroidBounds.longestAxis();
            
            /*Partition primitives into two sets and build children*/
            int mid = (start + end) / 2;
            if (centroidBounds.pMax[dim] == centroidBounds.pMin[dim])
            {
                int firstPrimOffset = orderedPrims.size();
                for (int i = start; i < end; ++i)
                {
                    int primNum = buildData[i].primitiveNumber;
                    orderedPrims.push_back(primitives[primNum]);
                }
                node->InitLeaf(firstPrimOffset, nPrimitives, bbox);
                return node;
            } else
            {
                
                /*Allocate BucketInfo for SAH partition buckets*/
                const int nBuckets = 12;
                BucketInfo buckets[nBuckets];
            
                /*Initialize BucketInfo for SAH partition buckets*/
                for (int i = start; i<end; ++i)
                {
                    int b = nBuckets * ((buildData[i].centroid[dim] - centroidBounds.pMin[dim])/ (centroidBounds.pMax[dim] - centroidBounds.pMin[dim]));
                    
                    if (b == nBuckets) b = nBuckets-1;
                    buckets[b].count++;
                    buckets[b].bounds = Union(buckets[b].bounds, buildData[i].bounds);
                }
            
                /*Compute costs for splitting after each bucket*/
                float cost[nBuckets-1];
                for (int i = 0; i < nBuckets-1; ++i)
                {
                    BBox b0, b1;
                    int count0 = 0, count1 = 0;
                    for (int j = 0; j <= i; ++j)
                    {
                        b0 = Union(b0, buckets[j].bounds);
                        count0 += buckets[j].count;
                    }
                    for (int j = i+1; j < nBuckets; ++j)
                    {
                        b1 = Union(b1, buckets[j].bounds);
                        count1 += buckets[j].count;
                    }
                    //std::cout<<b1.SurfaceArea()<<std::endl;
                    cost[i] = 1 + (count0 * b0.SurfaceArea() + count1 * b1.SurfaceArea()) /
                    bbox.SurfaceArea();
                }
                
                /*Given all of the costs, a linear scan through the cost array finds the partition with minimum cost.*/
                /*Find bucket to split at that minimizes SAH metric*/
                    
                float minCost = cost[0];
                int minCostSplit = 0;
                for (int i = 1; i < nBuckets-1; i++)
                {
                    if (cost[i] < minCost)
                    {
                        minCost = cost[i];
                        minCostSplit = i;
                    }
                }

                /*Either create leaf or split primitives at selected SAH bucket*/
                float leafCost = nPrimitives;
                if (nPrimitives > maxPrimsInNode ||
                    minCost < leafCost) {
                    BVHPrimitiveInfo *pmid = std::partition(&buildData[start],
                                                            &buildData[end-1]+1,
                                                            CompareToBucket(minCostSplit, nBuckets, dim, centroidBounds));
                    mid = pmid - &buildData[0];
                } else
                {
                /*Create leaf*/
                    int firstPrimOffset = (int)orderedPrims.size();
                    for (int i = start; i < end; ++i)
                    {
                        int primNum = buildData[i].primitiveNumber;
                        orderedPrims.push_back(primitives[primNum]);
                    }
                    node->InitLeaf(firstPrimOffset, nPrimitives, bbox);
                    return node;
                }
            }
            node->InitInterior(dim,recursiveBuild(buildArena, buildData, start, mid,
                                                  totalNodes, orderedPrims),
                                   recursiveBuild(buildArena, buildData, mid, end,
                                                  totalNodes, orderedPrims));
        }
        return node;
    }
    /***********************************************************************/
    void  BVH::buildBVH(int mp)
    {
        maxPrimsInNode = min(255, mp);
        if (primitives.size() == 0)
        {
            nodes = NULL;
            return;
        }
        
        /*Initialize buildData array for primitives*/
        std::vector<BVHPrimitiveInfo> buildData;
        buildData.reserve(primitives.size());
      
        for (int i = 0; i < primitives.size(); ++i)
        {
            BBox bbox = primitives[i]->getBounds();
            buildData.push_back(BVHPrimitiveInfo(i, bbox));
        }
        
        /*Recursively build BVH tree for primitives*/
        MemoryArena buildArena(1024*1024);
        int totalNodes = 0;
        std::vector<Primitive*> orderedPrims;
        orderedPrims.reserve(primitives.size());
        BVHBuildNode *root;
        root = recursiveBuild(buildArena,buildData, 0,
                                            (int)primitives.size(), &totalNodes,
                                            orderedPrims);
        
        std::cout<<"BVH created with "<<totalNodes<< " "<< "nodes for"<<" "<<(int)primitives.size()<<" "<<"primitives"<<" "<< float(totalNodes * sizeof(LinearBVHNode)) /(1024.f * 1024.f)<<std::endl;
        primitives.swap(orderedPrims);
        
    /* The built tree is transformed to the LinearBVHNode representation by the flattenBVH Tree() method, which performs a depth-first traversal and stores the nodes in memory  in linear order.*/
      int offset = 0;
      nodes = AllocAligned<LinearBVHNode>(totalNodes);
      flattenBVHTree(root, &offset);
    }
    
    /***********************************************************************/
    int BVH::flattenBVHTree(BVHBuildNode *node, int *offset){
        LinearBVHNode *linearNode = &nodes[*offset];
        linearNode->bounds = node->bounds;
        int myOffset = (*offset)++;
        if (node->nPrimitives > 0) {
            linearNode->primitivesOffset = node->firstPrimOffset;
            linearNode->nPrimitives = node->nPrimitives;
        }
        else {
            linearNode->axis= node->splitAxis;
            linearNode->nPrimitives = 0;
            flattenBVHTree(node->children[0], offset);
            linearNode->secondChildOffset = flattenBVHTree(node->children[1],
                                                        offset);
        }
        return myOffset;
    }
    /***********************************************************************/
    Intersection BVH::intersect(const Ray &ray,float previousBestDistance ) const
    {
        if (!nodes) return Intersection();
        Intersection smallestIntersection;
        Point origin = ray.o;
       Vector direction = ray.d;
        Vector invDir(1.f / ray.d.x, 1.f / ray.d.y, 1.f / ray.d.z);
        int dirIsNeg[3] = { invDir.x < 0, invDir.y < 0, invDir.z < 0 };
        //Follow ray through BVH nodes to find primitive intersections
        int todoOffset = 0, nodeNum = 0;
        int todo[64];
        while (true) {
            const LinearBVHNode *node = &nodes[nodeNum];
            if(IntersectP(node->bounds, ray, invDir, dirIsNeg))
            {
                // Check ray against BVH node
                if (node->nPrimitives > 0){
                    // Intersect ray with primitives in leaf BVH node
                    for (int i = 0; i<node->nPrimitives; i++)
                    {
                        Intersection intersection = primitives[node->primitivesOffset+i]->intersect(ray, previousBestDistance);
                        if (intersection && intersection.distance < previousBestDistance)
                        {
                            smallestIntersection = intersection;
                            previousBestDistance = intersection.distance;
                        }
                        
                    }
    
                    if (todoOffset == 0) break;
                        nodeNum = todo[--todoOffset];
                   
                } else {
                    
                    if (dirIsNeg[node->axis])
                    {
                        //Put far BVH node on todo stack, advance to near node
                            todo[todoOffset++] = nodeNum + 1;
                            nodeNum = node->secondChildOffset;
                        
                    } else
                    {
                        todo[todoOffset++] = node->secondChildOffset;
                        nodeNum = nodeNum + 1;
                    }
                }
                
            } else
             {
              if (todoOffset == 0) break;
                nodeNum = todo[--todoOffset];
            }
        }
        return smallestIntersection;
    }
    /***********************************************************************/
    inline bool BVH::IntersectP(const BBox &bounds, const Ray &ray,
                                const Vector &invDir, const int dirIsNeg[3]){
     
        float txmin = (bounds[  dirIsNeg[0]].x - ray.o.x) * invDir.x;
        float txmax = (bounds[1-dirIsNeg[0]].x - ray.o.x) * invDir.x;
        float tymin = (bounds[  dirIsNeg[1]].y - ray.o.y) * invDir.y;
        float tymax = (bounds[1-dirIsNeg[1]].y - ray.o.y) * invDir.y;
        
        float tzmin = (bounds[  dirIsNeg[2]].z - ray.o.z) * invDir.z;
        float tzmax = (bounds[1-dirIsNeg[2]].z - ray.o.z) * invDir.z;
       
 
        float tmin = max(tzmin, max(tymin, txmin));
        float tmax = min(tzmax, min(tymax, txmax));
        tmax *= 1.00000024f;
        return tmin<tmax;
    }
    /***********************************************************************/
    BVH::BVH(){}
    /***********************************************************************/
    BVH::~BVH(){}
    /***********************************************************************/
    BBox BVH::getBounds() const
    {
        BBox bbox;
        for (int i = 0; i < primitives.size(); ++i){
           bbox = Union(bbox, primitives[i]->getBounds());
            
        }
        return bbox;
    }
    /***********************************************************************/
    void BVH::setMaterial(Material* m){}
    /***********************************************************************/
    void BVH::setCoordMapper(CoordMapper *cm){}
    /***********************************************************************/
    void BVH::rebuildIndex()
    {
        buildBVH(3);
    }
    
}


