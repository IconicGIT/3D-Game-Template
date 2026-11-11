#include "GameObject.h"

GameObject::GameObject(GameObjectId objectId, std::string objectName)
    : id(objectId), name(std::move(objectName))
{
}