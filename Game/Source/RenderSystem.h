#pragma once

#include "System.h"

class Render;
class Scene;

class RenderSystem final : public System
{
public:
    explicit RenderSystem(Render& render);

    bool Update(
        Scene& scene,
        float dt
    ) override;

private:
    Render& render;
};