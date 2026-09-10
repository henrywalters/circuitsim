MESSAGE("FINDING HAGAME2 LIB")
set(CMAKE_FIND_DEBUG_MODE TRUE)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
# The places to look for the tinyxml2 folders

set(
        FIND_HAGAME2_PATHS
        /home/henry/development/games/HaGameLite
)

set(
        FIND_HGE_PATHS
        /home/henry/development/games/hg-editor
)

find_path(
        HAGAME2_INCLUDE_DIR hagame.h
        PATH_SUFFIXES include
        PATHS ${FIND_HAGAME2_PATHS}
)

find_path(
        HGE_INCLUDE_DIR hge.h
        PATH_SUFFIXES include
        PATHS ${FIND_HGE_PATHS}
)

MESSAGE("CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(HAGAME2_LIB_SUFFIX "lib-debug")
    set(HGE_LIB_SUFFIX "lib-debug")
    MESSAGE("Using DEBUG hagame2/hge libs")
else()
    set(HAGAME2_LIB_SUFFIX "lib")
    set(HGE_LIB_SUFFIX "lib")
endif()

if(EMSCRIPTEN)
    MESSAGE("EMSCRIPTEN")
    find_library(HAGAME2_LIBRARY
            NAMES HaGame2
            PATH_SUFFIXES wasm
            PATHS ${FIND_HAGAME2_PATHS}
    )
    find_library(HGE_LIBRARY
            NAMES hge
            PATH_SUFFIXES wasm
            PATHS ${FIND_HGE_PATHS})
else()
    MESSAGE("UNIX")
    find_library(HAGAME2_LIBRARY
            NAMES HaGame2
            PATH_SUFFIXES ${HAGAME2_LIB_SUFFIX}
            PATHS ${FIND_HAGAME2_PATHS}
    )
    find_library(HGE_LIBRARY
            NAMES hge
            PATH_SUFFIXES ${HGE_LIB_SUFFIX}
            PATHS ${FIND_HGE_PATHS})
endif()

MESSAGE("${FIND_HAGAME2_PATHS}/thirdparty/box2d/build/bin/")

find_library(BOX2D_LIBRARY
        NAMES box2d
        PATH_SUFFIXES lib
        PATHS ${FIND_HAGAME2_PATHS}/thirdparty/box2d/build/bin/
)

# add_subdirectory(${FIND_HAGAME2_PATHS}/thirdparty/box2d/)

include_directories("${FIND_HAGAME2_PATHS}/thirdparty/entt/src/")
include_directories("${FIND_HAGAME2_PATHS}/thirdparty/box2d/include/")
include_directories("${FIND_HAGAME2_PATHS}/thirdparty/glad/include/")

set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

MESSAGE("FIND_HAGAME2_PATHS = ${FIND_HAGAME2_PATHS}")
MESSAGE("HAGAME2_INCLUDE_DIR = ${HAGAME2_INCLUDE_DIR}")
MESSAGE("HAGAME2_LIBRARY = ${HAGAME2_LIBRARY}")
MESSAGE("HGE_INCLUDE_DIR = ${HGE_INCLUDE_DIR}")
MESSAGE("HGE_LIBRARY = ${HGE_LIBRARY}")
MESSAGE("BOX2D_LIBRARY = ${BOX2D_LIBRARY}")