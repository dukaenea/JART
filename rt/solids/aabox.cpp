#include <rt/solids/aabox.h>

namespace rt {

AABox::AABox(const Point& corner1, const Point& corner2, CoordMapper* texMapper, Material* material)
{
    this->corner1 = corner1;
    this->corner2 = corner2;
    this->texMapper = texMapper;
    this->material = material;
}

BBox AABox::getBounds() const {
    return BBox(corner1, corner2);
}

Solid::Sample AABox::sample() const {
    NOT_IMPLEMENTED;
}

float AABox::getArea() const {
    /* TODO */ NOT_IMPLEMENTED;
}

void swap(float & a, float & b){
    float temp = a;
    a = b;
    b = temp;
}


Intersection AABox::intersect(const Ray& ray, float previousBestDistance) const {
    
    char intPlane = 'x'; //so we know the plane parallel to which axis is intersected
    bool intAtMin = true; //so we know which of the 2 paralell planes is intersected true for corner1 and flase for corner 2
    bool swapped = false;

    float tmin = (corner1.x - ray.o.x) / ray.d.x;
    float tmax = (corner2.x - ray.o.x) / ray.d.x;

    if(tmin > tmax){
        swap(tmin, tmax);
        intAtMin = false;
    } 

    float ytmin = (corner1.y - ray.o.y) / ray.d.y;
    float ytmax = (corner2.y - ray.o.y) / ray.d.y;


    if(ytmin > ytmax){
        swap(ytmin, ytmax);
        swapped = true;
    }

    if((tmin > ytmax) || (ytmin > tmax)){
         //std::cout<<"Returned xy \n";
         return Intersection::failure();
    }


    if(ytmin > tmin){
        tmin = ytmin;
        intPlane = 'y';
        if(swapped)
            intAtMin = false;
        else
            intAtMin = true;
        swapped = false;
    }

    if(ytmax < tmax)
        tmax = ytmax;

    
    float ztmin = (corner1.z - ray.o.z) / ray.d.z;
    float ztmax = (corner2.z - ray.o.z) / ray.d.z;

    if(ztmin > ztmax){
        swap(ztmin, ztmax);
        swapped = true;
    } 

    if((ztmin > tmax) || (ztmax < tmin)){
        //std::cout<<"Returned yz\n";
        return Intersection::failure();
    }

    if(ztmin > tmin){
        tmin = ztmin;
        intPlane = 'z';
        if(swapped)
            intAtMin = false;
        else
            intAtMin = true;
        swapped = false;
    }

    // At this point we know we have an intersection and the distancs is tmin

    if(tmin > previousBestDistance || tmin < 0.0f){
        return Intersection::failure();
    }

    Point intPoint = ray.getPoint(tmin);    


    switch(intPlane){
        case 'x':
            if(intAtMin){
                return Intersection(tmin, ray, this, Vector(-1,0,0), intPoint);
            }
            else{
                return Intersection(tmin, ray, this, Vector(1,0,0), intPoint);
            }
            break;
        case 'y':
            if(intAtMin){
                return Intersection(tmin, ray, this, Vector(0,-1,0), intPoint);
            }
            else{
                return Intersection(tmin, ray, this, Vector(0,1,0), intPoint);
            }
            break;
        case 'z':
            if(intAtMin){
                return Intersection(tmin, ray, this, Vector(0,0,-1), intPoint);
            }
            else{
                return Intersection(tmin, ray, this, Vector(0,0,1), intPoint);
            }
            break;
    }

}


}
