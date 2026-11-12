#pragma once

#include "Component.h"

#include <cstddef>
#include <memory>
#include <vector>
#include <functional>

class Material;
class Model;

class ModelRendererComponent final : public Component
{
public:
    ModelRendererComponent() = default;

    explicit ModelRendererComponent(
        std::shared_ptr<Model> model
    );

    void SetModel(std::shared_ptr<Model> model);

    const std::shared_ptr<Model>&
        GetModel() const noexcept;

    void SetMaterialOverride(
        std::size_t partIndex,
        std::shared_ptr<Material> material
    );

    std::shared_ptr<Material>
        GetMaterialOverride(
            std::size_t partIndex
        ) const;

    void ClearMaterialOverride(
        std::size_t partIndex
    );

    void ClearMaterialOverrides();

    bool IsVisible() const noexcept;
    void SetVisible(bool value) noexcept;


    bool SetMaterialOverrideForAllParts(
        std::shared_ptr<Material> material
    );

    void ClearMaterialOverrideForAllParts();

private:
    std::shared_ptr<Model> model;

    std::vector<std::shared_ptr<Material>>
        materialOverrides;

    bool visible = true;
};