#include "IntroScene.h"

#include "App.h"
#include "Input.h"
#include "Material.h"
#include "Model.h"
#include "ModelRendererComponent.h"
#include "Render.h"
#include "RenderSystem.h"
#include "ResourceManager.h"
#include "TransformComponent.h"

#include <cmath>
#include <memory>

#include "Textures.h"

IntroScene::IntroScene(App& app)
    : Scene(app)
{
}

bool IntroScene::OnLoad()
{
    App& app = GetApp();

    //Load Model
    std::shared_ptr<Model> houseModel =
        GetApp().resources->LoadModel(
            "Assets/Models/baker_house.fbx"
        );

    if (!houseModel)
        return false;

    //Load Texture
    Texture texture_house;



    //Apply texture to model
    if (app.resources->LoadTexture("Assets/Textures/baker_house.png", texture_house))
    {
        houseModel->SetBaseAlbedoMapForAllParts(
            texture_house
        );
    }


    GameObject& houseA =
        CreateGameObject("Baker house A");

    TransformComponent& transformA =
        houseA.AddComponent<TransformComponent>();

    transformA.SetPosition(
        vec3(0.0f, 0.0f, 5.0f)
    );



    ModelRendererComponent& houseArenderer = houseA.AddComponent<ModelRendererComponent>(
        houseModel
    );


    std::shared_ptr<Material> houseAMaterial = std::make_shared<Material>(*houseModel->GetBaseMaterial(0));
    houseAMaterial->SetAlbedoMap(texture_house);

    houseAMaterial->transparent = true;
    houseAMaterial->depthWrite = false;
    houseAMaterial->tint = vec4(1, 1, 1, 0.2f);

    houseArenderer.SetMaterialOverrideForAllParts(houseAMaterial);

    houseId = houseA.GetId();

    // RenderSystem debe añadirse después de los sistemas
    // que modifican transformaciones.
    AddSystem<RenderSystem>(
        *app.render
    );

    return true;
}

bool IntroScene::OnUpdate(float dt)
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
        FindGameObject(houseId))
    {
        if (TransformComponent* transform =
            houseA->GetComponent<TransformComponent>())
        {
            transform->SetPosition(positionA);
        }
    }

    return true;
}

bool IntroScene::OnPostUpdate()
{
    return GetApp().input->GetKey(
        SDL_SCANCODE_ESCAPE
    ) != KEY_DOWN;
}

void IntroScene::OnUnload()
{
    houseId = 0;

    animationTime = 0.0f;
}

