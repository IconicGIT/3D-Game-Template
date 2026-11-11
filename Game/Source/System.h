#pragma once

class Scene;

class System
{
public:
    virtual ~System() = default;

    bool IsEnabled() const noexcept
    {
        return enabled;
    }

    void SetEnabled(bool value) noexcept
    {
        enabled = value;
    }

    virtual bool Update(
        Scene& scene,
        float dt
    ) = 0;

private:
    bool enabled = true;
};