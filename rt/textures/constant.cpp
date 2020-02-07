#include <rt/textures/constant.h>

namespace rt {

ConstantTexture::ConstantTexture()
{
    this->constantColor = RGBColor::rep(0);
}

ConstantTexture::ConstantTexture(const RGBColor& color)
{
    this->constantColor = color;
}

RGBColor ConstantTexture::getColor(const Point& coord) {
    return this->constantColor;
}

RGBColor ConstantTexture::getColorDX(const Point& coord) {
    return this->constantColor;
}

RGBColor ConstantTexture::getColorDY(const Point& coord) {
    return this->constantColor;
}

}