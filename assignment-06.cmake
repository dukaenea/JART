cmake_minimum_required(VERSION 3.0)

set(MY_ADDITIONAL_SOURCES
    # Please add your source files here
)

set(ASSIGNMENT_SOURCES
    rt/materials/lambertian.cpp
    rt/materials/lambertian.h
    rt/materials/phong.cpp
    rt/materials/phong.h
    rt/materials/mirror.cpp
    rt/materials/mirror.h
    rt/materials/combine.cpp
    rt/materials/combine.h
    rt/integrators/recraytrace.cpp
    rt/integrators/recraytrace.h
    rt/textures/texture.h
    rt/textures/constant.cpp
    rt/textures/constant.h
    core/interpolate.cpp
    core/interpolate-impl.h
    core/interpolate.h
    rt/coordmappers/coordmapper.h
    rt/coordmappers/cylindrical.cpp
    rt/coordmappers/cylindrical.h
    rt/coordmappers/plane.cpp
    rt/coordmappers/plane.h
    rt/coordmappers/spherical.cpp
    rt/coordmappers/spherical.h
    rt/coordmappers/tmapper.cpp
    rt/coordmappers/tmapper.h
    rt/coordmappers/world.cpp
    rt/coordmappers/world.h
    rt/materials/flatmaterial.cpp
    rt/materials/flatmaterial.h
    rt/textures/checkerboard.cpp
    rt/textures/checkerboard.h
    rt/textures/imagetex.cpp
    rt/textures/imagetex.h
    rt/textures/perlin.cpp
    rt/textures/perlin.h
    ${MY_ADDITIONAL_SOURCES}
    main/a_materials.cpp
    main/a_textures.cpp
    main/a_local.cpp
    main/a_mappers.cpp
    main/a_environment.cpp
    rt/coordmappers/environmental.h
    rt/coordmappers/environmental.cpp
    rt/materials/environment.h
    rt/materials/environment.cpp
    rt/solids/environment.h
    rt/solids/environment.cpp
    rt/materials/cooktorrancematerial.h
    rt/materials/cooktorrancematerial.cpp
)

file(COPY
    models/stones_diffuse.png
    models/posx.png
    models/posy.png 
    models/posz.png
    models/negx.png
    models/negy.png
    models/negz.png
    DESTINATION ${CMAKE_BINARY_DIR}/models
)
