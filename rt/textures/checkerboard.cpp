#include <rt/textures/checkerboard.h>
#include <core/point.h>
#include <math.h>

namespace rt {

CheckerboardTexture::CheckerboardTexture(const RGBColor& white, const RGBColor& black)
{
    this->black = black;
    this->white = white;
}

RGBColor CheckerboardTexture::getColor(const Point& coord) {
   //calculating the lattice coordinates
   float lu = floor(coord.x - floor(coord.x) + this->edgeLength);
   float lv = floor(coord.y - floor(coord.y) + this->edgeLength);
   float lw = floor(coord.z - floor(coord.z) + this->edgeLength);

    if(fmod((lu+lv+lw) , 2.f) == 1.f){
        return this->black;
    }
    else{
        return this->white;
    }
}

RGBColor CheckerboardTexture::getColorDX(const Point& coord) {
    /* TODO */ NOT_IMPLEMENTED;
}

RGBColor CheckerboardTexture::getColorDY(const Point& coord) {
    /* TODO */ NOT_IMPLEMENTED;
}

bool CheckerboardTexture::isOdd(int nr)const{
    return nr % 2 != 0;
}

}