#include <core/assert.h>
#include <core/scalar.h>
#include <core/image.h>

#include <rt/world.h>
#include <rt/groups/bvh.h>
#include <rt/cameras/perspective.h>
#include <rt/cameras/dofperspective.h>
#include <rt/loaders/obj.h>

#include <rt/renderer.h>
#include <rt/integrators/raytrace.h>
#include <rt/integrators/casting.h>
#include <rt/integrators/recraytrace.h>

#include <rt/lights/pointlight.h>
#include <rt/lights/spotlight.h>
#include <rt/materials/dummy.h>
#include <rt/materials/lambertian.h>
#include <rt/textures/constant.h>
#include <rt/textures/texture.h>
#include <rt/materials/flatmaterial.h>
#include <rt/materials/combine.h>
#include <rt/solids/infiniteplane.h>
#include <rt/groups/simplegroup.h>

#include <rt/textures/imagetex.h>

#include <rt/mediums/homogeniusmedium.h>
#include <rt/integrators/volint.h>

#include <rt/solids/quad.h>
#include <rt/lights/arealight.h>
#include <rt/solids/sphere.h>

#include <rt/materials/cooktorrancematerial.h>
#include <rt/materials/environment.h>
#include <rt/solids/environment.h>
#include <rt/coordmappers/environmental.h>

#include <rt/materials/glass.h>

#define LOG(x) std::cout<<x<<std::endl;


using namespace rt;

//had to write functions to render the objects separately so i can debug


void renderHoverBike(const char* filename){
    Image img(800, 600);        
    World world;
    BVH* hoverbike = new BVH();
    SimpleGroup scene = SimpleGroup();

    ImageTexture* bumptex = new ImageTexture("models/hoverbike/Bike_Normal.png", ImageTexture::REPEAT, ImageTexture::BILINEAR);
    loadOBJ(hoverbike, "models/hoverbike/", "hoverbike.obj");
    PointLight* light = new PointLight(
        Point(-8.25f, 2.0f, 5.0f),
        RGBColor::rep(500.0f)
    );

    
    
    world.light.push_back(light);
    hoverbike->rebuildIndex();
    scene.add(hoverbike);
    world.scene = &scene;


    PerspectiveCamera cam(
        Point(2.24f, 1.0f, 6.0f), Vector(1.f,0.0f,-0.6f), Vector(0, 1, 0), pi/4, pi/3
    );


    //RayCastingIntegrator integrator(&world);
    RayTracingIntegrator integrator(&world);
    Renderer engine(&cam, &integrator);
    engine.setSamples(1);
    engine.render(img);
    img.writePNG(filename);
}


void renderWorrior(const char* filename){
    Image img(800 * 4, 600 * 4);        
    World world;
    SimpleGroup scene = SimpleGroup();

    BVH* worrior = new BVH();
    ImageTexture* worriorBumptex = new ImageTexture("models/worrior/worrior_normal.png", ImageTexture::REPEAT, ImageTexture::BILINEAR);
    loadOBJ(worrior, "models/worrior/", "worrior.obj", nullptr, worriorBumptex);


    PointLight* light = new PointLight(
        Point(14.25f, 6.0f, 5.0f),
        RGBColor::rep(200.0f)
    );

    
    
    world.light.push_back(light);
    worrior->rebuildIndex();
    
    scene.add(worrior);
    world.scene = &scene;


    PerspectiveCamera cam(
        Point(12.0f, 2.f, 7.f), Vector(-1.2f,0.0f,-1.f), Vector(0, 1, 0), pi/4, pi/3
    );


    //RayCastingIntegrator integrator(&world);
    RayTracingIntegrator integrator(&world);
    Renderer engine(&cam, &integrator);
    engine.setSamples(1);
    engine.render(img);
    img.writePNG(filename);


}

