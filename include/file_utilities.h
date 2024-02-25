// *****************************************************
//    Copyright 2023 Videonetics Technology Pvt Ltd
// *****************************************************

#pragma once
#ifndef file_utilities_h
#define file_utilities_h
#include <fileutil_export.h>
#include <map>
#include <string>
#include <tuple>
#include <vector>
#include <version.h>

namespace vtpl {
namespace utilities {
std::string FILEUTIL_EXPORT get_filesystem_directory_seperator();
bool FILEUTIL_EXPORT        is_directory_exists(const std::string& dir_path);
bool FILEUTIL_EXPORT        is_regular_file_exists(const std::string& file_path);
bool FILEUTIL_EXPORT        create_directories(const std::string& dir_path);
std::string FILEUTIL_EXPORT create_directories_from_file_path(const std::string& file_path);
std::string FILEUTIL_EXPORT end_with_directory_seperator(const std::string& dir_path_0);
std::string FILEUTIL_EXPORT merge_directories(const std::string& dir_path_0, const std::string& dir_path_1);
std::string FILEUTIL_EXPORT merge_directories(const std::string& dir_path_0, const std::string& dir_path_1,
                                              const std::string& dir_path_2);
std::string FILEUTIL_EXPORT merge_directories(const std::string& dir_path_0, const std::string& dir_path_1,
                                              const std::string& dir_path_2, const std::string& dir_path_3);
std::string FILEUTIL_EXPORT merge_directories(const std::string& dir_path_0, const std::string& dir_path_1,
                                              const std::string& dir_path_2, const std::string& dir_path_3,
                                              const std::string& dir_path_4);
std::string FILEUTIL_EXPORT merge_directories(const std::string& dir_path, std::vector<std::string>& dir_n_list);
bool FILEUTIL_EXPORT        delete_directory(const std::string& dir_path);
bool FILEUTIL_EXPORT        delete_file(const std::string& file_path);
} // namespace utilities
} // namespace vtpl
#endif // file_utilities_h
