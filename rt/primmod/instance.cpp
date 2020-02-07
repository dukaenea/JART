#include <rt/primmod/instance.h>
#include <math.h>

namespace rt {

Instance::Instance(Primitive* content)
{
    this->primitive = content;
    this->transformationMatrix = Matrix::identity();
}

Primitive* Instance::content() {
    return this->primitive;
}

void Instance::reset() {
    transformationMatrix = Matrix::identity();
}

void Instance::translate(const Vector& t) {

    Matrix translateMatrix = Matrix::identity();
    translateMatrix[0][3] = t.x;
    translateMatrix[1][3] = t.y;
    translateMatrix[2][3] = t.z;
    this->transformationMatrix = product(translateMatrix, this->transformationMatrix);
}

void Instance::rotate(const Vector& nnaxis, float angle) {

    //calculating the vector perpendicular to nnaxis
    Vector r = nnaxis.normalize();
    Vector s;
    Vector t;
    float compMin = std::min(std::min(fabs(r.x), fabs(r.y)), fabs(r.z));

    if(fabs(compMin - fabs(r.x)) < __FLT_EPSILON__){
        s = Vector(0.0f, -r.z, r.y);
    }
    else if(fabs(compMin -fabs(r.y)) < __FLT_EPSILON__){
        s = Vector(-r.z, 0.0f, r.x);
    }
    else{
        s = Vector(-r.y, r.x, 0.0f);
    }

    s = s.normalize();
    t  = cross(r, s);

    Matrix newBase = Matrix::system(r, s, t);

    Matrix rotationMatrix = Matrix(
        Float4(1.0f, 0.0f, 0.0f, 0.0f),
        Float4(0.0f, std::cos(angle), -std::sin(angle), 0.0f),
        Float4(0.0f, std::sin(angle), std::cos(angle), 0.0f),
        Float4(0.0f, 0.0f, 0.0f, 1.0f)
    );
   auto rotation = product(newBase.transpose().invert(), product(rotationMatrix, newBase.transpose()));
   this->transformationMatrix = product(rotation, this->transformationMatrix);

}

void Instance::scale(float f) {

    Matrix scaleMatrix = Matrix(
        Float4(f, 0.0f, 0.0f, 0.0f),
        Float4(0.0f, f, 0.0f, 0.0f),
        Float4(0.0f, 0.0f, f, 0.0f),
        Float4(0.0f, 0.0f, 0.0f, 1)
    );
    
    this->transformationMatrix = product(scaleMatrix, this->transformationMatrix);
}

void Instance::scale(const Vector& s) {


    Matrix scaleMatrix = Matrix(
        Float4(s.x, 0.0f, 0.0f, 0.0f),
        Float4(0.0f, s.y, 0.0f, 0.0f),
        Float4(0.0f, 0.0f, s.z, 0.0f),
        Float4(0.0f, 0.0f, 0.0f, 1)
    );
    this->transformationMatrix = product(scaleMatrix, this->transformationMatrix);
}

void Instance::setMaterial(Material* m) {
    /* TODO */ NOT_IMPLEMENTED;
}

void Instance::setCoordMapper(CoordMapper* cm) {
    /* TODO */ NOT_IMPLEMENTED;
}

Intersection Instance::intersect(const Ray& ray, float previousBestDistance) const {
  

    auto a  = transformationMatrix.invert();

    Point tranOrigin = a * ray.o;
    Vector tranDirection = (a * ray.d).normalize();

    Ray tempRay = Ray(tranOrigin, tranDirection);

    Intersection inter  = Intersection::failure();
    inter = this->primitive->intersect(tempRay, previousBestDistance); 

    
    auto b = a.transpose();
    Vector v = b * inter.nrm;

    if(inter){
        inter.ray = ray;
        inter.nrm = (v).normalize();
        inter.distance = (transformationMatrix * tempRay.d * inter.distance).length(); 
    }
    return inter;
}

BBox Instance::getBounds() const {

    //transformin the bounding box
    BBox box = BBox::empty();
    Point min = this->primitive->getBounds().min;
    Point max = this->primitive->getBounds().max;

    box.extend(transformationMatrix * min);
    box.extend(transformationMatrix * Point(max.x, min.y, min.z));
    box.extend(transformationMatrix * Point(max.x, max.y, min.z));
    box.extend(transformationMatrix * Point(min.x, max.y, min.z));
    box.extend(transformationMatrix * Point(max.x, min.y, max.z));
    box.extend(transformationMatrix * Point(min.x, max.y, min.z));
    box.extend(transformationMatrix * Point(min.x, min.y, max.z));
    box.extend(transformationMatrix * max);

    return box;
}

}