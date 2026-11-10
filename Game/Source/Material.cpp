#include "Material.h"
#include <utility>

Material::Material(std::shared_ptr<Shader> shader)
    : shader(std::move(shader))
{
}

void Material::SetShader(std::shared_ptr<Shader> newShader)
{
    shader = std::move(newShader);
}

const std::shared_ptr<Shader>& Material::GetShader() const noexcept
{
    return shader;
}

void Material::SetAlbedoMap(const Texture& texture)
{
    albedoMap = texture;
}

const Texture& Material::GetAlbedoMap() const noexcept
{
    return albedoMap;
}

bool Material::HasAlbedoMap() const noexcept
{
    return albedoMap.IsValid();
}