#include "ResourceManager.h"

#include "App.h"
#include "Log.h"
#include "Model.h"
#include "Shader.h"
#include "Textures.h"

#include <algorithm>
#include <cctype>
#include <utility>


namespace
{
    std::string ExtractFilename(
        const std::string& path
    )
    {
        const std::size_t separator =
            path.find_last_of("/\\");

        if (separator == std::string::npos)
            return path;

        return path.substr(separator + 1);
    }
}



ResourceManager::ResourceManager(
    App& application
)
    : app(application)
{
    name.Create("resources");
}

bool ResourceManager::Start()
{
    default3DShader =
        LoadShader(
            "Assets/Shaders/default.vertex",
            "Assets/Shaders/default.fragment"
        );

    if (!default3DShader)
    {
        DEBUG_LOG(
            "Could not load default 3D shader"
        );

        return false;
    }

    return true;
}

std::string ResourceManager::NormalizePath(
    const std::string& path
)
{
    std::string normalized = path;

    std::replace(
        normalized.begin(),
        normalized.end(),
        '\\',
        '/'
    );

    while (normalized.find("./") == 0)
    {
        normalized.erase(0, 2);
    }

#ifdef _WIN32
    std::transform(
        normalized.begin(),
        normalized.end(),
        normalized.begin(),
        [](unsigned char character)
        {
            return static_cast<char>(
                std::tolower(character)
                );
        }
    );
#endif

    return normalized;
}

std::string ResourceManager::MakeShaderKey(
    const std::string& vertexPath,
    const std::string& fragmentPath
)
{
    return NormalizePath(vertexPath) +
        "|" +
        NormalizePath(fragmentPath);
}

std::shared_ptr<Shader>
ResourceManager::LoadShader(
    const std::string& vertexPath,
    const std::string& fragmentPath
)
{
    const std::string key =
        MakeShaderKey(
            vertexPath,
            fragmentPath
        );

    const auto iterator =
        shaders.find(key);

    if (iterator != shaders.end())
    {
        if (std::shared_ptr<Shader> existing =
            iterator->second.lock())
        {
            return existing;
        }

        shaders.erase(iterator);
    }

    auto shader =
        std::make_shared<Shader>(
            vertexPath.c_str(),
            fragmentPath.c_str()
        );

    if (!shader->IsValid())
    {
        DEBUG_LOG(
            "Could not load shader: %s | %s",
            vertexPath.c_str(),
            fragmentPath.c_str()
        );

        return nullptr;
    }

    shaders.emplace(
        key,
        shader
    );

    return shader;
}

bool ResourceManager::LoadTexture(
    const std::string& path,
    Texture& output
)
{
    if (path.empty())
    {
        DEBUG_LOG(
            "Cannot load texture: empty path"
        );

        return false;
    }

    const std::string filename =
        ExtractFilename(path);

    if (filename.empty())
    {
        DEBUG_LOG(
            "Cannot load texture: invalid path [%s]",
            path.c_str()
        );

        return false;
    }

    // Primero intenta reutilizar una textura cargada.
    const Texture* texture =
        app.tex->FindTexture(filename);

    if (texture != nullptr)
    {
        output = *texture;
        return true;
    }

    // Si no existe, solicita su carga.
    if (!app.tex->LoadFromPath(path.c_str()))
    {
        DEBUG_LOG(
            "Could not load texture [%s]",
            path.c_str()
        );

        return false;
    }

    // LoadFromPath almacena la textura dentro de Textures.
    // Ahora volvemos a buscarla para obtener su descriptor.
    texture =
        app.tex->FindTexture(filename);

    if (texture == nullptr)
    {
        DEBUG_LOG(
            "Texture was loaded but not found in cache: %s",
            filename.c_str()
        );

        return false;
    }

    output = *texture;
    return true;
}

std::shared_ptr<Model>
ResourceManager::LoadModel(
    const std::string& path
)
{
    const std::string key =
        NormalizePath(path);

    const auto iterator =
        models.find(key);

    if (iterator != models.end())
    {
        if (std::shared_ptr<Model> existing =
            iterator->second.lock())
        {
            return existing;
        }

        models.erase(iterator);
    }

    auto model =
        std::make_shared<Model>(
            path.c_str(),
            &app
        );

    if (!model->IsValid())
    {
        DEBUG_LOG(
            "Could not load model: %s",
            path.c_str()
        );

        return nullptr;
    }

    model->SetShaderForAllMaterials(
        default3DShader
    );

    models.emplace(
        key,
        model
    );

    return model;
}

const std::shared_ptr<Shader>&
ResourceManager::GetDefault3DShader() const noexcept
{
    return default3DShader;
}

void ResourceManager::RemoveExpiredResources()
{
    for (auto iterator = models.begin();
        iterator != models.end();)
    {
        if (iterator->second.expired())
            iterator = models.erase(iterator);
        else
            ++iterator;
    }

    for (auto iterator = shaders.begin();
        iterator != shaders.end();)
    {
        if (iterator->second.expired())
            iterator = shaders.erase(iterator);
        else
            ++iterator;
    }
}

bool ResourceManager::CleanUp()
{
    DEBUG_LOG("Freeing ResourceManager");

    models.clear();
    shaders.clear();

    default3DShader.reset();

    return true;
}

