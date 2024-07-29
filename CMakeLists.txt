cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(vEngine CXX)

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
 

set(PLATFORM_LINUX 0)
set(PLATFORM_WINDOWS 0)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    # 在Linux下执行的命令
    message(STATUS "Running on Linux")
    set(PLATFORM_LINUX 1)
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
    # 在Windows下执行的命令
    message(STATUS "Running on Windows")
    set(PLATFORM_WINDOWS 1)
endif()

add_compile_definitions(PLATFORM_WINDOWS=${PLATFORM_WINDOWS})
add_compile_definitions(PLATFORM_LINUX=${PLATFORM_LINUX})


################################################################################
# Global configuration types
################################################################################
set(CMAKE_CONFIGURATION_TYPES
    "Debug"
    "Release"
    CACHE STRING "" FORCE
)

################################################################################
# Global compiler options
################################################################################
if(MSVC)
    # remove default flags provided with CMake for MSVC
    set(CMAKE_CXX_FLAGS "")
    set(CMAKE_CXX_FLAGS_DEBUG "")
    set(CMAKE_CXX_FLAGS_RELEASE "")
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
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS}")
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS}")
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS}")
    set(CMAKE_STATIC_LINKER_FLAGS_RELEASE "${CMAKE_STATIC_LINKER_FLAGS}")
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



























set(PROJECT_NAME vEngine)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "Engine/Core/FrameInfo/FrameRenderInfo.cpp"
    "Engine/Core/FrameInfo/FrameRenderInfo.h"
    "Engine/Core/Render/FrameBuffer/FrameBufferData.cpp"
    "Engine/Core/Render/FrameBuffer/FrameBufferData.h"
    "Engine/Core/Render/RenderPass/vRenderPass.cpp"
    "Engine/Core/Render/RenderPass/vRenderPass.h"
    "Engine/Core/Render/RenderPostProcessing/RenderPostProcessing.cpp"
    "Engine/Core/Render/RenderPostProcessing/RenderPostProcessing.h"
    "Engine/Core/Texture/Texture2D/Texture2D.h"
    "Engine/Core/Texture/TextureInterface/ITexture.h"
    "Shader/CommonDef.hlsli"
    "Shader/CommonFunction.hlsli"
    "Shader/CommonStruct.hlsli"
    "Shader/DrawCube/DrawCube_vs_ps.hlsl"
    "Shader/DrawCube/DrawTexCube_vs_ps.hlsl"
    "Shader/DrawCube/PointLightCube_vs_ps.hlsl"
    "Shader/GlobalUniformBuffer.hlsli"
    "Shader/PostProcessing/Nothing_vs_ps.hlsl"
    "Shader/PostProcessing/PostProcessing.hlsli"
    "Shader/SkyBox/CommonSkyBox_vs_ps.hlsl"
)
source_group("" FILES ${no_group_source_files})

