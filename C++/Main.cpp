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
    //Making the window
    //SDL_Window * SDL_CreateWindow("Renderer", 0, 0, gScreenWidth, gScreenHeight, SDL_WINDOW_OPENGL);
    gGraphicsAppWindow = SDL_CreateWindow("Renderer", 0, 0, gScreenWidth, gScreenHeight, SDL_WINDOW_OPENGL);
    if(gGraphicsAppWindow == nullptr) {
        str::cout << "SDL Window was not able to be created" << std::endl;
        exit(1);
    }

    gOpenGLContext = SDL_GL_CreateContext(gGraphicsAppWindow);

    if(gOpenGLContext == nullptr) {
        std::cout << "OpenGL context not available\n";
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
