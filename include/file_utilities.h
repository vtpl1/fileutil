// *****************************************************
//    Copyright 2023 Videonetics Technology Pvt Ltd
// *****************************************************

#pragma once
#ifndef file_utilities_h
#define file_utilities_h
#include <map>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
namespace vtpl
{
namespace utilities
{
std::string get_filesystem_directory_seperator();
bool is_directory_exists(const std::string& dir_path);
bool is_regular_file_exists(const std::string& file_path);
bool create_directories(const std::string& dir_path);
std::string create_directories_from_file_path(const std::string& file_path);
std::stringstream end_with_directory_seperator(const std::string& dir_path_0);
std::string merge_directories(const std::string& dir_path_0, const std::string& dir_path_1);
std::string merge_directories(const std::string& dir_path_0, const std::string& dir_path_1,
                              const std::string& dir_path_2);
std::string merge_directories(const std::string& dir_path_0, const std::string& dir_path_1,
                              const std::string& dir_path_2, const std::string& dir_path_3);
std::string merge_directories(const std::string& dir_path_0, const std::string& dir_path_1,
                              const std::string& dir_path_2, const std::string& dir_path_3,
                              const std::string& dir_path_4);
std::string merge_directories(const std::string& dir_path, std::vector<std::string>& dir_n_list);
bool delete_directory(const std::string& dir_path);
bool delete_file(const std::string& file_path);
} // namespace utilities
} // namespace vtpl
#endif // file_utilities_h
