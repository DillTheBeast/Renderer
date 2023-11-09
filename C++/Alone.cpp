// Compile with: g++ -std=c++11 -o alone alone.cpp ./src/glad.c -I ./include/ -I /opt/homebrew/opt/sdl2/include -I /usr/local/include/SDL2 -L /opt/homebrew/opt/sdl2/lib -L /usr/local/lib -lSDL2 -lSDL2_gfx

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <vector>
#include <SDL2/SDL_gfxPrimitives.h>

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_Window* window = SDL_CreateWindow("OpenGL Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        fprintf(stderr, "Window could not be created! SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (context == nullptr) {
        fprintf(stderr, "OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
        return 1;
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        fprintf(stderr, "Glad could not be initialized\n");
        return 1;
    }

    bool quit = false;
    while (!quit) {
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        SDL_GL_SwapWindow(window);
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

class Cube {
private:
    std::vector<SDL_Point> cubePoints;
    std::vector<SDL_Point> cubeProjectedPoints;
    std::vector<std::vector<int>> cubeFaces;

public:
    Cube() {
        cubeFaces = {
            {0, 1, 2, 3},  // front face
            {4, 5, 6, 7},  // back face
            {0, 1, 5, 4},  // bottom face
            {3, 2, 6, 7},  // top face
            {0, 3, 7, 4},  // left face
            {1, 2, 6, 5}   // right face
        };
    }

    void appendCubePoints() {
        cubePoints.push_back({-1, -1});
        cubePoints.push_back({1, -1});
        cubePoints.push_back({1, 1});
        cubePoints.push_back({-1, 1});
        cubePoints.push_back({-1, -1});
        cubePoints.push_back({1, -1});
        cubePoints.push_back({1, 1});
        cubePoints.push_back({-1, 1});
    }

    void connectCubePoints(SDL_Renderer* renderer, std::vector<SDL_Point>& projectedPoints) {
        SDL_SetRenderDrawColor(renderer, 125, 23, 34, 255);

        // Drawing edges
        std::vector<std::pair<int, int>> edges = {
            {0, 1}, {1, 2}, {2, 3}, {3, 0},  // front face
            {4, 5}, {5, 6}, {6, 7}, {7, 4},  // back face
            {0, 4}, {1, 5}, {2, 6}, {3, 7}   // connecting edges
        };

        for (const auto& edge : edges) {
            SDL_RenderDrawLine(renderer, projectedPoints[edge.first].x, projectedPoints[edge.first].y,
                                       projectedPoints[edge.second].x, projectedPoints[edge.second].y);
        }

        // Drawing filled polygons for each face
        for (const auto& face : cubeFaces) {
            std::vector<SDL_Point> points;
            for (int i : face) {
                points.push_back(projectedPoints[i]);
            }
            Sint16 vx[4] = {points[0].x, points[1].x, points[2].x, points[3].x};
            Sint16 vy[4] = {points[0].y, points[1].y, points[2].y, points[3].y};
            filledPolygonRGBA(renderer, vx, vy, 4, 125, 23, 34, 255);
        }

        // Connecting points within each face
        for (int p = 0; p < 4; ++p) {
            connectPoints(p, (p + 1) % 4, projectedPoints, renderer);
            connectPoints(p + 4, ((p + 1) % 4) + 4, projectedPoints, renderer);
            connectPoints(p, p + 4, projectedPoints, renderer);
        }
    }

private:
    void connectPoints(int start, int end, const std::vector<SDL_Point>& projectedPoints, SDL_Renderer* renderer) {
        SDL_RenderDrawLine(renderer, projectedPoints[start].x, projectedPoints[start].y,
                                   projectedPoints[end].x, projectedPoints[end].y);
    }
};
