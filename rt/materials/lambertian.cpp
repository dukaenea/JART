#include <rt/materials/lambertian.h>
#include <rt/textures/texture.h>
#include <core/scalar.h>


namespace rt {

LambertianMaterial::LambertianMaterial(Texture* emission, Texture* diffuse)
{
    this->emissionTexture = emission;
    this->diffusionTexture = diffuse;
}

RGBColor LambertianMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {

    float cos = dot(inDir.normalize(), normal.normalize());
    RGBColor textureColor = this->diffusionTexture->getColor(texPoint) / pi;
    return textureColor * cos;
}

RGBColor LambertianMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return this->emissionTexture->getColor(texPoint);
}

Material::SampleReflectance LambertianMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
	UNREACHABLE;
}

Material::Sampling LambertianMaterial::useSampling() const {
	return SAMPLING_NOT_NEEDED;
}

}