// Need 3 stages for graphics application
// 1. Initialize
// 2. Main Loop
    // Handle Input
    // Updates
    // Render
// 3, Clean up

//Compile with this: g++ -o main main.cpp ./src/glad.c -I ./include/ -I /opt/homebrew/opt/sdl2/include -L /opt/homebrew/opt/sdl2/lib -lSDL2
//Run with ./main 
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <vector>

//Global Variables
int gScreenWidth = 640;
int gScreenHeight = 480;
SDL_Window* gGraphicsAppWindow = nullptr;
SDL_GLContext gOpenGLContext; // Changed this line

bool gQuit = false; //If true, we quit

//VAO
GLuint gVertexArrayObject = 0;
//VBO
GLuint gVertexBufferObject = 0;

void GetOpenGLVersionInfo() {
    std::cout << "Vendor: " <<glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " <<glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " <<glGetString(GL_VERSION ) << std::endl;
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void VertexSpecification() {

    //Lives on the CPU
    const std::vector<GLfloat> vertexPosition {
        //x     y     z
        -0.8f, -0.8f, 0.0f, //Vertex 1
        0.8f, -0.8f, 0.0f,  // Vertex 2
        0.0f, 0.8f, 0.0f   // Vertex 3
    };

    // Start to set things up
    // on the GPU
    gGenVertexArrays(1, &gVertexArrayObject)
    glBindVertexArray(gVertexArrayObject);

    //Start generating our VBO
    //Use the & because of a c base api
    glGenBuffer(1, &glVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertexPosition.size() * sizeof(GLfloat), vertexPosition.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);

}

void InitializeProgram() {
    //Setup SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL2 could not initialize video subsystem" << std::endl;
        exit(1);
    }

    //Using OpenGL version 4.1
    //Set up OpenGL attributes
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

    //Initialize the Glad Library
    if(!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        std::cout << "Glad was not initialized properly" << std::endl;
        exit(1);
    }

    GetOpenGLVersionInfo();
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

    VertexSpecification();

    CreateGraphicsPipeline();

    MainLoop();

    CleanUp();

    return 0;
}
