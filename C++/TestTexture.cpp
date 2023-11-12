//COMPILE WITH: g++ -std=c++11 -o TestTexture TestTexture.cpp ./src/glad.c -I ./include/ -I /opt/homebrew/Cellar/glm/0.9.9.8/include/ -I /opt/homebrew/opt/sdl2/include -L /opt/homebrew/opt/sdl2/lib -lSDL2 -framework OpenGL
#define STB_IMAGE_IMPLEMENTATION
#include "./stb-master/stb_image.h"
#include <SDL2/SDL.h>
#include "glad/glad.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <utility>

// Vertex shader source code
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec4 aColor;
    layout (location = 2) in vec2 aTexCoord;

    out vec4 FragColor;
    out vec2 TexCoord;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * view * model * vec4(aPos, 1.0f);
        FragColor = aColor;
        TexCoord = aTexCoord;
    }
)";

// Fragment shader source code
const char* fragmentShaderSource = R"(
    #version 330 core
    in vec4 FragColor;
    in vec2 TexCoord;

    out vec4 FragColorOut;
    
    uniform sampler2D texture1;
    // Add more texture samplers if you have multiple textures

    void main()
    {
        FragColorOut = texture(texture1, TexCoord) * FragColor;
    }
)";

//Global variables
bool pause = false;
bool shape = false;
bool background = false;
float rotationSpeed = 0.02f;
float blueColor[] = {0.0f, 0.0f, 1.0f, 1.0f};
float redColor[] = {1.0f, 0.0f, 0.0f, 1.0f};
float greenColor[] = {0.0f, 1.0f, 0.0f, 1.0f};
float blackColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
float whiteColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
float grayColor[] = {0.5f, 0.5f, 0.5f, 1.0f};
float cyanColor[] = {0.0f, 1.0f, 1.0f, 1.0f};
float purpleColor[] = {0.5f, 0.0f, 0.5f, 1.0f};
float pinkColor[] = {1.0f, 0.75f, 0.8f, 1.0f};
float orangeColor[] = {1.0f, 0.5f, 0.0f, 1.0f};
float* backgroundChosen = blueColor; // Uses a pointer
float* shapeChosen = greenColor; // Uses a pointer

float cubeCenterX = 0.0f;
float cubeCenterY = 0.0f;
float cubeCenterZ = 0.0f;

glm::vec3 calculateCubeCenter(const glm::mat4& model) {
    // The original vertices of the cube
    glm::vec3 minVertex(-0.5f, -0.5f, -0.5f);
    glm::vec3 maxVertex(0.5f, 0.5f, 0.5f);

    // Transform the vertices by the model matrix
    glm::vec4 minTransformed = model * glm::vec4(minVertex, 1.0f);
    glm::vec4 maxTransformed = model * glm::vec4(maxVertex, 1.0f);

    // Calculate the center point in world space
    glm::vec3 center = 0.5f * glm::vec3(minTransformed + maxTransformed);

    // Apply the position adjustments
    center.x += cubeCenterX;
    center.y += cubeCenterY;
    center.z += cubeCenterZ;

    return center;
}

void colorCheck(bool background, bool shape, float*& backgroundChosen, float*& shapeChosen, float color[]) {
    if (background) {
        backgroundChosen = color;
    } else if (shape) {
        shapeChosen = color;
    }
}

