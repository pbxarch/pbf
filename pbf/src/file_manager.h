#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include <string>

class FileManager {
public:
    static void listFiles(int argc, char* argv[]);
    static void searchFiles(int argc, char* argv[]);
};

#endif // FILE_MANAGER_H
