#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <filesystem>
#include <fstream>
#include <getopt.h>
#include <chrono>
#include <iomanip>

namespace fs = std::filesystem;

class FileTools {
private:
    bool dry_run = false;
    bool recursive = false;
    bool use_regex = false;
    bool ignore_case = false;

public:
    void rename_files(const std::string& path, const std::string& pattern,
                      const std::string& find_str, const std::string& replace_str);
    void search_files(const std::string& path, const std::string& pattern);
    void set_dry_run(bool value) { dry_run = value; }
    void set_recursive(bool value) { recursive = value; }
    void set_use_regex(bool value) { use_regex = value; }
    void set_ignore_case(bool value) { ignore_case = value; }

private:
    std::vector<fs::path> find_matching_files(const std::string& path, 
                                               const std::string& pattern);
    bool matches_pattern(const std::string& filename, const std::string& pattern);
    void printRename(const fs::path& old_path, const fs::path& new_path);
    void printMatch(const fs::path& file, int line_num, const std::string& line);
};

// Check if filename matches glob pattern
bool FileTools::matches_pattern(const std::string& filename, const std::string& pattern) {
    std::regex glob_pattern;
    
    // Convert glob pattern to regex
    std::string regex_str = pattern;
    // Escape special regex characters except * and ?
    std::string escaped;
    for (char c : regex_str) {
        if (c == '*') {
            escaped += ".*";
        } else if (c == '?') {
            escaped += ".";
        } else if (std::string("[]{}().\\^$|+", 10).find(c) != std::string::npos) {
            escaped += "\\";
            escaped += c;
        } else {
            escaped += c;
        }
    }
    
    if (ignore_case) {
        glob_pattern = std::regex(escaped, std::regex::icase);
    } else {
        glob_pattern = std::regex(escaped);
    }
    
    return std::regex_match(filename, glob_pattern);
}

