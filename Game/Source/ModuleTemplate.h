#ifndef __MODULE_TEMPLATE__
#define __MODULE_TEMPLATE__

#include "Module.h"

struct SDL_Texture;

class ModuleTemplate : public Module
{
public:

	ModuleTemplate();

	// Destructor
	virtual ~ModuleTemplate();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

private:
	SDL_Texture* img;
};

#endif // __SCENE_H__