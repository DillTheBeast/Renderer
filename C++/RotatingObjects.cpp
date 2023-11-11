//COMPILE WITH: g++ -std=c++11 -o RotatingObjects RotatingObjects.cpp ./src/glad.c -I ./include/ -I /opt/homebrew/Cellar/glm/0.9.9.8/include/ -I /opt/homebrew/opt/sdl2/include -L /opt/homebrew/opt/sdl2/lib -lSDL2

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
    layout (location = 1) in vec4 aColor; // Added input for color
    out vec4 FragColor; // Pass color to fragment shader

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * view * model * vec4(aPos, 1.0f);
        FragColor = aColor; // Pass color to fragment shader
    }
)";

// Fragment shader source code
const char* fragmentShaderSource = R"(
    #version 330 core
    in vec4 FragColor; // Input color from vertex shader
    out vec4 FragColorOut;

    void main()
    {
        FragColorOut = FragColor; // Output the color
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
float* shapeChosen = blackColor; // Uses a pointer

void colorCheck(bool background, bool shape, float*& backgroundChosen, float*& shapeChosen, float color[]) {
    if (!background && shape) {
        shapeChosen = color;
        std::cout << "Setting shape color: R=" << color[0] << " G=" << color[1] << " B=" << color[2] << " A=" << color[3] << std::endl;
    } else if (background) {
        backgroundChosen = color;
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
    // Positions           // Colors
        -0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,     1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,     1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,     1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,     1.0f, 0.0f, 0.0f, 1.0f,
        0.5f,  0.5f,  0.5f,     1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,   0.5f,    1.0f, 0.0f, 0.0f, 1.0f
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

    // Bind and fill the EBO with indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Unbind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Model, view, and projection matrices
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // Use the shader program
    glUseProgram(shaderProgram);

    // Get uniform locations
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

    // Set the view and projection matrices
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

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
                if (event.key.keysym.sym == SDLK_MINUS) {
                    std::cout << "TEST" << std::endl;
                    shape = true;
                    background = false;
                }
                if (event.key.keysym.sym == SDLK_EQUALS) {
                    shape = false;
                    background = true;
                }
                if (event.key.keysym.sym == SDLK_1) {
                    colorCheck(background, shape, backgroundChosen, shapeChosen, blackColor);
                }
                if (event.key.keysym.sym == SDLK_2) {
                    colorCheck(background, shape, backgroundChosen, shapeChosen, whiteColor);
                }
                if (event.key.keysym.sym == SDLK_3) {
                    colorCheck(background, shape, backgroundChosen, shapeChosen, blueColor);
                }
                if (event.key.keysym.sym == SDLK_4) {
                    colorCheck(background, shape, backgroundChosen, shapeChosen, redColor);
                }
                if (event.key.keysym.sym == SDLK_5) {
                    colorCheck(background, shape, backgroundChosen, shapeChosen, greenColor);
                }
                if (event.key.keysym.sym == SDLK_6) {
                    colorCheck(background, shape, backgroundChosen, shapeChosen, grayColor);
                }
                if (event.key.keysym.sym == SDLK_7) {
                    colorCheck(background, shape, backgroundChosen, shapeChosen, cyanColor);
                }
                if (event.key.keysym.sym == SDLK_8) {
                    colorCheck(background, shape, backgroundChosen, shapeChosen, orangeColor);
                }
                if (event.key.keysym.sym == SDLK_9) {
                    colorCheck(background, shape, backgroundChosen, shapeChosen, purpleColor);
                }
                if (event.key.keysym.sym == SDLK_0) {
                    colorCheck(background, shape, backgroundChosen, shapeChosen, pinkColor);
                }
            }
        }

        glClearColor(backgroundChosen[0], backgroundChosen[1], backgroundChosen[2], backgroundChosen[3]);

        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set up the model matrix and rotate the cube
        // Inside the main loop
        if (!pause) {
            model = glm::rotate(model, glm::radians(rotationSpeed), glm::vec3(0.5f, 1.0f, 0.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            std::cout << "Current shape color: R=" << shapeChosen[0] << " G=" << shapeChosen[1] << " B=" << shapeChosen[2] << " A=" << shapeChosen[3] << std::endl;
        }

        // Set the chosen color
        glUniform4fv(glGetUniformLocation(shaderProgram, "FragColor"), 1, shapeChosen);

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
