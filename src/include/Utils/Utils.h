#pragma once
#ifndef UTILS_H
#define UTILS_H
#include <fstream>
#include <string>
using std::string;
class Utils {
public:
    string getCurrentFile();
    string readShaderSource(const char* path);

private:
};

#endif