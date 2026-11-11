#include "GameScene.h"

#include "App.h"
#include "Input.h"
#include "Material.h"
#include "Model.h"
#include "ModelRendererComponent.h"
#include "Render.h"
#include "RenderSystem.h"
#include "Shader.h"
#include "Textures.h"
#include "TransformComponent.h"

#include <cmath>
#include <memory>

GameScene::GameScene(App& app)
    : Scene(app)
{
}

bool GameScene::OnLoad()
{
    App& app = GetApp();

    if (!app.tex->LoadFromPath(
        "Assets/Textures/Baker_house.png"
    ))
    {
        return false;
    }

    auto defaultShader =
        std::make_shared<Shader>(
            "Assets/Shaders/default.vertex",
            "Assets/Shaders/default.fragment"
        );

    if (!defaultShader->IsValid())
        return false;

    auto houseModel =
        std::make_shared<Model>(
            "Assets/Models/baker_house.fbx",
            &app
        );

    if (houseModel->GetParts().empty())
        return false;

    houseModel->SetShaderForAllMaterials(
        defaultShader
    );

    GameObject& houseA =
        CreateGameObject("Baker house A");

    TransformComponent& transformA =
        houseA.AddComponent<TransformComponent>();

    transformA.SetPosition(
        vec3(0.0f, 0.0f, 5.0f)
    );

    houseA.AddComponent<ModelRendererComponent>(
        houseModel
    );

    houseAId = houseA.GetId();

    GameObject& houseB =
        CreateGameObject("Baker house B");

    TransformComponent& transformB =
        houseB.AddComponent<TransformComponent>();

    transformB.SetPosition(
        vec3(0.0f, 0.0f, -5.0f)
    );

    houseB.AddComponent<ModelRendererComponent>(
        houseModel
    );

    houseBId = houseB.GetId();

    // RenderSystem debe añadirse después de los sistemas
    // que modifican transformaciones.
    AddSystem<RenderSystem>(
        *app.render
    );

    return true;
}

bool GameScene::OnUpdate(float dt)
{
    animationTime += dt;

    const vec3 positionA(
        std::sin(animationTime) * 2.0f,
        0.0f,
        5.0f
    );

    const vec3 positionB(
        std::sin(animationTime * 2.0f) * 2.0f,
        0.0f,
        -5.0f
    );

    if (GameObject* houseA =
        FindGameObject(houseAId))
    {
        if (TransformComponent* transform =
            houseA->GetComponent<TransformComponent>())
        {
            transform->SetPosition(positionA);
        }
    }

    if (GameObject* houseB =
        FindGameObject(houseBId))
    {
        if (TransformComponent* transform =
            houseB->GetComponent<TransformComponent>())
        {
            transform->SetPosition(positionB);
        }

        if (GetApp().input->GetKey(
            SDL_SCANCODE_T
        ) == KEY_DOWN)
        {
            houseB->SetActive(
                !houseB->IsActive()
            );
        }
    }

    return true;
}

bool GameScene::OnPostUpdate()
{
    return GetApp().input->GetKey(
        SDL_SCANCODE_ESCAPE
    ) != KEY_DOWN;
}

void GameScene::OnUnload()
{
    houseAId = 0;
    houseBId = 0;

    animationTime = 0.0f;
}

