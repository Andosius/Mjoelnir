cmake_minimum_required(VERSION 3.20)

# Declare project
project(Mjoelnir)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")


# Set main build flags
set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
set(CMAKE_CONFIGURATION_TYPES Release CACHE STRING "" FORCE)
set(CMAKE_GENERATOR_PLATFORM Win32 CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVK_PROTOTYPES" CACHE STRING "" FORCE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_PROTOTYPES" CACHE STRING "" FORCE)

set(PRECOMPILED_HEADER_FILE "${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h")

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin" CACHE STRING "" FORCE)

# Set Windows defines for SDK usage
add_compile_definitions(UNICODE _UNICODE _CONSOLE)

# Set Module Path for DirectX9
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Set source & header files
file(GLOB SOURCE_FILES src/*.cpp)
file(GLOB HEADER_FILES src/*.hpp src/*.h)

set(IMGUI_FILES libs/imgui/backends/imgui_impl_glfw.cpp libs/imgui/backends/imgui_impl_vulkan.cpp libs/imgui/imgui.cpp libs/imgui/imgui_draw.cpp libs/imgui/imgui_demo.cpp libs/imgui/imgui_tables.cpp libs/imgui/imgui_widgets.cpp libs/imgui/misc/cpp/imgui_stdlib.cpp)

# Configure GLFW before adding to project
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Add subdirectory for glm
add_subdirectory(libs/glm)

# Add subdirectory for glfw
add_subdirectory(libs/glfw)

# Include Dear ImGui
include_directories(libs/imgui libs/imgui/backends)

# Find Vulkan
find_package(Vulkan REQUIRED)

# Create .dll
add_library(Mjoelnir SHARED ${SOURCE_FILES} ${IMGUI_FILES} ${HEADER_FILES})

# Set properties
target_compile_definitions(Mjoelnir PUBLIC -DImTextureID=ImU32)

# Link target

target_include_directories(Mjoelnir PUBLIC libs/glm ${Vulkan_INCLUDE_DIRS})
target_link_libraries(Mjoelnir PUBLIC glm::glm glfw ${Vulkan_LIBRARIES})

# Set precompiled header file
target_precompile_headers(Mjoelnir PRIVATE ${PRECOMPILED_HEADER_FILE})

# Set Mjoelnir as startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Mjoelnir)

# File structure in Code Editor
source_group("Source Files\\Cheats" FILES src/IThreadedCheat.hpp src/ICheat.hpp src/Constants.hpp)
source_group("Source Files\\Cheats\\Aimbot" FILES src/CAimbot.hpp src/CAimbot.cpp)
source_group("Source Files\\Cheats\\Ammo" FILES src/CAmmo.hpp src/CAmmo.cpp)
source_group("Source Files\\Cheats\\Health" FILES src/CHealth.hpp src/CHealth.cpp)
source_group("Source Files\\Cheats\\ESP" FILES src/CESP.hpp src/CESP.cpp)
source_group("Source Files\\Cheats\\Recoil" FILES src/CRecoil.hpp src/CRecoil.cpp)
source_group("Source Files\\Cheats\\Debugging" FILES src/CDebugger.hpp src/CDebugger.cpp)
source_group("Source Files\\Cheats\\Utility" FILES src/Utils.hpp src/Utils.cpp)
source_group("Source Files\\Game-Logic" FILES src/Entity.hpp src/Entity.cpp src/ent.hpp)
source_group("Source Files\\Main-Logic" FILES src/dllmain.cpp src/Game.hpp src/Game.cpp)
source_group("Source Files\\Memory-Manipulation" FILES src/Hook.hpp src/Hook.cpp src/Memory.hpp src/Memory.cpp src/Process.hpp src/Process.cpp)
source_group("Source Files\\UI" FILES src/GameViewport.hpp src/Overlay.hpp src/Overlay.cpp src/IDrawing.hpp)
source_group("Source Files\\UI\\imgui" FILES ${IMGUI_FILES})
source_group("Precompile Header File" FILES src/pch.h src/pch.cpp src/framework.h)