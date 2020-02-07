
#include <core/assert.h>
#include <core/scalar.h>
#include <core/image.h>
#include <rt/world.h>
#include <rt/renderer.h>
#include <cmath>

#include <rt/cameras/perspective.h>


#include <rt/textures/imagetex.h>

#include <rt/solids/sphere.h>
#include <rt/integrators/recraytrace.h>

#include <rt/groups/simplegroup.h>


#include <rt/materials/environment.h>
#include <rt/solids/environment.h>
#include <rt/materials/mirror.h>
#include <rt/coordmappers/environmental.h>

using namespace rt;

void envmapper(const char* filename) {
    static const float scale = 0.001f;
    Image img(1400, 1400);
    World world;
    SimpleGroup scene;
    world.scene = &scene;

    PerspectiveCamera cam(Point(278*scale, 273*scale, -800*scale), Vector(0, 0, 1), Vector(0, 1, 0), pi/2.f, pi/2.f);

    //getting the texture for each side of the cube

    ImageTexture* posx = new ImageTexture("models/posx.png", ImageTexture::BorderHandlingType::REPEAT, ImageTexture::InterpolationType::BILINEAR);
    ImageTexture* posy = new ImageTexture("models/posy.png", ImageTexture::BorderHandlingType::REPEAT, ImageTexture::InterpolationType::BILINEAR);
    ImageTexture* posz = new ImageTexture("models/posz.png", ImageTexture::BorderHandlingType::REPEAT, ImageTexture::InterpolationType::BILINEAR);
    ImageTexture* negx = new ImageTexture("models/negx.png", ImageTexture::BorderHandlingType::REPEAT, ImageTexture::InterpolationType::BILINEAR);
    ImageTexture* negy = new ImageTexture("models/negy.png", ImageTexture::BorderHandlingType::REPEAT, ImageTexture::InterpolationType::BILINEAR);
    ImageTexture* negz = new ImageTexture("models/negz.png", ImageTexture::BorderHandlingType::REPEAT, ImageTexture::InterpolationType::BILINEAR);



    EnvironmentMaterial envMatr =EnvironmentMaterial();

    envMatr.addTexture(posx);
    envMatr.addTexture(negx);
   
    envMatr.addTexture(posy);
    envMatr.addTexture(negy);

    envMatr.addTexture(posz);
    envMatr.addTexture(negz);
   

    EnvironmentalCoordMapper* mapper = new EnvironmentalCoordMapper(); 

    Environment env = Environment(mapper);
    env.setMaterial(&envMatr);

    scene.add(&env);

    MirrorMaterial * mirror = new MirrorMaterial(0.0f, 0.0f);
    scene.add(new Sphere(Point(200.f,100.f,300.f)*scale, 550.f*scale, nullptr, mirror));

    RecursiveRayTracingIntegrator integrator(&world);

    Renderer engine(&cam, &integrator);
    engine.render(img);
    img.writePNG(filename);
}



void a_environment() {
    envmapper("env.png");
}