#pragma once
#include <iostream>

struct Texture {
    std::string name;
    unsigned int id;
    std::string type;
    std::string path;

    int width;
    int height;
    int nrChannels;
};