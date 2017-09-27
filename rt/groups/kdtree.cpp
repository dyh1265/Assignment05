#include <rt/groups/kdtree.h>
#include <vector>
#include <rt/groups/group.h>
#include <rt/bbox.h>
#include <rt/intersection.h>
#include <cfloat>
#include "KDNode.h"
#include <core/assert.h>
#include "kdtree.h"
#include <stack>
#include <core/point.h>

// Reference https://blog.frogslayer.com/kd-trees-for-faster-ray-tracing-with-triangles/
//http://ray-tracing.ru/articles181.html


namespace rt{
    
    KDTree::KDTree()
    {
        root = new KDNode();
    }

    
    void KDTree::add(Primitive * p)
    {
        area.push_back(p->getBounds().SurfaceArea());
        unsortedList.push_back(p);
        
    }
    void quickSort1(std::vector<float>& areas, std::vector<Primitive*> &prims, int left, int right) {
        int i = left, j = right;
        float tmp;
        float pivot = areas[(left + right) / 2];
        Primitive * tmP;
        /* partition */
        while (i <= j) {
            while (areas[i] < pivot)
                i++;
            while (areas[j] > pivot)
                j--;
            if (i <= j) {
                tmP = prims[i];
                prims[i] = prims[j];
                prims[j] = tmP;
                tmp = areas[i];
                areas[i] = areas[j];
                areas[j] = tmp;
                i++;
                j--;
            }
        };
        /* recursion */
        if (left < j)
            quickSort1(areas, prims,left, j);
        if (i < right)
            quickSort1(areas,prims, i, right);
    }
    
    void quickSort2(std::vector<float>& areas, std::vector<Primitive*> &prims, int left, int right) {
        int i = left, j = right;
        float tmp;
        float pivot = areas[(left + right) / 2];
        Primitive * tmP;
        /* partition */
        while (i <= j) {
            while (areas[i] > pivot)
                i++;
            while (areas[j] < pivot)
                j--;
            if (i <= j) {
                tmP = prims[i];
                prims[i] = prims[j];
                prims[j] = tmP;
                tmp = areas[i];
                areas[i] = areas[j];
                areas[j] = tmp;
                i++;
                j--;
            }
        };
        /* recursion */
        if (left < j)
            quickSort2(areas, prims,left, j);
        if (i < right)
            quickSort2(areas,prims, i, right);
    }
    // Sort primitives with respect to minimum
    void sortPrims2(int axis,std::vector<Primitive*>& prims ){
        std:: vector<float> areax;
        std:: vector<float> areay;
        std:: vector<float> areaz;
        
        std:: vector<float> array;
        BBox b;
        for (int i=0; i<prims.size(); i++) {
            b = prims[i]->getBounds();
            Vector d = b.pMax - b.pMin;
            switch (axis) {
                case 0:
                    areax.push_back(std::abs(d.x*d.y));
                    break;
                case 1:
                    areay.push_back(std::abs(d.x*d.z));
                    break;
                case 2:
                    areaz.push_back(std::abs(d.y*d.z));
                    break;
            }
        }
        
        switch (axis) {
            case 0: quickSort2(areax, prims, 0, (int)areax.size()-1 );
                break;
            case 1: quickSort2(areay, prims, 0, (int)areay.size()-1);
                break;
            case 2: quickSort2(areaz, prims, 0, (int)areaz.size()-1);
                break;
        }
    }
    // Sort primitives with respect to maximum
   void  sortPrims1(int axis,std::vector<Primitive*>& prims ){
        std:: vector<float> areax;
       
        std:: vector<float> areay;
        std:: vector<float> areaz;
        std:: vector<float> array;
        BBox b;
        for (int i=0; i<prims.size(); i++) {
            b = prims[i]->getBounds();
            Vector d = b.pMax - b.pMin;
            switch (axis) {
                case 0:
                    areax.push_back(std::abs(d.x*d.y));
                    break;
                case 1:
                    areay.push_back(std::abs(d.x*d.z));
                    break;
                case 2:
                    areaz.push_back(std::abs(d.y*d.z));
                    break;
            }
        }
       switch (axis) {
           case 0: quickSort1(areax, prims, 0, (int)areax.size()-1);
               break;
           case 1: quickSort1(areay, prims, 0, (int)areay.size()-1);
               break;
           case 2: quickSort1(areaz, prims, 0, (int)areaz.size()-1);
               break;
       }
    }
    
