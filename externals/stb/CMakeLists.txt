set(STB_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/stb)

add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${STB_PATHS})