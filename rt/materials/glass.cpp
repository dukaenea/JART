#include <rt/materials/glass.h>
#include <math.h>

namespace rt {

GlassMaterial::GlassMaterial(float eta)
{
    this->eta = eta;
}

RGBColor GlassMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    return RGBColor::rep(0);
}

RGBColor GlassMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return RGBColor::rep(0);
}

Material::SampleReflectance GlassMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    //in case of glass a part of the incoming light is reflected and the rest is refracted.
    //we use the the law of reflection to find the direction of the reflected ray
    //we use snells law to find the direction of the refracted ray
    //we use fresnels equations to find what part of the ray is reflected and what part is refracted

    Vector N = normal;
    Vector d = -outDir;
    float cosIN = dot(d.normalize(), N.normalize());
    cosIN = std::max(-1.f, std::min(cosIN, 1.f));
    float etaIn = 1.f;
    float etaOut = this->eta;

     if(cosIN > 0.f){ 
        //means that we are going out of the material
        std::swap(etaOut, etaIn);
    }

    float sinIN = std::sqrt(1.f - cosIN * cosIN);
    sinIN = std::max(0.f, std::min(sinIN, 1.f));
    float sinTR = (etaIn / etaOut) * sinIN;
    float cosTR = std::max(0.f, std::min(std::sqrt(1.f - sinTR * sinTR), 1.f));
    cosIN = std::abs(cosIN);
    float fr;

    //accounting for total reflection
     if(sinTR > 1.f){
         fr = 1.f;
     }
    else{

        float par = ((etaOut * cosIN) - (etaIn * cosTR)) / ((etaOut * cosIN) + (etaIn * cosTR));
        float pin = ((etaIn * cosIN) - (etaOut * cosTR)) / ((etaIn * cosIN) + (etaOut * cosTR));


        fr = 0.5*(par*par + pin*pin);
    }



    //we flip a coint to decide if we will reflect or refract
    float coin  = random();
    if(coin < 0.5f){
        //we reflect
        cosIN = dot(d.normalize(), N.normalize());
        Vector dir = (d - 2*cosIN*N).normalize();
        return {dir, RGBColor::rep(2 * fr)};
    }
    else{
        //we refract
        cosIN = dot(d.normalize(), N.normalize());
        cosIN = std::max(-1.f, std::min(cosIN, 1.f));

        //accounting for total inner reflection
        if(fr >= 1){
            return{(d - 2*cosIN*N).normalize(), RGBColor::rep(1)};
        }

        etaIn = 1.f;
        etaOut = this->eta;


        if(cosIN < 0){
            cosIN = -cosIN;
        }
        else{
            N = -N;
            std::swap(etaIn, etaOut);
        }


        sinIN = std::sqrt(1.f - cosIN * cosIN);
        sinTR = (etaIn / etaOut) * sinIN;

        cosTR = std::sqrt(1.f - sinTR * sinTR);
    
        Vector a = (d + cosIN * N)/sinIN * sinTR; 
        Vector b = -N*cosTR;
        Vector dir = a + b;

        float k = (1.f - fr);
        return {dir, RGBColor::rep(2 * k)};
    }
}

Material::Sampling GlassMaterial::useSampling() const {
    return Material::SAMPLING_ALL;
}

}