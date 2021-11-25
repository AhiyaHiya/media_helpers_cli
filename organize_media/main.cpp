
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
    ("date,d", "Move media items by date")
    ("photos,p", "Only operate on photos")
    ("movies,m", "Only operate on movies")
    ("preflight,f", "Output what operation is going to happen but don't move files")
    ("path", po::value<std::string>(), "Path to the files we are going to work on");
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

    const auto act_on_photos = (vm.count("photos") > 0 ? true : false);
    const auto act_on_movies = (vm.count("movies") > 0 ? true : false);

    // clang-format off
  if (act_on_photos)
    cout << "Photos" << "\n";

  if (act_on_movies)
    cout << "Movies" << "\n";
    // clang-format on

    // Let's get the files we are about to work on
    const auto current_path = fs::current_path();
    const auto as_string    = current_path.string();
    const auto as_c_str     = as_string.c_str();
    assert(as_string.length() && "Empty path string detected");
    assert(as_c_str && "nullptr path string detected");

    std::cout << "Current path is " << as_c_str << '\n';

    // Test 1
    if (vm.count("path"))
    {
        const auto user_provided_path = vm["path"].as< std::string >();
        const auto working_path       = std::filesystem::path(user_provided_path);
        assert(user_provided_path.length() && "Empty path detected");

        // Test 2 - list files in folder
        list_files_in_folder(working_path);

        // Test 3 - get files in folder as vector
        auto result_set = get_files_in_folder(working_path, get_photo_file_extensions());
        for (const auto& item : result_set)
        {
            std::cout << item.string().c_str() << "\n";
        }

        // Test 4 - get exif for each file
        for (const auto& item : result_set)
        {
            print_exif_for_file(item);
        }

    }
    else
    {
        std::cout << "No path was provided\n";
    }

    return 0;
}
