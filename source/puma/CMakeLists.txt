set(PUMA_LINK_LIBS  PUBLIC common
                    )

set(PUMA_EXECUTABLE_NAME puma)
file(GLOB PUMA_SOURCE_FILES *.cpp)

add_executable(${PUMA_EXECUTABLE_NAME} ${PUMA_SOURCE_FILES})
target_link_libraries(${PUMA_EXECUTABLE_NAME} ${PUMA_LINK_LIBS})

