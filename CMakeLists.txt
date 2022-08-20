cmake_minimum_required(VERSION 3.3 FATAL_ERROR)

set(Geant4_DIR "$G4SYS" )


project(absorber_simul VERSION 0.2
    DESCRIPTION "Experimenting with G4"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

option(WITH_GEANT4_UIVIS "Build with Geant4 UI and Vis drivers" ON)
# Find Geant4
if(WITH_GEANT4_UIVIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
else()
    find_package(Geant4 REQUIRED)
endif()

include(${Geant4_USE_FILE}) # Auto configure includes/flags
include_directories(${PROJECT_SOURCE_DIR}/include) # Add -I type paths

# We presume the existence of three directories
file(GLOB_RECURSE sources ${PROJECT_SOURCE_DIR}/source/*.cc)
file(GLOB_RECURSE headers ${PROJECT_SOURCE_DIR}/include/*.hh)
file(GLOB_RECURSE macros RELATIVE ${PROJECT_SOURCE_DIR} macros/*.mac)

# Enable macros for out-of-source build
foreach(_file ${macros})
    configure_file(
        ${_file}
        ${PROJECT_BINARY_DIR}/${_file}
        COPYONLY
        )
endforeach()

set(BME_SCRIPTS
    init_vis.mac
    vis.mac
    gui.mac
    )

foreach(_script ${BME_SCRIPTS})
    configure_file(
        ${_script}
        ${PROJECT_BINARY_DIR}/${_script}
        COPYONLY
        )
endforeach()
# Compile application
add_executable(absorber main.cc ${sources} ${headers})
# Link it to Geant4
target_link_libraries(absorber ${Geant4_LIBRARIES})

install(TARGETS absorber DESTINATION bin)
