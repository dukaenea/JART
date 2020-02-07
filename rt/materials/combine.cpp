#include <rt/materials/combine.h>
#define LOG(x) std::cout<<x<<std::endl;
namespace rt {

CombineMaterial::CombineMaterial()
{
}

void CombineMaterial::add(Material* m, float w) {
    this->materials.push_back(m);
    this->weights.push_back(w);
}

RGBColor CombineMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    //we iterate over all the objects and get the reflectance for all those that do not need sampling
    RGBColor ref = RGBColor::rep(0.f);
    for(int i = 0;i<this->materials.size();i++){
        if(this->materials[i] == nullptr){
            std::cout<<"null material"<<std::endl;
        }
        if(this->materials[i]->useSampling() == Material::SAMPLING_NOT_NEEDED){
            ref = ref + materials[i]->getReflectance(texPoint, normal, outDir, inDir) * this->weights[i];
        }
    }
    return ref;
}

RGBColor CombineMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    RGBColor em = RGBColor::rep(0.f);

    for(int i = 0;i<this->materials.size();i++){
        if(this->materials[i]->useSampling() == Material::SAMPLING_NOT_NEEDED){
            em = em + this->materials[i]->getEmission(texPoint, normal, outDir) * this->weights[i];
        }
    }

    return em;
}

Material::SampleReflectance CombineMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    //since we can at most have 1 material that needs sampling, we can break the loop as soon as we find it
    Material::SampleReflectance sr;
    for(int i = 0;i<this->materials.size();i++){
        if(this->materials[i]->useSampling() == Material::SAMPLING_ALL){
            auto sr1 = this->materials[i]->getSampleReflectance(texPoint, normal, outDir);
            sr = {sr1.direction, sr1.reflectance * this->weights[i]};
            //break;
        }
    }
    return sr;
}

Material::Sampling CombineMaterial::useSampling() const {
    Material::Sampling sampling = Material::SAMPLING_NOT_NEEDED;
    //we check if there is any material that needs sampling
    for(int i=0;i<this->materials.size();i++){
        if(this->materials[i]->useSampling() == Material::SAMPLING_ALL){
            sampling = Material::SAMPLING_SECONDARY;
            break;
        }
    }

    return sampling;
}

}