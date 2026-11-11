#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Camera3D.h"
#include "Window.h"
#include "SceneManager.h"

#include "Defs.h"
#include "Log.h"
#include "glew.h"
#include <algorithm>

#include "TransformComponent.h"
#include "ModelRendererComponent.h"
#include "RenderSystem.h"

SceneManager::SceneManager() : Module()
{
	name.Create("scene");
}

// Destructor
SceneManager::~SceneManager()
{}

// Called before render is available
bool SceneManager::Awake()
{
	DEBUG_LOG("Loading SceneManager");
	bool ret = true;

	return ret;
}
float a = 0;

// Called before the first frame
bool SceneManager::Start()
{
	app->tex->LoadFromPath("Assets/Textures/Baker_house.png");

	auto defaultShader =
		std::make_shared<Shader>(
			"Assets/Shaders/default.vertex",
			"Assets/Shaders/default.fragment"
		);

	if (!defaultShader->IsValid())
		return false;

	auto houseModel =
		std::make_shared<Model>(
			"Assets/Models/baker_house.fbx",
			app
		);

	houseModel->SetShaderForAllMaterials(
		defaultShader
	);


	//GameObject 1
	GameObject& houseA = 
		CreateGameObject("Baker house");

	TransformComponent& transformA =
		houseA.AddComponent<TransformComponent>();

	transformA.SetPosition(
		vec3(0.0f, 0.0f, 0.0f)
	);

	transformA.SetRotation(
		vec3(0.0f, 0.0f, 0.0f)
	);

	transformA.SetScale(
		vec3(1.0f, 1.0f, 1.0f)
	);

	houseA.AddComponent<ModelRendererComponent>(
		houseModel
	);

	

	goHouseA = houseA.GetId();

	//GameObject 1
	GameObject& houseB =
		CreateGameObject("Baker house");

	TransformComponent& transformB =
		houseB.AddComponent<TransformComponent>();

	transformB.SetPosition(
		vec3(0.0f, 0.0f, 0.0f)
	);

	transformB.SetRotation(
		vec3(0.0f, 0.0f, 0.0f)
	);

	transformB.SetScale(
		vec3(1.0f, 1.0f, 1.0f)
	);

	houseB.AddComponent<ModelRendererComponent>(
		houseModel
	);

	goHouseB = houseB.GetId();




	//Add systems
	systems.push_back(
		std::make_unique<RenderSystem>(
			*app->render
		)
	);
	return true;
}

// Called each loop iteration
bool SceneManager::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool SceneManager::Update(float dt)
{
	//Testing

	a += dt;

	vec3 posA = vec3(sin(a) * 2, 0.f, 5.f);
	vec3 posB = vec3(sin(a * 2) * 2, 0.f, -5.f);

	if (GameObject* house = FindGameObject(goHouseA))
	{
		if (TransformComponent* transform =
			house->GetComponent<TransformComponent>())
		{
			transform->SetPosition(posA);
		}
	}

	if (GameObject* house = FindGameObject(goHouseB))
	{
		if (TransformComponent* transform =
			house->GetComponent<TransformComponent>())
		{
			transform->SetPosition(posB);
		}
	}

	if (GameObject* house = FindGameObject(goHouseB))
	{
		if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
		{
			house->SetActive(!house->IsActive());
		}
	}


	//Systems
	bool success = true;

	for (const std::unique_ptr<System>& system :
		systems)
	{
		if (!system || !system->IsEnabled())
			continue;

		if (!system->Update(*this, dt))
			success = false;
	}

	DestroyPendingGameObjects();

	return success;
}

// Called each loop iteration
bool SceneManager::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool SceneManager::CleanUp()
{
	DEBUG_LOG("Freeing scene");

	systems.clear();
	gameObjects.clear();

	return true;
}

GameObject& SceneManager::CreateGameObject(
	const std::string& name
)
{
	auto object =
		std::make_unique<GameObject>(
			nextGameObjectId++,
			name
		);

	GameObject* objectPointer =
		object.get();

	gameObjects.push_back(
		std::move(object)
	);

	return *objectPointer;
}

const std::vector<std::unique_ptr<GameObject>>&
SceneManager::GetGameObjects() const noexcept
{
	return gameObjects;
}

void SceneManager::DestroyPendingGameObjects()
{
	gameObjects.erase(
		std::remove_if(
			gameObjects.begin(),
			gameObjects.end(),
			[](const std::unique_ptr<GameObject>& object)
			{
				return object->IsPendingDestroy();
			}
		),
		gameObjects.end()
	);
}

GameObject* SceneManager::FindGameObject(
	GameObjectId id
) noexcept
{
	for (const auto& object : gameObjects)
	{
		if (object && object->GetId() == id)
			return object.get();
	}

	return nullptr;
}
