# Set project name
project (Rocket)
# Set minimum version
cmake_minimum_required (VERSION 2.8.6)
if (COMMAND cmake_policy)
    cmake_policy (SET CMP0003 NEW)
    if (CMAKE_VERSION VERSION_GREATER 2.8.12 OR CMAKE_VERSION VERSION_EQUAL 2.8.12)
        # INTERFACE_LINK_LIBRARIES defines the link interface
        cmake_policy (SET CMP0022 NEW)
    endif ()
    if (CMAKE_VERSION VERSION_GREATER 3.0.0 OR CMAKE_VERSION VERSION_EQUAL 3.0.0)
        # Disallow use of the LOCATION target property - therefore we set to OLD as we still need it
        cmake_policy (SET CMP0026 OLD)
        # MACOSX_RPATH is enabled by default
        cmake_policy (SET CMP0042 NEW)
    endif ()
endif ()

set (URHO3D_HOME /home/nathan/Projects/Urho/UrhoRocket)
#set (ANDROID_NDK H:/Tools/Android/android-ndk-r10e)

# Set CMake modules search path
set (CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake/Modules)
# Include Urho3D Cmake common module
include (Urho3D-CMake-common)
# Find Urho3D library
find_package (Urho3D REQUIRED)
set (URHO3D_HOME /home/nathan/Projects/Urho/UrhoRocket)

#base include dir is .../include/Urho3D -- added by me
list (APPEND URHO3D_INCLUDE_DIRS ${BASE_INCLUDE_DIR} "../libRocket/Include")
#list (APPEND URHO3D_LIBS ${BASE_INCLUDE_DIR} "../libRocket/Include")

include_directories (${URHO3D_INCLUDE_DIRS})
# Define target name
set (TARGET_NAME RocketTest)

SET(CMAKE_CXX_FLAGS "-std=c++11 -g3 -gdwarf-2 -Wno-invalid-offsetof -pthread")# -L. -lRocketCore

#set (${LINKER_FLAGS} "${${LINKER_FLAGS}} -lRocketCore")


# Define source files    try adding RECURSE
define_source_files ( GLOB_CPP_PATTERNS *.cpp )
# Setup target with resource copying
setup_main_executable ()
