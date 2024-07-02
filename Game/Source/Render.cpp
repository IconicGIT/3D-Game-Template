#include "App.h"
#include "Window.h"
#include "Render.h"

#include "Defs.h"
#include "Log.h"

#include "Glew/include/GL/glew.h"

#define VSYNC true

Render::Render() : Module()
{
	name.Create("renderer");
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
Render::~Render()
{}

// Called before render is available
bool Render::Awake(pugi::xml_node& config)
{
	LOG("Create SDL OpenGL rendering context");
	bool ret = true;

	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if(config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}


	// Create OpenGL context
	glContext = SDL_GL_CreateContext(app->win->window);
	CHECK_ERROR(glContext != nullptr);

	// Init GLEW
	CHECK_ERROR(glewInit() == GLEW_OK);

	return ret;
}

// Called before the first frame
bool Render::Start()
{
	LOG("render start");
	// back background

	return true;
}

// Called each loop iteration
bool Render::PreUpdate()
{
	//clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//LOG("clear bg");
	glClearColor(0.1f, 0.f, 0.f, 1.f);
	return true;
}

bool Render::Update(float dt)
{
	
	return true;
}

bool Render::PostUpdate()
{
	//present drawn elements
	SDL_GL_SwapWindow(app->win->window);
	return true;
}

// Called before quitting
bool Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_GL_DeleteContext(glContext);
	return true;
}


