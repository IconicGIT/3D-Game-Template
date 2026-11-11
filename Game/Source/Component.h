#pragma once

class GameObject;

class Component
{
public:
    virtual ~Component() = default;

    GameObject* GetOwner() noexcept
    {
        return owner;
    }

    const GameObject* GetOwner() const noexcept
    {
        return owner;
    }

    bool IsEnabled() const noexcept
    {
        return enabled;
    }

    void SetEnabled(bool value) noexcept
    {
        enabled = value;
    }

private:
    friend class GameObject;

    GameObject* owner = nullptr;
    bool enabled = true;
};