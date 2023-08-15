cmake_minimum_required(VERSION 3.7)

project(drawtest)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Create a sources variable with a link to all cpp files to compile
set(SOURCES
    src/interfaces/inputs.c
    src/datastructures/linked_list.c
    src/environment/environment.c
    src/environment/models.c
    src/environment/tick.c
    src/render/render.c
    src/vector.c
    src/main.c
)


add_executable(drawtest ${SOURCES})


target_link_libraries(drawtest ${SDL2_LIBRARIES} m)

target_include_directories(drawtest
    PRIVATE 
        ${PROJECT_SOURCE_DIR}/include
)