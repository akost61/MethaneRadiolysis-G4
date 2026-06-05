cmake_minimum_required(VERSION 3.6 FATAL_ERROR)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_OSX_DEPLOYMENT_TARGET "13.0" CACHE STRING "Minimum macOS version" FORCE)

project(Simulation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Geant4 REQUIRED ui_all vis_all)
include(${Geant4_USE_FILE})

# (from conda root-env)
if(NOT DEFINED ROOT_DIR)
    execute_process(
        COMMAND root-config --prefix
        OUTPUT_VARIABLE ROOT_PREFIX
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(ROOT_DIR "${ROOT_PREFIX}/cmake")
endif()
find_package(ROOT REQUIRED COMPONENTS Core Tree)

include_directories(
    ${PROJECT_SOURCE_DIR}/management/include
    ${PROJECT_SOURCE_DIR}/molecules/include
    ${PROJECT_SOURCE_DIR}/processes/include
    ${PROJECT_SOURCE_DIR}/core/include
    ${ROOT_INCLUDE_DIRS}
)

file(GLOB sources
    ${PROJECT_SOURCE_DIR}/management/src/*.cpp
    ${PROJECT_SOURCE_DIR}/molecules/src/*.cpp
    ${PROJECT_SOURCE_DIR}/processes/src/*.cpp
    ${PROJECT_SOURCE_DIR}/core/src/*.cpp
)

file(GLOB headers
    ${PROJECT_SOURCE_DIR}/management/include/*.hh
    ${PROJECT_SOURCE_DIR}/molecules/include/*.hh
    ${PROJECT_SOURCE_DIR}/processes/include/*.hh
    ${PROJECT_SOURCE_DIR}/core/include/*.hh
)

add_executable(mr_sim mr_sim.cpp ${sources} ${headers})
target_link_libraries(mr_sim ${Geant4_LIBRARIES} ${ROOT_LIBRARIES})

add_custom_target(Simulation DEPENDS mr_sim)