set(Header_Files
    "Component/Common/BasicMove/BasicCameraMove.h"
    "Component/Common/EscExit/PressEscExit.h"
    "Component/Envriment/Skybox/Skybox.h"
    "Engine/Core/Camera/Camera.h"
    "Engine/Core/CommonRenderCmd/CommonRenderCmd.h"
    "Engine/Core/Component/Component.h"
    "Engine/Core/Component/SceneComponent/SceneComponent.h"
    "Engine/Core/Container/Name.h"
    "Engine/Core/Container/Transform.h"
    "Engine/Core/Fence/Fence.h"
    "Engine/Core/Level/Level.h"
    "Engine/Core/Material/Material.h"
    "Engine/Core/Material/MaterialRenderPipeline.h"
    "Engine/Core/MemoryBuffer/MeshVertexBuffer.h"
    "Engine/Core/Object/Object.h"
    "Engine/Core/Object/ObjectHandle/ObjectHandle.h"
    "Engine/Core/Render/RenderCommandQueue.h"
    "Engine/Core/Render/Rendering/ForwardRendering/ForwardRendering.h"
    "Engine/Core/Render/Rendering/IRendering.h"
    "Engine/Core/Render/RenderThread/RenderThread.h"
    "Engine/Core/Render/SceneComponentRenderInfo.h"
    "Engine/Core/Semaphore/Semaphore.h"
    "Engine/Core/ShaderModule/DescriptorHelper.h"
    "Engine/Core/ShaderModule/Shader.h"
    "Engine/Core/ShaderReflector/ShaderReflector.h"
    "Engine/Core/Texture/TextureCube/TextureCube.h"
    "Engine/Core/Texture/TextureSampler/TextureSampler.h"
    "Engine/Core/UniformBuffer/GlobalUniformBuffer/GlobalUniformBufferManager.h"
    "Engine/Core/UniformBuffer/UniformBuffer.h"
    "Engine/Runtime/Component/CubeComponent.h"
    "Engine/Runtime/Component/Light/PointLight.h"
    "Engine/SubSystem/AssetSystem/AssetSystem.h"
    "Engine/SubSystem/InputSystem/InputSysten.h"
    "Engine/TextureFile/TextureFile.h"
    "Engine/Toolkit/Container/Queue.h"
    "Engine/Toolkit/FileToolKit/FileToolKit.h"
    "Engine/Toolkit/math_utils.h"
    "Engine/Toolkit/Timer/Timer.h"
    "Engine/TypeDef.h"
    "Engine/vEngine.h"
    "Engine/VulkanHelper/VkHelper.h"
    "Header.h"
    "Level/Cube/DrawCube.h"
    "Level/Cube/DrawLightCube.h"
    "Level/Cube/DrawTexCube.h"
    "Level/Skybox/DrawSkybox.h"
    "LogPrinter/Log.h"
    "Meshs/Box/BoxVertices.h"
    "ThirdParty/imgui/backends/imgui_impl_sdl2.h"
    "ThirdParty/imgui/backends/imgui_impl_vulkan.h"
    "ThirdParty/imgui/backends/imgui_impl_win32.h"
    "ThirdParty/imgui/imconfig.h"
    "ThirdParty/imgui/imgui.h"
    "ThirdParty/imgui/imgui_internal.h"
    "ThirdParty/imgui/imstb_rectpack.h"
    "ThirdParty/imgui/imstb_textedit.h"
    "ThirdParty/imgui/imstb_truetype.h"
    "ThirdParty/imgui/misc/cpp/imgui_stdlib.h"
    "ThirdParty/imgui/misc/single_file/imgui_single_file.h"
    "ThirdParty/SPIRV-Cross/GLSL.std.450.h"
    "ThirdParty/SPIRV-Cross/spirv.h"
    "ThirdParty/SPIRV-Cross/spirv.hpp"
    "ThirdParty/SPIRV-Cross/spirv_cfg.hpp"
    "ThirdParty/SPIRV-Cross/spirv_common.hpp"
    "ThirdParty/SPIRV-Cross/spirv_cpp.hpp"
    "ThirdParty/SPIRV-Cross/spirv_cross.hpp"
    "ThirdParty/SPIRV-Cross/spirv_cross_c.h"
    "ThirdParty/SPIRV-Cross/spirv_cross_containers.hpp"
    "ThirdParty/SPIRV-Cross/spirv_cross_error_handling.hpp"
    "ThirdParty/SPIRV-Cross/spirv_cross_parsed_ir.hpp"
    "ThirdParty/SPIRV-Cross/spirv_cross_util.hpp"
    "ThirdParty/SPIRV-Cross/spirv_glsl.hpp"
    "ThirdParty/SPIRV-Cross/spirv_hlsl.hpp"
    "ThirdParty/SPIRV-Cross/spirv_msl.hpp"
    "ThirdParty/SPIRV-Cross/spirv_parser.hpp"
    "ThirdParty/SPIRV-Cross/spirv_reflect.hpp"
    "ThirdParty/stb/stb_image.h"
    "ThirdParty/StringFormater/StringFormater.h"
)
source_group("Header Files" FILES ${Header_Files})

