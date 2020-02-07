#ifndef CG1RAYTRACER_TEXTURES_IMAGETEX_HEADER
#define CG1RAYTRACER_TEXTURES_IMAGETEX_HEADER

#include <core/image.h>
#include <rt/textures/texture.h>

namespace rt {

class ImageTexture : public Texture {
public:
    enum BorderHandlingType {
        CLAMP,
        MIRROR,
        REPEAT
    };

    enum InterpolationType {
        NEAREST,
        BILINEAR
    };

    ImageTexture();
    ImageTexture(const Image& image, BorderHandlingType bh=REPEAT, InterpolationType i=BILINEAR);
    ImageTexture(const std::string& filename, BorderHandlingType bh=REPEAT, InterpolationType i=BILINEAR);
    virtual RGBColor getColor(const Point& coord);
    virtual RGBColor getColorDX(const Point& coord);
    virtual RGBColor getColorDY(const Point& coord);
    std::pair<float, float> getImageCoords(const Point&coord);
    RGBColor getColorFromImage(float x, float y);

private:

    BorderHandlingType bht;
    InterpolationType it;
    Image image;
    float _sobelX[3][3] = {
        {-1.f, 0.f, -1.f},
        {-2.f, 0.f, -2.f},
        {-1.f, 0.f, -1.f},
    };

    float _sobelY[3][3] = {
        {-1.f, -2.f, -1.f},
        {0.f, 0.f, 0.f},
        {-1.f, -2.f, -1.f},
    };
};

}

#endif