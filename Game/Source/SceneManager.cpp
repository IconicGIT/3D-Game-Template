#include "App.h"
#include "SceneManager.h"

#include "Defs.h"
#include "Log.h"

#include "GameScene.h"
#include "IntroScene.h"
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
		std::make_unique<IntroScene>(app);

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

	// Intentamos cargar la nueva escena antes
	// de descargar la escena actual.
	if (!pendingScene->Load())
	{
		DEBUG_LOG("Could not load pending scene");

		pendingScene.reset();

		// Si había una escena activa, el juego puede
		// continuar utilizándola.
		//
		// Si no había ninguna, estamos cargando la escena
		// inicial y la inicialización debe fallar.
		return activeScene != nullptr;
	}

	// La nueva escena se ha cargado correctamente.
	// Ahora podemos descargar la anterior.
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