
#include <filesystem>
#include <iostream>

#include <boost/program_options.hpp>
// #include <libraw/libraw.h>

#include "files.h"


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

// clang-format off

////////////////////////////////////////////////////////////////////////////////////////
// Thanks to https://stackoverflow.com/questions/56788745/how-to-convert-stdfilesystemfile-time-type-to-a-string-using-gcc-9/58237530#58237530
template <typename TP>
std::time_t to_time_t(TP tp)
{
    using namespace std::chrono;
    auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
              + system_clock::now());
    return system_clock::to_time_t(sctp);
}
// clang-format on

////////////////////////////////////////////////////////////////////////////////////////
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

  auto act_on_photos = (bool)(vm.count("photos"));
  auto act_on_movies = (bool)(vm.count("movies"));

  // clang-format off
  if (act_on_photos)
    cout << "Photos" << "\n";

  if (act_on_movies)
    cout << "Movies" << "\n";
  // clang-format on

  // Let's get the files we are about to work on
  auto currentPath = fs::current_path();
  std::cout << "Current path is " << currentPath.c_str() << '\n';

  // Test 1
  list_files_in_folder(currentPath);

}
