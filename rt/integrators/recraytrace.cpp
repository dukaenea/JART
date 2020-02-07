#include <rt/integrators/recraytrace.h>
#include <rt/intersection.h>
#include <rt/world.h>
#include <rt/lights/light.h>
#include <rt/solids/solid.h>
#include <rt/materials/material.h>
#include <rt/coordmappers/coordmapper.h>

namespace rt {

RGBColor RecursiveRayTracingIntegrator::getRadiance(const Ray& ray) const {
  
    RGBColor rad = RGBColor::rep(0);

    Intersection inter = this->world->scene->intersect(ray);

    if(inter){
        //we need to make the cases non excluding in case we have an object with combined materials
        if(inter.solid->material->useSampling() != Material::SAMPLING_NOT_NEEDED){
            rad =  rad + this->getRadianceWithSampling(ray, 6, inter.solid, inter.normal(), inter.hitPoint());
        }
        if(inter.solid->material->useSampling() != Material::SAMPLING_ALL){
            rad = rad + this->getRadianceWithNoSampling(ray);
        }
    }
    else{
          //intersect with the skybox
        Intersection sbInter = world->skybox->intersect(ray);
        float inDirCos = dot(ray.d, sbInter.normal());
        Point hp = world->skybox->texMapper->getCoords(sbInter);
        rad = 0.5 * world->skybox->material->getEmission(hp, (inDirCos > 0) ? sbInter.normal() : -sbInter.normal(), -ray.d);
    
    }

    return rad;

}



RGBColor RecursiveRayTracingIntegrator::getRadianceWithSampling(const Ray& ray, int depth,const Solid* solid, Vector intNormal, Point intPoint) const{

    RGBColor rad = RGBColor::rep(0);

    if(depth == 0){
        return rad;
    }


    Material::SampleReflectance sample = solid->material->getSampleReflectance(intPoint, intNormal, -ray.d);
     float outDirCos = dot(sample.direction, intNormal);

    float k = 1e-4;
    Ray secRay = Ray(intPoint - k*( (outDirCos <= 0) ? intNormal : -intNormal), sample.direction);

    Intersection inter = this->world->scene->intersect(secRay);

    if(inter){

        if(inter.solid->material->useSampling() != Material::SAMPLING_NOT_NEEDED){
            rad =  rad + getRadianceWithSampling(secRay, depth-1, inter.solid, inter.normal(), inter.hitPoint());
        }
        if(inter.solid->material->useSampling() != Material::SAMPLING_ALL){
            rad = rad + getRadianceWithNoSampling(secRay);
        }
    }

    return rad * sample.reflectance;

}







RGBColor RecursiveRayTracingIntegrator::getRadianceWithNoSampling(const Ray& ray) const{
      Intersection intersection = this->world->scene->intersect(ray);
            
    if(intersection){

        RGBColor reflectedLight = RGBColor::rep(0);
        float inDirCos = dot(ray.d, intersection.normal());
         


        for(int i = 0; i < this->world->light.size(); i++){
            
            LightHit lh = this->world->light[i]->getLightHit(intersection.hitPoint());
            float outDirCos = dot(-lh.direction, intersection.normal());
            
            //check if the primary ray and the shadow ray are coming out of the same side of the object
            //check if the cos have the same sign
            if(!((inDirCos > 0 && outDirCos>0) || (inDirCos < 0 && outDirCos<0))){
               continue;
            }

            //tracing the shadow ray
            //if another object was hit there should be a shadow at this point
            //should move the normal to account for the float operation errors
            //k found in an iterative manner
            float k = 1e-4;
            Ray shadowRay = Ray(intersection.hitPoint() - (k * ((inDirCos > 0 && outDirCos>0) ? intersection.normal() : -intersection.normal())), lh.direction);
            Intersection shadowIntersection = this->world->scene->intersect(shadowRay, lh.distance);
            if(shadowIntersection && shadowIntersection.distance < lh.distance){
                continue;
            }





            RGBColor lightIntensity = this->world->light[i]->getIntensity(lh);

            //std::cout<<lightIntensity.r<<" "<<lightIntensity.g<<" "<<lightIntensity.b<<" "<<std::endl;

            
            if(intersection.solid == nullptr){
                std::cout<<"NULL Solid"<<std::endl;
            }

            if(intersection.solid->material == nullptr){
                std::cout<<"NULL Material"<<std::endl;
            }

            Point hp = intersection.hitPoint();

            if(intersection.solid->texMapper != nullptr){
                hp = intersection.solid->texMapper->getCoords(intersection);
            }

            RGBColor a  = intersection.solid->material->getReflectance(
                hp,
                (inDirCos > 0 && outDirCos>0) ? intersection.normal() : -intersection.normal(),
                -ray.d,
                -lh.direction
            ) * lightIntensity;

        
            reflectedLight = reflectedLight + a;

        }

        Point hp = intersection.hitPoint();

        if(intersection.solid->texMapper != nullptr){
            hp = intersection.solid->texMapper->getCoords(intersection);
        }

         RGBColor b = intersection.solid->material->getEmission(hp, (inDirCos > 0) ? intersection.normal() : -intersection.normal(), -ray.d);

        return reflectedLight + b;


    }else{
        RGBColor c = RGBColor::rep(0.f);
        if(this->world->skybox != nullptr){
            Intersection sbInter = world->skybox->intersect(ray);
            float inDirCos = dot(ray.d, sbInter.normal());
            Point hp = world->skybox->texMapper->getCoords(sbInter);
            c = world->skyboxVisibilityFactor * world->skybox->material->getEmission(hp, (inDirCos > 0) ? sbInter.normal() : -sbInter.normal(), -ray.d);
        }
        return c;
    }
}

}