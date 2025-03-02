#include "color_manager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

std::map<std::string, Color> ColorManager::colors;

void ColorManager::loadColors(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        std::string debugName;
        int r, g, b;
        std::string description;

        iss >> debugName >> r >> g >> b;
        std::getline(iss, description);

        colors[debugName] = {r, g, b, description};
    }
}

void ColorManager::saveColors(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return;
    }

    for (const auto& [debugName, color] : colors) {
        file << debugName << " " << color.r << " " << color.g << " " << color.b << " " << color.description << std::endl;
    }
}

Color ColorManager::getColor(const std::string& debugName) {
    return colors[debugName];
}

void ColorManager::handleColorCommand(int argc, char* argv[]) {
    bool showDebug = false;
    bool editMode = false;

    // Parse command-line arguments
    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "-d") {
            showDebug = true;
        } else if (arg == "-e") {
            editMode = true;
        }
    }

    if (editMode) {
        // Edit mode
        if (showDebug) {
            // Show only debug names
            for (const auto& [debugName, _] : colors) {
                std::cout << debugName << std::endl;
            }
        } else {
            // Allow editing colors
            std::string debugName;
            std::cout << "Enter debug name of the color to edit: ";
            std::cin >> debugName;

            if (colors.find(debugName) == colors.end()) {
                std::cerr << "Error: Color '" << debugName << "' not found." << std::endl;
                return;
            }

            int r, g, b;
            std::cout << "Enter new RGB values (e.g., 255 0 0): ";
            std::cin >> r >> g >> b;

            // Validate RGB values
            if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
                std::cerr << "Error: RGB values must be between 0 and 255." << std::endl;
                return;
            }

            // Save old color for comparison
            Color oldColor = colors[debugName];
            colors[debugName] = {r, g, b, oldColor.description};

            // Show old and new colors
            std::cout << "\033[38;2;" << oldColor.r << ";" << oldColor.g << ";" << oldColor.b << "m#####\033[0m";
            std::cout << " ~> ";
            std::cout << "\033[38;2;" << r << ";" << g << ";" << b << "m#####\033[0m" << std::endl;
        }
    } else {
        // Display mode
        for (const auto& [debugName, color] : colors) {
            if (showDebug) {
                // Show debug name and RGB values
                std::cout << debugName << " " << color.r << " " << color.g << " " << color.b << " ";
            }

            // Show description
            std::cout << color.description << " ";

            // Show color sample
            std::cout << "\033[38;2;" << color.r << ";" << color.g << ";" << color.b << "m#####\033[0m" << std::endl;
        }
    }
}
