#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Camera3D.h"
#include "Window.h"
#include "Scene.h"

#include "Defs.h"
#include "Log.h"
#include "glew.h"



Scene::Scene() : Module()
{
	name.Create("scene");
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	DEBUG_LOG("Loading Scene");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool Scene::Start()
{
	//img = app->tex->Load("Assets/Textures/test.png");
	//app->audio->PlayMusic("Assets/Audio/Music/music_spy.ogg");

	app->tex->LoadFromPath("Assets/Textures/checker_pattern.png");
	app->tex->LoadFromPath("Assets/Textures/Baker_house.png");


	tempShader = std::make_shared<Shader>(
		"Assets/Shaders/default.vertex",
		"Assets/Shaders/default.fragment"
	);

	if (!tempShader->IsValid())
	{
		DEBUG_LOG("Default shader is invalid");
		return false;
	}

	tempModel = std::make_unique<Model>(
		"Assets/Models/baker_house.fbx",
		app
	);

	tempModel->SetShaderForAllMaterials(
		tempShader
	);


	//Testing materials
	std::shared_ptr<Material> material =
		tempModel->GetMaterial(1);

	if (material)
	{
		material->transparent = true;
		material->depthWrite = false;
		material->tint =
			vec4(1.0f, 0.3f, 0.3f, 0.5f);
	}



	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{


	const mat4x4 modelMatrix = IdentityMatrix;

	tempModel->Draw(
		*app->render,
		modelMatrix
	);


	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	DEBUG_LOG("Freeing scene");

	tempModel.reset();
	tempShader.reset();

	return true;
}
