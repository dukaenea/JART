#ifndef CG1RAYTRACER_GROUPS_BVH_HEADER
#define CG1RAYTRACER_GROUPS_BVH_HEADER

#include <vector>
#include <rt/groups/group.h>
#include <rt/bbox.h>

namespace rt {


struct PrimitiveInfo{

    PrimitiveInfo(const BBox& boundingBox, int primitiveNo) : primitiveNo(primitiveNo), boundingBox(boundingBox), bbCenter(
            Point(
                (boundingBox.max.x + boundingBox.min.x)*0.5,
                (boundingBox.max.y + boundingBox.min.y)*0.5,
                (boundingBox.max.z + boundingBox.min.z)*0.5
            )
        ){}

    int primitiveNo;
    BBox boundingBox;
    Point bbCenter;
};


struct Node{

    void buildLeaf(int off, int numP, const BBox& bBox){
        malloc(size_t(this));
        primitiveOffset = off;
        numPrimitives = numP;
        boundingBox = bBox;
        children[0] = children[1] = nullptr;
    }

    void buildInternalNode(int sAxis, Node* n1, Node* n2){
        malloc(size_t(this));
        children[0] = n1;
        children[1] = n2;
        BBox b = BBox::empty();
        b.extend(n1->boundingBox);
        b.extend(n2->boundingBox);
        boundingBox = b;
        splitAxis = sAxis;
        numPrimitives = 0;
    }

    BBox boundingBox;
    Node* children[2];
    int splitAxis;
    int primitiveOffset;
    int numPrimitives;

};


struct CompressedNode{

    CompressedNode(){
        malloc(size_t(this));
    }

    BBox boundingBox;
    union{
        int offset;
        int otherChildOffset;
    };
    uint16_t numPrim;
    uint8_t axis;
    uint8_t pad[1];
};



struct SAHSPlitData{
    SAHSPlitData(int dim, int splitInd, float cost) :
        splitDimension(dim),
        splitIndex(splitInd),
        splitCost(cost){}

    int splitDimension;
    int splitIndex;
    float splitCost; 
};



class BVH : public Group {
public:
    BVH();
	virtual ~BVH();

    typedef std::vector<PrimitiveInfo> PrimitivesInfo;
    typedef std::vector<CompressedNode*> CompressedNodes; 

    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const;
    virtual void rebuildIndex();
    virtual void add(Primitive* p);
    virtual void setMaterial(Material* m);
    virtual void setCoordMapper(CoordMapper* cm);

    void initPrimitivesInfo(PrimitivesInfo &pInfo);
    Node* buildBVHWithMidPoint(PrimitivesInfo& primInfo, int start, int end, int* totalNodes, Primitives& orderedPrims);
    Node* buildBVHWithSAH(PrimitivesInfo& primInfo, int start, int end, int* totalNodes, Primitives& orderedPrims);
    Node* buildLeafNode(int numPrims, int start, int end, BBox nodeBox, Primitives& orderedPrims, PrimitivesInfo& primInfo);
    int compressTree(Node* node, int* offset) const;
    std::pair<int, int> SAH(PrimitivesInfo slice, float parentBBoxSA) const;

    CompressedNodes renderingTree;
    
};



}

#endif