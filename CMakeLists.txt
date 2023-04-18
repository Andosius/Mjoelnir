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

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin" CACHE STRING "" FORCE)

# Set Windows defines for SDK usage
add_compile_definitions(UNICODE _UNICODE)


# Set source files
file(GLOB SOURCE_FILES src/*.cpp)
#set(SOURCE_FILES src/dllmain.cpp)
set(IMGUI_FILES libs/imgui/backends/imgui_impl_glfw.cpp libs/imgui/backends/imgui_impl_vulkan.cpp libs/imgui/imgui.cpp libs/imgui/imgui_draw.cpp libs/imgui/imgui_demo.cpp libs/imgui/imgui_tables.cpp libs/imgui/imgui_widgets.cpp)

# Configure GLFW before adding to project
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Add subdirectories for glfw and glm
add_subdirectory(libs/glfw)
add_subdirectory(libs/glm)

# Include Dear ImGui
include_directories(libs/imgui libs/imgui/backends)


# Find Vulkan
find_package(Vulkan REQUIRED)


# Create .dll
add_library(Mjoelnir SHARED ${SOURCE_FILES} ${IMGUI_FILES})

# Set properties
target_compile_definitions(Mjoelnir PUBLIC -DImTextureID=ImU32)

# Link target
## Vulkan
target_include_directories(Mjoelnir PUBLIC ${Vulkan_INCLUDE_DIRS})
## GLFW
target_link_libraries(Mjoelnir PUBLIC glfw ${Vulkan_LIBRARIES})