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




void DrawParShapesMesh(const par_shapes_mesh* mesh)
{
	if (mesh == nullptr)
		return;

	glBegin(GL_TRIANGLES);

	const int indexCount = mesh->ntriangles * 3;

	for (int i = 0; i < indexCount; ++i)
	{
		const PAR_SHAPES_T index = mesh->triangles[i];

		if (mesh->normals != nullptr)
			glNormal3fv(&mesh->normals[index * 3]);

		if (mesh->tcoords != nullptr)
			glTexCoord2fv(&mesh->tcoords[index * 2]);

		glVertex3fv(&mesh->points[index * 3]);
	}

	glEnd();
}


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
	app->tex->LoadFromPath("Assets/Textures/default_texture.png");
	app->tex->LoadFromPath("Assets/Textures/Baker_house.png");

	cube = par_shapes_create_icosahedron();
	tempShader = new Shader("Assets/Shaders/default.vertex", "Assets/Shaders/default.fragment");

	if (!tempShader->IsValid()) {
		DEBUG_LOG("Default shader is invalid");
		return false;
	}

	tempTexture = LoadTexture("Assets/Textures/checker_pattern.png");
	mesh = new Mesh(cube->points, cube->npoints, cube->triangles, cube->ntriangles);
	mesh->textures.push_back(tempTexture);

	tempModel = new Model("Assets/Models/baker_house.fbx", app);
	

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


	//tempShader->Use();

	//mat4x4 proj = app->render->projectionMatrix;
	//mat4x4 view = app->camera3D->GetViewMatrix();
	//mat4x4 model = IdentityMatrix;

	//tempShader->SetMat4x4("uProjection", proj);
	//tempShader->SetMat4x4("uView", view);
	//tempShader->SetMat4x4("uModel", model);


	//tempModel->Draw(*tempShader, false);

	//tempShader->Unuse();


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

	par_shapes_free_mesh(cube);
	cube = nullptr;

	delete tempShader;
	tempShader = nullptr;

	return true;
}
