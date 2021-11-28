
######################################################################
# Application
add_executable(organize_media main.cpp files.cpp files.h)

target_link_libraries(organize_media
  PRIVATE
    CONAN_PKG::boost
    CONAN_PKG::libexif
    CONAN_PKG::libraw
)
