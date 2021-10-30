#pragma once

#include <filesystem>
#include <string>
#include <vector>

std::vector< std::filesystem::path > get_files_in_folder(const std::filesystem::path& working_directory, const std::vector< std::string >& extensions);
void list_files_in_folder(const std::filesystem::path& working_directory);
