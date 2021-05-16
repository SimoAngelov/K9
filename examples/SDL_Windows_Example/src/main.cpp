//Using SDL and standard IO
#include <SDL.h>
#include <iostream>

//Screen dimension constants
static constexpr int SCREEN_WIDTH = 1280;
static constexpr int SCREEN_HEIGHT = 720;

int main(int argc, char* args[])
{
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << "\n";
	}
	else
	{
		//Create window
		window = SDL_CreateWindow("SDL Test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << "\n";
		}
		else
		{
			SDL_Event event;	 // used to store any events from the OS
			bool running = true; // used to determine if we're running the game

			while (running)
			{
				// poll for events from SDL
				while (SDL_PollEvent(&event))
				{
					// determine if the user still wants to have the window open
					// (this basically checks if the user has pressed 'X')
					running = event.type != SDL_QUIT;
				}

				//Get window surface
				screenSurface = SDL_GetWindowSurface(window);

				//Fill the surface white
				SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0x0, 0x0, 0xFF));

				//Update the surface
				SDL_UpdateWindowSurface(window);
			}



		}
	}

	//Destroy window
	SDL_DestroyWindow(window);

	//Quit SDL subsystems
	SDL_Quit();

	return 0;
}