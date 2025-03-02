#include <iostream>
#include <filesystem>
#include "color_manager.h"
#include "tag_manager.h"
#include "file_manager.h"

namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: pbf <command> [options]" << std::endl;
        std::cerr << "Commands: start, save, list, search, tag, color" << std::endl;
        return 1;
    }

    std::string command = argv[1];

    if (command == "start") {
        // Load colors and tags into memory
        if (!fs::exists("colors.col")) {
            std::cerr << "Error: colors.col file not found." << std::endl;
            return 1;
        }
        if (!fs::exists("tags.tag")) {
            std::cerr << "Error: tags.tag file not found." << std::endl;
            return 1;
        }

        ColorManager::loadColors("colors.col");
        TagManager::loadTags("tags.tag");
        std::cout << "pbf started successfully. Colors and tags loaded into memory." << std::endl;
    } else if (command == "save") {
        // Save colors and tags to disk
        ColorManager::saveColors("colors.col");
        TagManager::saveTags("tags.tag");
        std::cout << "Colors and tags saved successfully." << std::endl;
    } else if (command == "list") {
        // Handle list command
        FileManager::listFiles(argc, argv);
    } else if (command == "search") {
        // Handle search command
        FileManager::searchFiles(argc, argv);
    } else if (command == "tag") {
        // Handle tag command
        TagManager::handleTagCommand(argc, argv);
    } else if (command == "color") {
        // Handle color command
        ColorManager::handleColorCommand(argc, argv);
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        std::cerr << "Available commands: start, save, list, search, tag, color" << std::endl;
        return 1;
    }

    return 0;
}
