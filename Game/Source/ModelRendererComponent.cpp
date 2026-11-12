#include "ModelRendererComponent.h"

#include "Material.h"
#include "Model.h"

#include <utility>

ModelRendererComponent::ModelRendererComponent(
    std::shared_ptr<Model> initialModel
)
    : model(std::move(initialModel))
{
}

void ModelRendererComponent::SetModel(
    std::shared_ptr<Model> newModel
)
{
    model = std::move(newModel);
    materialOverrides.clear();
}

const std::shared_ptr<Model>&
ModelRendererComponent::GetModel() const noexcept
{
    return model;
}

void ModelRendererComponent::SetMaterialOverride(
    std::size_t partIndex,
    std::shared_ptr<Material> material
)
{
    if (partIndex >= materialOverrides.size())
    {
        materialOverrides.resize(
            partIndex + 1
        );
    }

    materialOverrides[partIndex] =
        std::move(material);
}

std::shared_ptr<Material>
ModelRendererComponent::GetMaterialOverride(
    std::size_t partIndex
) const
{
    if (partIndex >= materialOverrides.size())
        return nullptr;

    return materialOverrides[partIndex];
}

void ModelRendererComponent::ClearMaterialOverride(
    std::size_t partIndex
)
{
    if (partIndex < materialOverrides.size())
        materialOverrides[partIndex].reset();
}

void ModelRendererComponent::ClearMaterialOverrides()
{
    materialOverrides.clear();
}

bool ModelRendererComponent::IsVisible() const noexcept
{
    return visible;
}

void ModelRendererComponent::SetVisible(
    bool value
) noexcept
{
    visible = value;
}

bool ModelRendererComponent::
SetMaterialOverrideForAllParts(
    std::shared_ptr<Material> material
)
{
    if (!model || !material)
        return false;

    materialOverrides.assign(
        model->GetParts().size(),
        std::move(material)
    );

    return true;
}

void ModelRendererComponent::
ClearMaterialOverrideForAllParts()
{
    materialOverrides.clear();
}
