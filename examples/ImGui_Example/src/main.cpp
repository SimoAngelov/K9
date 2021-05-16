
#include <iostream>
#include <MainLoop/MainLoop.h>

int main(int argc, char* args[])
{
    K9::MainLoop mainLoop;
    if (mainLoop.Init())
    {
        mainLoop.Run();
    }
    mainLoop.Shutdown();

    return 0;
}