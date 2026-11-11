#include "RenderSystem.h"

#include "GameObject.h"
#include "Material.h"
#include "Model.h"
#include "ModelRendererComponent.h"
#include "Render.h"
#include "SceneManager.h"
#include "TransformComponent.h"

RenderSystem::RenderSystem(Render& renderer)
    : render(renderer)
{
}

bool RenderSystem::Update(
    SceneManager& scene,
    float dt
)
{
    bool success = true;

    for (const std::unique_ptr<GameObject>& object :
        scene.GetGameObjects())
    {
        if (!object)
            continue;

        if (!object->IsActive())
            continue;

        if (object->IsPendingDestroy())
            continue;

        const TransformComponent* transform =
            object->GetComponent<TransformComponent>();

        const ModelRendererComponent* modelRenderer =
            object->GetComponent<ModelRendererComponent>();

        if (transform == nullptr ||
            modelRenderer == nullptr)
        {
            continue;
        }

        if (!transform->IsEnabled() ||
            !modelRenderer->IsEnabled() ||
            !modelRenderer->IsVisible())
        {
            continue;
        }

        const std::shared_ptr<Model>& model =
            modelRenderer->GetModel();

        if (!model)
        {
            success = false;
            continue;
        }

        const mat4x4& worldMatrix =
            transform->GetWorldMatrix();

        const std::vector<ModelPart>& parts =
            model->GetParts();

        for (std::size_t partIndex = 0;
            partIndex < parts.size();
            ++partIndex)
        {
            const ModelPart& part =
                parts[partIndex];

            if (!part.mesh)
            {
                success = false;
                continue;
            }

            std::shared_ptr<Material> material =
                modelRenderer->GetMaterialOverride(
                    partIndex
                );

            if (!material)
                material = part.material;

            if (!material)
            {
                success = false;
                continue;
            }

            if (!render.Submit(
                part.mesh,
                material,
                worldMatrix
            ))
            {
                success = false;
            }
        }
    }

    return success;
}