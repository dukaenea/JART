#ifndef CG1RAYTRACER_INTEGRATORS_VOLRAYTRACING_HEADER
#define CG1RAYTRACER_INTEGRATORS_VOLRAYTRACING_HEADER

#include <rt/integrators/integrator.h>

namespace rt {
class VolumetricIntegrator : public Integrator {
public:
    VolumetricIntegrator(World* world) : Integrator(world) {}
    virtual RGBColor getRadiance(const Ray& ray) const;
};

}

#endif