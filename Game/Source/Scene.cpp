#include "Scene.h"

#include "GameObject.h"
#include "System.h"

#include <algorithm>

Scene::Scene(App& application)
    : app(application)
{
}

Scene::~Scene() = default;

bool Scene::Load()
{
    if (loaded)
        return true;

    if (!OnLoad())
    {
        OnUnload();

        systems.clear();
        gameObjects.clear();

        nextGameObjectId = 1;

        return false;
    }

    loaded = true;
    return true;
}

bool Scene::PreUpdate()
{
    if (!loaded)
        return false;

    return OnPreUpdate();
}

bool Scene::Update(float dt)
{
    if (!loaded)
        return false;

    bool success = OnUpdate(dt);

    for (const std::unique_ptr<System>& system :
        systems)
    {
        if (!system || !system->IsEnabled())
            continue;

        if (!system->Update(*this, dt))
            success = false;
    }

    DestroyPendingGameObjects();

    return success;
}

bool Scene::PostUpdate()
{
    if (!loaded)
        return false;

    return OnPostUpdate();
}

void Scene::Unload()
{
    if (!loaded)
        return;

    OnUnload();

    // Primero los sistemas, porque pueden contener
    // referencias al renderer u otros servicios.
    systems.clear();

    // Después objetos y componentes.
    gameObjects.clear();

    nextGameObjectId = 1;
    loaded = false;
}

bool Scene::IsLoaded() const noexcept
{
    return loaded;
}

App& Scene::GetApp() noexcept
{
    return app;
}

const App& Scene::GetApp() const noexcept
{
    return app;
}

GameObject& Scene::CreateGameObject(
    const std::string& name
)
{
    auto object =
        std::make_unique<GameObject>(
            nextGameObjectId++,
            name
        );

    GameObject* objectPointer =
        object.get();

    gameObjects.push_back(
        std::move(object)
    );

    return *objectPointer;
}

const std::vector<std::unique_ptr<GameObject>>&
Scene::GetGameObjects() const noexcept
{
    return gameObjects;
}

GameObject* Scene::FindGameObject(
    GameObjectId id
) noexcept
{
    for (const auto& object : gameObjects)
    {
        if (object && object->GetId() == id)
            return object.get();
    }

    return nullptr;
}

const GameObject* Scene::FindGameObject(
    GameObjectId id
) const noexcept
{
    for (const auto& object : gameObjects)
    {
        if (object && object->GetId() == id)
            return object.get();
    }

    return nullptr;
}

void Scene::DestroyPendingGameObjects()
{
    gameObjects.erase(
        std::remove_if(
            gameObjects.begin(),
            gameObjects.end(),
            [](const std::unique_ptr<GameObject>& object)
            {
                return !object ||
                    object->IsPendingDestroy();
            }
        ),
        gameObjects.end()
    );
}

bool Scene::OnPreUpdate()
{
    return true;
}

bool Scene::OnUpdate(float)
{
    return true;
}

bool Scene::OnPostUpdate()
{
    return true;
}

void Scene::OnUnload()
{
}
