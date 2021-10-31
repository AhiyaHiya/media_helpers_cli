# Media Helpers CLI
Command line apps to help organize media, which for now is movies and photos.

The project uses CMake, so that it can create projects for the development tools of your liking.
At the time of this writing, I use Visual Studio Code (on Linux, macOS and Windows).

# Projects

**organize_media**
This is a tool to look at a file, see if there is exif data, and move that file into a folder heirachy, based on the files known datetimestamp info.
So, if you have a photo that was shot in 2009-10-10, and created date and EXIF data show the same, then the photo is moved into a folder structure like:
	2009/10-10/photo_name.jpg


**print_out_exif**
This just prints out the images EXIF data to the command line output.


# Tools used

CMake (https://cmake.org/)
conan (https://docs.conan.io/en/latest/)
llvm (https://llvm.org/)
Xcode
Visual Studio 2019

# Libraries used in project

Boost
libExiv2
libRAW
