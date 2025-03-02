#ifndef COLOR_MANAGER_H
#define COLOR_MANAGER_H

#include <string>
#include <map>
#include <vector>

struct Color {
    int r, g, b;
    std::string description;
};

class ColorManager {
public:
    static void loadColors(const std::string& filename);
    static void saveColors(const std::string& filename);
    static void handleColorCommand(int argc, char* argv[]);
    static Color getColor(const std::string& debugName);

private:
    static std::map<std::string, Color> colors;
};

#endif // COLOR_MANAGER_H
