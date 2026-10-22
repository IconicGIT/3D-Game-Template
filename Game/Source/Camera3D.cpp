#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Window.h"
#include "Camera3D.h"

#include "Defs.h"
#include "Globals.h"

Camera3D::Camera3D() : Module()
{
	name.Create("camera 3D");

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 0.0f);
}

// Destructor
Camera3D::~Camera3D()
{}

// Called before render is available
bool Camera3D::Awake()
{
	LOG("Loading Camera3D");
	bool ret = true;




	return ret;
}

// Called before the first frame
bool Camera3D::Start()
{

	return true;
}

// Called each loop iteration
bool Camera3D::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Camera3D::Update(float dt)
{
	vec3 newPos(0, 0, 0);
	float speed = 20.0f * dt;
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 50.0f * dt;


	if (app->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) 
		newPos.y += speed;

	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) 
		newPos.y -= speed;

	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) 
		newPos -= Z * speed;

	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT
		&& !(app->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT || app->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_DOWN))
		newPos += Z * speed;

	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) 
		newPos -= X * speed;

	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) 
		newPos += X * speed;

	//if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	//{
	//	LookAt(vec3(0, 0, 0));
	//	if (!app->engineSystem->GetselectedGameObjects().empty())
	//	{
	//		LookAt(app->engineSystem->GetselectedGameObjects().at(app->engineSystem->GetselectedGameObjects().size() - 1)->transform->translationWorld.translation());
	//	}
	//}

	Position += newPos;

	LOG("position: %f %f %f", Position.x, Position.y, Position.z);
	//LOG("fps %f", 1.f / dt);


	return true;
}

// Called each loop iteration
bool Camera3D::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool Camera3D::CleanUp()
{
	LOG("Freeing camera 3D");

	return true;
}
