#include <core/assert.h>
#include <core/scalar.h>
#include <core/image.h>
#include <rt/world.h>
#include <rt/renderer.h>
#include <cmath>

#include <rt/cameras/perspective.h>

#include <rt/groups/simplegroup.h>

#include <rt/solids/sphere.h>
#include <rt/solids/infiniteplane.h>

#include <rt/materials/lambertian.h>
#include <rt/materials/flatmaterial.h>
#include <rt/textures/constant.h>
#include <rt/lights/spotlight.h>

#include <rt/integrators/volint.h>
#include <rt/integrators/raytrace.h>

#include <rt/mediums/homogeniusmedium.h>

using namespace rt;

void render_volumes1(const char* filename){
     Image img(400, 400);
     World world;
     SimpleGroup scene;
     world.scene = &scene;

     //creating a greyish fog
     RGBColor sigmaA = RGBColor(1, 0, 0);
     RGBColor sigmaS = RGBColor::rep(0.f);
     RGBColor emission = RGBColor(0, 0, 0.02);
     HomogeniousMedium medium = HomogeniousMedium(sigmaA, sigmaS, 0.f, emission); 

     PerspectiveCamera cam(Point(0,0,-1), Vector(0, 0, 1), Vector(0, 1, 0), 1, 1, &medium);


     //creating a red lambertian material
     ConstantTexture* em = new ConstantTexture(RGBColor::rep(0.f));
     ConstantTexture* diff = new ConstantTexture(RGBColor::rep(0.5f));
     LambertianMaterial* mattRed = new LambertianMaterial(em, diff);
     Material *greyconst = new FlatMaterial(new ConstantTexture(RGBColor::rep(0.5)));

     //creating a dark gray lambertian material
     diff = new ConstantTexture({0.2f, 0.2f, 0.2f});
     LambertianMaterial* darkGray = new LambertianMaterial(em, diff);

     scene.add(new Sphere(Point(-1.f,1.3f,5.f), .5f, nullptr, greyconst));
     scene.add(new InfinitePlane(Point(0,-2, 0), Vector(0, 1, 0), nullptr, darkGray));
     
    VolumetricIntegrator integrator(&world);
    Renderer engine(&cam, &integrator);
    engine.render(img);
    img.writePNG(filename);

}


void render_volumes2(const char* filename){
    Image img(400, 400);
     World world;
     SimpleGroup scene;
     world.scene = &scene;
    
     //creating a greyish fog
     RGBColor sigmaA = RGBColor(0, 0, 0);
     RGBColor sigmaS = RGBColor::rep(0.1f);
     RGBColor emission = RGBColor(0, 0, 0);
     HomogeniousMedium medium = HomogeniousMedium(sigmaA, sigmaS, 0.3f, emission); 

     PerspectiveCamera cam(Point(0,0,-1), Vector(0, 0, 1), Vector(0, 1, 0), 1, 1, &medium);


     //creating a red lambertian material
     ConstantTexture* em = new ConstantTexture(RGBColor::rep(0.f));
     ConstantTexture* diff = new ConstantTexture(RGBColor::rep(0.5f));
     LambertianMaterial* mattRed = new LambertianMaterial(em, diff);
     Material *greyconst = new FlatMaterial(new ConstantTexture(RGBColor::rep(0.5)));

     //creating a dark gray lambertian material
     diff = new ConstantTexture({0.2f, 0.2f, 0.2f});
     LambertianMaterial* darkGray = new LambertianMaterial(em, diff);

     scene.add(new Sphere({3, 3, 15}, 1.5f, nullptr, darkGray));
     scene.add(new InfinitePlane(Point(0,-2, 0), Vector(0, 1, 0), nullptr, darkGray));

world.light.push_back(
                new SpotLight(Point(6, 6, 15) + Vector(1, 1, 0) * 30, Vector(-3, -3, 0).normalize(), 0.23,
                              500,
                              RGBColor::rep(200000)));     
    VolumetricIntegrator integrator(&world);
    Renderer engine(&cam, &integrator);
    engine.render(img);
    img.writePNG(filename);
}



void a_volumes(){
    render_volumes1("vol1.png");
    render_volumes2("vol2.png");
}
