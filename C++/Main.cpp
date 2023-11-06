// Need 3 stages for graphics application
// 1. Initialize
// 2. Main Loop
    // Handle Input
    // Updates
    // Render
// 3, Clean up

#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init Error: %s", SDL_GetError());
        return 1;
    }

    SDL_Quit();
    return 0;
}