set(Source_Files
    "Component/Common/BasicMove/BasicCameraMove.cpp"
    "Component/Common/EscExit/PressEscExit.cpp"
    "Component/Envriment/Skybox/Skybox.cpp" 
    "Engine/Core/Camera/Camera.cpp"
    "Engine/Core/CommonRenderCmd/CommonRenderCmd.cpp"
    "Engine/Core/Component/Component.cpp"
    "Engine/Core/Component/SceneComponent/SceneComponent.cpp"
    "Engine/Core/Container/Name.cpp"
    "Engine/Core/Container/Transform.cpp"
    "Engine/Core/Fence/Fence.cpp"
    "Engine/Core/Level/Level.cpp"
    "Engine/Core/Material/Material.cpp"
    "Engine/Core/Material/MaterialRenderPipeline.cpp"
    "Engine/Core/MemoryBuffer/MeshVertexBuffer.cpp"
    "Engine/Core/Object/Object.cpp"
    "Engine/Core/Object/ObjectHandle/ObjectHandle.cpp"
    "Engine/Core/Render/RenderCommandQueue.cpp"
    "Engine/Core/Render/Rendering/ForwardRendering/ForwardRendering.cpp"
    "Engine/Core/Render/Rendering/IRendering.cpp"
    "Engine/Core/Render/RenderThread/RenderThread.cpp"
    "Engine/Core/Render/SceneComponentRenderInfo.cpp"
    "Engine/Core/Semaphore/Semaphore.cpp"
    "Engine/Core/ShaderModule/DescriptorHelper.cpp"
    "Engine/Core/ShaderModule/Shader.cpp"
    "Engine/Core/ShaderReflector/ShaderReflector.cpp"
    "Engine/Core/Texture/Texture2D/Texture2D.cpp"
    "Engine/Core/Texture/TextureCube/TextureCube.cpp"
    "Engine/Core/Texture/TextureInterface/ITexture.cpp"
    "Engine/Core/Texture/TextureSampler/TextureSampler.cpp"
    "Engine/Core/UniformBuffer/GlobalUniformBuffer/GlobalUniformBufferManager.cpp"
    "Engine/Core/UniformBuffer/UniformBuffer.cpp"
    "Engine/Runtime/Component/CubeComponent.cpp"
    "Engine/Runtime/Component/Light/PointLight.cpp"
    "Engine/SubSystem/AssetSystem/AssetSystem.cpp"
    "Engine/SubSystem/InputSystem/InputSysten.cpp"
    "Engine/TextureFile/TextureFile.cpp"
    "Engine/Toolkit/Container/Queue.cpp"
    "Engine/Toolkit/FileToolKit/FileToolKit.cpp"
    "Engine/Toolkit/FileToolKit/stb_image.cpp"
    "Engine/Toolkit/math_utils.cpp"
    "Engine/Toolkit/Timer/Timer.cpp"
    "Engine/vEngine.cpp"
    "Engine/VulkanHelper/VkHelper.cpp"
    "Level/Cube/DrawCube.cpp"
    "Level/Cube/DrawLightCube.cpp"
    "Level/Cube/DrawTexCube.cpp"
    "Level/Skybox/DrawSkybox.cpp"
    "LogPrinter/Log.cpp"
    "Main.cpp"
    "Meshs/Box/BoxVertices.cpp"
    "ThirdParty/imgui/backends/imgui_impl_sdl2.cpp"
    "ThirdParty/imgui/backends/imgui_impl_vulkan.cpp"
    "ThirdParty/imgui/imgui.cpp"
    "ThirdParty/imgui/imgui_demo.cpp"
    "ThirdParty/imgui/imgui_draw.cpp"
    "ThirdParty/imgui/imgui_tables.cpp"
    "ThirdParty/imgui/imgui_widgets.cpp"
    "ThirdParty/imgui/misc/cpp/imgui_stdlib.cpp"
    "ThirdParty/SPIRV-Cross/spirv_cfg.cpp"
    "ThirdParty/SPIRV-Cross/spirv_cpp.cpp"
    "ThirdParty/SPIRV-Cross/spirv_cross.cpp"
    "ThirdParty/SPIRV-Cross/spirv_cross_c.cpp"
    "ThirdParty/SPIRV-Cross/spirv_cross_parsed_ir.cpp"
    "ThirdParty/SPIRV-Cross/spirv_cross_util.cpp"
    "ThirdParty/SPIRV-Cross/spirv_glsl.cpp"
    "ThirdParty/SPIRV-Cross/spirv_hlsl.cpp"
    "ThirdParty/SPIRV-Cross/spirv_msl.cpp"
    "ThirdParty/SPIRV-Cross/spirv_parser.cpp"
    "ThirdParty/SPIRV-Cross/spirv_reflect.cpp"
    "ThirdParty/StringFormater/StringFormater.cpp"
)
source_group("Source Files" FILES ${Source_Files})

