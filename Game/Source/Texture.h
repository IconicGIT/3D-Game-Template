#pragma once

#include <string>

struct Texture
{
    std::string name;
    unsigned int id = 0;
    std::string type;
    std::string path;

    int width = 0;
    int height = 0;
    int nrChannels = 0;

    bool IsValid() const noexcept
    {
        return id != 0;
    }
};