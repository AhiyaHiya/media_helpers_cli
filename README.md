# media_helpers_cli
Command line apps to help organize media, which for now is movies and photos.

The project uses cmake, so that it can create projects for the development tools of your liking.
At the time of this writing, I use Visual Studio Code (on Linux and macOS).

# tools in this project

organize_media
This is a tool to look at a file, see if there is exif data, and move that file into a folder heirachy, based on the files known datetimestamp info.
So, if you have a photo that was shot in 2009-10-10, and created date and EXIF data show the same, then the photo is moved into a folder structure like:
	2009/10/10/photo_name.jpg


print_out_exif
This just prints out the images EXIF data to the command line output.


# External tools

Microsoft's vcpkg tool is a submodule in this project, and used to manage dependencies ( https://github.com/microsoft/vcpkg#quick-start-unix ).
To search for a package in vcpkg, from the project root, you can type `./vcpkg/vcpkg search libname` and to install the package, it's similar
so type `./vcpkg/vcpkg install somelibrary`.

It will be important to install the libraries listed below before calling CMake to create your project file.

# Libraries used in project

Boost
libVIPS
libRAW
