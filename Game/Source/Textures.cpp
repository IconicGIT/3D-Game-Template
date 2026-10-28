#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "TextureLoader.h"

#include "Defs.h"
#include "Log.h"

#include "SDL_image/include/SDL_image.h"
//#pragma comment(lib, "../Game/Source/External/SDL_image/libx86/SDL2_image.lib")

Textures::Textures() : Module()
{
	name.Create("textures");
}

// Destructor
Textures::~Textures()
{}

// Called before render is available
bool Textures::Awake(pugi::xml_node& config)
{
	DEBUG_LOG("Init Image library");
	bool ret = true;

	// Load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if((init & flags) != flags)
	{
		DEBUG_LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool Textures::Start()
{
	DEBUG_LOG("start textures");
	bool ret = true;


	imageExtensionsAccepted.push_back("png");
	imageExtensionsAccepted.push_back("jpg");
	imageExtensionsAccepted.push_back("bmp");
	imageExtensionsAccepted.push_back("tga");


	return ret;
}

// Called before quitting
bool Textures::CleanUp()
{
	DEBUG_LOG("Freeing textures and Image library");
	ListItem<SDL_Texture*>* item;

	for(item = textures.start; item != NULL; item = item->next)
	{
		SDL_DestroyTexture(item->data);
	}

	textures.Clear();
	IMG_Quit();
	return true;
}

// Load new texture from file path
//SDL_Texture* const Textures::Load(const char* path)
//{
//	SDL_Texture* texture = NULL;
//	SDL_Surface* surface = IMG_Load(path);
//
//	if(surface == NULL)
//	{
//		DEBUG_LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
//	}
//	else
//	{
//		texture = LoadSurface(surface);
//		SDL_FreeSurface(surface);
//	}
//
//	return texture;
//}

// Unload texture
bool Textures::UnLoad(SDL_Texture* texture)
{
	ListItem<SDL_Texture*>* item;

	for(item = textures.start; item != NULL; item = item->next)
	{
		if(texture == item->data)
		{
			SDL_DestroyTexture(item->data);
			textures.Del(item);
			return true;
		}
	}

	return false;
}

// Translate a surface into a texture
//SDL_Texture* const Textures::LoadSurface(SDL_Surface* surface) --> crear texture de opengl
//{
//	SDL_Texture* texture = SDL_CreateTextureFromSurface(app->render->renderer, surface); 
//
//	if(texture == NULL)
//	{
//		DEBUG_LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
//	}
//	else
//	{
//		textures.Add(texture);
//	}
//
//	return texture;
//}

// Retrieve size of a texture
void Textures::GetSize(const SDL_Texture* texture, uint& width, uint& height) const
{
	SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, (int*) &width, (int*) &height);
}

bool Textures::LoadFromPath(char* fileDir)
{
	bool ret = false;
	std::string sDraggedFileDir = fileDir;


	int dotPos = sDraggedFileDir.find_last_of(".");

	std::string fileExtension = sDraggedFileDir.substr(dotPos + 1);

	int a;
	//for (std::string ext : modelExtensionsAccepted)
	//{
	//	a = strncmp(fileExtension.c_str(), ext.c_str(), fileExtension.length());
	//	if (a == 0)
	//	{
	//		LoadModel(draggedFileDir, ext, createGameobject);
	//		ret = true;

	//		return ret;
	//	}
	//}

	for (int i = 0; i < imageExtensionsAccepted.size(); i++)
	{
		a = strncmp(fileExtension.c_str(), imageExtensionsAccepted[i].c_str(), fileExtension.length());
		if (a == 0)
		{
			std::string path_s = fileDir;
			int lastBar = path_s.find_last_of("\\\"");
			if (lastBar == -1)
			{
				lastBar = path_s.find_last_of("/");
			}

			std::string name = path_s.substr(lastBar + 1);

			bool alreadyLoaded = false;

			for (Texture tCmp : allTextures)
			{
				int b = strncmp(name.c_str(), tCmp.name.c_str(), name.length());

				if (b == 0)
					alreadyLoaded = true;

				if (alreadyLoaded)
					break;

			}

			if (!alreadyLoaded)
			{
				Texture tex = LoadTexture(fileDir);

				tex.name = name;

				//Save_Texture(&tex);
				allTextures.push_back(tex);

				ret = true;
				DEBUG_LOG("Loaded Texture [%s]. [path: %s]", name.c_str(), fileDir);
			}
			else
			{
				ret = true;
				DEBUG_LOG("Texture [%s] is already loaded! [path: %s]", name.c_str(), fileDir);
			}



			return ret;
		}
	}

	DEBUG_LOG("Could not load file. File not supported (%s)", fileExtension.c_str());


	return ret;
}