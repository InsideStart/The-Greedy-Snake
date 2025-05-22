#include "Utils/Utils.h"
#if(WIN32)
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
#endif
string Utils::getCurrentFile() {
    string currentPath;
#if(WIN32)
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    string::size_type pos = string(buffer).find_last_of("\\/");
    currentPath = (pos != string::npos) ? string(buffer).substr(0, pos) : "";
#else
    char buffer[PATH_MAX];
    if(getcwd(buffer,sizeof(buffer)) != nullptr) {
        currentPath = string(bffer);
    }
    else {
        perror("getcwd() error!");
    }
#endif
    return currentPath;
}

string Utils::readShaderSource(const char* path) {
    string content;
    std::ifstream fileStream(path,std::ios::in);
    string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + '\n');
    }
    fileStream.close();
    return content;
}