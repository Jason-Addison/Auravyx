# CMakeList.txt : CMake project for Auravyx, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("Auravyx")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Auravyx)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

file(GLOB_RECURSE AuravyxSource CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/Source/*.h" "${CMAKE_SOURCE_DIR}/Source/*.cpp")
# "${CMAKE_SOURCE_DIR}/Source/Auravyx/UI/Icon/Auravyx.rc"
add_executable(Auravyx ${AuravyxSource})

include_directories(src ${CMAKE_SOURCE_DIR}/Source)
include_directories(src ${CMAKE_SOURCE_DIR}/Dependencies/Include)#################

#####################
#       SOIL        #
#####################

add_subdirectory(${CMAKE_SOURCE_DIR}/Dependencies/Include/SOIL)
target_link_libraries(Auravyx SOIL)

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
#      OpenAL       #
#####################

set(OPENAL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/Dependencies/Include/AL/include)
if(WIN32)
    set(OPENAL_LIBRARY ${CMAKE_SOURCE_DIR}/Dependencies/Lib/windows/OpenAL32.lib)
    find_package(OpenAL 1.0 REQUIRED)
    if(OPENAL_FOUND)
        include_directories(${OPENAL_INCLUDE_DIR})
        target_link_libraries(Auravyx ${OPENAL_LIBRARY})
    endif()
elseif(UNIX)
add_library(openal SHARED IMPORTED)
set_property(TARGET openal PROPERTY IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/Dependencies/Lib/unix/libopenal.so)
target_link_libraries(Auravyx openal)
endif()
#####################
#      OpenSSL      #
#####################

add_subdirectory(${CMAKE_SOURCE_DIR}/Dependencies/Include/OpenSSL)

if(WIN32)
add_library(OpenSSL STATIC IMPORTED ${CMAKE_SOURCE_DIR}/Dependencies/Lib/windows/libssl_static.lib)
set_target_properties(OpenSSL PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/Dependencies/Lib/windows/libssl_static.lib)
target_link_libraries(Auravyx ${CMAKE_SOURCE_DIR}/Dependencies/Lib/windows/libssl_static.lib ${CMAKE_SOURCE_DIR}/Dependencies/Lib/windows/libcrypto_static.lib)
endif(WIN32)
if(UNIX)
add_library(OpenSSL STATIC IMPORTED ${CMAKE_SOURCE_DIR}/Dependencies/Lib/unix/libssl.a)
set_target_properties(OpenSSL PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/Dependencies/Lib/unix/libssl.a)
target_link_libraries(Auravyx ${CMAKE_SOURCE_DIR}/Dependencies/Lib/unix/libssl.a ${CMAKE_SOURCE_DIR}/Dependencies/Lib/unix/libcrypto.a)
endif(UNIX)
set_target_properties(OpenSSL PROPERTIES INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/Dependencies/Include/OpenSSL/include)
target_link_libraries(Auravyx OpenSSL)