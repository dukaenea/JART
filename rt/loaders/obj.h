#ifndef CG1RAYTRACER_LOADERS_OBJ_HEADER
#define CG1RAYTRACER_LOADERS_OBJ_HEADER

#include <string>
#include <map>
namespace rt {
class Group;
class Material;
class Texture;

typedef std::map<std::string, Material*> MatLib;

void loadOBJ(Group* dest, const std::string& path, const std::string& filename, MatLib* inmats=nullptr, Texture* bumptex=nullptr, bool flipGreenChannelOfTexture = false);
void loadOBJMat(MatLib* dest, const std::string& path, const std::string& filename);

}

#endif