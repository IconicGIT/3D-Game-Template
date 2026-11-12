#pragma once

#include "Scene.h"

class IntroScene final : public Scene
{
public:
    explicit IntroScene(App& app);

protected:
    bool OnLoad() override;
    bool OnUpdate(float dt) override;
    bool OnPostUpdate() override;

    void OnUnload() override;

private:
    GameObjectId houseId = 0;

    float animationTime = 0.0f;
};