#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "ModuleTemplate.h"

#include "Defs.h"
#include "Globals.h"

ModuleTemplate::ModuleTemplate() : Module()
{
	name.Create("camera 3D");
}

// Destructor
ModuleTemplate::~ModuleTemplate()
{}

// Called before render is available
bool ModuleTemplate::Awake()
{
	LOG("Loading ModuleTemplate");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool ModuleTemplate::Start()
{

	return true;
}

// Called each loop iteration
bool ModuleTemplate::PreUpdate()
{
	LOG("Loading ModuleTemplate--------------------------------------");
	return true;
}

// Called each loop iteration
bool ModuleTemplate::Update(float dt)
{

	return true;
}

// Called each loop iteration
bool ModuleTemplate::PostUpdate()
{
	bool ret = true;

	return ret;
}

// Called before quitting
bool ModuleTemplate::CleanUp()
{
	LOG("Freeing camera 3D");

	return true;
}