set(ALL_FILES
    ${no_group_source_files}
    ${Header_Files}
    ${Source_Files}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE vEngine)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_GLOBAL_KEYWORD "Win32Proj"
)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()
################################################################################
# MSVC runtime library
################################################################################
get_property(MSVC_RUNTIME_LIBRARY_DEFAULT TARGET ${PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY)
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    string(CONCAT "MSVC_RUNTIME_LIBRARY_STR"
        $<$<CONFIG:Release>:
            MultiThreadedDebugDLL
        >
        $<$<NOT:$<OR:$<CONFIG:Release>>>:${MSVC_RUNTIME_LIBRARY_DEFAULT}>
    )
endif()
set_target_properties(${PROJECT_NAME} PROPERTIES MSVC_RUNTIME_LIBRARY ${MSVC_RUNTIME_LIBRARY_STR})

################################################################################
# Include directories
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_include_directories(${PROJECT_NAME} PUBLIC
        "$<$<CONFIG:Debug>:"
            "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/lib-glfw/include/GLFW"
        ">"
        "$<$<CONFIG:Release>:"
            "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/lib-glfw/include"
        ">"
        "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/imgui;"
        "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/SDL/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/VulkanSdk/Include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/Vulkan-Hpp/glm;"
        "${CMAKE_CURRENT_SOURCE_DIR}/."
    )
endif()

################################################################################
# Compile definitions
################################################################################
if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "_CONSOLE;"
        "GLM_FORCE_RADIANS;"
        "GLM_FORCE_DEPTH_ZERO_TO_ONE;"
        "UNICODE;"
        "_UNICODE"
    )
elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "_DEBUG"
        ">"
        "$<$<CONFIG:Release>:"
            "NDEBUG"
        ">"
        "WIN32;"
        "_CONSOLE;"
        "UNICODE;"
        "_UNICODE"
    )
