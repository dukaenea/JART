#include <rt/groups/simplegroup.h>

namespace rt {

BBox SimpleGroup::getBounds() const {

    //starting with an empty bounding box
    BBox groupBox = BBox::empty();

    //iterating over all the primitives of the group and extnding our box
    for(int i = 0; i < primitives.size();i++){
        BBox primitiveBBox = primitives[i]->getBounds();
        groupBox.extend(primitiveBBox);
    }

    return groupBox;
}

Intersection SimpleGroup::intersect( const Ray& ray, float previousBestDistance) const {
    
    Intersection minDistanceIntersection = Intersection::failure();
    //iterate over all the objects of the group and get the intersection with the shortest distance
    for(int i = 0; i < primitives.size(); i++){
        Intersection inter = primitives[i]->intersect(ray, previousBestDistance);
        if(inter){
            minDistanceIntersection = inter;
            previousBestDistance = inter.distance;
        }
    }

    //std::cout<<"\n\n"<<minDistanceIntersection.distance;

    return minDistanceIntersection; 
}

void SimpleGroup::rebuildIndex() {
    //do nothing
}

void SimpleGroup::add(Primitive* p) {
    this->primitives.push_back(p);
}

void SimpleGroup::setMaterial(Material* m){
    for(int i=0;i<this->primitives.size();i++){
        primitives[i]->setMaterial(m);
    }
}

void SimpleGroup::setCoordMapper(CoordMapper* cm) {
    /* TODO */ NOT_IMPLEMENTED;
}

}
