// // Compile with: g++ -std=c++11 -o alone alone.cpp glad.c -I/opt/homebrew/Cellar/sdl2/2.28.5/include -I/System/Volumes/Data/opt/homebrew/include -L/opt/homebrew/opt/sdl2/lib -lSDL2 -framework OpenGL 
// #include <SDL2/SDL.h>
// #include <glad/glad.h>
// #include <vector>

// // Class to represent a cube in 3D space
// class Cube {
// private:
//     std::vector<SDL_Point> cubePoints; // List of cube vertices in 3D
//     std::vector<SDL_Point> cubeProjectedPoints; // List of projected points (2D)
//     std::vector<std::vector<int>> cubeFaces; // List of faces, where each face is a list of vertex indices

// public:
//     Cube() {
//         // Define faces using vertex indices
//         cubeFaces = {
//             {0, 1, 2, 3},  // front face
//             {4, 5, 6, 7},  // back face
//             {0, 1, 5, 4},  // bottom face
//             {3, 2, 6, 7},  // top face
//             {0, 3, 7, 4},  // left face
//             {1, 2, 6, 5}   // right face
//         };
//     }

//     // Define cube vertices
//     void appendCubePoints() {
//         cubePoints.push_back({-1, -1});
//         cubePoints.push_back({1, -1});
//         cubePoints.push_back({1, 1});
//         cubePoints.push_back({-1, 1});
//         cubePoints.push_back({-1, -1});
//         cubePoints.push_back({1, -1});
//         cubePoints.push_back({1, 1});
//         cubePoints.push_back({-1, 1});
//     }

//     // Function to draw edges and filled faces of the cube
//     void connectCubePoints(SDL_Renderer* renderer, std::vector<SDL_Point>& projectedPoints) {
//         SDL_SetRenderDrawColor(renderer, 125, 23, 34, 255);

//         // Drawing edges
//         std::vector<std::pair<int, int>> edges = {
//             {0, 1}, {1, 2}, {2, 3}, {3, 0},  // front face
//             {4, 5}, {5, 6}, {6, 7}, {7, 4},  // back face
//             {0, 4}, {1, 5}, {2, 6}, {3, 7}   // connecting edges
//         };

//         for (const auto& edge : edges) {
//             SDL_RenderDrawLine(renderer, projectedPoints[edge.first].x, projectedPoints[edge.first].y,
//                                        projectedPoints[edge.second].x, projectedPoints[edge.second].y);
//         }

//         // Drawing filled polygons for each face
//         for (const auto& face : cubeFaces) {
//             std::vector<SDL_Point> points;
//             for (int i : face) {
//                 points.push_back(projectedPoints[i]);
//             }
//             Sint16 vx[4] = {points[0].x, points[1].x, points[2].x, points[3].x};
//             Sint16 vy[4] = {points[0].y, points[1].y, points[2].y, points[3].y};
//             filledPolygonRGBA(renderer, vx, vy, 4, 125, 23, 34, 255);
//         }

//         // Connecting points within each face
//         for (int p = 0; p < 4; ++p) {
//             connectPoints(p, (p + 1) % 4, projectedPoints, renderer);
//             connectPoints(p + 4, ((p + 1) % 4) + 4, projectedPoints, renderer);
//             connectPoints(p, p + 4, projectedPoints, renderer);
//         }
//     }

// private:
//     // Function to draw a line between two points
//     void connectPoints(int start, int end, const std::vector<SDL_Point>& projectedPoints, SDL_Renderer* renderer) {
//         SDL_RenderDrawLine(renderer, projectedPoints[start].x, projectedPoints[start].y,
//                                    projectedPoints[end].x, projectedPoints[end].y);
//     }
// };

// int main() {
//     // Initialize SDL
//     if (SDL_Init(SDL_INIT_VIDEO) < 0) {
//         fprintf(stderr, "SDL could not initialize! SDL Error: %s\n", SDL_GetError());
//         return 1;
//     }

//     // Create a window
//     SDL_Window* window = SDL_CreateWindow("SDL Cube", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
//     if (window == nullptr) {
//         fprintf(stderr, "Window could not be created! SDL Error: %s\n", SDL_GetError());
//         return 1;
//     }

//     // Create a renderer
//     SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//     if (renderer == nullptr) {
//         fprintf(stderr, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
//         return 1;
//     }

//     // Create an instance of the Cube class
//     Cube cube;
//     cube.appendCubePoints();

//     bool quit = false;
//     while (!quit) {
//         // Handle events
//         SDL_Event e;
//         while (SDL_PollEvent(&e) != 0) {
//             if (e.type == SDL_QUIT) {
//                 quit = true;
//             }
//         }

//         // Clear the renderer
//         SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//         SDL_RenderClear(renderer);

//         // Apply projection matrix (to be implemented)

//         // Apply cube rotation (to be implemented)
        
//         // Get projected points (to be implemented)
//         std::vector<SDL_Point> projectedPoints = cube.getProjectedPoints();

//         // Connect cube points and draw
//         cube.connectCubePoints(renderer, projectedPoints);

//         // Update the renderer
//         SDL_RenderPresent(renderer);
//     }

//     // Clean up resources
//     SDL_DestroyRenderer(renderer);
//     SDL_DestroyWindow(window);
//     SDL_Quit();

//     return 0;
// }


// Compile with: g++ -std=c++11 -o main main.cpp glad.c -I ./include/ -I /opt/homebrew/opt/sdl2/include -L /opt/homebrew/opt/sdl2/lib -lSDL2

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <vector>
#include <SDL2/SDL_gfxPrimitives.h>

