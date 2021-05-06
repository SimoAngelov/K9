#include <iostream>

#include <SDL2/SDL.h>

// if we're compiling for iOS (iPhone/iPad)
#ifdef __IPHONEOS__
#	include <SDL2/SDL_opengles.h> // we want to use OpenGL ES
#else
#	include <SDL2/SDL_opengl.h> // otherwise we want to use OpenGL
#endif

namespace K9
{
class App
{
public:
	int Run();
	void Test()
	{
		std::cout << " testing K9::App!\n";
	}
};
} // namespace K9
