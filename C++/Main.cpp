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
int gScreenWidth = 640;
int gScreenHeight = 480;
SDL_Window* gGraphicsAppWindow = nullptr;
SDL_GLContext gOpenGLContext; // Changed this line

bool gQuit = false; //If true, we quit

void GetOpenGLVersionInfo() {
    std::cout << "Vendor: " <<glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " <<glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " <<glGetString(GL_VERSION ) << std::endl;
    std::cout << "Shading Language: " <<glGetString(GL_SHADING_LANGUAGE) << std::endl;
}

void InitializeProgram() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL2 could not initialize video subsystem" << std::endl;
        exit(1);
    }

    //Using OpenGL version 4.1
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    //Making the window
    //SDL_Window * SDL_CreateWindow("Renderer", 0, 0, gScreenWidth, gScreenHeight, SDL_WINDOW_OPENGL);
    gGraphicsAppWindow = SDL_CreateWindow("Renderer", 0, 0, gScreenWidth, gScreenHeight, SDL_WINDOW_OPENGL);
    if(gGraphicsAppWindow == nullptr) {
        std::cout << "SDL Window was not able to be created" << std::endl;
        exit(1);
    }

    gOpenGLContext = SDL_GL_CreateContext(gGraphicsAppWindow);

    if(gOpenGLContext == nullptr) {
        std::cout << "OpenGL context not available\n";
        exit(1);
    }
}

void Input() {
    SDL_Event e;

    while(SDL_PollEvent(&e) != 0) {
        if(e.type == SDL_QUIT) {
            std::cout << "See ya" << std::endl;
            gQuit = true;
        } else if(e.type == SDL_KEYDOWN) {
            if(e.key.keysym.sym == SDLK_ESCAPE) {
                std::cout << "See ya" << std::endl;
                gQuit = true;
            }
        }
    }
}

void PreDraw() {
     
}

void Draw() {

}

void MainLoop() {

    while(!gQuit) {
        Input();

        PreDraw();

        Draw();

        //Update the screen
        SDL_GL_SwapWindow(gGraphicsAppWindow);
    }

}

void CleanUp() {
    SDL_DestroyWindow(gGraphicsAppWindow); // Also delete the window
    SDL_Quit();
}

int main() {

    InitializeProgram();
    MainLoop();
    CleanUp();

    return 0;
}
