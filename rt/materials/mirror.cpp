#include <rt/materials/mirror.h>
#include <core/scalar.h>

namespace rt {

MirrorMaterial::MirrorMaterial(float eta, float kappa)
{
    this->eta = eta;
    this->kappa = kappa;
}

RGBColor MirrorMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    return RGBColor::rep(0);
}

RGBColor MirrorMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return RGBColor::rep(0);
}

Material::SampleReflectance MirrorMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    //calculating the cos of the angle between the outgoing direction and the surface normal
    float cosIN = dot(outDir.normalize(), normal.normalize());
    //getting the directio of th reflected ray
    Vector r = (outDir - 2*(cosIN)*normal).normalize();
    RGBColor reflectedColor;
    //we are supposing that there is no air
    float etaOut = 1.f;
    float etaIn = this->eta;
    
    if(this->eta == 0.f && this->kappa == 0.f){
        reflectedColor = RGBColor::rep(1);
    }
    //this is the case when we are dealing with a dielactric
    else if(this->eta != 0.f && this->kappa == 0.f){
        //clamp cos value between -1 and 1
        cosIN = dot(-outDir.normalize(), normal.normalize());


        //we need to swap the values of eta if we are moving from inside the material to the outside
        if(cosIN < 0.f){ //means that we are going out of the material
            std::swap(etaOut, etaIn);
            //since we changed refraction indexes we need to have the cos positive as we are now 'looking' from the inside of the material
            cosIN = std::abs(cosIN);
        }


        //we can compute cosTR using Snells law

        //using trig identity
        float sinIN = std::sqrt(1.f - cosIN * cosIN);
        float sinTR = (etaIn / etaOut) * sinIN;

        float cosTR = std::sqrt(1.f - sinTR * sinTR);


        float par = ((etaOut * cosIN) - (etaIn * cosTR)) / ((etaOut * cosIN) + (etaIn * cosTR));
        float pin = ((etaIn * cosIN) - (etaOut * cosTR)) / ((etaIn * cosIN) + (etaOut * cosTR));


        float fr = 0.5*(par*par + pin*pin);

        reflectedColor = RGBColor::rep(fr);
    }
    //dealing whith the case of having a conductor
    else if(this->eta != 0.f && this->kappa != 0.f){
        cosIN = dot(-outDir.normalize(), normal.normalize());
        float eta = this->eta / etaOut;
        float kappa = this->kappa / etaOut;
        float sinInSq = 1.f - cosIN * cosIN;


        float abSQ = std::sqrt(std::pow((eta*eta - kappa*kappa - cosIN*cosIN), 2) + 4*eta*eta*kappa*kappa); 
        float a = std::sqrt(0.5 * (abSQ + eta*eta - kappa*kappa - sinInSq));

        float pin = (abSQ - (2*a*cosIN) + cosIN*cosIN) / (abSQ + (2*a*cosIN) + cosIN * cosIN);
        float par = pin * ((cosIN * cosIN * abSQ) - (2*a*cosIN * sinInSq) + sinInSq * sinInSq) / ((cosIN * cosIN * abSQ) + (2*a*cosIN * sinInSq) + sinInSq * sinInSq); 

        float fr = 0.5 * (par+pin);
        reflectedColor = RGBColor::rep(fr);
    }
    
    return {r, reflectedColor};
}

Material::Sampling MirrorMaterial::useSampling() const {
    return SAMPLING_ALL;
}

}