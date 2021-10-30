cmake_minimum_required(VERSION 3.19)

project(media_helpers_cli VERSION 0.1.0)

# C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Folders for IDEs that support it
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# We need to specify these types since we are supporting multi-config IDEs like Xcode and Visual Studio
list(APPEND CMAKE_CONFIGURATION_TYPES "Debug" "Release")

######################################################################
# Conan helper to download required files
include(ConanHelper.cmake)

conan_cmake_run(CONANFILE conanfile.txt
                BASIC_SETUP CMAKE_TARGETS
                BUILD missing)

include(${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)

add_subdirectory(organize_media)
