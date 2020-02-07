#include <rt/solids/solid.h>

namespace rt {

Solid::Solid(CoordMapper* texMapper, Material* material)
{
    this->texMapper = texMapper;
    this->material = material;
}

}
