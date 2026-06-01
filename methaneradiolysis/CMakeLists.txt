cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Simulation)

find_package(Geant4 REQUIRED ui_all vis_all)


include(${Geant4_USE_FILE})

include_directories(${PROJECT_SOURCE_DIR}/management/include
                   ${PROJECT_SOURCE_DIR}/molecules/include
                   ${PROJECT_SOURCE_DIR}/processes/include
                   ${PROJECT_SOURCE_DIR}/core/include
                   )

file(GLOB sources ${PROJECT_SOURCE_DIR}/management/src/*.cpp
                  ${PROJECT_SOURCE_DIR}/molecules/src/*.cpp
                  ${PROJECT_SOURCE_DIR}/processes/src/*.cpp
                  ${PROJECT_SOURCE_DIR}/core/src/*.cpp
                  )

file(GLOB headers ${PROJECT_SOURCE_DIR}/management/include/*.hh
                  ${PROJECT_SOURCE_DIR}/molecules/include/*.hh
                  ${PROJECT_SOURCE_DIR}/processes/include/*.hh
                  ${PROJECT_SOURCE_DIR}/core/include/*.hh
                  )


add_executable(mr_sim mr_sim.cpp ${sources} ${headers})
target_link_libraries(mr_sim ${Geant4_LIBRARIES})

add_custom_target(Simulation DEPENDS mr_sim)