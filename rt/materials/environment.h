#ifndef CG1RAYTRACER_MATERIALS_ENV_HEADER
#define CG1RAYTRACER_MATERIALS_ENV_HEADER

#include <rt/materials/material.h>
#include <rt/textures/texture.h>
#include <vector>

namespace rt {

class Texture;

class EnvironmentMaterial : public Material {
public:
    EnvironmentMaterial() {};
    virtual RGBColor getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const;
    virtual RGBColor getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
    virtual SampleReflectance getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const;
    virtual Sampling useSampling() const { return Material::SAMPLING_NOT_NEEDED; }
    void addTexture(Texture* tex);

private:
    std::vector<Texture*> textures;
};

}

#endif