    BBox bounds(std::vector<Primitive*>& prims){
        BBox b = *new BBox;
        b=b.empty();
        for (int i=0;i <prims.size(); i++ ){
            b.extend(prims[i]->getBounds());
        }
        return b;
    }
    void KDTree::rebuildIndex(){
     float   prevminSAH = -1;
        root=buildKDTree(unsortedList, 0,  prevminSAH );
    }
    
   KDNode* KDTree::buildKDTree(Primitives prims, int depth, float prevminSAH){
        KDNode* node = new KDNode;
        node->primitives = prims;
       
        if (prims.size() == 0 ){
            return node;
        }
        if (prims.size() == 1 ){
            node->isLeaf = true;
            node->left = new KDNode();
            node->right = new KDNode();
            node->left->primitives = std::vector<Primitive*>();
            node->right->primitives = std::vector<Primitive*>();
            return node;
        }
       
       float split=-9;
       float minSplit = -9;;
       int   minAxis = -1;
       int   leftPrims = -1;
       float minSAH = 1000000000;
       
       BBox boxFull = bounds(prims);
       float bfsa = boxFull.SurfaceArea();
       // find lower corner of the "global" bounding box of node
       float boxmin = 0.0;
      
       
       for (int axis = 0; axis < 2; axis++){
           // sort primitives belong to current axis and min bounds
      
        boxmin = boxFull.pMin[axis];
        // split max
        float boxmax = 0.0;
        boxmax = boxFull.pMax[axis];
        //Sort primitives
          sortPrims1(axis, prims);
           
           for (int i = 1; i<prims.size(); i++ ){
               int onLeftSide = i;
               int onRightSide = (int)prims.size()-i;
               // initialize split
               split = prims[i]->getBounds().pMin[axis];
               // if split is in the minimum corner of the "global" bounding box of node
               if (split==boxmin){
                   continue;
               }
               //evaluate SAH
               BBox boxLeft = boxFull;
               BBox boxRight = boxFull;
               
               switch (axis) {
                   case 0:
                       boxLeft.pMax.x = split;
                       boxRight.pMin.x  = split;
                       break;
                   case 1:
                       boxLeft.pMax.y = split;
                       boxRight.pMin.y  = split;
                       break;
                   case 2:
                       boxLeft.pMax.z = split;
                       boxRight.pMin.z  = split;
                       break;
               }
               float blsa = boxLeft.SurfaceArea();
               float brsa = boxRight.SurfaceArea();
               float SAH = 0.125f + (blsa * onLeftSide + brsa * onRightSide)/bfsa;
               
               if (SAH<minSAH){
                   minSAH = SAH;
                   minSplit = split;
                   minAxis = axis;
                   leftPrims = onLeftSide;
               }
           }
           
           sortPrims2(axis, prims);
           for (int i = (int)prims.size()-2;i>=0;i--){
               int onLeftSide = i+1;
               int onRightSide = (int)prims.size()-i-1;
               // initialize split
               split = prims[i]->getBounds().pMax[axis];
               // find bigger corner of the "global" bounding box of node
              
               if (split==boxmax){
                   continue;
               }
               //evaluate SAH
               BBox boxLeft = boxFull;
               BBox boxRight = boxFull;
               switch (axis) {
                   case 0:
                       boxLeft.pMax.x = split;
                       boxRight.pMin.x  = split;
                       break;
                   case 1:
                       boxLeft.pMax.y = split;
                       boxRight.pMin.y  = split;
                       break;
                   case 2:
                       boxLeft.pMax.z = split;
                       boxRight.pMin.z  = split;
                       break;
               }
               float blsa = boxLeft.SurfaceArea();
               float brsa = boxRight.SurfaceArea();
                  float SAH = 0.125f + (blsa * onLeftSide + brsa * onRightSide)/bfsa;
               
               if (SAH<minSAH){
                   minSAH = SAH;
                   minSplit = split;
                   minAxis = axis;
                   leftPrims = onLeftSide;
               }
               
           }
           
       }
       int nPrims  = prims.size()-leftPrims;
       if ((minSAH > nPrims||depth>20)){
           node->isLeaf = true;
           node->left = new KDNode();
           node->right = new KDNode();
           node->left->primitives = std::vector<Primitive*>();
           node->right->primitives = std::vector<Primitive*>();
       } else {
       node->split = minSplit;
       node->axis = minAxis;
       std::vector<Primitive*> left_prims;
        std::vector<Primitive*> right_prims;
        for (int i=0; i<leftPrims; i++){
        left_prims.push_back(prims[i]);
           }
           for (int j=leftPrims; j<(int)prims.size(); j++){
               right_prims.push_back(prims[j]);
           }
       node->left = buildKDTree(left_prims, depth + 1,   minSAH);
       node->right = buildKDTree(right_prims, depth+1,   minSAH);
       
       }
       return node;
    }
   

