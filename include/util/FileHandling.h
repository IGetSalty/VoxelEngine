#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

std::string ReadTextFile(const std::string& filePath) {
    std::string fileContents;
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filePath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    fileContents = buffer.str();

    file.close();

    return fileContents;
}