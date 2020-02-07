#include <rt/integrators/castingdist.h>
#include <rt/intersection.h>
#include <rt/world.h>

#define LOG(x) std::cout<<x<<std::endl;


namespace rt {

RayCastingDistIntegrator::RayCastingDistIntegrator(World* world, const RGBColor& nearColor, float nearDist, const RGBColor& farColor, float farDist)
    : Integrator(world)
{
    this->nearColor = nearColor;
    this->farColor = farColor;
    this->farDist = farDist;
    this->nearDist = nearDist;
}

RGBColor RayCastingDistIntegrator::getRadiance(const Ray& ray) const {

    Intersection intersection = this->world->scene->intersect(ray);
    if(intersection){
       
        RGBColor col= this->nearColor;

        if(intersection.distance>this->nearDist && intersection.distance<this->farDist){
            //Interpolate between colors

            float rap = (intersection.distance - this->nearDist)/(this->farDist-this->nearDist);

            float red=(this->farColor.r-this->nearColor.r)*rap + this->nearColor.r;
            float green=(this->farColor.g-this->nearColor.g)*rap + this->nearColor.g;
            float blue=(this->farColor.b-this->nearColor.b)*rap + this->nearColor.b;

            col=RGBColor(red,green,blue);
        }else if(intersection.distance>this->farDist){
            col=this->farColor;
        }

        float coef = dot(-intersection.ray.d, intersection.normal());
        return RGBColor(coef*col.r, coef*col.g, coef*col.b);
    }
    else{
        //std::cout<< "Intersection not found\n";
        return RGBColor::rep(0.0f);
    }
    
}

}
