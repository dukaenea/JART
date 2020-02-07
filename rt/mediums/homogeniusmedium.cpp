#include <rt/mediums/homogeniusmedium.h>
#include <core/point.h>
#include <core/vector.h>
#include <rt/intersection.h>
#include <rt/ray.h>
#include <rt/lights/light.h>
#include <core/random.h>
#include <math.h>

namespace rt{

    RGBColor HomogeniousMedium::attenuate(Point& start, Point& end) const{
        //since we have a constant sigma_t, we can computer attenuation as below
        RGBColor exponent = RGBColor::rep((end-start).length()) * this->sigmaT;
        return {std::exp(-exponent.r), std::exp(-exponent.g), std::exp(-exponent.b)};
    }

    RGBColor HomogeniousMedium::attenuateMarching(Point& start, Point& end) const{
        // float dist = (end-start).length();
        // float stepSize = random(dist/200, dist/100);
        float deltaT = (end - start).length() / this->marchingSteps;
        //int steps = dist / stepSize;
        Vector drivingVector = (end - start).normalize() * deltaT;
        RGBColor accumulator = RGBColor::rep(0.f);
        Point currentPoint = start;

        //we march
        for(int i = 0; i < this->marchingSteps; i++){
            //we step
            currentPoint = currentPoint + drivingVector;
            accumulator = accumulator + attenuate(currentPoint, end) * deltaT;
        }

        return accumulator;   
    }

    RGBColor HomogeniousMedium::emmit(Point& start, Point& end) const{
        //find the step length
        // float dist = (end-start).length();
        // float stepSize = random(dist/200, dist/100);
        float deltaT = (end - start).length() / this->marchingSteps;
        //int steps = dist / stepSize;
        Vector drivingVector = (end - start).normalize() * deltaT;
        RGBColor accumulator = RGBColor::rep(0.f);
        Point currentPoint = start;

        //we march
        for(int i = 0; i < this->marchingSteps; i++){
            //we step
            currentPoint = currentPoint + drivingVector;
            accumulator = accumulator + emission * attenuate(currentPoint, end) * deltaT;
        }

        return accumulator;   
    }

    RGBColor HomogeniousMedium::inScatter(Point& start, Point& end, World* world) const{
         //find the step length
        // float dist = (end-start).length();
        // float stepSize = random(dist/200, dist/100);
        float deltaT = (end - start).length() / this->marchingSteps;
        //int steps = dist / stepSize;
        Vector drivingVector = (end - start).normalize() * deltaT;
        RGBColor accumulator = RGBColor::rep(0.f);
        Point currentPoint = start;

        //we march
        for(int i = 0; i < this->marchingSteps; i++){
            //we step
            currentPoint = currentPoint + drivingVector;
            Ray r =  Ray(start, (currentPoint - start).normalize(), this);
            Intersection currentIntersection = Intersection((currentPoint - start).length(),r , nullptr, {}, currentPoint);
            accumulator = accumulator + this->sigmaS * attenuate(currentPoint, end) * marchSecondaryRays(world, currentIntersection) * deltaT;
        }

        return accumulator;   
    }

    RGBColor HomogeniousMedium::marchSecondaryRays(World* world, Intersection intersection) const{
        //trace shadow rays towards all the light sources in the world
        RGBColor rad = RGBColor::rep(0.f);

        for(uint i = 0; i < world->light.size(); i++){
            RGBColor atten = RGBColor::rep(1.f);

            LightHit lh = world->light[i]->getLightHit(intersection.hitPoint());
            Ray shadowRay = Ray(intersection.hitPoint(), lh.direction, intersection.ray.medium);
            Intersection inter = world->scene->intersect(shadowRay, lh.distance);
            if(inter && inter.distance < lh.distance){
                continue;
            }
            //atten = atten * shadowRay.medium->attenuate(start, end);

            while (true) {
                inter = world->scene->intersect(shadowRay, lh.distance);
                if (!inter) {
                    break;
                }

                if (shadowRay.medium != nullptr) {
                    auto end = inter.hitPoint();
                    atten = atten * shadowRay.medium->attenuate(shadowRay.o, end);
                }
                Vector normal2 = inter.normal();
                if (dot(shadowRay.d, inter.normal()) > 0.0f) {
                    normal2 = -inter.normal();
                }
                float k = 1e-4;
                shadowRay = Ray(inter.hitPoint()- (k*normal2), shadowRay.d,
                          inter.ray.medium);
            }



            RGBColor lightIntensity = world->light[i]->getIntensity(lh);

            //calculating Henyeye-Greenstain function value
            float cos = dot(intersection.ray.d, lh.direction);
            float enu = 1.f - (this->g * this->g);
            float den = std::pow(( 1+ this->g*this->g + 2*cos*this->g), 3/2);
            float hg = (1.f/ (4*pi)) * (enu/den);

            rad = rad + lightIntensity * atten * hg;
        }
        
        return rad;

    }




}