    Intersection KDTree::intersect(const Ray &ray,float previousBestDistance)const{
        Intersection inter;
        KDNode *currentNode = root;
        auto   t1 = bounds(currentNode->primitives).intersect(ray);
        float  t_near = std::get<0>(t1);
        float  t_far = std::get<1>(t1);
        
        bool isIntersected = t_near<t_far&&t_near>0?true:false;
        
        if (!isIntersected || t_near > previousBestDistance)
        {
            return Intersection();
        }
        
        
        float ray_or_split     = 0.0;
        float ray_invdir_split = 0.0;
        float  ray_dir_axis = 0.0;
        std::stack<MyStack> stack = node_stack;
        while (true){
            while (!currentNode->isLeaf) {
                
                switch (currentNode->axis) {
                    case 0:
                        ray_or_split     = ray.o.x;
                        ray_invdir_split = ray.invdir.x;
                        ray_dir_axis = ray.d.x;
                        break;
                    case 1:
                        ray_or_split     = ray.o.y;
                        ray_invdir_split = ray.invdir.y;
                         ray_dir_axis = ray.d.y;
                        break;
                    case 2:
                        ray_or_split     = ray.o.z;
                        ray_invdir_split = ray.invdir.z;
                         ray_dir_axis = ray.d.z;
                        break;
                }
//                if (ray_dir_axis < 0){
//                    KDNode* swap;
//                    swap=currentNode->right;
//                    currentNode->right = currentNode->left;
//                    currentNode->left = swap;
//                }
                float t_split = (currentNode->split - ray_or_split) * ray_invdir_split;
                
                if (t_split <= t_near){
                    currentNode= currentNode->right;
                } else if (t_split >= t_far){
                    currentNode= currentNode->left;
                } else {
                    stack.push(MyStack(currentNode->right, t_split, t_far));
                    currentNode = currentNode->right;
                    t_far = t_split;
                }
            }
             inter = IntersectAllPrimitivesInLeaf(ray, currentNode);
            float t_hit = inter.distance;
            
//            if (t_hit  <= t_far){
//                return Intersection(); // early ray termination
//            }
            
            if (stack.empty()){
                return inter; // nothing else to traverse any more...
            }
            t_near = t_far;
            auto currentElement = stack.top();
            currentNode = currentElement.node;
            t_far       = stack.top().t_far;
            stack.pop();
        
        }
        
        return inter;
    }
    

   
    Intersection KDTree::IntersectAllPrimitivesInLeaf(const Ray& ray, KDNode* node) const{
        
        float  previousBestDistance = FLT_MAX;
        Intersection smallestIntersection;
        for (int i = 0; i <= node->primitives.size()-1; i++)
        {
            Intersection intersection = node->primitives[i]->intersect(ray, previousBestDistance);
            if (intersection && (intersection.distance < previousBestDistance) && intersection.distance > 0)
            {
                smallestIntersection = intersection;
                previousBestDistance = intersection.distance;
            }
        }
        return  smallestIntersection;
    }
    
  
    
    void KDTree::setMaterial(Material* m){
        NOT_IMPLEMENTED;
    }
    void KDTree::setCoordMapper(CoordMapper* cm){
       NOT_IMPLEMENTED;
    }
    KDTree::~KDTree(){
        delete root;
    }
    BBox KDTree::getBounds() const{
        NOT_IMPLEMENTED;
    }
}
