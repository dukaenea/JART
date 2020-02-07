#include <rt/integrators/raytrace.h>
#include <rt/intersection.h>
#include <rt/world.h>
#include <rt/lights/light.h>
#include <rt/solids/solid.h>
#include <rt/materials/material.h>
#include <rt/coordmappers/coordmapper.h>

#define LOG(x) std::cout<<x<<std::endl;

namespace rt {

RGBColor RayTracingIntegrator::getRadiance(const Ray& ray) const {

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
            Ray shadowRay = Ray(intersection.hitPoint() - (k * ((inDirCos >= 0 && outDirCos >= 0) ? intersection.normal() : -intersection.normal())), lh.direction);
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
        //intersect with the skybox
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