// Find all files matching the pattern
std::vector<fs::path> FileTools::find_matching_files(const std::string& path, 
                                                       const std::string& pattern) {
    std::vector<fs::path> matching_files;
    
    try {
        if (recursive) {
            for (const auto& entry : fs::recursive_directory_iterator(path)) {
                if (entry.is_regular_file() && matches_pattern(entry.path().filename().string(), pattern)) {
                    matching_files.push_back(entry.path());
                }
            }
        } else {
            for (const auto& entry : fs::directory_iterator(path)) {
                if (entry.is_regular_file() && matches_pattern(entry.path().filename().string(), pattern)) {
                    matching_files.push_back(entry.path());
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error accessing path: " << e.what() << std::endl;
    }
    
    return matching_files;
}

// Rename files
void FileTools::rename_files(const std::string& path, const std::string& pattern,
                               const std::string& find_str, const std::string& replace_str) {
    auto files = find_matching_files(path, pattern);
    
    if (files.empty()) {
        std::cout << "No files matching pattern '" << pattern << "' found." << std::endl;
        return;
    }
    
    std::cout << "Found " << files.size() << " file(s) to rename." << std::endl;
    
    for (const auto& file : files) {
        std::string filename = file.filename().string();
        std::string new_filename;
        
        if (use_regex) {
            try {
                std::regex find_regex(find_str, ignore_case ? std::regex::icase : std::regex::ECMAScript);
                new_filename = std::regex_replace(filename, find_regex, replace_str);
            } catch (const std::regex_error& e) {
                std::cerr << "Regex error: " << e.what() << std::endl;
                continue;
            }
        } else {
            if (ignore_case) {
                std::string lower_filename = filename;
                std::string lower_find = find_str;
                std::transform(lower_filename.begin(), lower_filename.end(), lower_filename.begin(), ::tolower);
                std::transform(lower_find.begin(), lower_find.end(), lower_find.begin(), ::tolower);
                
                size_t pos = 0;
                new_filename = filename;
                while ((pos = lower_filename.find(lower_find, pos)) != std::string::npos) {
                    new_filename.replace(pos, find_str.length(), replace_str);
                    lower_filename.replace(pos, find_str.length(), replace_str);
                    pos += replace_str.length();
                }
            } else {
                new_filename = filename;
                size_t pos = 0;
                while ((pos = new_filename.find(find_str, pos)) != std::string::npos) {
                    new_filename.replace(pos, find_str.length(), replace_str);
                    pos += replace_str.length();
                }
            }
        }
        
        if (new_filename == filename) {
            continue; // No change needed
        }
        
        fs::path new_path = file.parent_path() / new_filename;
        printRename(file, new_path);
        
        if (!dry_run) {
            try {
                fs::rename(file, new_path);
                std::cout << "  [OK]" << std::endl;
            } catch (const fs::filesystem_error& e) {
                std::cout << "  [FAILED]" << std::endl;
                std::cerr << "  Error: " << e.what() << std::endl;
            }
        }
    }
}

void FileTools::printRename(const fs::path& old_path, const fs::path& new_path) {
    std::cout << "Rename: " << old_path.filename().string();
    std::cout << " -> " << new_path.filename().string();
    if (dry_run) {
        std::cout << " (dry-run)";
    }
    std::cout << std::endl;
}

// Search files for pattern
void FileTools::search_files(const std::string& path, const std::string& pattern) {
    auto files = find_matching_files(path, "*"); // Get all files
    
    std::regex search_pattern;
    std::string regex_flags = ignore_case ? std::regex::icase : std::regex::ECMAScript;
    
    if (use_regex) {
        try {
            search_pattern = std::regex(pattern, regex_flags);
        } catch (const std::regex_error& e) {
            std::cerr << "Regex error: " << e.what() << std::endl;
            return;
        }
    } else {
        // Escape special regex characters
        std::string escaped_pattern = std::regex_replace(pattern, std::regex("[.*+?^${}()|[\\]\\\\]"), "\\$&");
        search_pattern = std::regex(escaped_pattern, regex_flags);
    }
    
    int total_matches = 0;
    
    for (const auto& file : files) {
        std::ifstream infile(file);
        if (!infile.is_open()) {
            continue;
        }
        
        std::string line;
        int line_num = 0;
        bool file_has_match = false;
        
        while (std::getline(infile, line)) {
            line_num++;
            if (std::regex_search(line, search_pattern)) {
                if (!file_has_match) {
                    std::cout << "\n" << file.string() << ":\n";
                    file_has_match = true;
                }
                printMatch(file, line_num, line);
                total_matches++;
            }
        }
        
        infile.close();
    }
    
    std::cout << "\n--- Total matches: " << total_matches << " ---" << std::endl;
}

void FileTools::printMatch(const fs::path& file, int line_num, const std::string& line) {
    std::cout << "  " << std::setw(4) << line_num << ": " << line << std::endl;
}

void print_usage(const char* program_name) {
    std::cout << "Usage: " << program_name << " <command> [options]\n\n";
    std::cout << "Commands:\n";
    std::cout << "  rename    Batch rename files\n";
    std::cout << "  search    Search for text in files\n\n";
    std::cout << "Options:\n";
    std::cout << "  -p, --path <dir>       Directory path (required)\n";
    std::cout << "  -t, --pattern <glob>   File pattern (for rename) or search pattern (for search)\n";
    std::cout << "  -f, --find <str>       String to find (rename only)\n";
    std::cout << "  -r, --replace <str>    String to replace (rename only)\n";
    std::cout << "  -d, --dry-run          Preview changes without renaming\n";
    std::cout << "  -R, --recursive        Search recursively\n";
    std::cout << "  -e, --regex            Use regular expression\n";
    std::cout << "  -i, --ignore-case      Case insensitive matching\n";
    std::cout << "  -h, --help             Show this help message\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    
    std::string command = argv[1];
    
    // Define options
    static struct option long_options[] = {
        {"path", required_argument, 0, 'p'},
        {"pattern", required_argument, 0, 't'},
        {"find", required_argument, 0, 'f'},
        {"replace", required_argument, 0, 'r'},
        {"dry-run", no_argument, 0, 'd'},
        {"recursive", no_argument, 0, 'R'},
        {"regex", no_argument, 0, 'e'},
        {"ignore-case", no_argument, 0, 'i'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };
    
    std::string path, pattern, find_str, replace_str;
    bool dry_run = false, recursive = false, use_regex = false, ignore_case = false;
    
    int opt;
    int option_index = 0;
    
    // Reset getopt
    optind = 1;
    
    while ((opt = getopt_long(argc - 1, argv + 1, "p:t:f:r:dReih", 
                              long_options, &option_index)) != -1) {
        switch (opt) {
            case 'p': path = optarg; break;
            case 't': pattern = optarg; break;
            case 'f': find_str = optarg; break;
            case 'r': replace_str = optarg; break;
            case 'd': dry_run = true; break;
            case 'R': recursive = true; break;
            case 'e': use_regex = true; break;
            case 'i': ignore_case = true; break;
            case 'h':
                print_usage(argv[0]);
                return 0;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }
    
    if (path.empty()) {
        std::cerr << "Error: --path is required" << std::endl;
        return 1;
    }
    
    if (!fs::exists(path)) {
        std::cerr << "Error: Path does not exist: " << path << std::endl;
        return 1;
    }
    
    FileTools tools;
    tools.set_dry_run(dry_run);
    tools.set_recursive(recursive);
    tools.set_use_regex(use_regex);
    tools.set_ignore_case(ignore_case);
    
    if (command == "rename") {
        if (pattern.empty() || find_str.empty()) {
            std::cerr << "Error: --pattern and --find are required for rename" << std::endl;
            return 1;
        }
        if (replace_str.empty()) {
            replace_str = "";
        }
        std::cout << "=== Batch Rename Tool ===" << std::endl;
        std::cout << "Path: " << path << std::endl;
        std::cout << "Pattern: " << pattern << std::endl;
        std::cout << "Find: " << find_str << std::endl;
        std::cout << "Replace: " << replace_str << std::endl;
        if (dry_run) {
            std::cout << "Mode: DRY-RUN (no changes will be made)" << std::endl;
        }
        std::cout << std::endl;
        tools.rename_files(path, pattern, find_str, replace_str);
    } else if (command == "search") {
        if (pattern.empty()) {
            std::cerr << "Error: --pattern is required for search" << std::endl;
            return 1;
        }
        std::cout << "=== Text Search Tool ===" << std::endl;
        std::cout << "Path: " << path << std::endl;
        std::cout << "Pattern: " << pattern << std::endl;
        std::cout << "Recursive: " << (recursive ? "yes" : "no") << std::endl;
        std::cout << "Regex: " << (use_regex ? "yes" : "no") << std::endl;
        std::cout << "Case insensitive: " << (ignore_case ? "yes" : "no") << std::endl;
        std::cout << std::endl;
        tools.search_files(path, pattern);
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        print_usage(argv[0]);
        return 1;
    }
    
    return 0;
}
