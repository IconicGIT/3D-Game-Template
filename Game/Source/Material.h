#pragma once

#include "Shader.h"
#include "Texture.h"
#include "glmath.h"

#include <memory>

enum class CullMode
{
    None,
    Back,
    Front
};


class Material
{
public:
    Material() = default;

    explicit Material(
        std::shared_ptr<Shader> shader
    );

    void SetShader(std::shared_ptr<Shader> shader);
    const std::shared_ptr<Shader>& GetShader() const noexcept;

    void SetAlbedoMap(const Texture& texture);
    const Texture& GetAlbedoMap() const noexcept;

    bool HasAlbedoMap() const noexcept;

public:
    vec4 tint = vec4(1.0f, 1.0f, 1.0f, 1.0f);

    bool depthTest = true;
    bool depthWrite = true;

    CullMode cullMode = CullMode::Back;

    bool transparent = false;
    bool wireframe = false;

private:
    std::shared_ptr<Shader> shader;
    Texture albedoMap;
};