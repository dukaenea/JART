#ifndef CG1RAYTRACER_INTEGRATORS_RECURSIVERAYTRACING_HEADER
#define CG1RAYTRACER_INTEGRATORS_RECURSIVERAYTRACING_HEADER

#include <rt/integrators/integrator.h>
#include <rt/solids/solid.h>

namespace rt {

class RecursiveRayTracingIntegrator : public Integrator {
public:
    RecursiveRayTracingIntegrator(World* world) : Integrator(world) {}
    virtual RGBColor getRadiance(const Ray& ray) const;

private:
    RGBColor getRadianceWithNoSampling(const Ray& ray) const;
    RGBColor getRadianceWithSampling(const Ray& ray, int depth, const Solid* solid, Vector intNormal, Point intPoint) const;
};

}

#endif