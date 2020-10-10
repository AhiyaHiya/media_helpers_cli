cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
	CACHE STRING "Vcpkg toolchain file")

project(media_helpers_cli VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(Boost 1.69 REQUIRED COMPONENTS filesystem)

add_executable(organize_media organize_media/main.cpp)

target_link_libraries(organize_media Boost::filesystem)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