void renderRobot(const char* filename){
    Image img(800 * 1, 600 * 1);        
    World world;
    //BVH* robot = new BVH();
    SimpleGroup scene = SimpleGroup();

    BVH* building = new BVH();
    ImageTexture* buildingBumptex = new ImageTexture("models/building/building_normal.png", ImageTexture::REPEAT, ImageTexture::BILINEAR);
    loadOBJ(building, "models/building/", "ft.obj");
    PointLight* light = new PointLight(
        Point(20.25f, 4.0f, 5.0f),
        RGBColor::rep(2000.0f)
    );

    
    
    world.light.push_back(light);
    building->rebuildIndex();
    scene.add(building);
    world.scene = &scene;


    PerspectiveCamera cam(
        Point(10.24f, 3.f, 5.0f), Vector(-3.5f,0.0f,-1.f), Vector(0, 1, 0), pi/4, pi/3
    );


    //RayCastingIntegrator integrator(&world);
    RayTracingIntegrator integrator(&world);
    //RecursiveRayTracingIntegrator integrator(&world);
    Renderer engine(&cam, &integrator);
    engine.setSamples(1);
    engine.render(img);
    img.writePNG(filename);
}

void renderCyborg(const char* filename){
    Image img(800 * 2, 600 * 2);        
    World world;
    BVH* cyborg = new BVH();
    SimpleGroup scene = SimpleGroup();

    ImageTexture* bumptex = new ImageTexture("models/cyborg/default_normal.png", ImageTexture::REPEAT, ImageTexture::BILINEAR);
    loadOBJ(cyborg, "models/cyborg/", "cyborg.obj", nullptr, bumptex);
    //loadOBJ(cyborg, "models/cyborg/", "cyborg.obj");
    PointLight* light = new PointLight(
        Point(10.25f, 6.0f, 5.0f),
        RGBColor::rep(400.0f)
    );

    
    
    world.light.push_back(light);
    cyborg->rebuildIndex();
    scene.add(cyborg);
    world.scene = &scene;


    PerspectiveCamera cam(
        Point(6.0f, 2.5f, .5f), Vector(-.4f,0.0f,-1.f), Vector(0, 1, 0), pi/4, pi/3
    );


    //RayCastingIntegrator integrator(&world);
    //RayTracingIntegrator integrator(&world);
    RecursiveRayTracingIntegrator integrator(&world);
    Renderer engine(&cam, &integrator);
    engine.setSamples(1);
    engine.render(img);
    img.writePNG(filename);
}

void renderScene(){

}

void renderCar(const char* filename){
    Image img(800 * 2, 600 * 2);        
    World world;
    BVH* car = new BVH();
    SimpleGroup scene = SimpleGroup();

    ImageTexture* carBumptex = new ImageTexture("models/car/c3_n.png", ImageTexture::REPEAT, ImageTexture::BILINEAR);
    loadOBJ(car, "models/car/", "car.obj", nullptr, carBumptex);
    PointLight* light = new PointLight(
        Point(10.25f, 6.0f, 5.0f),
        RGBColor::rep(400.0f)
    );

    
    
    world.light.push_back(light);
    car->rebuildIndex();
    scene.add(car);
    world.scene = &scene;


    PerspectiveCamera cam(
        Point(4.0f, 2.5f, .5f), Vector(-3.4f,0.0f,-1.f), Vector(0, 1, 0), pi/4, pi/3
    );


    //RayCastingIntegrator integrator(&world);
    RayTracingIntegrator integrator(&world);
    Renderer engine(&cam, &integrator);
    engine.setSamples(1);
    engine.render(img);
    img.writePNG(filename);
}