int gScreenWidth = 640;
int gScreenHeight = 480;
SDL_Window* gGraphicsAppWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr; 

bool gQuit = false;
GLuint gGraphicsPipelineShaderProgram = 0;
GLuint gVertexArrayObject = 0;
GLuint gVertexBufferObject = 0;

const std::string gVertexShaderSource = 
    "#version 410 core\n"
    "in vec4 position;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(position.x, position.y, position.z, position.w);\n"
    "}\n";

const std::string gFragmentShaderSource =
    "#version 410 core\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "   color = vec4(1, 0, 0.5, 1.0);\n"
    "}\n";

GLuint CompileShader(GLuint type, const std::string& source) {
    GLuint shaderObject;

    if(type == GL_VERTEX_SHADER) {
        shaderObject = glCreateShader(GL_VERTEX_SHADER);
    } else if(type == GL_FRAGMENT_SHADER) {
        shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
    } 

    const char* src = source.c_str();
    glShaderSource(shaderObject, 1, &src, nullptr);
    glCompileShader(shaderObject);

    int result;
    glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

    if(result == GL_FALSE) {
        int length;
        glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
        char* errorMessage = new char[length];
        glGetShaderInfoLog(shaderObject, length, &length, errorMessage);

        if(type == GL_VERTEX_SHADER) {
            std::cout << "ERROR: GL_VERTEX_SHADER compilation failed!\n" << errorMessage << "\n";
        } else if (type == GL_FRAGMENT_SHADER) {
            std::cout << "ERROR: GL_FRAGMENT_SHADER compilation failed!\n" << errorMessage << "\n";
        }
        delete[] errorMessage;
        glDeleteShader(shaderObject);
        return 0;
    }

    return shaderObject;
}

GLuint CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader) {
    GLuint programObject = glCreateProgram();

    GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(programObject, myVertexShader);
    glAttachShader(programObject, myFragmentShader);
    glLinkProgram(programObject);

    //Validate our program
    glValidateProgram(programObject);
    //glDetachShader, glDeletShader
    glDetachShader(programObject, myVertexShader);
    glDetachShader(programObject, myFragmentShader);

    glDeleteShader(myVertexShader);
    glDeleteShader(myFragmentShader);

    return programObject;
}

void CreateGraphicsPipeline() {
    gGraphicsPipelineShaderProgram = CreateShaderProgram(gVertexShaderSource, gFragmentShaderSource);
}

void VertexSpecification() {
    //Lives on the CPU
    const std::vector<GLfloat> vertexPositions {
        -0.8f, -0.8f, 0.0f, // Left Vertex Position
        0.8f, -0.8f, 0.0f,  // Right Vertex Position
        0.0f, 0.8f, 0.0f   // Top Vertex Position
    };

    // Start to set things up
    // on the GPU
    glGenVertexArrays(1, &gVertexArrayObject);
    glBindVertexArray(gVertexArrayObject);

    //Start generating our VBO
    //Use the & because of a c base api
    glGenBuffers(1, &gVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertexPositions.size() * sizeof(GLfloat), vertexPositions.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
}

void InitializeProgram() {
    //Lives on the CPU
    const std::vector<GLfloat> vertexPositions {
        -0.8f, -0.8f, 0.0f, // Left Vertex Position
        0.8f, -0.8f, 0.0f,  // Right Vertex Position
        0.0f, 0.8f, 0.0f   // Top Vertex Position
    };

    // Start to set things up
    // on the GPU
    glGenVertexArrays(1, &gVertexArrayObject);
    glBindVertexArray(gVertexArrayObject);

    //Start generating our VBO
    //Use the & because of a c base api
    glGenBuffers(1, &gVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertexPositions.size() * sizeof(GLfloat), vertexPositions.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    VertexSpecification();
    CreateGraphicsPipeline();
}

class Cube {
    private:
    std::vector<SDL_Point> cubePoints; // List of cube vertices in 3D
    std::vector<SDL_Point> cubeProjectedPoints; // List of projected points (2D)
    std::vector<std::vector<int>> cubeFaces; // List of faces, where each face is a list of vertex indices

public:
    Cube() {
        // Define faces using vertex indices
        cubeFaces = {
            {0, 1, 2, 3},  // front face
            {4, 5, 6, 7},  // back face
            {0, 1, 5, 4},  // bottom face
            {3, 2, 6, 7},  // top face
            {0, 3, 7, 4},  // left face
            {1, 2, 6, 5}   // right face
        };
    }

    // Define cube vertices
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

    // Function to draw edges and filled faces of the cube
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
    // Function to draw a line between two points
    void connectPoints(int start, int end, const std::vector<SDL_Point>& projectedPoints, SDL_Renderer* renderer) {
        SDL_RenderDrawLine(renderer, projectedPoints[start].x, projectedPoints[start].y,
                                projectedPoints[end].x, projectedPoints[end].y);
    }
};

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
    //Responsible for setting openGL's state
   glDisable(GL_DEPTH_TEST);  
   glDisable(GL_CULL_FACE);

   glViewport(0, 0, gScreenWidth, gScreenHeight);
   glClearColor(1.f, 1.f, 0.f, 1.f);

   glUseProgram(gGraphicsPipelineShaderProgram);
}

void Draw() {
    glBindVertexArray(gVertexArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);

    // Draw the cube
    Cube cube;
    cube.appendCubePoints();

    std::vector<SDL_Point> projectedPoints; // You'll need to implement this function in the Cube class
    cube.connectCubePoints(nullptr, projectedPoints); // Pass nullptr for renderer since we're not using it here
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
    CreateGraphicsPipeline();
    MainLoop();
    CleanUp();
    return 0;
}
