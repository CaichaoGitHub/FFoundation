cmake_minimum_required(VERSION 3.13)
project(FFoundation)

set(CMAKE_CXX_STANDARD 11)

macro(ADD_FRAMEWORK PROJECT_NAME FRAMEWORK_NAME)
    find_library(
            "FRAMEWORK_${FRAMEWORK_NAME}"
            NAMES ${FRAMEWORK_NAME}
            PATHS ${CMAKE_OSX_SYSROOT}/System/Library
            PATH_SUFFIXES Frameworks
            NO_DEFAULT_PATH
    )
    if(${FRAMEWORK_${FRAMEWORK_NAME}} STREQUAL FRAMEWORK_${FRAMEWORK_NAME}-NOTFOUND)
        MESSAGE(ERROR ": Framework ${FRAMEWORK_NAME} not found")
    else()
        TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${FRAMEWORK_${FRAMEWORK_NAME}})
        message(STATUS "find framework: ${FRAMEWORK_${FRAMEWORK_NAME}}")
    endif()
endmacro(ADD_FRAMEWORK)

macro(ADD_APPLE_LIB PROJECT_NAME LIB_NAME)
    find_library(
            "LIB_${LIB_NAME}"
            NAMES ${LIB_NAME}
            PATHS ${CMAKE_OSX_SYSROOT}/usr/lib
            PATH_SUFFIXES tbd
            NO_DEFAULT_PATH
    )
    if(${LIB_${LIB_NAME}} STREQUAL LIB_${LIB_NAME}-NOTFOUND)
        MESSAGE(ERROR ": LIB ${LIB_NAME} not found")
    else()
        TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LIB_${LIB_NAME}})
        message(STATUS "find LIB: ${LIB_${LIB_NAME}}")
    endif()
endmacro(ADD_APPLE_LIB)

file(GLOB FilesInclude                              ${PROJECT_SOURCE_DIR}/include/*.*)
file(GLOB FilesIncludeAsset                         ${PROJECT_SOURCE_DIR}/include/FFAsset/*.*)
file(GLOB FilesSource                               ${PROJECT_SOURCE_DIR}/source/*.*)
file(GLOB FilesSourceAsset                          ${PROJECT_SOURCE_DIR}/source/FFAsset/*.*)

set(    FilesFoundation
        ${FilesInclude}
        ${FilesIncludeAsset}
        ${FilesSource}
        ${FilesSourceAsset}
        )


add_executable(FFoundation ${FilesFoundation})


if (IOS)
    
endif ()









