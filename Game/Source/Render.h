#ifndef __RENDER_H__
#define __RENDER_H__

#include "Module.h"

#include "Point.h"

#include "SDL/include/SDL.h"

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


public:
	SDL_GLContext glContext;
	SDL_Rect camera;
	SDL_Rect viewport;
	SDL_Color background;
};

#endif // __RENDER_H__