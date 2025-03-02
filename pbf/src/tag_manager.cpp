#include "tag_manager.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::map<std::string, std::vector<std::pair<std::string, std::string>>> TagManager::tags;

void TagManager::loadTags(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string path;
        std::string tag, color;

        iss >> path;
        while (iss >> tag >> color) {
            tags[path].emplace_back(tag, color);
        }
    }
}

void TagManager::saveTags(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    for (const auto& [path, tagList] : tags) {
        file << path;
        for (const auto& [tag, color] : tagList) {
            file << " " << tag << " " << color;
        }
        file << std::endl;
    }
}

void TagManager::handleTagCommand(int argc, char* argv[]) {
    if (argc < 3) {
        // Display tags for all files or a specific file
        std::string path = (argc == 2) ? argv[1] : ".";
        if (tags.find(path) != tags.end()) {
            for (const auto& [tag, color] : tags[path]) {
                std::cout << "#" << tag << " (Color: " << color << ")" << std::endl;
            }
        } else {
            std::cout << "No tags found for " << path << std::endl;
        }
        return;
    }

    std::string option = argv[2];
    if (option == "-a") {
        // Add a tag
        if (argc < 6) {
            std::cerr << "Usage: pbf tag -a <path> <tag> <color>" << std::endl;
            return;
        }

        std::string path = argv[3];
        std::string tag = argv[4];
        std::string color = argv[5];

        // Check if the tag already exists for the path
        auto& tagList = tags[path];
        auto it = std::find_if(tagList.begin(), tagList.end(), [&](const std::pair<std::string, std::string>& p) {
            return p.first == tag;
        });

        if (it != tagList.end()) {
            std::cerr << "Error: Tag '" << tag << "' already exists for " << path << std::endl;
            return;
        }

        // Add the tag
        tagList.emplace_back(tag, color);
        std::cout << "Added tag '" << tag << "' to " << path << std::endl;
    } else if (option == "-r") {
        // Remove a tag
        if (argc < 5) {
            std::cerr << "Usage: pbf tag -r <path> <tag>" << std::endl;
            return;
        }

        std::string path = argv[3];
        std::string tag = argv[4];

        if (tags.find(path) == tags.end()) {
            std::cerr << "Error: No tags found for " << path << std::endl;
            return;
        }

        auto& tagList = tags[path];
        auto it = std::find_if(tagList.begin(), tagList.end(), [&](const std::pair<std::string, std::string>& p) {
            return p.first == tag;
        });

        if (it == tagList.end()) {
            std::cerr << "Error: Tag '" << tag << "' not found for " << path << std::endl;
            return;
        }

        // Remove the tag
        tagList.erase(it);
        std::cout << "Removed tag '" << tag << "' from " << path << std::endl;
    } else {
        std::cerr << "Error: Unknown option '" << option << "'" << std::endl;
        std::cerr << "Usage: pbf tag [-a <path> <tag> <color> | -r <path> <tag>]" << std::endl;
    }
}

// Implement the getTags() method
const std::map<std::string, std::vector<std::pair<std::string, std::string>>>& TagManager::getTags() {
    return tags;
}
