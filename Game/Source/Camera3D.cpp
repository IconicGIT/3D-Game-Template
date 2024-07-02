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
