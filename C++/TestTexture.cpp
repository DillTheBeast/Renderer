//COMPILE WITH: g++ -std=c++11 -o TestTexture TestTexture.cpp ./src/glad.c -I ./include/ -I /opt/homebrew/Cellar/glm/0.9.9.8/include/ -I /opt/homebrew/opt/sdl2/include -L /opt/homebrew/opt/sdl2/lib -lSDL2 -framework OpenGL
#define STB_IMAGE_IMPLEMENTATION
#include "./stb-master/stb_image.h"

#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Global Variables
int gScreenWidth = 640;
int gScreenHeight = 480;
SDL_Window* gGraphicsAppWindow = nullptr;
SDL_GLContext gOpenGLContext = nullptr;
bool gQuit = false;
GLuint gGraphicsPipelineShaderProgram = 0;
GLuint gVertexArrayObject = 0;
GLuint gVertexBufferObject = 0;
GLuint gTextureID = 0;
// Add these global matrices
glm::mat4 modelMatrix = glm::mat4(1.0f);
glm::mat4 viewMatrix = glm::lookAt(
    glm::vec3(0.0f, 0.0f, 5.0f),
    glm::vec3(0.0f, 0.0f, 0.0f),
    glm::vec3(0.0f, 1.0f, 0.0f)
);
glm::mat4 projectionMatrix = glm::perspective(glm::radians(60.0f), (float)gScreenWidth / (float)gScreenHeight, 0.1f, 100.0f);

const std::string gVertexShaderSource = 
    "#version 410 core\n"
    "in vec4 position;\n"
    "in vec2 texCoord;\n"
    "out vec2 fragTexCoord;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * position;\n"
    "   fragTexCoord = texCoord;\n"
    "}\n";

const std::string gFragmentShaderSource =
    "#version 410 core\n"
    "in vec2 fragTexCoord;\n"
    "out vec4 color;\n"
    "uniform sampler2D textureSampler;\n"
    "void main()\n"
    "{\n"
    "   color = texture(textureSampler, fragTexCoord);\n"
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

    // Validate our program
    glValidateProgram(programObject);
    // glDetachShader, glDeleteShader
    glDetachShader(programObject, myVertexShader);
    glDetachShader(programObject, myFragmentShader);

    glDeleteShader(myVertexShader);
    glDeleteShader(myFragmentShader);

    return programObject;
}

void CreateGraphicsPipeline() {
    gGraphicsPipelineShaderProgram = CreateShaderProgram(gVertexShaderSource, gFragmentShaderSource);

    // Set the texture sampler uniform location
    GLint textureLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "textureSampler");
    glUniform1i(textureLocation, 0);
}

void GetOpenGLVersionInfo() {
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION ) << std::endl;
    std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
}

void VertexSpecification() {
    const std::vector<GLfloat> vertexPositions {
        // Triangle 1 (front)
        -1.0f, -1.0f, 1.0f,
         1.0f, -1.0f, 1.0f,
         0.0f,  1.0f, 0.0f,

        // Triangle 2 (right)
         1.0f, -1.0f, 1.0f,
         1.0f, -1.0f, -1.0f,
         0.0f,  1.0f, 0.0f,

        // Triangle 3 (left)
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         0.0f,  1.0f, 0.0f,
    };

    const std::vector<GLfloat> textureCoordinates {
        0.0f, 0.0f,  // Left Vertex
        1.0f, 0.0f,  // Right Vertex
        0.5f, 1.0f,  // Top Vertex
        0.0f, 0.0f,  // Left Vertex
        1.0f, 0.0f,  // Right Vertex
        0.5f, 1.0f,  // Top Vertex
        0.0f, 0.0f,  // Left Vertex
        1.0f, 0.0f,  // Right Vertex
        0.5f, 1.0f   // Top Vertex
    };

    glGenVertexArrays(1, &gVertexArrayObject);
    glBindVertexArray(gVertexArrayObject);

    glGenBuffers(1, &gVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, vertexPositions.size() * sizeof(GLfloat), vertexPositions.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Texture attribute
    glGenBuffers(1, &gVertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, textureCoordinates.size() * sizeof(GLfloat), textureCoordinates.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindVertexArray(0);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}


GLuint LoadTexture(const char* texturePath) {
    int width, height, numChannels;
    unsigned char* image = stbi_load(texturePath, &width, &height, &numChannels, 0);

    if (!image) {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
        exit(1);
    }

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

    // Set texture parameters (optional)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(image);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}

void InitializeProgram() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL2 could not initialize! SDL Error: " << SDL_GetError() << "\n";
        exit(1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    gGraphicsAppWindow = SDL_CreateWindow("Renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gScreenWidth, gScreenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    if(gGraphicsAppWindow == nullptr) {
        std::cout << "SDL Window was not able to be created! SDL Error: " << SDL_GetError() << "\n";
        exit(1);
    }

    gOpenGLContext = SDL_GL_CreateContext(gGraphicsAppWindow);

    if(gOpenGLContext == nullptr) {
        std::cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
        exit(1);
    }

    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        std::cout << "GLAD failed to initialize!" << std::endl;
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
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    glViewport(0, 0, gScreenWidth, gScreenHeight);
    glClearColor(1.f, 1.f, 0.f, 1.f);

    glUseProgram(gGraphicsPipelineShaderProgram);

    // Set matrix uniforms
    GLint modelLoc = glGetUniformLocation(gGraphicsPipelineShaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    GLint viewLoc = glGetUniformLocation(gGraphicsPipelineShaderProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    GLint projectionLoc = glGetUniformLocation(gGraphicsPipelineShaderProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void Draw() {
    glBindVertexArray(gVertexArrayObject);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gTextureID);

    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void MainLoop() {
    while(!gQuit) {
        Input();

        // Rotate the entire scene (camera)
        viewMatrix = glm::rotate(viewMatrix, glm::radians(0.02f), glm::vec3(0.0f, 1.0f, 0.0f));

        PreDraw();
        Draw();
        SDL_GL_SwapWindow(gGraphicsAppWindow);
    }
}

void CleanUp() {
    SDL_DestroyWindow(gGraphicsAppWindow);
    SDL_Quit();
}

int main() {
    InitializeProgram();
    VertexSpecification();
    CreateGraphicsPipeline();

    gTextureID = LoadTexture("/Users/dillonmaltese/Documents/GitHub/Renderer/C++/texture1.jpeg");

    MainLoop();
    CleanUp();

    return 0;
}
