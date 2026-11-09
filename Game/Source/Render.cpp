#include "App.h"
#include "Camera3D.h"
#include "Window.h"
#include "Render.h"

#include "Defs.h"
#include "Log.h"

#include "Glew/include/GL/glew.h"

#define VSYNC true

Render::Render() : Module()
{
	name.Create("3d renderer");
}

// Destructor
Render::~Render()
{}

// Called before render is available
bool Render::Awake(pugi::xml_node& config)
{
	DEBUG_LOG("Creating 3D Renderer Context");
	bool ret = true;

	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if(config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		DEBUG_LOG("Using vsync");
	}


	// Create OpenGL context
	glContext = SDL_GL_CreateContext(app->win->window);
	CHECK_ERROR(glContext != nullptr);

	// Init GLEW
	CHECK_ERROR(glewInit() == GLEW_OK);




	//Check for error
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		DEBUG_LOG("Error initializing OpenGL! %s\n", error);
		ret = false;
	}

	//Check for error
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		DEBUG_LOG("Error initializing OpenGL! %s\n", error);
		ret = false;
	}

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glClearDepth(1.0f);

	//Initialize clear color
	glClearColor(0.f, 0.f, 0.f, 1.f);

	//Check for error
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		DEBUG_LOG("Error initializing OpenGL! %s\n", error);
		ret = false;
	}



	GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

	//lights[0].ref = GL_LIGHT0;
	//lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
	//lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
	//lights[0].SetPos(0.0f, 0.0f, 2.5f);
	//lights[0].Init();

	GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//lights[0].Active(true);
	//glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);


	// Projection matrix for
	//OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	//GLenum error = glewInit();
	//if (GLEW_OK != error)
	//{
	//	DEBUG_DEBUG_LOG("Glew failed, Error: %s\n", glewGetErrorString(error));
	//}
	//DEBUG_DEBUG_LOG("Glew version: %s\n", glewGetString(GLEW_VERSION));


	return ret;
}

// Called before the first frame
bool Render::Start()
{
	//DEBUG_LOG("render start");
	projectionMatrix = perspective(fov, (float)app->win->width / (float)app->win->height, 0.125f, 512.0f);

	// back background

	return true;
}

// Called each loop iteration
bool Render::PreUpdate()
{
	//clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//View Modes

	if (attributes.Depth_test == true)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else glDisable(GL_DEPTH_TEST);

	if (attributes.Cull_Face == true)
	{
		glEnable(GL_CULL_FACE);
	}
	else glDisable(GL_CULL_FACE);

	if (attributes.Wireframe == true)
	{
		glPolygonMode(GL_FRONT, GL_LINE);
	}
	else glPolygonMode(GL_FRONT, GL_FILL);




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
	DEBUG_LOG("Destroying SDL render");
	SDL_GL_DeleteContext(glContext);
	return true;
}


void Render::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);
	projectionMatrix = perspective(fov, (float)width / (float)height, 0.125f, 512.0f);

}