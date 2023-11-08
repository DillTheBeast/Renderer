// Need 3 stages for graphics application
// 1. Initialize
// 2. Main Loop
    // Handle Input
    // Updates
    // Render
// 3, Clean up

//Compile with this: g++ -std=c++11 -o main main.cpp ./src/glad.c -I ./include/ -I /opt/homebrew/opt/sdl2/include -L /opt/homebrew/opt/sdl2/lib -lSDL2
//Run with ./main 
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <vector>

//Global Variables
//Screen dimensions
int gScreenWidth = 640;
int gScreenHeight = 480;
SDL_Window* gGraphicsAppWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr; // Changed this line

bool gQuit = false; //If true, we quit

//shader
//Stores unique id for graphics pipeline
//program objects = used for opengl draw calls
GLuint gGraphicsPipelineShaderProgram = 0;

//OpenGL objects
//Vertex Array Object (VAO)
//VAO = encapsulates all of the items needed to render an object
GLuint gVertexArrayObject = 0;
// Vertex Buffer Object (VBO)
//Stores info relating to vertices (positions, normals, textures)
GLuint gVertexBufferObject = 0;

//Vertex shader executes once per vertex, and will be in charge of
// the final position of the vertex.
// Mechanism for arranging geometry on the GPU
const std::string gVertexShaderSource = 
    "#version 410 core\n"
    "in vec4 position;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(position.x, position.y, position.z, position.w);\n"
    "}\n";
//Fragment shader
//The frahment shader executes once per frame (i.e. roughly for every pixel that will be rasterized),
//and in part determines the final color that will be sent to the screen.
const std::string gFragmentShaderSource =
    "#version 410 core\n"
    "out vec4 color;\n"
    "void main()\n"
    "{\n"
    "   color = vec4(1,0f, 0.5f, 0.0f, 1.0f);\n"
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


    return programObject;
}

void CreateGraphicsPipeline() {
    gGraphicsPipelineShaderProgram = CreateShaderProgram(gVertexShaderSource, gFragmentShaderSource);
}

void GetOpenGLVersionInfo() {
    std::cout << "Vendor: " <<glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " <<glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " <<glGetString(GL_VERSION ) << std::endl;
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void VertexSpecification() {

    //Lives on the CPU
    const std::vector<GLfloat> vertexPosition {
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
    glBufferData(GL_ARRAY_BUFFER, vertexPosition.size() * sizeof(GLfloat), vertexPosition.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);

}

//Open up a window + openGL context
void InitializeProgram() {
    //Setup SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL2 could not initialize! SDL Error: " << SDL_GetError() << "\n";
        exit(1);
    }

    //Using OpenGL version 4.1
    //Set up OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    //Request double buffer for smooth updating
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    //Making the window
    gGraphicsAppWindow = SDL_CreateWindow("Renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gScreenWidth, gScreenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    //Check if window did not create
    if(gGraphicsAppWindow == nullptr) {
        std::cout << "SDL Window was not able to be created! SDL Error: " << SDL_GetError() << "\n";
        exit(1);
    }

    //Create an OpenGL graphics Context
    gOpenGLContext = SDL_GL_CreateContext(gGraphicsAppWindow);
    if(gOpenGLContext == nullptr) {
        std::cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
        exit(1);
    }

    //Initialize the Glad Library
    if(!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        std::cout << "Glad was not initialized properly" << std::endl;
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

    glDrawArrays(GL_TRIANGLES, 0, 3);
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

//Main entry point for program 
int main() {

    //Setup graphics program
    InitializeProgram();

    //Setup geometry
    VertexSpecification();

    //Create vertex + graphics pipeline
    //Min = vertex + fragment shader
    CreateGraphicsPipeline();

    //Call the main application loop
    MainLoop();

    //Call the cleanup function when our program ends 
    CleanUp();

    return 0;
}
