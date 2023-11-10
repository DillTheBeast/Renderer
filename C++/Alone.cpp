#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Compile with: g++ -std=c++11 -o alone alone.cpp ./src/glad.c -I ./include/ -I /opt/homebrew/Cellar/glm/0.9.9.8/include/ -I /opt/homebrew/opt/sdl2/include -L /opt/homebrew/opt/sdl2/lib -lSDL2

//Global Variables
//Screen dimensions
int gScreenWidth = 640;
int gScreenHeight = 480;
SDL_Window* gGraphicsAppWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr; // Changed this line
float gRotationAngleX = 0.0f;
float gRotationAngleY = 0.0f;


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
    "   color = vec4(0, 1, 0, 1.0);\n" // Change the color to green
    "}\n";


const std::string VertexShaderSource = 
    "#version 410 core\n"
    "in vec4 position;\n"
    "uniform float rotationX;\n"
    "uniform float rotationY;\n"
    "void main()\n"
    "{\n"
    "   float angleX = radians(rotationX);\n"
    "   float angleY = radians(rotationY);\n"
    "   mat4 rotationMatrixX = mat4(1.0);\n"
    "   mat4 rotationMatrixY = mat4(1.0);\n"
    "   rotationMatrixX = mat4(\n"
    "       1,   0,          0, 0,\n"
    "       0, cos(angleX), -sin(angleX), 0,\n"
    "       0, sin(angleX),  cos(angleX), 0,\n"
    "       0,   0,          0, 1\n"
    "   );\n"
    "   rotationMatrixY = mat4(\n"
    "       cos(angleY), 0, sin(angleY), 0,\n"
    "       0,           1, 0,          0,\n"
    "      -sin(angleY), 0, cos(angleY), 0,\n"
    "       0,           0, 0,          1\n"
    "   );\n"
    "   mat4 rotationMatrix = rotationMatrixY * rotationMatrixX;\n"
    "   gl_Position = rotationMatrix * position;\n"
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

    GLuint rotationMatrixLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "rotationMatrix");
    GLuint rotationXLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "rotationX");
    GLuint rotationYLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "rotationY");

    glUseProgram(gGraphicsPipelineShaderProgram);
    glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
    glUniform1f(rotationXLocation, gRotationAngleX);
    glUniform1f(rotationYLocation, gRotationAngleY);
}


void GetOpenGLVersionInfo() {
    std::cout << "Vendor: " <<glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " <<glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " <<glGetString(GL_VERSION ) << std::endl;
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void VertexSpecification() {

    //Lives on the CPU
    //For Triangle
    // const std::vector<GLfloat> vertexPositions {
    //     -0.8f, -0.8f, 0.0f, // Left Vertex Position
    //     0.8f, -0.8f, 0.0f,  // Right Vertex Position
    //     0.0f, 0.8f, 0.0f   // Top Vertex Position
    // };
    //For Square
    const std::vector<GLfloat> vertexPositions {
        -0.5f, -0.5f, 0.0f, // Bottom-left vertex
        0.5f, -0.5f, 0.0f, // Bottom-right vertex
        0.5f,  0.5f, 0.0f, // Top-right vertex
        -0.5f,  0.5f, 0.0f  // Top-left vertex
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

    //Actual drawing
    //glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

}

void MainLoop() {
    while(!gQuit) {
        Input();

        gRotationAngleX += 0.01f; // Adjust the speed of rotation around the x-axis as needed
        gRotationAngleY += 0.01f; // Adjust the speed of rotation around the y-axis as needed

        // Update the rotation uniforms
        GLuint rotationXLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "rotationX");
        GLuint rotationYLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "rotationY");
        glUseProgram(gGraphicsPipelineShaderProgram);
        glUniform1f(rotationXLocation, gRotationAngleX);
        glUniform1f(rotationYLocation, gRotationAngleY);

        PreDraw();

        Draw();

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