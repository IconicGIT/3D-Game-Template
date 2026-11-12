#include "Model.h"
#include "Textures.h"
#include <vector>
#include <memory>


namespace
{
    std::string ExtractFilename(const std::string& path)
    {
        const std::size_t separator =
            path.find_last_of("/\\");

        if (separator == std::string::npos)
            return path;

        return path.substr(separator + 1);
    }
}

Model::Model(const char* path,App* application) : app(application)
{
    valid = LoadModel(path);
}

bool Model::IsValid() const noexcept
{
    return valid && !parts.empty();
}

bool Model::Set(const char* path)
{
    parts.clear();
    directory.clear();

    valid = LoadModel(path);
    return valid;
}

void Model::Set(const Model& model)
{
    parts = model.parts;
    directory = model.directory;
    valid = model.valid;
}

void Model::SetShaderForAllMaterials(
    const std::shared_ptr<Shader>& shader
)
{
    for (ModelPart& part : parts)
    {
        if (!part.baseMaterial)
            part.baseMaterial = std::make_shared<Material>();

        part.baseMaterial->SetShader(shader);
    }
}

bool Model::SetMaterial(
    std::size_t partIndex,
    std::shared_ptr<Material> material
)
{
    if (partIndex >= parts.size())
        return false;

    if (!material)
        return false;

    parts[partIndex].baseMaterial = std::move(material);
    return true;
}

std::shared_ptr<Material> Model::GetMaterial(
    std::size_t partIndex
) const
{
    if (partIndex >= parts.size())
        return nullptr;

    return parts[partIndex].baseMaterial;
}

bool Model::SetBaseMaterial(
    std::size_t partIndex,
    std::shared_ptr<Material> material
)
{
    if (partIndex >= parts.size())
        return false;

    if (!material)
        return false;

    parts[partIndex].baseMaterial =
        std::move(material);

    return true;
}

std::shared_ptr<Material>
Model::GetBaseMaterial(
    std::size_t partIndex
) const
{
    if (partIndex >= parts.size())
        return nullptr;

    return parts[partIndex].baseMaterial;
}

bool Model::LoadModel(std::string path)
{
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);


    directory = path.substr(0, path.find_last_of('/'));

    if (!scene ||
        (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) ||
        !scene->mRootNode)
    {
        DEBUG_LOG(
            "Assimp error loading [%s]: %s",
            path.c_str(),
        import.GetErrorString()
        );

        return false;
    }

    directory = path.substr(
        0,
        path.find_last_of("/\\")
    );

    ProcessNode(scene->mRootNode, scene);

    return true;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        parts.push_back(ProcessMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

ModelPart Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    /*App->ui->AppendToOutput(DEBUG_LOG("Loading Meshes..."));*/

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;

        // process vertex positions, normals and texture coordinates
        vec3 vector;
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        if (mesh->HasNormals())
        {
            vertex.Normal = vec3(
                mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z
            );
        }
        else
        {
            vertex.Normal = vec3(0.0f, 1.0f, 0.0f);
        }

        if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
        {
            vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = vec2(0.0f, 0.0f);

        vertices.push_back(vertex);
    }



    // process indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }


    ModelPart part;

    part.mesh = std::make_shared<Mesh>(
        vertices,
        indices
    );

    part.baseMaterial =
        std::make_shared<Material>();

    const Texture* defaultTexture =
        app->tex->GetDefaultTexture();

    if (defaultTexture != nullptr)
    {
        part.baseMaterial->SetAlbedoMap(
            *defaultTexture
        );
    }

    return part;
}

std::vector<Texture> Model::LoadMaterialTextures(
    aiMaterial* material,
    aiTextureType type,
    std::string typeName
)
{
    std::vector<Texture> loadedTextures;

    const unsigned int textureCount =
        material->GetTextureCount(type);

    for (unsigned int i = 0;
        i < textureCount;
        ++i)
    {
        aiString importedPath;

        if (material->GetTexture(
            type,
            i,
            &importedPath
        ) != AI_SUCCESS)
        {
            continue;
        }

        const std::string relativePath =
            importedPath.C_Str();

        const std::string filename =
            ExtractFilename(relativePath);

        // Primero intenta reutilizar una textura ya cargada.
        const Texture* texture =
            app->tex->FindTexture(filename);

        if (texture == nullptr)
        {
            // Assimp normalmente devuelve una ruta relativa
            // al archivo del modelo.
            const std::string fullPath =
                directory + "/" + relativePath;

            if (app->tex->LoadFromPath(
                fullPath.c_str()
            ))
            {
                texture =
                    app->tex->FindTexture(filename);
            }
        }

        if (texture != nullptr)
        {
            Texture loadedTexture = *texture;
            loadedTexture.type = typeName;

            loadedTextures.push_back(
                std::move(loadedTexture)
            );
        }
        else
        {
            DEBUG_LOG(
                "Could not load model texture: %s",
                relativePath.c_str()
            );

            const Texture* defaultTexture =
                app->tex->GetDefaultTexture();

            if (defaultTexture != nullptr)
            {
                Texture fallback = *defaultTexture;
                fallback.type = typeName;

                loadedTextures.push_back(
                    std::move(fallback)
                );
            }
        }
    }

    return loadedTextures;
}


bool Model::SetBaseAlbedoMap(
    std::size_t partIndex,
    const Texture& texture
)
{
    if (partIndex >= parts.size())
        return false;

    ModelPart& part = parts[partIndex];

    if (!part.baseMaterial)
    {
        part.baseMaterial =
            std::make_shared<Material>();
    }

    part.baseMaterial->SetAlbedoMap(
        texture
    );

    return true;
}

void Model::SetBaseAlbedoMapForAllParts(
    const Texture& texture
)
{
    for (ModelPart& part : parts)
    {
        if (!part.baseMaterial)
        {
            part.baseMaterial =
                std::make_shared<Material>();
        }

        part.baseMaterial->SetAlbedoMap(
            texture
        );
    }
}

