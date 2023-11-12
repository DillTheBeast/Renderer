//COMPILE WITH: g++ -std=c++11 -o TestTexture TestTexture.cpp ./src/glad.c -I ./include/ -I /opt/homebrew/Cellar/glm/0.9.9.8/include/ -I /opt/homebrew/opt/sdl2/include -L /opt/homebrew/opt/sdl2/lib -lSDL2 -framework OpenGL
#define STB_IMAGE_IMPLEMENTATION
#include "./stb-master/stb_image.h"
#include <SDL2/SDL.h>
#include "glad/glad.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec4 aColor;
    layout (location = 2) in vec2 aTexCoord; // Texture coordinates

    out vec4 FragColor;
    out vec2 TexCoord; // Output texture coordinates

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    uniform vec4 shapeColor;

    void main()
    {
        gl_Position = projection * view * model * vec4(aPos, 1.0f);
        FragColor = shapeColor;
        TexCoord = aTexCoord;
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    in vec4 FragColor;
    in vec2 TexCoord;

    out vec4 FragColorOut;

    uniform sampler2D texture1; // Declare texture uniform

    void main()
    {
        FragColorOut = texture(texture1, TexCoord) * FragColor;
    }
)";

bool pause = false;
bool shape = false;
bool background = false;
float rotationSpeed = 0.02f;

// Colors
float blueColor[] = {0.0f, 0.0f, 1.0f, 1.0f};
float redColor[] = {1.0f, 0.0f, 0.0f, 1.0f};
float greenColor[] = {0.0f, 1.0f, 0.0f, 1.0f};

float* backgroundChosen = blueColor;
float* shapeChosen = greenColor;

float cubeCenterX = 0.0f;
float cubeCenterY = 0.0f;
float cubeCenterZ = 0.0f;

void colorCheck(bool background, bool shape, float*& backgroundChosen, float*& shapeChosen, float color[]) {
    if (background) {
        backgroundChosen = color;
    } else if (shape) {
        shapeChosen = color;
    }
}

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Textured Cube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    if (!window) {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

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

    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

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

    float vertices[] = {
        // Positions           // Colors           // Texture coords
        -0.5f, -0.5f, -0.5f,     1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,      1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,      1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,     1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,     1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,      1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,      1.0f, 0.0f, 0.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,   0.5f,    1.0f, 0.0f, 0.0f,   0.0f, 1.0f
    };

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

    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glUseProgram(shaderProgram);

    unsigned int shapeColorLoc = glGetUniformLocation(shaderProgram, "shapeColor");
    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");
    unsigned int textureLoc = glGetUniformLocation(shaderProgram, "texture1");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    shapeChosen = greenColor;

    unsigned int texture1, texture2;
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char* data = stbi_load("texture1.jpeg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture1" << std::endl;
    }
    stbi_image_free(data);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glUniform1i(textureLoc, 0);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // Load and generate the second texture (similar to the first one)
    // ...

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
                    shape = true;
                    background = false;
                } else if (event.key.keysym.sym == SDLK_EQUALS) {
                    background = true;
                    shape = false;
                }
                // Handle color change keys
                // ...

                // Handle the scale of the shape
                // ...

                // Handle the speed of the shape
                // ...

                // Handle the position of the shape
                // ...
            }
        }

        glClearColor(backgroundChosen[0], backgroundChosen[1], backgroundChosen[2], backgroundChosen[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (!pause) {
            model = glm::rotate(model, glm::radians(rotationSpeed), glm::vec3(0.5f, 1.0f, 0.0f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        }

        glUseProgram(shaderProgram);
        glUniform4fv(shapeColorLoc, 1, shapeChosen);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);
    }

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
