#include "file_utilities.h"

#include <cassert>
#include <algorithm>
#include <map>
#include <regex>
#include <string>
#include <stdexcept>

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

namespace fs = filesystem;
namespace vtpl {
namespace glob {

static const auto     ESCAPE_SET_OPER    = std::regex(std::string{R"([&~|])"});
static const auto     ESCAPE_REPL_STR    = std::string{R"(\\\1)"};

static const std::string SPECIAL_CHARACTERS = "()[\\]{}?*+-|^$\\.&~# \t\n\r\v\f";

// Replace the first occurrence of 'from' with 'to'
inline bool string_replace(std::string& str, const std::string& from, const std::string& to) {
    size_t pos = str.find(from);
    if (pos == std::string::npos) return false;
    str.replace(pos, from.length(), to);
    return true;
}

// Convert glob pattern to regex
inline std::string translate(const std::string& pattern) {
    std::string result;
    size_t i = 0;
    while (i < pattern.size()) {
        char c = pattern[i++];
        if (c == '*') {
            if (i < pattern.size() && pattern[i] == '*') {
                ++i;
                result += ".*";
            } else {
                result += "[^/]*";
            }
        } else if (c == '?') {
            result += "[^/]";
        } else if (c == '[') {
            std::string set;
            if (i < pattern.size() && pattern[i] == '!') {
                set += '^';
                ++i;
            } else if (i < pattern.size() && pattern[i] == '^') {
                set += "\\^";
                ++i;
            }
            if (i < pattern.size() && pattern[i] == ']') {
                set += "\\]";
                ++i;
            }

            while (i < pattern.size() && pattern[i] != ']') {
                if (pattern[i] == '\\') {
                    set += "\\\\";
                    ++i;
                } else {
                    set += pattern[i++];
                }
            }
            if (i >= pattern.size()) {
                throw std::runtime_error("Unterminated character set in pattern");
            }
            ++i;
            result += "[" + set + "]";
        } else {
            if (SPECIAL_CHARACTERS.find(c) != std::string::npos)
                result += '\\';
            result += c;
        }
    }
    return "^" + result + "$";
}

inline bool fnmatch(const std::string& name, const std::string& pattern) {
    std::regex re(translate(pattern), std::regex::ECMAScript);
    return std::regex_match(name, re);
}

// Recursive or non-recursive glob
inline std::vector<std::string> glob(const std::string& pattern) {
    std::vector<std::string> results;

    fs::path full_pattern = fs::absolute(pattern);
    std::string pattern_str = full_pattern.string();

    // Find root directory to search
    fs::path root;
    for (fs::path p = full_pattern; !p.empty(); p = p.parent_path()) {
        if (fs::exists(p)) {
            root = p;
            break;
        }
    }

    if (root.empty())
        root = fs::current_path();

    std::string regex_pattern = translate(pattern_str);

    std::regex matcher(regex_pattern, std::regex::ECMAScript);

    fs::recursive_directory_iterator it(root), end;
    for (; it != end; ++it) {
        if (!fs::is_regular_file(it->status()))
            continue;

        std::string file_path = fs::absolute(it->path()).string();
        if (std::regex_match(file_path, matcher)) {
            results.push_back(file_path);
        }
    }

    return results;
}
} // namespace utilities

} // namespace vtpl