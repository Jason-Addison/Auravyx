cmake_minimum_required(VERSION 3.15.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(Auravyx CXX)

################################################################################
# Set target arch type if empty. Visual studio solution generator provides it.
################################################################################
if(NOT CMAKE_VS_PLATFORM_NAME)
    set(CMAKE_VS_PLATFORM_NAME "x64")
endif()
message("${CMAKE_VS_PLATFORM_NAME} architecture in use")

if(NOT ("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64"
     OR "${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86"))
    message(FATAL_ERROR "${CMAKE_VS_PLATFORM_NAME} arch is not supported!")
endif()

################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "DebugR"
    "Release"
    "ReleaseVerbose"
    CACHE STRING "" FORCE
)

################################################################################
# Global compiler options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_DEBUGR "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
    set(CMAKE_CXX_FLAGS_RELEASEVERBOSE "")
endif()

################################################################################
# Global linker options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_EXE_LINKER_FLAGS "")
    set(CMAKE_MODULE_LINKER_FLAGS "")
    set(CMAKE_SHARED_LINKER_FLAGS "")
    set(CMAKE_STATIC_LINKER_FLAGS "")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUG "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUGR "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUGR "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUGR "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_DEBUGR "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS_RELEASEVERBOSE "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASEVERBOSE "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASEVERBOSE "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASEVERBOSE "${CMAKE_STATIC_LINKER_FLAGS}")
endif()

################################################################################
# Nuget packages function stub.
################################################################################
function(use_package TARGET PACKAGE VERSION)
    message(WARNING "No implementation of use_package. Create yours. "
                    "Package \"${PACKAGE}\" with version \"${VERSION}\" "
                    "for target \"${TARGET}\" is ignored!")
endfunction()

################################################################################
# Common utils
################################################################################
include(CMake/Utils.cmake)

################################################################################
# Additional Global Settings(add specific info there)
################################################################################
include(CMake/GlobalSettingsInclude.cmake OPTIONAL)

################################################################################
# Use solution folders feature
################################################################################
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

################################################################################
# Sub-projects
################################################################################



























set(PROJECT_NAME Auravyx)

################################################################################
# Source groups
################################################################################
set(Header_Files
    "Mouse.h"
    "resource.h"
    "ServerHost/Client.h"
    "ServerHost/HostChunk.h"
    "ServerHost/M.h"
    "ServerHost/Packet.h"
    "ServerHost/PacketMsg.h"
    "ServerHost/PacketPlayerPosition.h"
    "ServerHost/PacketTileChange.h"
    "ServerHost/Player.h"
    "ServerHost/Server.h"
    "ServerHost/ServerManager.h"
    "Source/Audio/Sound.h"
    "Source/Audio/SoundManager.h"
    "Source/Audio/WAVE.h"
    "Source/Auravyx.h"
    "Source/Engine/Action.h"
    "Source/Engine/Camera.h"
    "Source/Engine/Chat.h"
    "Source/Engine/Clock.h"
    "Source/Engine/Control.h"
    "Source/Engine/Controller.h"
    "Source/Engine/Entity.h"
    "Source/Engine/FBO.h"
    "Source/Engine/GameManager.h"
    "Source/Engine/GameState.h"
    "Source/Engine/GLManager.h"
    "Source/Engine/Model.h"
    "Source/Engine/Mouse.h"
    "Source/Engine/OBJMaterial.h"
    "Source/Engine/State.h"
    "Source/Engine/Texture.h"
    "Source/Engine/WindowManager.h"
    "Source/Graphics/Font.h"
    "Source/Graphics/FontChar.h"
    "Source/Graphics/FontLoader.h"
    "Source/Graphics/FontShader.h"
    "Source/Graphics/GFX.h"
    "Source/Graphics/GLContext.h"
    "Source/Graphics/Renderer.h"
    "Source/GUI/Button.h"
    "Source/GUI/DropdownMenu.h"
    "Source/GUI/GUI.h"
    "Source/GUI/GUIContainer.h"
    "Source/GUI/GUIContainerData.h"
    "Source/GUI/Image.h"
    "Source/GUI/ListMenu.h"
    "Source/GUI/ScrollBar.h"
    "Source/GUI/TextBox.h"
    "Source/Modify/Mod.h"
    "Source/Modify/Modify.h"
    "Source/pch.h"
    "Source/Physics/Physics.h"
    "Source/Physics/PhysicsAABB.h"
    "Source/Physics/PhysicsObject.h"
    "Source/Physics/PhysicsPrism.h"
    "Source/Physics/PhysicsSphere.h"
    "Source/Physics/PhysicsTriangles.h"
    "Source/Physics/PhysicsWorld.h"
    "Source/Server/ClientManager.h"
    "Source/Server/HostChunk.h"
    "Source/Shader/DeferredShader.h"
    "Source/Shader/ImageShader.h"
    "Source/Shader/LineShader.h"
    "Source/Shader/ModelShader.h"
    "Source/Shader/RectShader.h"
    "Source/Shader/Shader.h"
    "Source/Shader/Shaders.h"
    "Source/Shader/ShadowMap.h"
    "Source/Shader/ShadowShader.h"
    "Source/Shader/SkyShader.h"
    "Source/stdafx.h"
    "Source/targetver.h"
    "Source/Utilities/Assets.h"
    "Source/Utilities/Clipboard.hpp"
    "Source/Utilities/FileIO.h"
    "Source/Utilities/Log.h"
    "Source/Utilities/Matrix.h"
    "Source/Utilities/Matrix4f.h"
    "Source/Utilities/Profiler.h"
    "Source/Utilities/RandomNoise.h"
    "Source/Utilities/Resource.h"
    "Source/Utilities/Settings.h"
    "Source/Utilities/StringUtil.h"
    "Source/Utilities/Util.h"
    "Source/Utilities/Vec3f.h"
    "Source/Utilities/Vec4f.h"
    "Source/World/Chunk.h"
    "Source/World/ChunkHeight.h"
    "Source/World/ChunkIO.h"
    "Source/World/Voxel.h"
    "Source/World/VoxelShader.h"
    "Source/World/World.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Resource_Files
    "README.md"
)
source_group("Resource Files" FILES ${Resource_Files})