endif()
set_source_files_properties(Engine/Core/Container/Transform.cpp PROPERTIES
    COMPILE_DEFINITIONS "_DEBUG;_CONSOLE;GLM_FORCE_RADIANS;GLM_FORCE_DEPTH_ZERO_TO_ONE;_UNICODE;UNICODE;"
)
set_source_files_properties(Engine/Core/Render/RenderPostProcessing/RenderPostProcessing.cpp PROPERTIES
    COMPILE_DEFINITIONS "_DEBUG;_CONSOLE;GLM_FORCE_RADIANS;GLM_FORCE_DEPTH_ZERO_TO_ONE;_UNICODE;UNICODE;"
)
set_source_files_properties(Engine/Core/Texture/TextureCube/TextureCube.cpp PROPERTIES
    COMPILE_DEFINITIONS "_DEBUG;_CONSOLE;GLM_FORCE_RADIANS;GLM_FORCE_DEPTH_ZERO_TO_ONE;_UNICODE;UNICODE;"
)
set_source_files_properties(Engine/Runtime/Component/CubeComponent.cpp PROPERTIES
    COMPILE_DEFINITIONS "_DEBUG;_CONSOLE;GLM_FORCE_RADIANS;GLM_FORCE_DEPTH_ZERO_TO_ONE;_UNICODE;UNICODE;"
)
set_source_files_properties(Engine/Toolkit/FileToolKit/stb_image.cpp PROPERTIES
    COMPILE_DEFINITIONS "_DEBUG;_CONSOLE;GLM_FORCE_RADIANS;GLM_FORCE_DEPTH_ZERO_TO_ONE;_UNICODE;UNICODE;"
)
set_source_files_properties(Level/Cube/DrawCube.cpp PROPERTIES
    COMPILE_DEFINITIONS "_DEBUG;_CONSOLE;GLM_FORCE_RADIANS;GLM_FORCE_DEPTH_ZERO_TO_ONE;_UNICODE;UNICODE;"
)

################################################################################
# Compile and link options
################################################################################
if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /arch:AVX2;
                /fp:fast
            >
            $<$<CONFIG:Release>:
                /O2;
                /Oi;
                /Gy
            >
            /permissive-;
            /MP;
            /std:c++17;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            /utf-8;
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /Oi;
                /Gy
            >
            /permissive-;
            /sdl;
            /W3;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )
    endif()
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /DEBUG:FULL
            >
            $<$<CONFIG:Release>:
                /DEBUG;
                /OPT:REF;
                /OPT:ICF
            >
            /SUBSYSTEM:CONSOLE
        )
    elseif("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x86")
        target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF
            >
            /DEBUG;
            /SUBSYSTEM:CONSOLE
        )
    endif()
endif()

################################################################################
# Pre build events
################################################################################
#if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
#    add_custom_command_if(
#        TARGET ${PROJECT_NAME}
#        PRE_BUILD
#        COMMANDS
#        COMMAND   $<CONFIG:Debug> call $<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/>Shader\\CompileShader.bat
#        COMMAND $<CONFIG:Release> $<SHELL_PATH:${CMAKE_CURRENT_SOURCE_DIR}/>Shader\\CompileShader.bat
#    )
#endif()

################################################################################
# Dependencies
################################################################################

if(MSVC)
    if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
        set(ADDITIONAL_LIBRARY_DEPENDENCIES
                "vulkan-1;"
                "glfw3;"
                "SDL2;"
                "winmm;"
                "setupapi;"
                "version"
        )
    endif()
endif ()

if(${PLATFORM_LINUX})
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
            "libvulkan.so;"
#            "glfw3;"
#            "SDL2;"
#            "winmm;"
#            "setupapi;"
#            "version"
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/lib-sdl/libSDL2.a")
    set(VULKAN_HPP_SAMPLES_BUILD False)
#    add_subdirectory("ThirdParty/Vulkan-Hpp/")
    
endif ()

target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

if("${CMAKE_VS_PLATFORM_NAME}" STREQUAL "x64")
    target_link_directories(${PROJECT_NAME} PRIVATE
        "$<$<CONFIG:Debug>:"
            "${CMAKE_CURRENT_SOURCE_DIR}//ThirdParty/lib-sdl/x64/Debug;"
            "${CMAKE_CURRENT_SOURCE_DIR}//ThirdParty/lib-glfw/Debug"
        ">"
        "$<$<CONFIG:Release>:"
            "${CMAKE_CURRENT_SOURCE_DIR}//ThirdParty/lib-sdl/x64/Release;"
            "${CMAKE_CURRENT_SOURCE_DIR}//ThirdParty/lib-glfw/Release;"
            "$ENV{UniversalCRT_LibraryPath_x64}"
        ">"
        "${CMAKE_CURRENT_SOURCE_DIR}//ThirdParty/VulkanSdk/Lib"
    )
endif()

