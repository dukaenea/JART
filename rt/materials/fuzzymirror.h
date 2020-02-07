#ifndef CG1RAYTRACER_MATERIALS_FUZZYMIRROR_HEADER
#define CG1RAYTRACER_MATERIALS_FUZZYMIRROR_HEADER

#include <rt/materials/material.h>
#include <rt/solids/disc.h>

namespace rt {

class FuzzyMirrorMaterial : public Material {
public:
    FuzzyMirrorMaterial(float eta, float kappa, float fuzzyangle);
    virtual RGBColor getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const;
    virtual RGBColor getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
    virtual SampleReflectance getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
    virtual Sampling useSampling() const;

private:
    float eta;
    float kappa;
    float fuzzyangle;
};

}

#endif