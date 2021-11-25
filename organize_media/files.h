#pragma once

#include <filesystem>
#include <string>
#include <vector>

std::vector< std::filesystem::path > get_files_in_folder(const std::filesystem::path& working_directory, const std::vector< std::string >& extensions);

std::vector<std::string> get_photo_file_extensions();

void list_files_in_folder(const std::filesystem::path& working_directory);

void print_exif_for_file(const std::filesystem::path& file_path);
