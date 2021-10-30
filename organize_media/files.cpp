
#include <iostream>

#include "files.h"


// clang-format off
static const auto photoFileExtensions = std::vector< std::string >{
  ".jpg", ".jpeg", ".tiff", ".png", ".nef",
  ".JPG", ".JPEG", ".TIFF", ".PNG", ".NEF",
};
// clang-format on
namespace fs = std::filesystem;

////////////////////////////////////////////////////////////////////////////////////////
// get_files_in_folder
std::vector< fs::path > get_files_in_folder(const fs::path&         workingDirectory,
                                    const std::vector< std::string >& extensions)
{
  auto resultSet = std::vector< fs::path >();

  for (const auto& file : fs::directory_iterator(workingDirectory))
  {
    for (auto& ext : extensions)
    {
      if (file.path().extension() == ext)
      {
        resultSet.push_back(file);
        continue;
      }
    }
  }

  return resultSet;
}

////////////////////////////////////////////////////////////////////////////////////////
// list_files_in_folder
void list_files_in_folder(const fs::path& workingDirectory)
{
  for (const auto& file : fs::directory_iterator(workingDirectory))
  {
    for (auto& ext : photoFileExtensions)
    {
      if (file.path().extension() == ext)
      {
        std::cout << file.path() << "\n";
        continue;
      }
    }
  }
}