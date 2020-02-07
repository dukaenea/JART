#include <rt/primmod/bmap.h>
#include <core/assert.h>
#include <rt/solids/triangle.h>
#include <core/color.h>
#include <rt/textures/texture.h>
#include <core/interpolate.h>
#include <core/matrix.h>


namespace rt {

BumpMapper::BumpMapper(SmoothTriangle* base, Texture* bumpmap, const Point& bv1, const Point& bv2, const Point& bv3, MappingType mappingtype, float vscale, bool flipgreen)
{
    this->base = base;
    this->bumpmap = bumpmap;
    this->verteces[0] = bv1;
    this->verteces[1] = bv2;
    this->verteces[2] = bv3;
    this->vScale = vscale;
    this->mappingtype = mappingtype;
    this->flipGreenChannel = flipgreen;
}

BBox BumpMapper::getBounds() const {
    return this->base->getBounds();
}

Intersection BumpMapper::intersect(const Ray& ray, float previousBestDistance) const {
    //getting the uv texture coordinates
    Intersection intersection = this->base->intersect(ray, previousBestDistance);

    if(intersection){
        Point textureHP = lerpbar(this->verteces[0], this->verteces[1], this->verteces[2], intersection.local().x, intersection.local().y);



        //constructing the world matrix
        Vector xT = (this->base->verteces[1] - this->base->verteces[0]).normalize();
        Vector yT = (this->base->verteces[2] - this->base->verteces[0]).normalize(); 
        Vector zT = cross(xT,yT).normalize();
        Matrix worldMatrix = Matrix::system(xT, yT, zT);

        //constructing the texture matrix
        Vector x = (this->verteces[1] - this->verteces[0]).normalize();
        Vector y = (this->verteces[2] - this->verteces[0]).normalize(); 
        Vector z = cross(x,y).normalize();
        Matrix textureMatrix = Matrix::system(x, y, z);

         bool flipped = false;
        if(zT.x + zT.y + zT.z <= 0.f)
            flipped = true;

        if(this->mappingtype == MappingType::BUMP){
            float dx = this->bumpmap->getColorDX(textureHP).b * vScale;
            float dy = this->bumpmap->getColorDY(textureHP).b * vScale;


            Vector wx = worldMatrix * (textureMatrix * Vector(flipped?1:-1, 0, 0));
            Vector wy = worldMatrix * (textureMatrix * Vector(0,flipped?1:-1, 0));

            Vector newNormal = (intersection.normal()  + (dx * cross(intersection.normal(), wy) - dy * cross(intersection.normal(), wx))).normalize();
            intersection.nrm = newNormal;
        }
        else if(this->mappingtype == MappingType::NORMAL){
             //calculating the tangent and bitangent vector
            float f = 1.0f / (x.x * y.y - y.x * x.y);

            float tX = f * (y.y * xT.x - x.y * yT.x);
            float tY = f * (y.y * xT.y - x.y * yT.y);
            float tZ = f * (y.y * xT.z - x.y * yT.z);
            Vector tangent = Vector(tX, tY, tZ).normalize();


            float btX = f * (-y.x * xT.x + x.x * yT.x);
            float btY = f * (-y.x * xT.y + x.x * yT.y);
            float btZ = f * (-y.x * xT.z + x.x * yT.z);
            Vector bitangent = Vector(btX, btY, btZ).normalize();  

            Vector btNormal = cross(tangent, bitangent).normalize();


            Matrix BTN = Matrix::system(tangent, bitangent, btNormal);

            RGBColor sampledPixel = 2.f * bumpmap->getColor(textureHP) - RGBColor::rep(1.f);
            float greenChannel;
            if(this->flipGreenChannel){
                greenChannel = -sampledPixel.g;
            }else{
                greenChannel = flipped ? -sampledPixel.g : sampledPixel.g;
            }
            Vector sampledNormal = Vector(sampledPixel.r, greenChannel, flipped ? sampledPixel.b : sampledPixel.b
                                    ).normalize();
            intersection.nrm = (BTN * sampledNormal); 
        }
       
    }
    return intersection;
}

void BumpMapper::setMaterial(Material* m) {
    this->base->setMaterial(m);
}

void BumpMapper::setCoordMapper(CoordMapper* cm) {
    this->base->setCoordMapper(cm);
}

}