void renderScene(const char* filename){
    Image img(1920, 1080);        
    World world;
    BVH* scene = new BVH();

    RGBColor sigmaA = RGBColor(0, 0, 0);
    RGBColor sigmaS = RGBColor::rep(0.15f);
    RGBColor emission = RGBColor(0, 0, 0);
    HomogeniousMedium medium = HomogeniousMedium(sigmaA, sigmaS, 0.1f, emission); 


    
    BVH* worrior = new BVH();
    ImageTexture* worriorBumptex = new ImageTexture("models/worrior/worrior_normal.png", ImageTexture::REPEAT, ImageTexture::BILINEAR);
    loadOBJ(worrior, "models/worrior/", "worrior.obj", nullptr, worriorBumptex);
    
    BVH* cyborg = new BVH();
    ImageTexture* cyborgBumptex = new ImageTexture("models/cyborg/default_normal.png", ImageTexture::REPEAT, ImageTexture::BILINEAR);
    loadOBJ(cyborg, "models/cyborg/", "cyborg.obj", nullptr, cyborgBumptex); //last argument stands for flipping the green channel. check bmap.h to see why

    BVH* robot = new BVH();
    ImageTexture* robotBumptex = new ImageTexture("models/robot/robot_normal.png", ImageTexture::REPEAT, ImageTexture::BILINEAR);
    loadOBJ(robot, "models/robot/", "robot.obj", nullptr, robotBumptex, true);

    BVH* car = new BVH();
    ImageTexture* carBumptex = new ImageTexture("models/car/c3_n.png", ImageTexture::REPEAT, ImageTexture::BILINEAR);
    loadOBJ(car, "models/car/", "car.obj", nullptr, carBumptex);

    BVH* hoverbike = new BVH();
    ImageTexture* hoverbikeBumptex = new ImageTexture("models/hoverbike/Bike_Normal.png", ImageTexture::REPEAT, ImageTexture::BILINEAR);
    loadOBJ(hoverbike, "models/hoverbike/", "hoverbike.obj", nullptr, hoverbikeBumptex);

    BVH* terrain = new BVH();
    ImageTexture* terrainBumptex = new ImageTexture("models/terrain/terrain_normal.png", ImageTexture::REPEAT, ImageTexture::BILINEAR);
    loadOBJ(terrain, "models/terrain/", "terrain.obj", nullptr, terrainBumptex);

    BVH* lightsaber = new BVH();
    loadOBJ(lightsaber, "models/lightsaber/", "ls.obj");

    BVH* building = new BVH();
    ImageTexture* buildingBumptex = new ImageTexture("models/building/building_normal.png", ImageTexture::REPEAT, ImageTexture::BILINEAR);
    loadOBJ(building, "models/building/", "ft.obj");

    BVH* ship = new BVH();
    ImageTexture* shipBumptex = new ImageTexture("models/ship/ship_normal.png", ImageTexture::REPEAT, ImageTexture::BILINEAR);
    loadOBJ(ship, "models/ship/", "ss.obj", nullptr, shipBumptex);

    BVH* box = new BVH();
    ImageTexture* boxBumptex = new ImageTexture("models/box/box_normal.png", ImageTexture::REPEAT, ImageTexture::BILINEAR);
    loadOBJ(box, "models/box/", "box.obj", nullptr, boxBumptex);


    float scale = 30.f;

    Texture* blacktex = new ConstantTexture(RGBColor::rep(0.0f));
    ConstantTexture* lightsrctex = new ConstantTexture(RGBColor(0.1 * scale, 0.11 * scale, 0.13 * scale));
    Material* lightsource = new LambertianMaterial(lightsrctex, blacktex);

    Quad* light = new Quad(Point(2.2,9.1,19.3), Vector(16.1,-0.5f,-6.2), Vector(-4.7,16,-12.8), nullptr, lightsource);
    AreaLight als(light);

    //environment
    ImageTexture* posx = new ImageTexture("models/skybox/front.png", ImageTexture::BorderHandlingType::REPEAT, ImageTexture::InterpolationType::BILINEAR);
    ImageTexture* posy = new ImageTexture("models/skybox/top.png", ImageTexture::BorderHandlingType::REPEAT, ImageTexture::InterpolationType::BILINEAR);
    ImageTexture* posz = new ImageTexture("models/skybox/left.png", ImageTexture::BorderHandlingType::REPEAT, ImageTexture::InterpolationType::BILINEAR);
    ImageTexture* negx = new ImageTexture("models/skybox/back.png", ImageTexture::BorderHandlingType::REPEAT, ImageTexture::InterpolationType::BILINEAR);
    ImageTexture* negy = new ImageTexture("models/skybox/bottom.png", ImageTexture::BorderHandlingType::REPEAT, ImageTexture::InterpolationType::BILINEAR);
    ImageTexture* negz = new ImageTexture("models/skybox/right.png", ImageTexture::BorderHandlingType::REPEAT, ImageTexture::InterpolationType::BILINEAR);

    EnvironmentMaterial envMatr = EnvironmentMaterial();

    envMatr.addTexture(posx);
    envMatr.addTexture(negx);
   
    envMatr.addTexture(posy);
    envMatr.addTexture(negy);

    envMatr.addTexture(posz);
    envMatr.addTexture(negz);
   

    EnvironmentalCoordMapper* mapper = new EnvironmentalCoordMapper(); 

    Environment env = Environment(mapper);
    env.setMaterial(&envMatr);

    //world.light.push_back(light);
    float lightScale = 2000;
    world.light.push_back(
                new SpotLight(Point(-24.2f, 6.1f, 5.8f), Vector(0, 1.5, -1).normalize(), 0.23,
                              500,
                              RGBColor(1 * lightScale, 0.76 * lightScale, 0.25f * lightScale)));
    world.light.push_back(
                new SpotLight(Point(-24.2f, 10.45f, 5.04f), Vector(.2f, 0.1, -1).normalize(), 0.23,
                              500,
                              RGBColor(1 * lightScale, 0.76 * lightScale, 0.25f * lightScale))); 
    
    world.light.push_back(&als);
    worrior->rebuildIndex();
    cyborg->rebuildIndex();
    robot->rebuildIndex();
    car->rebuildIndex();
    hoverbike->rebuildIndex();
    terrain->rebuildIndex();
    lightsaber->rebuildIndex();
    building->rebuildIndex();
    ship->rebuildIndex();
    box->rebuildIndex();
    
    scene->add(worrior);
    scene->add(robot);
    scene->add(car);
    scene->add(hoverbike);
    scene->add(terrain);
    scene->add(lightsaber);
    scene->add(building);
    scene->add(ship);
    scene->add(box);

    scene->add(cyborg);
    scene->rebuildIndex();
    world.scene = scene;
    world.skybox = &env;
    world.skyboxVisibilityFactor = 0.5;



    PerspectiveCamera cam(Point(12.5f, 1.7f, 5.3f), Vector(-2.6f,0.14f,-1.f), Vector(0, 1, 0), pi/5.37, pi/3, &medium);

    VolumetricIntegrator integrator(&world);
    Renderer engine(&cam, &integrator);
    engine.setSamples(100);
    engine.render(img);
    img.writePNG(filename);
}


