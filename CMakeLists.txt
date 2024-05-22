cmake_minimum_required(VERSION 3.20)

project(duck VERSION 0.1 LANGUAGES CXX)

# external libraries
# *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=
add_subdirectory(externals/glm)

set(DUCK_LINK_LIBS PUBLIC glm)

add_executable(duck source/main.cpp)
target_link_libraries(duck ${DUCK_LINK_LIBS})
