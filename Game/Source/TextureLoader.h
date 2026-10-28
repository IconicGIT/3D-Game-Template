#ifndef __TEXTURE_LOADER_H__
#define __TEXTURE_LOADER_H__


#include <iostream>
#include "Glew/include/GL/glew.h"
#include "Texture.h"
#include <vector>

Texture LoadTexture(std::string file_path);

std::vector<Texture> LoadSlicedTexture(std::string file_path, int horizontalDivisions, int vertialDivisions, int amount);


#endif // !__TEXTURE_LOADER_H__


