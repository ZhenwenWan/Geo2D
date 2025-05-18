set(dir /home/Zhenwen/DongJun/Geo2d/SRC)

project(Boolean CXX)
cmake_minimum_required(VERSION 2.6.0)

set(Boolean_lib
${dir}/../lib/ewa_base.lib 
)

include_directories(
${dir}/../include
)

set(SRC
geo2d.h
dash.h
dashTrain.h
dashLoop.h
geo2d_math.h
dash.cpp
dashTrain.cpp
dashLoop.cpp
base2d.h
base2d.cpp
rectangle.h
rectangle.cpp
triangle.h
triangle.cpp
#poly_generator.h
#poly_generator.cpp
polygon.h
polygon.cpp
geometric_structures.h
geometric_structures.cpp
main.cpp
)

add_executable(Boolean ${SRC})
target_link_libraries(Boolean ${Boolean_lib})
SET_TARGET_PROPERTIES(Boolean PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(Boolean PROPERTIES OUTPUT_NAME Boolean)


