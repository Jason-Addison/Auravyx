# CMakeList.txt : CMake project for Auravyx, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("Auravyx")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()
# Add source to this project's executable.

file(GLOB_RECURSE AuravyxSource CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/Source/*.h" "${CMAKE_SOURCE_DIR}/Source/*.cpp")

add_executable(Auravyx ${AuravyxSource})

file(GLOB_RECURSE SRC_UI
    "GUI/TextMesh.cpp"
)
source_group("ui"            FILES ${SRC_UI})

include_directories(src ${CMAKE_SOURCE_DIR}/Source)
include_directories(src ${CMAKE_SOURCE_DIR}/Dependencies/Include)#################
#####################
#       GLFW        #
#####################

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/Dependencies/Include/GLFW)
target_link_libraries(Auravyx glfw)

#####################
#       GLEW        #
#####################

if(WIN32)
add_library(glew STATIC IMPORTED ${CMAKE_SOURCE_DIR}/Dependencies/Lib/windows/glew32s.lib)
set_target_properties(glew PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/Dependencies/Lib/windows/glew32s.lib)
endif(WIN32)
if(UNIX)
add_library(glew STATIC IMPORTED ${CMAKE_SOURCE_DIR}/Dependencies/Lib/unix/glew32.a)
set_target_properties(glew PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/Dependencies/Lib/unix/glew32.a)
endif(UNIX)
set_target_properties(glew PROPERTIES INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/Dependencies/Include/GL/include)
target_link_libraries(Auravyx glew)

#####################
#       SOIL        #
#####################

add_subdirectory(${CMAKE_SOURCE_DIR}/Dependencies/Include/SOIL)
target_link_libraries(Auravyx SOIL)

#####################
#      OpenAL       #
#####################

set(OPENAL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependencies/Include/AL/include)
set(OPENAL_LIBRARY ${CMAKE_SOURCE_DIR}/Dependencies/Lib/windows/OpenAL32.lib)

find_package(OpenAL 1.0 REQUIRED)
if(OPENAL_FOUND)
    include_directories(${OPENAL_INCLUDE_DIR})
    target_link_libraries(Auravyx ${OPENAL_LIBRARY})
endif()
#add_subdirectory(${CMAKE_SOURCE_DIR}/Dependencies/Include/openal-soft)
#add_library(alOpen ${CMAKE_SOURCE_DIR}/Dependencies/Include/openal-soft/OpenAL32.dll)
#set_target_properties(alOpen PROPERTIES LINKER_LANGUAGE C)
#target_link_libraries(Auravyx alOpen)
#include_directories(src ${CMAKE_SOURCE_DIR}/)

#####################
#      OpenSSL      #
#####################

add_subdirectory(${CMAKE_SOURCE_DIR}/Dependencies/Include/openssl)

if(WIN32)
add_library(OpenSSL STATIC IMPORTED ${CMAKE_SOURCE_DIR}/Dependencies/Lib/windows/libssl_static.lib)
set_target_properties(OpenSSL PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/Dependencies/Lib/windows/libssl_static.lib)
target_link_libraries(Auravyx ${CMAKE_SOURCE_DIR}/Dependencies/Lib/windows/libssl_static.lib ${CMAKE_SOURCE_DIR}/Dependencies/Lib/windows/libcrypto_static.lib)
endif(WIN32)
if(UNIX)
add_library(OpenSSL STATIC IMPORTED ${CMAKE_SOURCE_DIR}/Dependencies/Lib/unix/libssl.a)
set_target_properties(OpenSSL PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/Dependencies/Lib/unix/libssl.a)
target_link_libraries(Auravyx ${CMAKE_SOURCE_DIR}/Dependencies/Lib/unix/libssl.a ${CMAKE_SOURCE_DIR}/Dependencies/Lib/unix/libcrpyto.a)
endif(UNIX)
set_target_properties(OpenSSL PROPERTIES INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/Dependencies/Include/OpenSSL/include)
target_link_libraries(Auravyx OpenSSL)