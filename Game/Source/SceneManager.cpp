#include "App.h"
#include "SceneManager.h"

#include "Defs.h"
#include "Log.h"

#include "GameScene.h"
#include "Scene.h"

#include <memory>
#include <utility>

SceneManager::SceneManager(App& application)
	: app(application)
{
	name.Create("scene manager");
}

// Destructor
SceneManager::~SceneManager()
{}

// Called before render is available
bool SceneManager::Awake(
	pugi::xml_node&
)
{
	DEBUG_LOG("Loading SceneManager");
	return true;
}


// Called before the first frame
bool SceneManager::Start()
{
	pendingScene =
		std::make_unique<GameScene>(app);

	return ApplyPendingScene();
}
// Called each loop iteration
bool SceneManager::PreUpdate()
{
	if (!ApplyPendingScene())
		return false;

	if (!activeScene)
		return true;

	return activeScene->PreUpdate();
}

// Called each loop iteration
bool SceneManager::Update(float dt)
{
	if (!activeScene)
		return true;

	return activeScene->Update(dt);
}

// Called each loop iteration
bool SceneManager::PostUpdate()
{
	if (!activeScene)
		return true;

	return activeScene->PostUpdate();
}

// Called before quitting
bool SceneManager::CleanUp()
{
	DEBUG_LOG("Freeing SceneManager");

	pendingScene.reset();

	if (activeScene)
	{
		activeScene->Unload();
		activeScene.reset();
	}

	return true;
}

void SceneManager::RequestSceneChange(
	std::unique_ptr<Scene> scene
)
{
	pendingScene = std::move(scene);
}

bool SceneManager::ApplyPendingScene()
{
	if (!pendingScene)
		return true;

	// Primero intenta cargar la nueva.
	if (!pendingScene->Load())
	{
		DEBUG_LOG("Could not load pending scene");

		pendingScene.reset();

		// La escena anterior sigue activa.
		return false;
	}

	// Solo descarga la anterior cuando sabemos
	// que la nueva se ha cargado correctamente.
	if (activeScene)
		activeScene->Unload();

	activeScene = std::move(pendingScene);

	return true;
}

Scene* SceneManager::GetActiveScene() noexcept
{
	return activeScene.get();
}

const Scene*
SceneManager::GetActiveScene() const noexcept
{
	return activeScene.get();
}