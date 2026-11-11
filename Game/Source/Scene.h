#pragma once

#include "GameObject.h"

#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

class App;
class System;

class Scene
{
public:
    explicit Scene(App& app);
    virtual ~Scene();

    Scene(const Scene&) = delete;
    Scene& operator=(const Scene&) = delete;

    Scene(Scene&&) = delete;
    Scene& operator=(Scene&&) = delete;

    bool Load();

    bool PreUpdate();
    bool Update(float dt);
    bool PostUpdate();

    void Unload();

    bool IsLoaded() const noexcept;

    GameObject& CreateGameObject(
        const std::string& name
    );

    GameObject* FindGameObject(
        GameObjectId id
    ) noexcept;

    const GameObject* FindGameObject(
        GameObjectId id
    ) const noexcept;

    const std::vector<std::unique_ptr<GameObject>>&
        GetGameObjects() const noexcept;

    void DestroyPendingGameObjects();

    template<typename T, typename... Arguments>
    T& AddSystem(Arguments&&... arguments)
    {
        static_assert(
            std::is_base_of<System, T>::value,
            "T must derive from System"
            );

        auto system =
            std::make_unique<T>(
                std::forward<Arguments>(arguments)...
            );

        T* systemPointer = system.get();

        systems.push_back(
            std::move(system)
        );

        return *systemPointer;
    }

protected:
    App& GetApp() noexcept;
    const App& GetApp() const noexcept;

    virtual bool OnLoad() = 0;

    virtual bool OnPreUpdate();
    virtual bool OnUpdate(float dt);
    virtual bool OnPostUpdate();

    virtual void OnUnload();

private:
    App& app;

    bool loaded = false;

    GameObjectId nextGameObjectId = 1;

    std::vector<std::unique_ptr<GameObject>>
        gameObjects;

    std::vector<std::unique_ptr<System>>
        systems;
};