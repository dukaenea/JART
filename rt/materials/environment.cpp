#include <rt/materials/environment.h>
#include <rt/textures/texture.h>
#include <core/point.h>

namespace rt {


RGBColor EnvironmentMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
     return RGBColor::rep(0);
}

RGBColor EnvironmentMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    //std::cout<<"here"<<std::endl;
    Texture* t = this->textures[texPoint.z];
    RGBColor c = t->getColor(texPoint);
    return c;
}

Material::SampleReflectance EnvironmentMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    UNREACHABLE;
}

void EnvironmentMaterial::addTexture(Texture* tex){
    this->textures.push_back(tex);
}

}