set(Shaders
    "x64/Release/Shaders/Base/2DImage.frag"
    "x64/Release/Shaders/Base/2DImage.vert"
    "x64/Release/Shaders/Base/Font.frag"
    "x64/Release/Shaders/Base/Font.vert"
    "x64/Release/Shaders/Base/Rect.frag"
    "x64/Release/Shaders/Base/Rect.vert"
    "x64/Release/Shaders/Regular/3DLine.frag"
    "x64/Release/Shaders/Regular/3DLine.vert"
    "x64/Release/Shaders/Regular/Deferred.frag"
    "x64/Release/Shaders/Regular/Deferred.vert"
    "x64/Release/Shaders/Regular/Model.frag"
    "x64/Release/Shaders/Regular/Model.vert"
    "x64/Release/Shaders/Regular/shader1.vert"
    "x64/Release/Shaders/Regular/Shadow.frag"
    "x64/Release/Shaders/Regular/Shadow.vert"
    "x64/Release/Shaders/Regular/Sky.frag"
    "x64/Release/Shaders/Regular/Sky.vert"
    "x64/Release/Shaders/Regular/Voxel.frag"
    "x64/Release/Shaders/Regular/Voxel.vert"
)
source_group("Shaders" FILES ${Shaders})

set(Source_Files
    "ServerHost/Client.cpp"
    "ServerHost/Packet.cpp"
    "ServerHost/PacketMsg.cpp"
    "ServerHost/PacketPlayerPosition.cpp"
    "ServerHost/PacketTileChange.cpp"
    "ServerHost/Player.cpp"
    "ServerHost/Server.cpp"
    "ServerHost/ServerManager.cpp"
    "Source/Audio/Sound.cpp"
    "Source/Audio/SoundManager.cpp"
    "Source/Audio/WAVE.cpp"
    "Source/Auravyx.cpp"
    "Source/Engine/Action.cpp"
    "Source/Engine/Camera.cpp"
    "Source/Engine/Clock.cpp"
    "Source/Engine/Control.cpp"
    "Source/Engine/Controller.cpp"
    "Source/Engine/Entity.cpp"
    "Source/Engine/FBO.cpp"
    "Source/Engine/GameManager.cpp"
    "Source/Engine/GameState.cpp"
    "Source/Engine/GLManager.cpp"
    "Source/Engine/Model.cpp"
    "Source/Engine/Mouse.cpp"
    "Source/Engine/OBJMaterial.cpp"
    "Source/Engine/State.cpp"
    "Source/Engine/Texture.cpp"
    "Source/Engine/WindowManager.cpp"
    "Source/Graphics/Font.cpp"
    "Source/Graphics/FontChar.cpp"
    "Source/Graphics/FontLoader.cpp"
    "Source/Graphics/FontShader.cpp"
    "Source/Graphics/GFX.cpp"
    "Source/Graphics/GLContext.cpp"
    "Source/Graphics/Renderer.cpp"
    "Source/GUI/Button.cpp"
    "Source/GUI/DropdownMenu.cpp"
    "Source/GUI/GUI.cpp"
    "Source/GUI/GUIContainer.cpp"
    "Source/GUI/GUIContainerData.cpp"
    "Source/GUI/Image.cpp"
    "Source/GUI/ListMenu.cpp"
    "Source/GUI/ScrollBar.cpp"
    "Source/GUI/TextBox.cpp"
    "Source/Modify/Mod.cpp"
    "Source/Modify/Modify.cpp"
    "Source/pch.cpp"
    "Source/Physics/Physics.cpp"
    "Source/Physics/PhysicsAABB.cpp"
    "Source/Physics/PhysicsObject.cpp"
    "Source/Physics/PhysicsPrism.cpp"
    "Source/Physics/PhysicsSphere.cpp"
    "Source/Physics/PhysicsTriangles.cpp"
    "Source/Physics/PhysicsWorld.cpp"
    "Source/Server/Chat.cpp"
    "Source/Server/ClientManager.cpp"
    "Source/Shader/DeferredShader.cpp"
    "Source/Shader/ImageShader.cpp"
    "Source/Shader/LineShader.cpp"
    "Source/Shader/ModelShader.cpp"
    "Source/Shader/RectShader.cpp"
    "Source/Shader/Shader.cpp"
    "Source/Shader/Shaders.cpp"
    "Source/Shader/ShadowMap.cpp"
    "Source/Shader/ShadowShader.cpp"
    "Source/Shader/SkyShader.cpp"
    "Source/stdafx.cpp"
    "Source/Utilities/Assets.cpp"
    "Source/Utilities/FileIO.cpp"
    "Source/Utilities/Log.cpp"
    "Source/Utilities/M.cpp"
    "Source/Utilities/Matrix.cpp"
    "Source/Utilities/Matrix4f.cpp"
    "Source/Utilities/Profiler.cpp"
    "Source/Utilities/RandomNoise.cpp"
    "Source/Utilities/Resource.cpp"
    "Source/Utilities/Settings.cpp"
    "Source/Utilities/Source.cpp"
    "Source/Utilities/StringUtil.cpp"
    "Source/Utilities/Util.cpp"
    "Source/Utilities/Vec3f.cpp"
    "Source/Utilities/Vec4f.cpp"
    "Source/World/Chunk.cpp"
    "Source/World/ChunkHeight.cpp"
    "Source/World/ChunkIO.cpp"
    "Source/World/Voxel.cpp"
    "Source/World/VoxelShader.cpp"
    "Source/World/World.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${Header_Files}
    ${Resource_Files}
    ${Shaders}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE Auravyx)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASEVERBOSE "TRUE"
        INTERPROCEDURAL_OPTIMIZATION_RELEASE        "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASEVERBOSE "TRUE"
        INTERPROCEDURAL_OPTIMIZATION_RELEASE        "TRUE"
    )
