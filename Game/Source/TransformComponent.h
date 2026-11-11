#pragma once

#include "Component.h"
#include "glmath.h"

class TransformComponent final : public Component
{
public:
    void SetPosition(const vec3& value) noexcept;
    void SetRotation(const vec3& value) noexcept;
    void SetScale(const vec3& value) noexcept;

    const vec3& GetPosition() const noexcept;
    const vec3& GetRotation() const noexcept;
    const vec3& GetScale() const noexcept;

    const mat4x4& GetWorldMatrix() const;

private:
    void RecalculateMatrix() const;

private:
    vec3 position = vec3(0.0f, 0.0f, 0.0f);
    vec3 rotation = vec3(0.0f, 0.0f, 0.0f);
    vec3 scaleValue = vec3(1.0f, 1.0f, 1.0f);

    mutable mat4x4 worldMatrix =
        IdentityMatrix;

    mutable bool matrixDirty = true;
};