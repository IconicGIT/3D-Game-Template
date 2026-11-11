#pragma once

#include "Module.h"
#include <type_traits>


class App;
class Scene;

class SceneManager final : public Module
{
public:
    explicit SceneManager(App& app);
    ~SceneManager() override;

    bool Awake(
        pugi::xml_node& config
    ) override;

    bool Start() override;

    bool PreUpdate() override;
    bool Update(float dt) override;
    bool PostUpdate() override;

    bool CleanUp() override;

    Scene* GetActiveScene() noexcept;
    const Scene* GetActiveScene() const noexcept;

    void RequestSceneChange(
        std::unique_ptr<Scene> scene
    );

    template<typename T, typename... Arguments>
    void RequestSceneChange(
        Arguments&&... arguments
    )
    {
        static_assert(
            std::is_base_of<Scene, T>::value,
            "T must derive from Scene"
            );

        RequestSceneChange(
            std::make_unique<T>(
                app,
                std::forward<Arguments>(arguments)...
            )
        );
    }

private:
    bool ApplyPendingScene();

private:
    App& app;

    std::unique_ptr<Scene> activeScene;
    std::unique_ptr<Scene> pendingScene;
};