#pragma once

#include "App.h"
#include "Mesh.h"
#include "Shader.h"

#include <Assimp/include/Importer.hpp>
#include <Assimp/include/scene.h>
#include <Assimp/include/postprocess.h>
#include <Assimp/include/cimport.h>

#include "Stb/stb_image.h"
#include "TextureLoader.h"

#include "glmath.h"

class Model
{
public:
    Model(const char* path, App* app);
    
    void Draw(Shader& shader, bool drawMode);

    bool Set(const char* path);
    bool Set(Model model);
    void Set(Model* model);

    std::vector<Mesh> GetMeshes() const
    {
        return meshes;
    }

    std::string GetDirectory() const
    {
        return directory;
    }

    std::vector<Texture> GetTextures() const
    {
        return textures_loaded;
    }

private:
    App* app;
    std::vector<Texture> textures_loaded;

    // model data
    std::vector<Mesh> meshes;
    std::string directory = "";
   

    bool LoadModel(std::string path);

    void ProcessNode(aiNode* node, const aiScene* scene);

    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

};