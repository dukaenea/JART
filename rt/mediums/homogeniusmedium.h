#ifndef CG1RAYTRACER_MEDIUMS_HOMOGENIOUSMEDIUM_HEADER
#define CG1RAYTRACER_MEDIUMS_HOMOGENIOUSMEDIUM_HEADER

#include <rt/mediums/medium.h>
#include <rt/world.h>

namespace rt{


    class HomogeniousMedium : public Medium{
        public:

            HomogeniousMedium(RGBColor sigma_a, RGBColor sigma_s, float g, RGBColor emission) : 
                            sigmaA(sigma_a), sigmaS(sigma_s), 
                            sigmaT(sigma_a + sigma_s), g(g), emission(emission) {};
            
            RGBColor attenuate(Point& start, Point& end) const;
            RGBColor attenuateMarching(Point& start, Point& end) const;
            RGBColor emmit(Point& start, Point& end) const;
            RGBColor inScatter(Point& start, Point& end, World* world) const;
            RGBColor marchSecondaryRays(World* world, Intersection intersection) const;

        private:
            RGBColor sigmaA, sigmaS, sigmaT; 
            float g;
            RGBColor emission;
    };



}
#endif