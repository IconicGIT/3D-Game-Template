#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Mesh.h"
#include "Texture.h"
#include "TextureLoader.h"
#include "Model.h"
#include "GameObject.h"

#include <memory>
#include <string>
#include <vector>

class System;
class RenderSystem;


class Scene : public Module
{
public:


	GameObject& CreateGameObject(
		const std::string& name
	);

	const std::vector<std::unique_ptr<GameObject>>&
		GetGameObjects() const noexcept;

	void DestroyPendingGameObjects();

	GameObject* FindGameObject(GameObjectId id) noexcept;


	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:

	GameObjectId goHouseA = 0;
	GameObjectId goHouseB = 0;

	GameObjectId nextGameObjectId = 0;

	std::vector<std::unique_ptr<GameObject>>
		gameObjects;

	std::vector<std::unique_ptr<System>>
		systems;

};

#endif // __SCENE_H__