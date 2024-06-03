set(DUCK_LINK_LIBS  PUBLIC common)
                    # PUBLIC glfw
                    # PUBLIC glad
                    # PUBLIC imgui
                    # PUBLIC stb)

set(DUCK_EXECUTABLE_NAME duck)
file(GLOB DUCK_SOURCE_FILES *.cpp)

add_executable(${DUCK_EXECUTABLE_NAME} ${DUCK_SOURCE_FILES})
target_link_libraries(${DUCK_EXECUTABLE_NAME} ${DUCK_LINK_LIBS})