endif()
################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "$<$<CONFIG:Debug>:"
            "${CMAKE_CURRENT_SOURCE_DIR}/ServerHost;"
            "${CMAKE_CURRENT_SOURCE_DIR}/Source"
        ">"
        "$<$<CONFIG:ReleaseVerbose>:"
            "${CMAKE_CURRENT_SOURCE_DIR}/ServerHost;"
            "${CMAKE_CURRENT_SOURCE_DIR}/Source"
        ">"
        "$<$<CONFIG:Release>:"
            "${CMAKE_CURRENT_SOURCE_DIR}/ServerHost;"
            "${CMAKE_CURRENT_SOURCE_DIR}/Source"
        ">"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "$<$<CONFIG:Debug>:"
            "${CMAKE_CURRENT_SOURCE_DIR}/ServerHost;"
            "${CMAKE_CURRENT_SOURCE_DIR}/Source"
        ">"
        "$<$<CONFIG:ReleaseVerbose>:"
            "${CMAKE_CURRENT_SOURCE_DIR}/ServerHost;"
            "${CMAKE_CURRENT_SOURCE_DIR}/Source"
        ">"
        "$<$<CONFIG:Release>:"
            "${CMAKE_CURRENT_SOURCE_DIR}/ServerHost;"
            "${CMAKE_CURRENT_SOURCE_DIR}/Source"
        ">"
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_CRT_SECURE_NO_WARNINGS;"
            "UNICODE;"
            "_UNICODE"
        ">"
        "$<$<CONFIG:ReleaseVerbose>:"
            "_CRT_SECURE_NO_WARNINGS;"
            "UNICODE;"
            "_UNICODE"
        ">"
        "$<$<CONFIG:Release>:"
            "_CRT_SECURE_NO_WARNINGS;"
            "UNICODE;"
            "_UNICODE"
        ">"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_CRT_SECURE_NO_WARNINGS;"
            "UNICODE;"
            "_UNICODE"
        ">"
        "$<$<CONFIG:ReleaseVerbose>:"
            "_CRT_SECURE_NO_WARNINGS;"
            "UNICODE;"
            "_UNICODE"
        ">"
        "$<$<CONFIG:Release>:"
            "_CRT_SECURE_NO_WARNINGS;"
            "UNICODE;"
            "_UNICODE"
        ">"
    )
