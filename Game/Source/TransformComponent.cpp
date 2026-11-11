#include "TransformComponent.h"

void TransformComponent::SetPosition(
    const vec3& value
) noexcept
{
    position = value;
    matrixDirty = true;
}

void TransformComponent::SetRotation(
    const vec3& value
) noexcept
{
    rotation = value;
    matrixDirty = true;
}

void TransformComponent::SetScale(
    const vec3& value
) noexcept
{
    scaleValue = value;
    matrixDirty = true;
}

const vec3& TransformComponent::GetPosition() const noexcept
{
    return position;
}

const vec3& TransformComponent::GetRotation() const noexcept
{
    return rotation;
}

const vec3& TransformComponent::GetScale() const noexcept
{
    return scaleValue;
}

const mat4x4& TransformComponent::GetWorldMatrix() const
{
    if (matrixDirty)
        RecalculateMatrix();

    return worldMatrix;
}

void TransformComponent::RecalculateMatrix() const
{
    const mat4x4 translationMatrix =
        translate(
            position.x,
            position.y,
            position.z
        );

    const mat4x4 rotationX =
        rotate(
            rotation.x,
            vec3(1.0f, 0.0f, 0.0f)
        );

    const mat4x4 rotationY =
        rotate(
            rotation.y,
            vec3(0.0f, 1.0f, 0.0f)
        );

    const mat4x4 rotationZ =
        rotate(
            rotation.z,
            vec3(0.0f, 0.0f, 1.0f)
        );

    const mat4x4 scaleMatrix =
        scale(
            scaleValue.x,
            scaleValue.y,
            scaleValue.z
        );

    worldMatrix =
        translationMatrix *
        rotationZ *
        rotationY *
        rotationX *
        scaleMatrix;

    matrixDirty = false;
}