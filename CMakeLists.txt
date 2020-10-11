cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
	CACHE STRING "Vcpkg toolchain file")

	set(CMAKE_CXX_STANDARD 17)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	set(CMAKE_BUILD_TYPE debug)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

project(media_helpers_cli VERSION 0.1.0)

include(CTest)
enable_testing()

# Boost
find_package(Boost 1.69 REQUIRED 
COMPONENTS filesystem program_options)

# libVIPS
find_package(PkgConfig REQUIRED)
pkg_check_modules(VIPS REQUIRED vips)

# find_library(LIBVIPS_LIBRARY
#     NAMES libvips libvips.42
#     HINTS "/usr/local/Cellar/vips/8.10.1_1/lib/"
# )
# target_link_libraries(LibVips -lvips)
# target_link_libraries(LibVips -L/usr/local/Cellar/vips/8.10.1_1/lib/)
# link_directories(/usr/local/Cellar/vips/8.10.1_1/lib/)
# include_directories(/usr/local/include/)
# include_directories(/usr/local/include/glib-2.0)
# include_directories(/usr/local/lib/glib-2.0/include)

add_executable(organize_media organize_media/main.cpp)

# `pkg-config vips --cflags --libs`
# include_directories(${VIPS_INCLUDE_DIRS})
# link_directories(${VIPS_LIBRARY_DIRS})

target_include_directories(organize_media PUBLIC ${VIPS_INCLUDE_DIRS})
target_link_directories(organize_media PUBLIC ${VIPS_LIBRARY_DIRS})
target_link_libraries(organize_media Boost::filesystem Boost::program_options ${VIPS_LIBRARIES})
target_compile_options(organize_media PUBLIC ${VIPS_CFLAGS_OTHER})
add_definitions(${VIPS_CFLAGS} ${VIPS_CFLAGS_OTHER})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
