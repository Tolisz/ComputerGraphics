set(COMMON_LINK_LIBS PUBLIC glm
                     PUBLIC glfw
                     PUBLIC glad
                     PUBLIC imgui
                     PUBLIC stb)

set(COMMON_LIB_NAME common)
file(GLOB COMMON_SOURCE_FILES *.cpp)

add_library(${COMMON_LIB_NAME} STATIC ${COMMON_SOURCE_FILES})
target_link_libraries(${COMMON_LIB_NAME} ${COMMON_LINK_LIBS})
target_include_directories(${COMMON_LIB_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})