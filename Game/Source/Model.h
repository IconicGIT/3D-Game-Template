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
#include "Material.h"

class Render;

struct ModelPart
{
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Material> material;
};

class Model
{
public:
    Model(const char* path, App* app);
    
    void Draw(Render& render, const mat4x4& modelMatrix) const;

    bool Set(const char* path);
    void Set(const Model& model);
    //void Set(Model* model);

    //std::vector<Mesh> GetMeshes() const
    //{
    //    return meshes;
    //}

    std::string GetDirectory() const
    {
        return directory;
    }

    const std::vector<ModelPart>& GetParts() const noexcept
    {
        return parts;
    }

    void SetShaderForAllMaterials(
        const std::shared_ptr<Shader>& shader
    );

    bool SetMaterial(std::size_t partIndex, std::shared_ptr<Material> material);
    std::shared_ptr<Material> GetMaterial(std::size_t partIndex) const;

private:
    App* app;

    // model data
    std::vector<ModelPart> parts;
    std::string directory = "";
   

    bool LoadModel(std::string path);

    void ProcessNode(aiNode* node, const aiScene* scene);

    ModelPart ProcessMesh(aiMesh* mesh, const aiScene* scene);

    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

};