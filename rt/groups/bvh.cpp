#include <rt/groups/bvh.h>
#include <stack>
#include <ctime>

namespace rt {

BVH::BVH()
{
    //this->renderingTree = new CompressedNodes();
}

BVH::~BVH() {
    //delete this->renderingTree;
}


void BVH::rebuildIndex() {

    PrimitivesInfo pInfo;

    this->initPrimitivesInfo(pInfo);


    // std::cout<<this->primitives.size()<<std::endl;
    // std::cout<<pInfo.size()<<std::endl;

    int totalNodes = 0;
    Primitives orderedPrims;
    Node* root;

    root = this->buildBVHWithSAH(pInfo, 0, this->primitives.size(), &totalNodes, orderedPrims);
    //root = this->buildBVHWithMidPoint(pInfo, 0, this->primitives.size(), &totalNodes, orderedPrims);
    primitives.swap(orderedPrims);
 
    int offset = 0;
    for(int i=0;i<totalNodes;i++){
        CompressedNode* n = new CompressedNode();   
        renderingTree.push_back(n);
    }

    this->compressTree(root, &offset);
     std::cout<<"Number of nodes is "<<totalNodes<<std::endl;
}


void BVH::setMaterial(Material* m){
    for(int i=0;i<this->primitives.size();i++){
        primitives[i]->setMaterial(m);
    }
}



int BVH::compressTree(Node* node, int* offset) const{

    
    
    CompressedNode* cNode = renderingTree.at(*offset);
    cNode->boundingBox = node->boundingBox;
    int otherOffset = (*offset)++;
    if(node->numPrimitives > 0){
        cNode->offset = node->primitiveOffset;
        cNode->numPrim = node->numPrimitives;
    }
    else{
        cNode->axis = node->splitAxis;
        cNode->numPrim = 0;
        compressTree(node->children[0], offset);
        cNode->otherChildOffset = compressTree(node->children[1], offset);
    }

    return otherOffset;

}




BBox BVH::getBounds() const {
    BBox bvhBounds = BBox::empty();

    for(int i = 0; i < this->primitives.size(); i++){
        bvhBounds.extend(primitives[i]->getBounds());
    }

    return bvhBounds;
}







Intersection BVH::intersect(const Ray& ray, float previousBestDistance) const {
    int currentIndex = 0;    
    std::stack<int> stk;


    int negDir[3] = {ray.d.x < 0, ray.d.y < 0, ray.d.z < 0};

    Intersection intersection = Intersection::failure();

    while(true){
        const CompressedNode* currentNode = renderingTree.at(currentIndex);
        std::pair<float, float> intPoints = currentNode->boundingBox.intersect(ray);
        if(intPoints.first < intPoints.second){
            
            if(currentNode->numPrim > 0){
                for(int i = 0; i < currentNode->numPrim;i++){
                    Intersection inter = primitives[currentNode->offset + i]->intersect(ray, previousBestDistance);
                    if(inter){
                        //std::cout<<"In"<<std::endl;
                        intersection = inter;
                        previousBestDistance = inter.distance;
                    }
                }
                if(stk.empty()) break;
                currentIndex = stk.top();
                stk.pop();
            }
            else{
                if(negDir[currentNode->axis]){
                    stk.push(currentIndex + 1);
                    currentIndex = currentNode->otherChildOffset;
                }
                else{
                    stk.push(currentNode->otherChildOffset);
                    currentIndex +=1;
                }
            }
        }
        else{
            if(stk.empty()) break;
            currentIndex = stk.top();
            stk.pop();
        }
    }

    //if(!intersection)
        //std::cout<<"Out"<<std::endl;
    return intersection;


}






void BVH::add(Primitive* p) {
    this->primitives.push_back(p);
}

void BVH::setCoordMapper(CoordMapper* cm) {
    /* TODO */ NOT_IMPLEMENTED;
}

void BVH::initPrimitivesInfo(PrimitivesInfo& pInfo){

    pInfo.clear();

    for(int i=0;i<this->primitives.size();i++){
        pInfo.push_back(PrimitiveInfo(primitives[i]->getBounds(),i));
    }
    
}






Node* BVH::buildBVHWithMidPoint(PrimitivesInfo& primInfo, int start, int end, int* totalNodes, Primitives& orderedPrims){

    (*totalNodes)++;
    BBox nodeBox = BBox::empty();

    for(int i=start; i<end;i++){
        nodeBox.extend(primInfo[i].boundingBox);
    }

    int numPrimitives = end-start;


    if(numPrimitives <= 2){
        //we need to construct the leaf that contains the primitives
        return this->buildLeafNode(numPrimitives, start, end, nodeBox, orderedPrims, primInfo);
    }
    else{
        Node* node = new Node();
        //calcumate the bounding boc of the centroids
        BBox centroidBox = BBox::empty();
        for(int i=start;i<end;i++){
            centroidBox.extend(primInfo[i].bbCenter);
        }

        //check which is the dimension at which the centroid extends the most
        //0 for x, 1 for y, 2 for z
        int dim;

        Vector diag = centroidBox.diagonal();

        //check which of the dimensions of the diagonal is the largest a.k.a on which demension the box spans the most
    
        float x = centroidBox.max.x - centroidBox.min.x;
        float y = centroidBox.max.y - centroidBox.min.y;
        float z = centroidBox.max.z - centroidBox.min.z; 
    

        if(x > y && x > z)
            dim = 0;
        else if(y > z)
            dim = 1;
        else 
            dim = 2;

        //let us now find the midpoint and split the primitives into two separate groups
        float midPoint = (centroidBox.max[dim] + centroidBox.min[dim]) * 0.5f;
        //let us sort according in ascending order according to the centroids max span dimension
        std::sort(&primInfo[start], &primInfo[end-1]+1, [dim](PrimitiveInfo a, PrimitiveInfo b){
            return a.bbCenter[dim] < b.bbCenter[dim];
        });

        //let us fing the first element beyond the minPoint
        int mid;
        
        for(int i = start; i< end;i++){
            if(primInfo[i].bbCenter[dim] >= midPoint){
                mid = i;
                break;
            }
        }

        node->buildInternalNode(dim,
            buildBVHWithMidPoint(primInfo, start, mid, totalNodes, orderedPrims),
            buildBVHWithMidPoint(primInfo, mid, end, totalNodes, orderedPrims)            
        );


        return node;
        
    }

}

Node* BVH::buildBVHWithSAH(PrimitivesInfo& primInfo, int start, int end, int* totalNodes, Primitives& orderedPrims){


    (*totalNodes)++;
    BBox nodeBox = BBox::empty();

    for(int i=start; i<end;i++){
        nodeBox.extend(primInfo[i].boundingBox);
    }

    int numPrimitives = end-start;


    if(numPrimitives <= 2){
        //we need to construct the leaf that contains the primitives
        return this->buildLeafNode(numPrimitives, start, end, nodeBox, orderedPrims, primInfo);
    }
    else{ 
        std::pair<int, int> dimMid = this->SAH(PrimitivesInfo(&primInfo[start], &primInfo[end]), nodeBox.getSurfaceArea());


        if(dimMid.first == -1){
            return buildLeafNode(numPrimitives, start, end, nodeBox, orderedPrims, primInfo);
        }
        else{
            Node* node = new Node();
            std::sort(&primInfo[start], &primInfo[end-1]+1, [dimMid](PrimitiveInfo a, PrimitiveInfo b){
             return a.bbCenter[dimMid.first] < b.bbCenter[dimMid.first];
            });


            node->buildInternalNode(dimMid.first,
                buildBVHWithSAH(primInfo, start, start + dimMid.second, totalNodes, orderedPrims),
                buildBVHWithSAH(primInfo, start + dimMid.second, end, totalNodes, orderedPrims)            
            );


            return node;
        }
        
    }

}



Node* BVH::buildLeafNode(int numPrims, int start, int end, BBox nodeBox, Primitives& orderedPrims, PrimitivesInfo& primInfo){
    
    Node* node = new Node();
    
    int offset = orderedPrims.size();
    for(int i=start; i< end;i++){
        int primIndex = primInfo[i].primitiveNo;
        orderedPrims.push_back(primitives[primIndex]);
    }

    node->buildLeaf(offset, numPrims, nodeBox);
    return node;
}


std::pair<int, int> BVH::SAH(PrimitivesInfo slice, float parentBBoxSA) const{
    int dim = -1;
    int mid = -1;
    float minSAH = __FLT_MAX__;


    for(int j = 0; j < 3 ; j++){

        //sorting the array according to the dimension
        std::sort(slice.begin(), slice.end(), [j](PrimitiveInfo a, PrimitiveInfo b){
             return a.bbCenter[j] < b.bbCenter[j];
         });


        //swipping from left to right and saving the values in an array
        std::vector<float> l2rCosts;
        BBox tempBBox = BBox::empty();
        float primitiveNum = 0.0f;

        //calculating the cost for every step of the swipe
        for(int i = 0; i < slice.size()-1;i++){

            //expanding the bounding box
            tempBBox.extend(slice[i].boundingBox);
            primitiveNum ++;

            //calculating the cost 
            l2rCosts.push_back(primitiveNum * (tempBBox.getSurfaceArea()/parentBBoxSA));
        }


        //swipping from right to left
        tempBBox = BBox::empty();
        primitiveNum = 0.0f;

        for(int i = slice.size()-1; i > 0; i--){
            
            //expanding the bounding box
            tempBBox.extend(slice[i].boundingBox);
            primitiveNum ++;

            float cost = l2rCosts[i-1] + (primitiveNum * (tempBBox.getSurfaceArea()/parentBBoxSA));

            if((minSAH - cost) > __FLT_EPSILON__){
                minSAH = cost;
                dim = j;
                mid = i;
            }

        }

    } 

    //checking if it is worth splitting at all
    if(((float)slice.size() - minSAH) > __FLT_EPSILON__){
        return std::make_pair(dim, mid);
    }
    else{
        return std::make_pair(-1,-1);
    }
    
}


}