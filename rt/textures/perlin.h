#ifndef CG1RAYTRACER_TEXTURES_PERLIN_HEADER
#define CG1RAYTRACER_TEXTURES_PERLIN_HEADER

#include <rt/textures/texture.h>
#include <vector>

namespace rt {

class PerlinTexture : public Texture {
public:
    PerlinTexture(const RGBColor& white, const RGBColor& black);
    void addOctave(float amplitude, float frequency);
    virtual RGBColor getColor(const Point& coord);
    virtual RGBColor getColorDX(const Point& coord);
    virtual RGBColor getColorDY(const Point& coord);

private:

    std::vector<float> amplitudes;
    std::vector<float> frequencies;
    RGBColor white;
    RGBColor black;
};

}

#endif