endif()

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /permissive-;
                /std:c++17;
                /Od;
                /sdl;
                /W3;
                /TP
            >
            $<$<CONFIG:ReleaseVerbose>:
                /permissive-;
                /std:c++17;
                /O2;
                /Oi;
                /sdl;
                /Gy;
                /W3;
                /TP;
                /Y-
            >
            $<$<CONFIG:Release>:
                /permissive-;
                /std:c++17;
                /O2;
                /Oi;
                /sdl;
                /Gy;
                /W3;
                /TP;
                /Y-
            >
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /permissive-;
                /std:c++17;
                /Od;
                /sdl;
                /W3;
                /TP
            >
            $<$<CONFIG:ReleaseVerbose>:
                /permissive-;
                /std:c++17;
                /O2;
                /Oi;
                /sdl;
                /Gy;
                /W3;
                /TP;
                /Y-
            >
            $<$<CONFIG:Release>:
                /permissive-;
                /std:c++17;
                /O2;
                /Oi;
                /sdl;
                /Gy;
                /W3;
                /TP;
                /Y-
            >
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:DebugR>:
                /DEBUG:FULL
            >
            $<$<CONFIG:Debug>:
                /DEBUG;
                /SUBSYSTEM:CONSOLE;
                /INCREMENTAL
            >
            $<$<CONFIG:ReleaseVerbose>:
                /DEBUG;
                /SUBSYSTEM:WINDOWS;
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            $<$<CONFIG:Release>:
                /DEBUG;
                /SUBSYSTEM:CONSOLE;
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:DebugR>:
                /DEBUG:FULL
            >
            $<$<CONFIG:Debug>:
                /DEBUG;
                /MACHINE:X64;
                /SUBSYSTEM:CONSOLE;
                /INCREMENTAL
            >
            $<$<CONFIG:ReleaseVerbose>:
                /DEBUG;
                /MACHINE:X64;
                /SUBSYSTEM:WINDOWS;
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
            $<$<CONFIG:Release>:
                /DEBUG;
                /MACHINE:X64;
                /SUBSYSTEM:CONSOLE;
                /OPT:REF;
                /OPT:ICF;
                /INCREMENTAL:NO
            >
        )
    endif()
endif()

################################################################################
# Dependencies
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "$<$<CONFIG:Debug>:"
            "glew32s;"
            "ALut;"
            "OpenAL32;"
            "opengl32;"
            "glfw3;"
            "SOIL"
        ">"
        "$<$<CONFIG:ReleaseVerbose>:"
            "glew32s;"
            "ALut;"
            "OpenAL32;"
            "opengl32;"
            "glfw3;"
            "SOIL"
        ">"
        "$<$<CONFIG:Release>:"
            "glew32s;"
            "ALut;"
            "OpenAL32;"
            "opengl32;"
            "glfw3;"
            "SOIL"
        ">"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "$<$<CONFIG:Debug>:"
            "glew32s;"
            "ALut;"
            "OpenAL32;"
            "opengl32;"
            "glfw3;"
            "SOIL"
        ">"
        "$<$<CONFIG:ReleaseVerbose>:"
            "glew32s;"
            "ALut;"
            "OpenAL32;"
            "opengl32;"
            "glfw3;"
            "SOIL"
        ">"
        "$<$<CONFIG:Release>:"
            "glew32s;"
            "ALut;"
            "OpenAL32;"
            "opengl32;"
            "glfw3;"
            "SOIL"
        ">"
    )
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_link_directories(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "${CMAKE_CURRENT_SOURCE_DIR}//Source/Library"
        ">"
        "$<$<CONFIG:ReleaseVerbose>:"
            "${CMAKE_CURRENT_SOURCE_DIR}//Source/Library"
        ">"
        "$<$<CONFIG:Release>:"
            "${CMAKE_CURRENT_SOURCE_DIR}//Source/Library"
        ">"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_link_directories(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "${CMAKE_CURRENT_SOURCE_DIR}//Source/Library"
        ">"
        "$<$<CONFIG:ReleaseVerbose>:"
            "${CMAKE_CURRENT_SOURCE_DIR}//Source/Library"
        ">"
        "$<$<CONFIG:Release>:"
            "${CMAKE_CURRENT_SOURCE_DIR}//Source/Library"
        ">"
    )
endif()

