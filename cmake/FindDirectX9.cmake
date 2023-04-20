# FindDirectX9.cmake
#
# Look for DirectX9 and define the following variables if found:
#
#   DIRECTX9_FOUND          - System has DirectX9
#   DirectX9_LIBRARY      - Libraries needed to use DirectX9
#   DirectX9_INCLUDE_DIR       - DirectX9 include directories
#   DIRECTX9_LIB_PATH       - Path to DirectX9 libraries
#
# Additionally, if the "REQUIRED" argument is provided to find_package(),
# the configuration process will fail if DirectX9 is not found.

# Check if the DXSDK_DIR environment variable is set and set DIRECTX9_FOUND accordingly.
if(DEFINED ENV{DXSDK_DIR})
    set(DIRECTX9_FOUND TRUE)
else()
    set(DIRECTX9_FOUND FALSE)
endif()

# Set the default value of DIRECTX9_BITS to "x86".
if (NOT DEFINED DIRECTX9_BITS)
    set(DIRECTX9_BITS "x86")
endif()

# If DirectX9 is found, set the include directories and library paths.
if(DIRECTX9_FOUND)
    # Set the path to the DirectX9 libraries.
    set(DIRECTX9_LIB_PATH "$ENV{DXSDK_DIR}Lib\\${DIRECTX9_BITS}")

    # Check if the required DirectX9 library files are present.
    if(EXISTS "${DIRECTX9_LIB_PATH}\\d3dx9.lib" AND EXISTS "${DIRECTX9_LIB_PATH}\\d3d9.lib")
        # Set the library files for DirectX9.
        set(DirectX9_LIBRARY "${DIRECTX9_LIB_PATH}\\d3dx9.lib;${DIRECTX9_LIB_PATH}\\d3d9.lib")

        # Set the include directories for DirectX9.
        set(DirectX9_INCLUDE_DIR "$ENV{DXSDK_DIR}Include")
        include_directories(${DirectX9_INCLUDE_DIR})

        # Print a message indicating that DirectX9 was found.
        message(STATUS "DirectX9 found: libraries = ${DirectX9_LIBRARY}, includes = ${DirectX9_INCLUDE_DIR}")
    else()
        # DirectX9 libraries not found.
        message(FATAL_ERROR "DirectX9 libraries not found in ${DIRECTX9_LIB_PATH}")
    endif()
endif()

# Report the results of the search to the user.
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DirectX9 DEFAULT_MSG DirectX9_LIBRARY DirectX9_INCLUDE_DIR)

# If DirectX9 was not found and the REQUIRED option was specified, fail the configuration process.
if(DIRECTX9_REQUIRED AND NOT DIRECTX9_FOUND)
    message(FATAL_ERROR "Could not find DirectX9")
endif()
