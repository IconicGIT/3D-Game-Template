#pragma once

#include "glmath.h"

#include <memory>

class Mesh;
class Material;

struct RenderCommand
{
    std::shared_ptr<const Mesh> mesh;
    std::shared_ptr<const Material> material;

    mat4x4 modelMatrix = IdentityMatrix;

    // Se calcula al enviar el comando.
    // Se usa para ordenar transparencias.
    float distanceToCameraSquared = 0.0f;
};