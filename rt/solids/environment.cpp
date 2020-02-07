#include <rt/solids/environment.h>

namespace rt {

Environment::Environment(CoordMapper* texMapper){
    this->texMapper = texMapper;
}

BBox Environment::getBounds() const {
    //since this is the full environment,the bounding box has to be one that spans all space
    return BBox::full();
}

Intersection Environment::intersect(const Ray& ray, float previousBestDistance) const {
    
    Vector r = ray.d;
    Vector normal;
    if(std::abs(r.x) > std::abs(r.y) && std::abs(r.x) > std::abs(r.z)){
        normal = Vector(-r.x, 0.0f, 0.0f).normalize();
    }
    else if(std::abs(r.y) > std::abs(r.z)){
        normal = Vector(0.0f, -r.y, 0.0f).normalize();
    }
    else{
        normal = Vector(0.0f, 0.0f, -r.z).normalize();
    }

    if(this == nullptr){
        std::cout<<"Im NULL"<<std::endl;
    }
    
    return Intersection(__FLT_MAX__, ray, this, -normal, Point::rep(__FLT_MAX__));
}

Solid::Sample Environment::sample() const {
    NOT_IMPLEMENTED;
}

float Environment::getArea() const {
    return __FLT_MAX__;
}

}
