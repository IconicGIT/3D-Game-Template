#pragma once

#include "System.h"

class Render;

class RenderSystem final : public System
{
public:
    explicit RenderSystem(Render& render);

    bool Update(
        SceneManager& scene,
        float dt
    ) override;

private:
    Render& render;
};