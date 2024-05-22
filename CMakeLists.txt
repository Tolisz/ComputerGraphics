cmake_minimum_required(VERSION 3.20)

project(duck VERSION 0.1 LANGUAGES CXX)

# external libraries
# *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=
add_subdirectory(externals/glm)
add_subdirectory(externals/glfw)
add_subdirectory(externals/glad)
add_subdirectory(externals/imgui)
set(DUCK_LINK_LIBS  PUBLIC glm
                    PUBLIC glfw
                    PUBLIC glad
                    PUBLIC imgui)

# executable
# *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=
set(DUCK_EXECUTABLE_NAME duck)
file(GLOB DUCK_SOURCE_FILES source/*.cpp)

add_executable(${DUCK_EXECUTABLE_NAME} ${DUCK_SOURCE_FILES})
target_link_libraries(${DUCK_EXECUTABLE_NAME} ${DUCK_LINK_LIBS})