
#include <filesystem>
#include <iostream>

#include <boost/program_options.hpp>

/**
The idea of the program is that you have a folder full of media items, which hopefully is one type
of media item that you want organized by date.

Usage:
organize_media --date --photos --preflight
organize_media --date --photos --preflight
*/

namespace fs = std::filesystem;
namespace po = boost::program_options;

using namespace std;

int main(int argc, char** argv)
{
  // Setup program options
  auto desc = po::options_description("Options");
  // clang-format off
  desc.add_options()
  ("help,h", "Show help message")
  ("date,d", "Move things by date")
  ("photos,p", "Only operate on photos")
  ("movies,m", "Only operate on movies")
  ("preflight,f", "Output what operation is going to do but don't move the files");
  // clang-format on

  // Parse the passed in options
  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, desc), vm);
  po::notify(vm);

  // Now act on what we have
  if (vm.count("help"))
  {
    cout << desc << "\n";
    return 1;
  }

  auto actOnPhotos = (bool)(vm.count("photos"));
  auto actOnMovies = (bool)(vm.count("movies"));

  // clang-format off
  if (actOnPhotos)
    cout << "Photos" << "\n";

  if (actOnMovies)
    cout << "Movies" << "\n";
  // clang-format on

  // Let's get the files we are about to work on
  auto currentPath = fs::current_path();
  std::cout << "Current path is " << currentPath.c_str() << '\n';
  auto photoFileExtensions = std::vector< std::string >{
    ".jpeg", ".tiff", ".png", ".nef",
    ".JPEG", ".TIFF", ".PNG", ".NEF",
    };

  for (const auto& file : fs::directory_iterator(currentPath))
  {
    for (auto& ext : photoFileExtensions)
    {
      if (file.path().extension() == ext)
      {
        std::cout << file.path() << std::endl;
        continue;
      }
    }
  }
}
