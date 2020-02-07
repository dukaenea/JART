cmake_minimum_required(VERSION 3.0)

set(MY_ADDITIONAL_SOURCES
    # Please add your source files here
    main/a_volumes.cpp
    main/a_renderingcompetition.cpp
    rt/integrators/volint.h
    rt/integrators/volint.cpp
    rt/mediums/medium.h
    rt/mediums/homogeniusmedium.h
    rt/mediums/homogeniusmedium.cpp
)

set(ASSIGNMENT_SOURCES
    rt/primmod/bmap.cpp
    rt/primmod/bmap.h
    ${MY_ADDITIONAL_SOURCES}
    main/a_bump.cpp
)

file(COPY
    models/stones_bump.png
    models/stones_spec.png
    DESTINATION ${CMAKE_BINARY_DIR}/models
)


file(COPY
	models/hoverbike/hoverbike.obj
    models/hoverbike/hoverbike.mtl
    models/hoverbike/Bike_Deffuse.png
    models/hoverbike/Bike_Normal.png
    models/hoverbike/Bike_Emissive.png
    models/hoverbike/Bike_Specular.png
	DESTINATION ${CMAKE_BINARY_DIR}/models/hoverbike
)


file(COPY
	models/robot/robot.obj
    models/robot/robot.mtl
    models/robot/robot_deffuse.png
    models/robot/robot_bump.png
    models/robot/robot_emission.png
    models/robot/robot_specular.png
    models/robot/robot_normal.png
	DESTINATION ${CMAKE_BINARY_DIR}/models/robot
)

file(COPY
	models/cyborg/cyborg.obj
    models/cyborg/cyborg.mtl
    models/cyborg/default_defuse.png
    models/cyborg/default_bump.png
    models/cyborg/default_emissive.png
	models/cyborg/default_normal.png
    models/cyborg/default_metallic.png
	models/cyborg/default_AO.png
	models/cyborg/default_roughness.png
	DESTINATION ${CMAKE_BINARY_DIR}/models/cyborg
)

file(COPY
	models/worrior/worrior.obj
    models/worrior/worrior.mtl
    models/worrior/lightsaber_difuse.png
    models/worrior/lightsaber_emissive.png
	models/worrior/worrior_difuse.png
	models/worrior/worrior_normal.png
    models/worrior/worrior_emissive.png
    models/worrior/worrior_specular.png
	DESTINATION ${CMAKE_BINARY_DIR}/models/worrior
)

file(COPY
	models/car/car.obj
    models/car/car.mtl
    models/car/c3.png
    models/car/c3_m.png
	models/car/c3_n.png
	DESTINATION ${CMAKE_BINARY_DIR}/models/car
)

file(COPY
	models/terrain/terrain.obj
    models/terrain/terrain.mtl
    models/terrain/terrain_difuse.png
    models/terrain/terrain_normal.png
	DESTINATION ${CMAKE_BINARY_DIR}/models/terrain
)

file(COPY
	models/skybox/back.png
	models/skybox/bottom.png
	models/skybox/front.png
	models/skybox/left.png
	models/skybox/right.png
	models/skybox/top.png
	DESTINATION ${CMAKE_BINARY_DIR}/models/skybox
)

file(COPY
	models/lightsaber/ls.obj
	models/lightsaber/ls.mtl
	models/lightsaber/emissive.png
	models/lightsaber/lightsaber_defuse.png
	DESTINATION ${CMAKE_BINARY_DIR}/models/lightsaber
)

file(COPY
	models/ship/ss.obj
	models/ship/ss.mtl
	models/ship/ship_emissive.png
	models/ship/ship_defused.png
	models/ship/ship_normal.png
	models/ship/ship_specular.png
	DESTINATION ${CMAKE_BINARY_DIR}/models/ship
)

file(COPY
	models/building/ft.obj
	models/building/ft.mtl
	models/building/building_emissive.png
	models/building/building_defused.png
	models/building/building_normal.png
	models/building/building_specular.png
	DESTINATION ${CMAKE_BINARY_DIR}/models/building
)


file(COPY
	models/box/box.obj
	models/box/box.mtl
	models/box/box_emissive.png
	models/box/box_defuse.png
	models/box/box_normal.png
	models/box/box_specular.png
	DESTINATION ${CMAKE_BINARY_DIR}/models/box
)


