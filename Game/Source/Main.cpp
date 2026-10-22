#include "App.h"

#include "Defs.h"
#include "Log.h"

// NOTE: SDL redefines main function
#include "SDL/include/SDL.h"

// NOTE: Library linkage is configured in Linker Options
//#pragma comment(lib, "../Game/Source/External/SDL/libx86/SDL2.lib")
//#pragma comment(lib, "../Game/Source/External/SDL/libx86/SDL2main.lib")

#include <stdlib.h>

enum MainState
{
	CREATE = 1,
	AWAKE,
	START,
	LOOP,
	CLEAN,
	FAIL,
	EXIT
};

App* app = NULL;

int main(int argc, char* args[])
{
	DEBUG_LOG("Engine starting ...");

	MainState state = CREATE;
	int result = EXIT_FAILURE;

	while(state != EXIT)
	{
		switch(state)
		{
			// Allocate the engine --------------------------------------------
			case CREATE:
			DEBUG_LOG("CREATION PHASE ===============================");

			app = new App(argc, args);

			if(app != NULL)
				state = AWAKE;
			else
				state = FAIL;

			break;

			// Awake all modules -----------------------------------------------
			case AWAKE:
			DEBUG_LOG("AWAKE PHASE ===============================");
			if(app->Awake() == true)
				state = START;
			else
			{
				DEBUG_LOG("ERROR: Awake failed");
				state = FAIL;
			}

			break;

			// Call all modules before first frame  ----------------------------
			case START:
			DEBUG_LOG("START PHASE ===============================");
			if(app->Start() == true)
			{
				state = LOOP;
				DEBUG_LOG("UPDATE PHASE ===============================");
			}
			else
			{
				state = FAIL;
				DEBUG_LOG("ERROR: Start failed");
			}
			break;

			// Loop all modules until we are asked to leave ---------------------
			case LOOP:
			if(app->Update() == false)
				state = CLEAN;
			break;

			// Cleanup allocated memory -----------------------------------------
			case CLEAN:
			DEBUG_LOG("CLEANUP PHASE ===============================");
			if(app->CleanUp() == true)
			{
				RELEASE(app);
				result = EXIT_SUCCESS;
				state = EXIT;
			}
			else
				state = FAIL;

			break;

			// Exit with errors and shame ---------------------------------------
			case FAIL:
			DEBUG_LOG("Exiting with errors :(");
			result = EXIT_FAILURE;
			state = EXIT;
			break;
		}
	}

	DEBUG_LOG("... Bye! :)\n");

	// Dump memory leaks
	return result;
}