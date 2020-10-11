
#include <filesystem>
#include <iostream>

#include <boost/program_options.hpp>
#include <vips/vips.h>

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
static const auto photoFileExtensions = std::vector< std::string >{
    ".jpeg", ".tiff", ".png", ".nef",
    ".JPEG", ".TIFF", ".PNG", ".NEF",
    };
// clang-format on

////////////////////////////////////////////////////////////////////////////////////////
auto GetFilesInFolder(const fs::path& workingDirectory, const vector< string >& extensions) -> vector< fs::path >;
void ListFilesInFolder(const fs::path& workingDirectory);

void* HeaderCallbackFunc(VipsImage* image, const char* field, GValue* value, void* my_data);

class ScopedVips
{
public:
  ScopedVips() { VIPS_INIT(nullptr); }

  ~ScopedVips() {}
};

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

  // Test 1
  ListFilesInFolder(currentPath);

  // Test 2
  if (actOnPhotos)
  {
    // Get the files
    auto foundFiles = GetFilesInFolder(currentPath, photoFileExtensions);
    std::cout << "Found files\n";

    ScopedVips scopedVips;

    for (auto& file : foundFiles)
    {
      std::cout << file << "\n";

      // Get the date created from the file info
      std::error_code failed;
      auto            lastWriteTime = fs::last_write_time(file, failed);
      if (failed)
      {
        std::cout << "Failed to get last_write_time for file" << file << "\n";
        continue;
      }

      auto lastWriteTimeT = decltype(lastWriteTime)::clock::to_time_t(lastWriteTime);
      std::cout << "File write time is " << std::asctime(std::localtime(&lastWriteTimeT)) << '\n';

      // Get the date created from the EXIF info
      auto vipsImage = vips_image_new_from_file(file.c_str(), NULL);
      if (vipsImage == nullptr)
      {
        std::cout << "Something bad happened\n";
      }
      // key/value, key is field, value is field value
      map<string, string> headerValues;
      void* my_data = reinterpret_cast<map<string, string>*>(&headerValues);
      auto nullIsSuccess = vips_image_map(vipsImage, HeaderCallbackFunc, my_data);
      if(nullIsSuccess==nullptr)
      {
        for(const auto& entry : headerValues)
        {
          const auto& key = entry.first;
          const auto& value = entry.second;
          cout << key.c_str() << " = " << value.c_str() << "\n";
          // printf("%s = %s\n", key.c_str(), value.c_str());
        }
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////
auto GetFilesInFolder(const fs::path& workingDirectory, const vector< string >& extensions) -> vector< fs::path >
{
  auto resultSet = vector< fs::path >();

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
void ListFilesInFolder(const fs::path& workingDirectory)
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

////////////////////////////////////////////////////////////////////////////////////////
void* HeaderCallbackFunc(VipsImage* image, const char* field, GValue* value, void* my_data)
{
  map<string, string>* headerValues = reinterpret_cast<map<string, string>*>(my_data);
  char* valueAsStr = g_strdup_value_contents(value);
  // printf("%s = %s", field, valueAsStr);
  (*headerValues)[string(field)] = string(valueAsStr);
  g_free(valueAsStr);

  return nullptr;
}