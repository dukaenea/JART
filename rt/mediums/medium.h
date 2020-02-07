#ifndef CG1RAYTRACER_MEDIUMS_MEDIUM_HEADER
#define CG1RAYTRACER_MEDIUMS_MEDIUM_HEADER

#include <core/color.h>
#include <core/point.h>
#include <rt/world.h>

namespace rt{
    class Medium{

        public:
        virtual RGBColor attenuate(Point& start, Point& end) const = 0;
        virtual RGBColor attenuateMarching(Point& start, Point& end) const = 0;
        virtual RGBColor emmit(Point& start, Point& end) const = 0;
        virtual RGBColor inScatter(Point& start, Point& end, World* world) const = 0;


        uint marchingSteps = 50;

        // struct MediumInterface{
            
        //     MediumInterface(){ };
        //     MediumInterface(const Medium *medium) : inside(medium), outside(medium){};
        //     MediumInterface(const Medium *inside, const Medium *outside) : inside(inside), outside(outside) {};
        //     bool transitingMediumn() { return inside != outside; }
        //     const Medium *inside, *outside;  
        // };



    };
} // namespace r







#endif