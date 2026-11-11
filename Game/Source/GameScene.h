#pragma once

#include "Scene.h"

class GameScene final : public Scene
{
public:
    explicit GameScene(App& app);

protected:
    bool OnLoad() override;
    bool OnUpdate(float dt) override;
    bool OnPostUpdate() override;

    void OnUnload() override;

private:
    GameObjectId houseAId = 0;
    GameObjectId houseBId = 0;

    float animationTime = 0.0f;
};