#pragma once

class SceneManager;

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
        SceneManager& scene,
        float dt
    ) = 0;

private:
    bool enabled = true;
};