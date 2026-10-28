#include "Model.h"
#include "Textures.h"
#include <vector>


Model::Model(const char* path, App* app) 
{
    this->app = app;
    LoadModel(path);
}

bool Model::Set(const char* path)
{
    return LoadModel(path);
}

void Model::Set(Model *model)
{
    for (Mesh m : model->meshes)
    {
        meshes.push_back(m);
    }

    for (Texture  t : model->textures_loaded)
    {
        textures_loaded.push_back(t);
    }

    directory = model->directory;
}

bool Model::Set(Model otherModel)
{
    bool ret = true;

    std::copy(otherModel.textures_loaded.begin(), otherModel.textures_loaded.end(), textures_loaded.begin());
    std::copy(otherModel.GetMeshes().begin(), otherModel.GetMeshes().end(), meshes.begin());
    directory = otherModel.GetDirectory();

    if (textures_loaded.empty() || meshes.empty() || std::strcmp(directory.c_str(), ""))
    {
        ret = false;
    }

    return ret;
}

void Model::Draw(Shader& shader, bool wireframeActive)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader, wireframeActive);
}

bool Model::LoadModel(std::string path)
{
    bool ret = true;
    Assimp::Importer import;
    const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        //std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
        /*App->ui->AppendToOutput(DEBUG_LOG("ERROR::ASSIMP:: %s", import.GetErrorString()));*/

        ret = false;
    }
    directory = path.substr(0, path.find_last_of('/'));

    ProcessNode(scene->mRootNode, scene);

    return ret;
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(ProcessMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
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

        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;

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

    //App->ui->AppendToOutput(DEBUG_LOG("Loading Textures..."));

    // process material
    if (mesh->mMaterialIndex >= 0)
    {
        if (mesh->mMaterialIndex >= 0)
        {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            std::vector<Texture> diffuseMaps = LoadMaterialTextures(material,
                aiTextureType_DIFFUSE, "texture_diffuse");

            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            std::vector<Texture> specularMaps = LoadMaterialTextures(material,
                aiTextureType_SPECULAR, "texture_specular");

            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }
    }

    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> loaded_textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < app->tex->GetAllTextures().size(); j++)
        {
            if (std::strcmp(app->tex->GetAllTextures()[j].path.c_str(), str.C_Str()) == 0)
            {
                loaded_textures.push_back(app->tex->GetAllTextures()[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            //Texture texture = LoadTexture("Assets/Project_1/Assets/Textures/baker_house.png");
            //std::string name = "baker_house.png ";/*[" + std::to_string(texture.id);
            //name += "]";*/
            //texture.name = name;
            //texture.type = typeName;
            loaded_textures.push_back(app->tex->GetAllTextures()[0]);
            //textures_loaded.push_back(texture); // add to loaded textures
        }
    }
    return loaded_textures;
}