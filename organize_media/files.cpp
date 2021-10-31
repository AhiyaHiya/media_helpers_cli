// System
#include <iostream>

// Third party
#include <exiv2/exiv2.hpp>

// Project
#include "files.h"

namespace fs = std::filesystem;

ExifHelper::ExifHelper() { Exiv2::XmpParser::initialize(); }

ExifHelper::~ExifHelper() { Exiv2::XmpParser::terminate(); }

////////////////////////////////////////////////////////////////////////////////////////
// get_files_in_folder
std::vector< fs::path > get_files_in_folder(const fs::path&                   working_directory,
                                            const std::vector< std::string >& extensions)
{
    auto result_set = std::vector< fs::path >();

    for (const auto& file : fs::directory_iterator(working_directory))
    {
        for (const auto& ext : extensions)
        {
            if (file.path().extension() == ext)
            {
                result_set.push_back(file);
                continue;
            }
        }
    }

    return result_set;
}

////////////////////////////////////////////////////////////////////////////////////////
// get_photo_file_extensions
std::vector< std::string > get_photo_file_extensions()
{
// clang-format off
  static const auto file_extensions = std::vector< std::string >{
    ".jpg", ".jpeg", ".tiff", ".png", ".nef",
    ".JPG", ".JPEG", ".TIFF", ".PNG", ".NEF",
  };
// clang-format on
    return file_extensions;
}

////////////////////////////////////////////////////////////////////////////////////////
// list_files_in_folder
void list_files_in_folder(const fs::path& working_directory)
{
    const auto& file_extensions = get_photo_file_extensions();
    for (const auto& file : fs::directory_iterator(working_directory))
    {
        for (const auto& ext : file_extensions)
        {
            if (file.path().extension() == ext)
            {
                std::cout << file.path() << "\n";
                continue;
            }
        }
    }
}

// print_exif_for_file
void print_exif_for_file(const std::filesystem::path& file_path)
{
    auto image = Exiv2::ImageFactory::open(file_path);
    assert(image.get() != 0);
    image->readMetadata();

    const auto& exifData = image->exifData();
    if (exifData.empty())
    {
        std::string error("No Exif data found in file");
        // throw Exiv2::Error(Exiv2::kerErrorMessage, error);
    }
}