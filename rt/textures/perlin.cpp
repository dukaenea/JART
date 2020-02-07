#include <rt/textures/perlin.h>
#include <math.h>
#include <core/point.h>
#include <core/interpolate.h>

namespace rt {

// returns a value in range -1 to 1
static inline float noise(int x, int y, int z) {
    int n = x + y * 57 + z * 997;
    n = (n << 13) ^ n;
    return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}

PerlinTexture::PerlinTexture(const RGBColor& white, const RGBColor& black)
{
    this->white = white;
    this->black = black;
}

rt::RGBColor PerlinTexture::getColor(const Point& coord) {
    
    float finalWeight = 0.f;

    //iterating over all the textures
    for(int i = 0; i<this->amplitudes.size(); i++){
        //get noise coords and offset
        int nx = floor(coord.x * frequencies[i]);
        int ny = floor(coord.y * frequencies[i]);
        int nz = floor(coord.z * frequencies[i]);

        float dx = coord.x * frequencies[i] - nx;
        float dy = coord.y * frequencies[i] - ny;
        float dz = coord.z * frequencies[i] - nz;

        //compute the weights;
        //we need the weights to be able to do a cubical interpolation on a unit cube
        float w000 = noise(nx, ny, nz);
        float w100 = noise(nx + 1, ny, nz);
        float w010 = noise(nx, ny + 1, nz);
        float w001 = noise(nx, ny, nz + 1);
        float w110 = noise(nx + 1, ny + 1, nz);
        float w011 = noise(nx, ny + 1, nz + 1);
        float w101 = noise(nx + 1, ny, nz + 1);
        float w111 = noise(nx + 1, ny + 1, nz + 1);

        float noise = lerp3d(w000, w100, w010, w110, w001, w101, w011, w111, dx, dy, dz);

        finalWeight += amplitudes[i] * noise;
    }

    //trying to remap the weight between 0 and 1 in order to be able to interpolate between the colors and clamping between 0 and 1 in case it exceeds [-1. 1]
    finalWeight = (finalWeight + 1.0f) * 0.5f;
    finalWeight = std::max(0.f, std::min(1.f,finalWeight));

    return lerp(this->black, this->white, finalWeight);
}

rt::RGBColor PerlinTexture::getColorDX(const Point& coord) {
    /* TODO */ NOT_IMPLEMENTED;
}

rt::RGBColor PerlinTexture::getColorDY(const Point& coord) {
    /* TODO */ NOT_IMPLEMENTED;
}

void PerlinTexture::addOctave(float amplitude, float frequency) {
    this->amplitudes.push_back(amplitude);
    this->frequencies.push_back(frequency);

}

}