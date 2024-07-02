#ifndef __CAMERA_3D__
#define __CAMERA_3D__

#include "Module.h"

struct SDL_Texture;

class Camera3D : public Module
{
public:

	Camera3D();

	// Destructor
	virtual ~Camera3D();

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