#include <rt/integrators/volint.h>
#include <rt/intersection.h>
#include <rt/world.h>
#include <rt/lights/light.h>
#include <rt/solids/solid.h>
#include <rt/materials/material.h>
#include <rt/coordmappers/coordmapper.h>

#define LOG(x) std::cout<<x<<std::endl;


namespace rt {

RGBColor VolumetricIntegrator::getRadiance(const Ray& ray) const {

    Intersection intersection = this->world->scene->intersect(ray);
    RGBColor rad = RGBColor::rep(0.f);
    RGBColor att = RGBColor::rep(1.f);
    RGBColor emm = RGBColor::rep(0.f);
    RGBColor ins = RGBColor::rep(0.f);
    Point start = ray.o;

    //the distance check here works as a far plane in order for us to make volume rendering more efficient while still producing a visually correct result
    if(intersection && intersection.distance <= 50.f){

        //comented out so we can do ray marching only if we hit the skybox. done for performance reasons
        //accounting for the aditional light reflected by the fog (camera to solid/infinity)
        // Point end = intersection.hitPoint();
        // att = att * ray.medium->attenuateMarching(start, end);
        // emm = emm + ray.medium->emmit(start, end);
        // ins = ins + ray.medium->inScatter(start, end, this->world);
         
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
            Ray shadowRay = Ray(intersection.hitPoint() - (k * ((inDirCos > 0 && outDirCos>0) ? intersection.normal() : -intersection.normal())), lh.direction, intersection.ray.medium);
            Intersection shadowIntersection = this->world->scene->intersect(shadowRay, lh.distance);
            if(shadowIntersection && shadowIntersection.distance < lh.distance){
                continue;
            }


            //we will only calculate the attenuation since the reflected light from the shadow rays is not needed
            auto start = shadowRay.o;
            auto end = shadowIntersection.hitPoint();
            RGBColor attenuation = RGBColor::rep(1.f); //* shadowRay.medium->attenuate(start, end);

            //comented out so we can do ray marching only if we hit the skybox. done for performance reasons
            // while (true) {
            //     shadowIntersection = world->scene->intersect(shadowRay, lh.distance);
            //     if (!shadowIntersection) {
            //         break;
            //     }

            //     if (shadowRay.medium != nullptr) {
            //         auto end = shadowIntersection.hitPoint();
            //         attenuation = attenuation * shadowRay.medium->attenuateMarching(shadowRay.o, end);
            //     }
            //     Vector normal2 = shadowIntersection.normal();
            //     if (dot(shadowRay.d, shadowIntersection.normal()) > 0.0f) {
            //         normal2 = -shadowIntersection.normal();
            //     }
            //     shadowRay = Ray(shadowIntersection.hitPoint() - (k*normal2), shadowRay.d,
            //               shadowIntersection.ray.medium);
            // }

            RGBColor lightIntensity = this->world->light[i]->getIntensity(lh);
    


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

            reflectedLight = reflectedLight + a;// * attenuation;

        }

       
        Point hp = intersection.hitPoint();
       
        if(intersection.solid->texMapper != nullptr){
            hp = intersection.solid->texMapper->getCoords(intersection);
        }

        RGBColor b = intersection.solid->material->getEmission(hp, (inDirCos > 0) ? intersection.normal() : -intersection.normal(), -ray.d);
    
        return reflectedLight + b;//(reflectedLight * att)  + b + emm + ins;


    }
    else{
        RGBColor c = RGBColor::rep(0);
        if(this->world->skybox != nullptr){
            Intersection sbInter = world->skybox->intersect(ray);
            float inDirCos = dot(ray.d, sbInter.normal());
            Point hp = world->skybox->texMapper->getCoords(sbInter);
            c = world->skyboxVisibilityFactor * world->skybox->material->getEmission(hp, (inDirCos > 0) ? sbInter.normal() : -sbInter.normal(), -ray.d);
        }
        //we take the plane at z = 100 to be at infinity so we do not get infinite marching
        Point end = ray.getPoint(50);
        Point start = ray.getPoint(20);
        emm = emm + ray.medium->emmit(start, end);  
        ins = ins + ray.medium->inScatter(start, end, this->world);
        return emm+ins + c;
    }
}

}