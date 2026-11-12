#pragma once
#pragma once

#include "Module.h"

#include <memory>
#include <string>
#include <unordered_map>
#include "Texture.h"

class App;
class Model;
class Shader;


class ResourceManager final : public Module
{
public:
    explicit ResourceManager(App& app);
    ~ResourceManager() override = default;

    bool Start() override;
    bool CleanUp() override;

    std::shared_ptr<Model> LoadModel(
        const std::string& path
    );

    std::shared_ptr<Shader> LoadShader(
        const std::string& vertexPath,
        const std::string& fragmentPath
    );

    bool ResourceManager::LoadTexture(
        const std::string& path,
        Texture& output
    );

    const std::shared_ptr<Shader>&
        GetDefault3DShader() const noexcept;

    void RemoveExpiredResources();

private:
    static std::string NormalizePath(
        const std::string& path
    );

    static std::string MakeShaderKey(
        const std::string& vertexPath,
        const std::string& fragmentPath
    );

private:
    App& app;

    std::shared_ptr<Shader> default3DShader;

    std::unordered_map<
        std::string,
        std::weak_ptr<Model>
    > models;

    std::unordered_map<
        std::string,
        std::weak_ptr<Shader>
    > shaders;
};