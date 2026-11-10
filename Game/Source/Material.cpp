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

std::uint32_t Material::GetStateKey() const noexcept
{
    std::uint32_t key = 0;

    key |= depthTest  ? 1u << 0 : 0;
    key |= depthWrite ? 1u << 1 : 0;
    key |= wireframe  ? 1u << 2 : 0;

    key |=
        static_cast<std::uint32_t>(cullMode) << 3;

    return key;
}