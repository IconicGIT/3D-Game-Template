#pragma once

#include "Component.h"

#include <memory>
#include <string>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <utility>

using GameObjectId = std::uint64_t;

class GameObject
{
public:
    explicit GameObject(GameObjectId id, std::string name);

    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;

    GameObject(GameObject&&) = delete;
    GameObject& operator=(GameObject&&) = delete;

    template<typename T, typename... Arguments>
    T& AddComponent(Arguments&&... arguments)
    {
        static_assert(
            std::is_base_of<Component, T>::value,
            "T must derive from Component"
            );

        const std::type_index type =
            std::type_index(typeid(T));

        auto existing = components.find(type);

        if (existing != components.end())
        {
            return *static_cast<T*>(
                existing->second.get()
                );
        }

        std::unique_ptr<T> component =
            std::make_unique<T>(
                std::forward<Arguments>(arguments)...
            );

        component->owner = this;

        T* componentPointer = component.get();

        components.emplace(
            type,
            std::move(component)
        );

        return *componentPointer;
    }

    template<typename T>
    T* GetComponent() noexcept
    {
        const auto iterator = components.find(
            std::type_index(typeid(T))
        );

        if (iterator == components.end())
            return nullptr;

        return static_cast<T*>(
            iterator->second.get()
            );
    }

    template<typename T>
    const T* GetComponent() const noexcept
    {
        const auto iterator = components.find(
            std::type_index(typeid(T))
        );

        if (iterator == components.end())
            return nullptr;

        return static_cast<const T*>(
            iterator->second.get()
            );
    }

    template<typename T>
    bool HasComponent() const noexcept
    {
        return GetComponent<T>() != nullptr;
    }

    template<typename T>
    bool RemoveComponent()
    {
        return components.erase(
            std::type_index(typeid(T))
        ) > 0;
    }

    const std::string& GetName() const noexcept
    {
        return name;
    }

    GameObjectId GetId() const noexcept
    {
        return id;
    }

    bool IsActive() const noexcept
    {
        return active;
    }

    void SetActive(bool value) noexcept
    {
        active = value;
    }

    bool IsPendingDestroy() const noexcept
    {
        return pendingDestroy;
    }

    void Destroy() noexcept
    {
        pendingDestroy = true;
    }

private:
    GameObjectId id = 0;
    std::string name;

    bool active = true;
    bool pendingDestroy = false;

    std::unordered_map<
        std::type_index,
        std::unique_ptr<Component>
    > components;
};