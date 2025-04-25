// *****************************************************
//    Copyright 2023 Videonetics Technology Pvt Ltd
// *****************************************************
#include "file_utilities.h"
#include <cstddef>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>
#include <vector>

#ifdef WIN32
#include <filesystem>
namespace filesystem = std::filesystem;
#define stat _stat
#else
/* https://github.com/scylladb/seastar/issues/648 */
#if __cplusplus >= 201703L && __has_include(<filesystem>)
#include <filesystem>
namespace filesystem = std::filesystem;
#else
#include <experimental/filesystem>
namespace filesystem = std::experimental::filesystem;
#endif
#endif

std::string vtpl::utilities::get_filesystem_directory_seperator() {
#ifdef _WIN32
  return "\\";
#else
  return "/";
#endif
  // return "/";
}

bool vtpl::utilities::is_directory_exists(const std::string& dir_path) {
  if (dir_path.empty()) {
    throw std::runtime_error("Input argument is empty");
  }
  return (filesystem::exists(dir_path) && filesystem::is_directory(dir_path));
}

bool vtpl::utilities::is_regular_file_exists(const std::string& file_path) {
  if (file_path.empty()) {
    throw std::runtime_error("Input argument is empty");
  }
  return (filesystem::exists(file_path) && filesystem::is_regular_file(file_path));
}

bool vtpl::utilities::create_directories(const std::string& dir_path) {
  if (dir_path.empty()) {
    throw std::runtime_error("Input argument is empty");
  }
  if (is_directory_exists(dir_path)) {
    return true;
  }
  std::error_code error_code;
  if (!filesystem::create_directories(dir_path, error_code) && (error_code.value() != 0)) {
    std::cout << "Unable to create directories [" << dir_path << "], "
              << "Error Code[" << error_code.value() << "], "
              << "Error Message[" << error_code.message() << "], "
              << "Error category[" << error_code.category().name() << "] " << '\n';
    return false;
  }
  return is_directory_exists(dir_path);
}

std::string vtpl::utilities::create_directories_from_file_path(const std::string& file_path) {
  if (file_path.empty()) {
    throw std::runtime_error("Input argument is empty");
  }
  std::string  dir_path;
  const size_t pos = file_path.find_last_of(get_filesystem_directory_seperator());
  if (pos != std::string::npos) {
    dir_path = file_path.substr(0, pos);
  }
  if (dir_path.empty()) {
    // throw std::runtime_error("Directory path is empty" + dir_path);
    return std::string{};
  }
  if (create_directories(dir_path)) {
    return dir_path;
  }
  return std::string{};
}

std::string vtpl::utilities::end_with_directory_seperator(const std::string& dir_path_0) {
  std::stringstream ss_path;
  ss_path << dir_path_0;
  if ((dir_path_0.back() != '/') && (dir_path_0.back() != '\\')) {
    ss_path << get_filesystem_directory_seperator();
  }
  return ss_path.str();
}

std::string vtpl::utilities::merge_directories(const std::string& dir_path_0, const std::string& dir_path_1) {
  std::stringstream ss_path;
  if ((dir_path_0.empty()) || (dir_path_1.empty())) {
    throw std::runtime_error("Input argument is empty");
  }
  ss_path << dir_path_0;
  std::stringstream ss_path1;
  ss_path1 << end_with_directory_seperator(ss_path.str());
  ss_path.swap(ss_path1);
  ss_path << dir_path_1;
  // if (!((ss_path.str().back() == '/') || (ss_path.str().back() == '\\')))
  //   ss_path << get_filesystem_directory_seperator();
  return ss_path.str();
}

std::string vtpl::utilities::merge_directories(const std::string& dir_path_0, const std::string& dir_path_1,
                                               const std::string& dir_path_2) {
  // NOLINTNEXTLINE(readability-suspicious-call-argument)
  return merge_directories(dir_path_0, merge_directories(dir_path_1, dir_path_2));
}

std::string vtpl::utilities::merge_directories(const std::string& dir_path_0, const std::string& dir_path_1,
                                               const std::string& dir_path_2, const std::string& dir_path_3) {
  // NOLINTNEXTLINE(readability-suspicious-call-argument)
  return merge_directories(dir_path_0, merge_directories(dir_path_1, dir_path_2, dir_path_3));
}

std::string vtpl::utilities::merge_directories(const std::string& dir_path_0, const std::string& dir_path_1,
                                               const std::string& dir_path_2, const std::string& dir_path_3,
                                               const std::string& dir_path_4) {
  // NOLINTNEXTLINE(readability-suspicious-call-argument)
  return merge_directories(dir_path_0, merge_directories(dir_path_1, dir_path_2, dir_path_3, dir_path_4));
}

std::string vtpl::utilities::merge_directories(const std::string& dir_path, std::vector<std::string>& dir_n_list) {
  std::string path(dir_path);
  for (auto& dir : dir_n_list) {
    path = merge_directories(path, dir);
  }
  return path;
}

bool vtpl::utilities::delete_directory(const std::string& dir_path) {
  if (dir_path.empty()) {
    throw std::runtime_error("Input argument is empty");
  }
  if (is_directory_exists(dir_path)) {
    return filesystem::remove_all(dir_path) != 0U;
  }
  return true;
}

bool vtpl::utilities::delete_file(const std::string& file_path) {
  if (file_path.empty()) {
    throw std::runtime_error("Input argument is empty");
  }
  if (is_regular_file_exists(file_path)) {
    return filesystem::remove(file_path);
  }
  return false;
}

bool vtpl::utilities::copy_file(const std::string& source_file, const std::string& destination_file) {
  try {
    filesystem::copy_file(source_file, destination_file, filesystem::copy_options::overwrite_existing);
    return true;
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
  }
  return false;
}