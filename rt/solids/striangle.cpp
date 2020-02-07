#include <rt/solids/striangle.h>
#include <rt/intersection.h>

#define LOG(x) std::cout<<x<<std::endl;


namespace rt {

SmoothTriangle::SmoothTriangle(Point vertices[3], Vector normals[3], CoordMapper* texMapper, Material* material)
{
    this->normals[0] = normals[0];
    this->normals[1] = normals[1];
    this->normals[2] = normals[2];
    this-> verteces[0] = vertices[0];
    this-> verteces[1] = vertices[1];
    this-> verteces[2] = vertices[2];
    this->texMapper = texMapper;
    this->material = material;
}

SmoothTriangle::SmoothTriangle(const Point& v1, const Point& v2, const Point& v3, const Vector& n1, const Vector& n2, const Vector& n3, CoordMapper* texMapper, Material* material)
{
    this->normals[0] = n1;
    this->normals[1] = n2;
    this->normals[2] = n3;
    this-> verteces[0] = v1;
    this-> verteces[1] = v2;
    this-> verteces[2] = v3;
    this->texMapper = texMapper;
    this->material = material;
}

BBox SmoothTriangle::getBounds() const {
    return BBox(
        Point(
            std::min(std::min(verteces[0].x, verteces[1].x), verteces[2].x),
            std::min(std::min(verteces[0].y, verteces[1].y), verteces[2].y),
            std::min(std::min(verteces[0].z, verteces[1].z), verteces[2].z)
        ),
        Point(
            std::max(std::max(verteces[0].x, verteces[1].x), verteces[2].x),
            std::max(std::max(verteces[0].y, verteces[1].y), verteces[2].y),
            std::max(std::max(verteces[0].z, verteces[1].z), verteces[2].z)
        )
    );
}

Intersection SmoothTriangle::intersect(const Ray& ray, float previousBestDistance) const {
     // let's first see if the ray intersects the plane the triangle spans

    // let's construct 2 Vectors that will span the plan and vec2CrossVec1find the normal as their cross product
    Vector vec1 = this->verteces[1] - this->verteces[0];
    //std::cout<<"Vec1 "<<vec1.x<<"  "<<vec1.y<<"  "<<vec1.z<<"\n\n";
    Vector vec2 = this->verteces[2] - this->verteces[0];
    //std::cout<<"Vec2 "<<vec2.x<<"  "<<vec2.y<<"  "<<vec2.z<<"\n\n";

    Vector normal = cross(vec1, vec2).normalize();

    //check if the ray intersects the plane

    float rayNormal = dot(ray.d, normal);

    //std::cout<<normal.x<<"  "<<normal.y<<"  "<<normal.z<<"\n\n";

    if(rayNormal == 0.0f){
        //std::cout<<"Intersection not found\n\n";
        return Intersection::failure();
    }

    float t = dot(this->verteces[0] - ray.o, normal) / rayNormal;

    if(t > previousBestDistance ||  t < 0){
        return Intersection::failure();
    }

    //check if the intersection point is in the triangle
    Point intPoint = ray.getPoint(t);
    Vector vecP = intPoint - this->verteces[0];
    
    Vector vec1CrossVecP = cross(vec1,vecP);
    Vector vec1CrossVec2 = cross(vec1,vec2);

    if(dot(vec1CrossVecP, vec1CrossVec2) < 0.0f){
        return Intersection::failure();
    }

    Vector vec2CrossVecP = cross(vec2,vecP);
    Vector vec2CrossVec1 = cross(vec2,vec1);

    if(dot(vec2CrossVecP, vec2CrossVec1) < 0.0f){
        return Intersection::failure();
    }


    float denominator = vec2CrossVec1.length();
    float gamma = vec1CrossVecP.length()/denominator;
    float beta = vec2CrossVecP.length()/denominator;
    float alpha = 1-beta-gamma;

    if(beta+gamma >= 1.0f){
        return Intersection::failure();
    }

    Vector pointNormal = (this->normals[0] * alpha + this->normals[1] * beta + this->normals[2] * gamma).normalize();

    return Intersection(t, ray, this, pointNormal, Point(alpha, beta,gamma));
    
}
}