#ifndef __RENDER_H__
#define __RENDER_H__

#include "Module.h"
#include "Globals.h"
#include "glmath.h"	
#include "Mesh.h"
#include "Material.h"
#include "RenderCommand.h"
#include <vector>


struct Gl_Attributes
{
	bool AmbientOclussion = false;
	bool Color_Materials = false;
	bool Cull_Face = true;
	bool Depth_test = true;
	bool Front = true;
	bool Lightning = false;
	bool Wireframe = false;
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

	void BeginFrame();

	bool Submit(
		const std::shared_ptr<Mesh>& mesh,
		const std::shared_ptr<Material>& material,
		const mat4x4& modelMatrix
	);

	bool Flush();

public:
	SDL_GLContext glContext;
	mat3x3 normalMatrix;
	mat4x4 modelMatrix, viewMatrix, projectionMatrix;
	float fov = 60.f;
	Gl_Attributes attributes;

private:
	void ApplyMaterialState(
		const Material& material
	);

	bool DrawMeshImmediate(
		const Mesh& mesh,
		const Material& material,
		const mat4x4& modelMatrix
	);

	void SortCommands();

	std::vector<RenderCommand> opaqueCommands;
	std::vector<RenderCommand> transparentCommands;

};

#endif // __RENDER_H__