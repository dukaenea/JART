#include <rt/materials/cooktorrancematerial.h>
#include <rt/textures/texture.h>
#include <core/scalar.h>
#include <math.h>

namespace rt {

CookTorranceMaterial::CookTorranceMaterial(Texture* specular, float eta,float kappa,float m)
{
    this->specular = specular;
    this->eta = eta;
    this->kappa = kappa;
    this->m = m;
}

RGBColor CookTorranceMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
   UNREACHABLE;
}

RGBColor CookTorranceMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return RGBColor::rep(0);
}

float FresnelTerm(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir,float eta,float kappa){
       //calculating the cos of the angle between the outgoing direction and the surface normal
    float cosIN = dot(outDir.normalize(), normal.normalize());
    //getting the directio of th reflected ray
    Vector r = (outDir - 2*(cosIN)*normal).normalize();
    //we are supposing that there is no air
    float etaOut = 1.f;
    float etaIn =  eta;
    
    if( eta == 0.f &&  kappa == 0.f){
        return 1.0f;
    }
    //this is the case when we are dealing with a dielactric
    else if( eta != 0.f &&  kappa == 0.f){
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


        float par = ((etaOut * cosIN) - (etaIn * cosTR)) / ((etaOut * cosIN) - (etaIn * cosTR));
        float pin = ((etaIn * cosIN) - (etaOut * cosTR)) / ((etaIn * cosIN) - (etaOut * cosTR));


        float fr = 0.5*(par*par + pin*pin);

         return fr;
    }
    //dealing whith the case of having a conductor
   
    cosIN = dot(-outDir.normalize(), normal.normalize());
    eta =  eta / etaOut;
    kappa =  kappa / etaOut;
    float sinInSq = 1.f - cosIN * cosIN;


    float abSQ = std::sqrt(std::pow((eta*eta - kappa*kappa - cosIN*cosIN), 2) + 4*eta*eta*kappa*kappa); 
    float a = std::sqrt(0.5 * (abSQ + eta*eta - kappa*kappa - sinInSq));

    float pin = (abSQ - (2*a*cosIN) + cosIN*cosIN) / (abSQ + (2*a*cosIN) + cosIN * cosIN);
    float par = pin * ((cosIN * cosIN * abSQ) - (2*a*cosIN * sinInSq) + sinInSq * sinInSq) / ((cosIN * cosIN * abSQ) + (2*a*cosIN * sinInSq) + sinInSq * sinInSq); 

    float fr = 0.5 * (par+pin);

    return fr;
    
}
Material::SampleReflectance CookTorranceMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {

     //cos between the incomming ray direction and surface normal
    float cosIN = dot(outDir.normalize(), normal.normalize());
    Vector inDir = (outDir - 2*(cosIN)*normal).normalize();

    Vector H = (inDir + outDir.normalize()).normalize();

    float F = FresnelTerm(texPoint,normal,outDir,inDir,this->eta,this->kappa);
    float alpha = dot(normal.normalize(),H);

    float D = cosh(log(2)/(log(cosh(m*alpha))));

    float G = std::min(1.0f,(std::min((2*dot(normal.normalize(),H)*dot(normal.normalize(),outDir.normalize()))/(dot(outDir.normalize(),H))
    ,(2*dot(normal.normalize(),H)*dot(normal.normalize(),inDir.normalize()))/(dot(outDir.normalize(),H)))));

    float coeff=(F*D*G)/(pi*dot(normal.normalize(),outDir.normalize())*dot(normal.normalize(),inDir.normalize()));

    return {inDir,RGBColor::rep(coeff)};
}

Material::Sampling CookTorranceMaterial::useSampling() const {
	return SAMPLING_ALL;
}


}