int main() {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create an SDL window
    SDL_Window* window = SDL_CreateWindow("Spinning Cube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    if (!window) {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Create an OpenGL context
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        std::cerr << "Failed to create OpenGL context: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Load OpenGL functions with GLAD
    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Vertex and fragment shader creation and linking
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Vertex data for the cube
    float vertices[] = {
        // Front face
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,

        // Right face
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,

        // Left face
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f,

        // Top face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,

        // Bottom face
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f
    };

    // Indices for drawing the cube
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
        4, 5, 6,
        6, 7, 4,
        0, 3, 7,
        7, 4, 0,
        1, 2, 6,
        6, 5, 1,
        0, 1, 5,
        5, 4, 0,
        2, 3, 7,
        7, 6, 2
    };

    // Texture objects
    unsigned int texture[6];

    // Vertex Array Object (VAO), Vertex Buffer Object (VBO), and Element Buffer Object (EBO) creation
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind the VAO
    glBindVertexArray(VAO);

    // Bind and fill the VBO with vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Unbind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Load texture images
    int width, height, nrChannels;
    unsigned char *textureData[6];
    for (int i = 0; i < 6; ++i) {
        textureData[i] = stbi_load(("path_to_texture_image" + std::to_string(i + 1) + ".jpg").c_str(), &width, &height, &nrChannels, 0);
    }


    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Model, view, and projection matrices
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // Use the shader program
    glUseProgram(shaderProgram);

    // Get uniform locations
    unsigned int shapeColorLoc = glGetUniformLocation(shaderProgram, "shapeColor");
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

    // Set the view and projection matrices
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    // Set the initial color of the shape after OpenGL context creation
    shapeChosen = greenColor;

    // Main loop
    bool quit = false;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                std::cout << "See ya" << std::endl;
                quit = true;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_ESCAPE) {
                    std::cout << "See ya" << std::endl;
                    quit = true;
                } 
                if (event.key.keysym.sym == SDLK_SPACE) {
                    pause = !pause;
                }

                // Handle color change keys
                if (event.key.keysym.sym == SDLK_MINUS) {
                    shape = true;
                    background = false;
                } else if (event.key.keysym.sym == SDLK_EQUALS) {
                    background = true;
                    shape = false;
                } else if (event.key.keysym.sym == SDLK_1) {
                    colorCheck(background, shape, backgroundChosen, shapeChosen, blackColor);
                } else if (event.key.keysym.sym == SDLK_2) {
                    colorCheck(background, shape, backgroundChosen, shapeChosen, whiteColor);
                } else if (event.key.keysym.sym == SDLK_3) {
                    colorCheck(background, shape, backgroundChosen, shapeChosen, blueColor);
                } else if (event.key.keysym.sym == SDLK_4) {
                    colorCheck(background, shape, backgroundChosen, shapeChosen, redColor);
                } else if (event.key.keysym.sym == SDLK_5) {
                    colorCheck(background, shape, backgroundChosen, shapeChosen, greenColor);
                } else if (event.key.keysym.sym == SDLK_6) {
                    colorCheck(background, shape, backgroundChosen, shapeChosen, grayColor);
                } else if (event.key.keysym.sym == SDLK_7) {
                    colorCheck(background, shape, backgroundChosen, shapeChosen, cyanColor);
                } else if (event.key.keysym.sym == SDLK_8) {
                    colorCheck(background, shape, backgroundChosen, shapeChosen, orangeColor);
                } else if (event.key.keysym.sym == SDLK_9) {
                    colorCheck(background, shape, backgroundChosen, shapeChosen, purpleColor);
                } else if (event.key.keysym.sym == SDLK_0) {
                    colorCheck(background, shape, backgroundChosen, shapeChosen, pinkColor);
                } 
                //Handle the scale of the shape
                else if (event.key.keysym.sym == SDLK_UP) {
                    model = glm::scale(model, glm::vec3(1.1f, 1.1f, 1.1f));
                } else if (event.key.keysym.sym == SDLK_DOWN) {
                    model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
                } 
                //Handle the speed of the shape
                else if (event.key.keysym.sym == SDLK_RIGHT) {
                    rotationSpeed += 0.01f;
                } else if (event.key.keysym.sym == SDLK_LEFT) {
                    if (rotationSpeed != 0.0f) {
                        rotationSpeed -= 0.01f;
                    }
                }

                //Handle the positon of the shape
                else if (event.key.keysym.sym == SDLK_w) {
                    cubeCenterY += 0.1f;
                } else if (event.key.keysym.sym == SDLK_s) {
                    cubeCenterY -= 0.1f;
                } else if (event.key.keysym.sym == SDLK_a) {
                    cubeCenterX -= 0.1f;
                } else if (event.key.keysym.sym == SDLK_d) {
                    cubeCenterX += 0.1f;
                }
            }
        }

        glClearColor(backgroundChosen[0], backgroundChosen[1], backgroundChosen[2], backgroundChosen[3]);

        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set up the model matrix and rotate the cube
        // Inside the main loop
        if (!pause) {
            // Rotate the cube
            model = glm::rotate(model, glm::radians(rotationSpeed), glm::vec3(0.5f, 1.0f, 0.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        }


        // Set the chosen color
        glUseProgram(shaderProgram);
        glUniform4fv(shapeColorLoc, 1, shapeChosen);

        // Draw the cube
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // Swap the front and back buffers
        SDL_GL_SwapWindow(window);
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}