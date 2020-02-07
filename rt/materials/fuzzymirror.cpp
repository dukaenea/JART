#include <rt/materials/fuzzymirror.h>
#include <math.h>

namespace rt {

FuzzyMirrorMaterial::FuzzyMirrorMaterial(float eta, float kappa, float fuzzyangle)
{
    this->eta = eta;
    this->kappa = kappa;
    this->fuzzyangle = fuzzyangle;
}

RGBColor FuzzyMirrorMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    return RGBColor::rep(0);
}

RGBColor FuzzyMirrorMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return RGBColor::rep(0);
}

Material::SampleReflectance FuzzyMirrorMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    //calculating the cos of the angle between the outgoing direction and the surface normal
    Vector d = -outDir;
    float cosIN = dot(d.normalize(), normal.normalize());
    //getting the directio of th reflected ray
    Vector r = (d - 2*(cosIN)*normal).normalize();

    //we need to pertrube the direction of the reflected ray

    //we can calculate the radius of the disc knowing the distance from the hitpoint and fuzzyangle
    float radius = std::tan(fuzzyangle);
    Disc disc = Disc(texPoint + r, -r, radius, nullptr, nullptr);
    r = disc.sample().point - texPoint;

    cosIN = dot(r.normalize(), normal.normalize());
    cosIN = std::max(-1.f, std::min(1.f, cosIN));
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


        //we need to swap the values of eta if we are moving from inside the material to the outside
        if(cosIN < 0.f){ //means that we are going out of the material
            std::swap(etaOut, etaIn);
            //since we changed refraction indexes we need to have the cos positive as we are now 'looking' from the inside of the material
        }


        //we can compute cosTR using Snells law

        //using trig identity
        float sinIN = std::sqrt(1.f - cosIN * cosIN);
        sinIN = std::max(0.f, std::min(sinIN, 1.f));
        float sinTR = (etaIn / etaOut) * sinIN;

        float cosTR = std::sqrt(1.f - sinTR * sinTR);


        float par = ((etaOut * cosIN) - (etaIn * cosTR)) / ((etaOut * cosIN) + (etaIn * cosTR));
        float pin = ((etaIn * cosIN) - (etaOut * cosTR)) / ((etaIn * cosIN) + (etaOut * cosTR));


        float fr = 0.5*(par*par + pin*pin);

        reflectedColor = RGBColor::rep(fr);
    }
    //dealing whith the case of having a conductor
    else if(this->eta != 0.f && this->kappa != 0.f){
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

Material::Sampling FuzzyMirrorMaterial::useSampling() const {
    return SAMPLING_ALL;
}

}
