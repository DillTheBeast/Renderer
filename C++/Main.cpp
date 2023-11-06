// Need 3 stages for graphics application
// 1. Initialize
// 2. Main Loop
    // Handle Input
    // Updates
    // Render
// 3, Clean up

//Compile with this: g++ -o main main.cpp -I /opt/homebrew/opt/sdl2/include -L /opt/homebrew/opt/sdl2/lib -lSDL2

#include <SDL2/SDL.h>
#include <iostream>

//Global Variables
int gScreenHeight = 640;
int gScreenWidth = 480;
SDL_Window* gGraphicsAppWindow = nullptr;
SDL_GLContext* gOpenGLContext = nullptr;

void InitializeProgram() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL2 could not initialize video subsystem" << std::endl;
        exit(1);
    }
}

void MainLoop() {

}

void CleanUp() {
    SDL_Quit();
}

int main() {

    InitializeProgram();
    MainLoop();
    CleanUp();

    return 0;
}
