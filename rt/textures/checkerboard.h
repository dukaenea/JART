#ifndef CG1RAYTRACER_TEXTURES_CHECKERBOARD_HEADER
#define CG1RAYTRACER_TEXTURES_CHECKERBOARD_HEADER

#include <rt/textures/texture.h>

namespace rt {

class CheckerboardTexture : public Texture {
public:
    CheckerboardTexture(const RGBColor& white, const RGBColor& black);
    virtual RGBColor getColor(const Point& coord);
    virtual RGBColor getColorDX(const Point& coord);
    virtual RGBColor getColorDY(const Point& coord);


private:
    RGBColor white;
    RGBColor black;
    float edgeLength = 0.5;
    bool isOdd(int nr) const;
};

}

#endif