//this section is for rendering images displaying different effects we have implemented
void volumeRenering(const char* filename){
    Image img(800, 600);        
    World world;
    SimpleGroup scene = SimpleGroup();
    
    RGBColor sigmaA = RGBColor(0, 0, 0);
    RGBColor sigmaS = RGBColor::rep(0.1f);
    RGBColor emission = RGBColor(0, 0, 0);
    HomogeniousMedium medium = HomogeniousMedium(sigmaA, sigmaS, 0.1f, emission); 


    float lightScale = 5000;
    for(int i=0; i<12;i++){
        float angle = i * (pi / 6);
         world.light.push_back( new SpotLight(Point(0.f, 0.f, -30.f), Vector(tan(angle), sin(angle), cos(angle)).normalize(), 0.23,
                            500,
                            RGBColor(1 * lightScale, 0.76 * lightScale, 0.25f * lightScale)));
    }
    //scene.add(new Sphere(Point(0,0,0), 0.1, nullptr, new LambertianMaterial( new ConstantTexture(RGBColor::rep(1)) , new ConstantTexture(RGBColor::rep(0.f))   )));
    world.scene = &scene;


    PerspectiveCamera cam(
        Point(0, 0, 1), Vector(0, 0, -1), Vector(0, 1, 0), pi/4, pi/3, & medium
    );


    VolumetricIntegrator integrator(&world);
    Renderer engine(&cam, &integrator);
    engine.setSamples(1);
    engine.render(img);
    img.writePNG(filename);
}




void a_renderingcompetition(){
    //renderMoon("moon.png");
    //renderHoverBike("hoverbike.png");
    //renderRobot("robot.png");
    //renderCyborg("cyborg.png");
    //renderWorrior("worrior.png");
    //renderCar("car.png");
    //volumeRenering("volrend.png");
    renderScene("scene.png");
}