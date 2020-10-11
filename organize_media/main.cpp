
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

////////////////////////////////////////////////////////////////////////////////////////
vector<fs::path> GetFilesInFolder(const fs::path& workingDirectory, const vector< string >& extensions);
void ListFilesInFolder(const fs::path& workingDirectory);

void* HeaderCallbackFunc(VipsImage* image, const char* field, GValue* value, void* my_data);

////////////////////////////////////////////////////////////////////////////////////////
class ScopedVips
{
public:
  ScopedVips() 
  {
    VIPS_INIT(nullptr);
  }

  ~ScopedVips() 
  {
    // We may need shutdown code in the future.
  }
};

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

    // Do something with each photo we found in the current working folder.
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

      // This line doesn't work on Linux
      // auto lastWriteTimeT = decltype(lastWriteTime)::clock::to_time_t(lastWriteTime);
      time_t lastWriteTimeT = to_time_t(lastWriteTime);
      std::cout << "File write time is " << std::asctime(std::localtime(&lastWriteTimeT)) << '\n';

      // Get the date created from the EXIF info
      auto image = vips_image_new_from_file(file.c_str(), NULL);
      if (image == nullptr)
      {
        std::cout << "Something bad happened\n";
      }
      // key/value, key is field, value is field value
      map< string, string > headerValues;
      void* hrdValMapAsVPtr = reinterpret_cast< map< string, string >* >(&headerValues);

      auto failure = vips_image_map(image, HeaderCallbackFunc, hrdValMapAsVPtr);
      if (!failure)
      {
        // Now print out each EXIF entry we found in the image.
        for (const auto& entry : headerValues)
        {
          const auto& key   = entry.first;
          const auto& value = entry.second;
          cout << key.c_str() << " = " << value.c_str() << "\n";
        }
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////
vector<fs::path> GetFilesInFolder(const fs::path& workingDirectory, const vector< string >& extensions)
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
  auto headerValues = reinterpret_cast< map< string, string >* >(my_data);
  auto valueAsStr   = g_strdup_value_contents(value);

  (*headerValues)[string(field)] = string(valueAsStr);
  g_free(valueAsStr);

  return nullptr;
}