set(NCPROJECT_NAME "JugiCineHL")
set(NCPROJECT_EXE_NAME "JugiCineHL")
set(NCPROJECT_VENDOR "Jugilus")
set(NCPROJECT_COPYRIGHT "Copyright ©2016-2022 ${NCPROJECT_VENDOR}")
set(NCPROJECT_DESCRIPTION "JugiCineHL")
set(NCPROJECT_HOMEPAGE "https://jugicine.github.io")
set(NCPROJECT_REVERSE_DNS "io.github.jugicine.playerTestbed")

set(NCPROJECT_INCLUDE_DIRS
3rdParty
3rdParty/box2d
jugimap
jugimap/map
jugimap/source
jugimap/animation
jugimap/gui
jugicineHL
jugicineHL/actions
jugicineHL/widgets
jugicineHL/items
jugicineHL/components
jugicineHL/gfxObjects
jugicineHL/systems
jugicineHL/movements
jugicineHL/ai
jugicineHL/scripting
jugicineHL/entitySystem
)


set(NCPROJECT_SOURCES)


include(3rdParty/sources.cmake)
foreach(source IN LISTS 3rdParty_SOURCES)
	list(APPEND NCPROJECT_SOURCES 3rdParty/${source})
endforeach()

include(jugimap/sources.cmake)
foreach(source IN LISTS JUGIMAP_SOURCES)
	list(APPEND NCPROJECT_SOURCES jugimap/${source})
endforeach()

include(jugicineHL/sources.cmake)
foreach(source IN LISTS JUGICINEHL_SOURCES)
	list(APPEND NCPROJECT_SOURCES jugicineHL/${source})
endforeach()

include(player/sources.cmake)
foreach(source IN LISTS PLAYER_SOURCES)
	list(APPEND NCPROJECT_SOURCES player/${source})
endforeach()




set(hardcodedPaths OFF CACHE BOOL "Hard-coded paths")


if(${hardcodedPaths} STREQUAL "ON")

    cmake_minimum_required(VERSION 3.13)
    project(${NCPROJECT_NAME})

    add_executable(${NCPROJECT_EXE_NAME} WIN32 ${NCPROJECT_SOURCES})

    target_compile_definitions(${NCPROJECT_EXE_NAME} PUBLIC HARDCODED_PROJECT_PATH)


    list(APPEND NCPROJECT_INCLUDE_DIRS
        c:/Development/nCine/nCine/include
        c:/Development/nCine/nCine-external/include/GLFW
        c:/Development/nCine/nCine-build/generated/include
        c:/Development/nCine/nCine-build/imgui-src/include_only
        )

    target_include_directories(${NCPROJECT_EXE_NAME} PRIVATE ${NCPROJECT_INCLUDE_DIRS})

    #if($<CONFIG:Debug>)
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        target_link_directories(${NCPROJECT_EXE_NAME} PUBLIC "c:/Development/nCine/nCine-build/Debug")
        target_link_libraries(${NCPROJECT_EXE_NAME} ncine_main_d)
        target_link_libraries(${NCPROJECT_EXE_NAME} ncine_d)
        message("DEBUG linking")

    else()
        target_link_directories(${NCPROJECT_EXE_NAME} PUBLIC "c:/Development/nCine/nCine-build/Release")
        target_link_libraries(${NCPROJECT_EXE_NAME} ncine_main)
        target_link_libraries(${NCPROJECT_EXE_NAME} ncine)
        message("RELEASE linking")

    endif()


else()


    # Don't edit beyond this line
    if(CMAKE_SYSTEM_NAME STREQUAL "Android")
            return()
    endif()

    cmake_minimum_required(VERSION 3.13)
    project(${NCPROJECT_NAME})

    find_path(NCPROJECT_DIR cmake/project_main.cmake
            PATHS ${nCine_DIR} $ENV{NCINE_DIR}
            PATH_SUFFIXES project ../project ../../../project ../../../share/ncine/project
            NO_CMAKE_FIND_ROOT_PATH # For Emscripten
            DOC "Path to the nCine template project scripts")

    if(IS_DIRECTORY ${NCPROJECT_DIR})
            include(${NCPROJECT_DIR}/cmake/project_main.cmake)
    else()
            if(NOT DEFINED nCine_DIR AND "$ENV{NCINE_DIR}" STREQUAL "")
                    message(FATAL_ERROR "Set the nCine_DIR CMake variable or the NCINE_DIR environment variable to a valid nCine directory.")
            elseif(DEFINED nCine_DIR)
                    message(FATAL_ERROR "The nCine_DIR CMake variable \"${nCine_DIR}\" is not a valid nCine directory.")
            elseif(NOT "$ENV{NCINE_DIR}" STREQUAL "")
                    message(FATAL_ERROR "The NCINE_DIR environment variable \"$ENV{NCINE_DIR}\" is not a valid nCine directory.")
            endif()
    endif()

endif()
