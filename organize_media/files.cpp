// System
#include <iostream>

// Third party
#include <libexif/exif-data.h>

// Project
#include "files.h"

namespace fs = std::filesystem;


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

// TODO cleanup or make generalized
class exif_cleanup
{
  public:
    exif_cleanup(ExifData* ptr)
    : m_ptr(ptr)
    {};

    ~exif_cleanup()
    {
      if (m_ptr)
      {
          exif_data_unref(m_ptr);
      }
    };

    private:
    ExifData* m_ptr = nullptr;
};

static void print_tag(const ExifData* data, const ExifIfd ifd, const ExifTag tag)
{
    /* See if this tag exists */
    auto entry_ptr = exif_content_get_entry(data->ifd[ifd], tag);
    if (entry_ptr)
    {
        auto buf = std::vector<char>(1024, '0');

        /* Get the contents of the tag in human-readable form */
        exif_entry_get_value(entry_ptr, buf.data(), buf.size());// sizeof(buf.data()));

        /* Don't bother printing it if it's entirely blank */
        //trim_spaces(buf);
        if (*buf.data())
        {
            printf("%s: %s\n", exif_tag_get_name_in_ifd(tag, ifd), buf.data());
        }
    }
}
// print_exif_for_file
void print_exif_for_file(const std::filesystem::path& file_path)
{
    auto exif_data_raw_ptr = exif_data_new_from_file(file_path.string().c_str());
    if (!exif_data_raw_ptr)
    {
        printf("Unable to load data\n");
        return;
    }
    auto exif_data_helper = exif_cleanup(exif_data_raw_ptr);

    print_tag(exif_data_raw_ptr, EXIF_IFD_0, EXIF_TAG_DATE_TIME);
}