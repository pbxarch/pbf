#ifndef TAG_MANAGER_H
#define TAG_MANAGER_H

#include <string>
#include <map>
#include <vector>

class TagManager {
public:
    static void loadTags(const std::string& filename);
    static void saveTags(const std::string& filename);
    static void handleTagCommand(int argc, char* argv[]);

    // Add this method to access the tags map
    static const std::map<std::string, std::vector<std::pair<std::string, std::string>>>& getTags();

private:
    static std::map<std::string, std::vector<std::pair<std::string, std::string>>> tags;
};

#endif // TAG_MANAGER_H
