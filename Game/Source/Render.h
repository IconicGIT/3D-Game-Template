#ifndef __RENDER_H__
#define __RENDER_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"	


struct Gl_Attributes
{
	bool AmbientOclussion = false;
	bool Color_Materials = false;
	bool Cull_Face = true;
	bool Depth_test = true;
	bool Front = true;
	bool Lightning = false;
	bool Wireframe = true;
};

class Render : public Module
{
public:

	Render();

	// Destructor
	virtual ~Render();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void OnResize(int width, int height);


public:
	SDL_GLContext glContext;
	mat3x3 normalMatrix;
	mat4x4 modelMatrix, viewMatrix, projectionMatrix;
	float fov = 60.f;
	Gl_Attributes attributes;
};

#endif // __RENDER_H__