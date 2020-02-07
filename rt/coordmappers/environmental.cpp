#include <rt/coordmappers/environmental.h>
#include <rt/intersection.h>
#include <core/scalar.h>

namespace rt {

Point EnvironmentalCoordMapper::getCoords(const Intersection& hit) const {
    //we will try to implemet a cubical environment mapper
    //the ray will intersect with only one of the sides of the cube, thus one coordinate will always be dropped 
    //we will use the convention of writing it as inf or -inf

    //let us check which plane the ray will hit by checking the direction of the ray
    //1 for x
    //2 for y
    //3 for z
    //the unfolded cube image where we based our mapping on can be found here https://community.arm.com/developer/tools-software/graphics/b/blog/posts/reflections-based-on-local-cubemaps-in-unity
    //in our case we jave to flip the v coord because our images are centeren on the top left
    Vector r = hit.ray.d;
    int dir;

    float u, v, face, den;

    if(std::abs(r.x) > std::abs(r.y) && std::abs(r.x) > std::abs(r.z)){
        dir = 1;
    }
    else if(std::abs(r.y) > std::abs(r.z)){
        dir = 2;
    }
    else{
        dir = 3;
    }


    //we map u and v to values from -1 to 1 depending on the direction

    if(dir == 1){
        den = std::abs(r.x);
        if(r.x > 0){
            u = -r.z;
            v = -r.y;
            face = 0;
        }
        else{
            u = r.z;
            v = -r.y;
            face = 1;
        }
    }

    if(dir == 2){
        den = std::abs(r.y);
        if(r.y > 0){
            u = r.x;
            v = r.z;
            face = 2;
        }
        else{
            u = r.x;
            v = -r.z;
            face = 3;
        }
    }

    if(dir == 3){
        den = std::abs(r.z);
        if(r.z > 0){
            u = r.x;
            v = -r.y;
            face = 4;
        }
        else{
            u = -r.x;
            v = -r.y;
            face = 5;
        }
    }

    //we need to map the coordinates from 0 to 1 in order to use the image texture qe already have
    //we need to devide by the dominant axis coordinate in order to know how much the other 2 coords change if the dominant one changes by 1 unit
    return Point(
        0.5f * (u/den + 1.f),
        0.5f * (v/den + 1.f),
        face
    );
    
}

}

