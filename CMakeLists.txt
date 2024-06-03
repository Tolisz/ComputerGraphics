cmake_minimum_required(VERSION 3.20)

project(ComputerGraphics VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

# external libraries
# *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=
add_subdirectory(externals/glm)
add_subdirectory(externals/glfw)
add_subdirectory(externals/glad)
add_subdirectory(externals/imgui)
add_subdirectory(externals/stb)

# common elements library
# *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=
add_subdirectory(source/common)

# executable
# *=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=

# duck project
add_subdirectory(source/duck)
# tessellation project
add_subdirectory(source/tessellation)