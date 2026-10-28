#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
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

	cube = par_shapes_create_icosahedron();
	tempShader = new Shader("Assets/Shaders/default.vertex", "Assets/Shaders/default.fragment");
	tempTexture = LoadTexture("Assets/Textures/checker_pattern.png");
	mesh = new Mesh(cube->points, cube->npoints, cube->triangles, cube->ntriangles);
	mesh->textures.push_back(tempTexture);

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

	/*DrawParShapesMesh(cube);*/

	glDisable(GL_LIGHTING);
	////glDisable(GL_CULL_FACE);

	////glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glColor3f(1.0f, 0.0f, 1.0f);

	//glPushMatrix();

	//// Centra el cubo y lo coloca delante de la cámara.
	//glTranslatef(-0.5f, -0.5f, -5.0f);

	//DrawParShapesMesh(cube);

	//glPopMatrix();

	tempShader->Use();
	mesh->Draw(*tempShader);
	tempShader->Unuse();

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
