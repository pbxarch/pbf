#include "file_manager.h"
#include "color_manager.h"
#include "tag_manager.h"
#include <iostream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <set>

namespace fs = std::filesystem;

// Helper function to get the color for a file based on its type
std::string getFileColor(const fs::path& path) {
    if (fs::is_directory(path)) {
        return "show_directory_col";
    } else if (path.extension() == ".cpp" || path.extension() == ".h") {
        return "show_cpp_col";
    } else if (path.extension() == ".py") {
        return "show_python_col";
    } else if (path.extension() == ".js") {
        return "show_js_col";
    } else if (path.extension() == ".txt") {
        return "show_txt_col";
    } else if (path.filename().string()[0] == '.') {
        return "show_hidden_col";
    } else {
        return "show_file_col";
    }
}

// Helper function to display a file with its tags
void displayFileWithTags(const fs::path& path, bool showTags) {
    std::string colorName = getFileColor(path);
    Color color = ColorManager::getColor(colorName);

    // Print the file name in the appropriate color
    std::cout << "\033[38;2;" << color.r << ";" << color.g << ";" << color.b << "m" << path.filename().string() << "\033[0m";

    if (showTags) {
        // Print tags if they exist
        auto it = TagManager::getTags().find(path.string());
        if (it != TagManager::getTags().end()) {
            for (const auto& [tag, tagColorName] : it->second) {
                Color tagColor = ColorManager::getColor(tagColorName);
                std::cout << " \033[38;2;" << tagColor.r << ";" << tagColor.g << ";" << tagColor.b << "m#" << tag << "\033[0m";
            }
        }
    }

    std::cout << std::endl;
}

void FileManager::listFiles(int argc, char* argv[]) {
    bool showHidden = false;
    bool showTags = false;
    fs::path path = fs::current_path();

    // Parse command-line arguments
    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-h") {
            showHidden = true;
        } else if (arg == "-t") {
            showTags = true;
        } else {
            path = arg;
        }
    }

    // List files in the directory
    std::vector<fs::path> directories, files;
    for (const auto& entry : fs::directory_iterator(path)) {
        if (!showHidden && entry.path().filename().string()[0] == '.') continue;

        if (fs::is_directory(entry)) {
            directories.push_back(entry.path());
        } else {
            files.push_back(entry.path());
        }
    }

    // Sort directories and files alphabetically
    std::sort(directories.begin(), directories.end());
    std::sort(files.begin(), files.end());

    // Display directories first
    for (const auto& dir : directories) {
        displayFileWithTags(dir, showTags);
    }

    // Display files
    for (const auto& file : files) {
        displayFileWithTags(file, showTags);
    }
}

void FileManager::searchFiles(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: pbf search [-t=any] [-t=all] <name/tags>" << std::endl;
        return;
    }

    std::string searchType = "name";
    std::set<std::string> searchTags;
    std::string searchName;

    // Parse command-line arguments
    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        if (arg.substr(0, 3) == "-t=") {
            searchType = arg.substr(3);
        } else {
            if (searchType == "name") {
                searchName = arg;
            } else {
                searchTags.insert(arg);
            }
        }
    }

    // Perform the search
    for (const auto& [path, tagList] : TagManager::getTags()) {
        bool match = false;

        if (searchType == "name") {
            // Search by file name
            if (fs::path(path).filename().string().find(searchName) != std::string::npos) {
                match = true;
            }
        } else if (searchType == "any") {
            // Search by any tag
            for (const auto& tag : searchTags) {
                if (std::any_of(tagList.begin(), tagList.end(), [&](const std::pair<std::string, std::string>& p) {
                    return p.first == tag;
                })) {
                    match = true;
                    break;
                }
            }
        } else if (searchType == "all") {
            // Search by all tags
            match = true;
            for (const auto& tag : searchTags) {
                if (!std::any_of(tagList.begin(), tagList.end(), [&](const std::pair<std::string, std::string>& p) {
                    return p.first == tag;
                })) {
                    match = false;
                    break;
                }
            }
        }

        if (match) {
            displayFileWithTags(path, true);
        }
    }
}
