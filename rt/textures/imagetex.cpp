#include <rt/textures/imagetex.h>
#include <core/point.h>
#include <core/interpolate.h>

namespace rt {

ImageTexture::ImageTexture()
{
    /* TODO */
}

ImageTexture::ImageTexture(const std::string& filename, BorderHandlingType bh, InterpolationType i)
{
    this->image.readPNG(filename);
    this->bht = bh;
    this->it = i;
}

ImageTexture::ImageTexture(const Image& image, BorderHandlingType bh, InterpolationType i)
{
    this->image = image;
    this->bht = bh;
    this->it = i;
}

RGBColor ImageTexture::getColor(const Point& coord) {
    std::pair<float, float> coords = getImageCoords(coord); 
    return getColorFromImage(coords.first, coords.second);
}

RGBColor ImageTexture::getColorDX(const Point& coord) {
    //just giving it a shot
    std::pair<float, float> imageCoords = getImageCoords(coord);
  
    float prevX = std::fmod((imageCoords.first -1.f) + (float)image.width(), (float)image.width());
    float nextX = std::fmod((imageCoords.first + 1.f), (float)image.width());
    return (getColorFromImage(nextX, imageCoords.second)* 255.f - getColorFromImage(prevX, imageCoords.second)* 255.f)/2.f;
    }

RGBColor ImageTexture::getColorDY(const Point& coord) {
    //just giving it a shot
    std::pair<float, float> imageCoords = getImageCoords(coord);

    float prevY = std::fmod((imageCoords.second -1.f) + (float)image.height(), (float)image.height());
    float nextY = std::fmod((imageCoords.second + 1.f), (float)image.height());
    return (getColorFromImage(imageCoords.first, nextY) * 255.f - getColorFromImage(imageCoords.first, prevY)* 255.f)/2.f;
}

std::pair<float, float> ImageTexture::getImageCoords(const Point& coord){
    float tu = coord.x;
    float tv = coord.y;
    float i;
    float j;

    if(tu < 0.f || tu > 1.f || tv < 0.f || tv > 1.f){
        switch(this->bht){
            case this->REPEAT:
                tu = tu - floor(tu);
                tv = tv - floor(tv);
                break;
            case this->MIRROR:{
                tu = tu - floor(tu);
                tv = tv - floor(tv);
                //getting the latice coords
                int lu = floor(coord.x);
                int lv = floor(coord.y);
                if(lu % 2 == 0){
                    tu = 1-tu;
                }
                if(lv % 2 == 0){
                    tv = 1-tv;
                }
                break;
            }
            case this->CLAMP:{
                tu = std::max(0.f, std::min(1.f, coord.x));
                tv = std::max(0.f, std::min(1.f, coord.y));
                break;
            }
        }
    }


    i = tu * (this->image.width()-1);
    j = tv * (this->image.height()-1);

    return std::pair<float, float>(i,j);
}


RGBColor ImageTexture::getColorFromImage(float u, float v){

    RGBColor imageTextureColor = RGBColor::rep(0);

    switch(this->it){
        case this->NEAREST:{   
            int x = (int)floor(u);
            int y = (int)floor(v);
            imageTextureColor = image(std::min(x , (int)this->image.width()-1), std::min(y , (int)this->image.width() -1));
            break;
        }
        case this->BILINEAR:{
            int x0 = (int)floor(u);
            int y0 = (int)floor(v);
            int x1 = std::min(x0+1, (int)image.width() -1);
            int y1 = std::min(y0+1, (int)image.height() -1);
            imageTextureColor = lerp2d(
                this->image(x0,y0),
                this->image(x1,y0),
                this->image(x0,y1),
                this->image(x1,y1),
                u - x0, v - y0);
            break;
        }
    }
    return imageTextureColor;
}









}