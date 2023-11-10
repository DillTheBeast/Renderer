// #include <SDL2/SDL.h>
// #include <glad/glad.h>
// #include <iostream>
// #include <vector>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>

// // Compile with: g++ -std=c++11 -o alone alone.cpp ./src/glad.c -I ./include/ -I /opt/homebrew/Cellar/glm/0.9.9.8/include/ -I /opt/homebrew/opt/sdl2/include -L /opt/homebrew/opt/sdl2/lib -lSDL2

// //Global Variables
// //Screen dimensions
// int gScreenWidth = 640;
// int gScreenHeight = 480;
// SDL_Window* gGraphicsAppWindow = nullptr;
// SDL_GLContext gOpenGLContext = nullptr; // Changed this line
// float gRotationAngleX = 0.0f;
// float gRotationAngleY = 0.0f;


// bool gQuit = false; //If true, we quit

// //shader
// //Stores unique id for graphics pipeline
// //program objects = used for opengl draw calls
// GLuint gGraphicsPipelineShaderProgram = 0;

// //OpenGL objects
// //Vertex Array Object (VAO)
// //VAO = encapsulates all of the items needed to render an object
// GLuint gVertexArrayObject = 0;
// // Vertex Buffer Object (VBO)
// //Stores info relating to vertices (positions, normals, textures)
// GLuint gVertexBufferObject = 0;

// //Vertex shader executes once per vertex, and will be in charge of
// // the final position of the vertex.
// // Mechanism for arranging geometry on the GPU
// const std::string gVertexShaderSource = 
//     "#version 410 core\n"
//     "in vec4 position;\n"
//     "void main()\n"
//     "{\n"
//     "   gl_Position = vec4(position.x, position.y, position.z, position.w);\n"
//     "}\n";
// //Fragment shader
// //The frahment shader executes once per frame (i.e. roughly for every pixel that will be rasterized),
// //and in part determines the final color that will be sent to the screen.
// const std::string gFragmentShaderSource =
//     "#version 410 core\n"
//     "out vec4 color;\n"
//     "void main()\n"
//     "{\n"
//     "   color = vec4(0, 1, 0, 1.0);\n" // Change the color to green
//     "}\n";


// const std::string VertexShaderSource = 
//     "#version 410 core\n"
//     "in vec4 position;\n"
//     "uniform float rotationX;\n"
//     "uniform float rotationY;\n"
//     "void main()\n"
//     "{\n"
//     "   float angleX = radians(rotationX);\n"
//     "   float angleY = radians(rotationY);\n"
//     "   mat4 rotationMatrixX = mat4(1.0);\n"
//     "   mat4 rotationMatrixY = mat4(1.0);\n"
//     "   rotationMatrixX = mat4(\n"
//     "       1,   0,          0, 0,\n"
//     "       0, cos(angleX), -sin(angleX), 0,\n"
//     "       0, sin(angleX),  cos(angleX), 0,\n"
//     "       0,   0,          0, 1\n"
//     "   );\n"
//     "   rotationMatrixY = mat4(\n"
//     "       cos(angleY), 0, sin(angleY), 0,\n"
//     "       0,           1, 0,          0,\n"
//     "      -sin(angleY), 0, cos(angleY), 0,\n"
//     "       0,           0, 0,          1\n"
//     "   );\n"
//     "   mat4 rotationMatrix = rotationMatrixY * rotationMatrixX;\n"
//     "   gl_Position = rotationMatrix * position;\n"
//     "}\n";


// GLuint CompileShader(GLuint type, const std::string& source) {
//     GLuint shaderObject;

//     if(type == GL_VERTEX_SHADER) {
//         shaderObject = glCreateShader(GL_VERTEX_SHADER);
//     } else if(type == GL_FRAGMENT_SHADER) {
//         shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
//     } 

//     const char* src = source.c_str();
//     glShaderSource(shaderObject, 1, &src, nullptr);
//     glCompileShader(shaderObject);

//     int result;
//     glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

//     if(result == GL_FALSE) {
//         int length;
//         glGetShaderiv(shaderObject, GL_INFO_LOG_LENGTH, &length);
//         char* errorMessage = new char[length];
//         glGetShaderInfoLog(shaderObject, length, &length, errorMessage);

//         if(type == GL_VERTEX_SHADER) {
//             std::cout << "ERROR: GL_VERTEX_SHADER compilation failed!\n" << errorMessage << "\n";
//         } else if (type == GL_FRAGMENT_SHADER) {
//             std::cout << "ERROR: GL_FRAGMENT_SHADER compilation failed!\n" << errorMessage << "\n";
//         }
//         delete[] errorMessage;
//         glDeleteShader(shaderObject);
//         return 0;
//     }

//     return shaderObject;
// }

// GLuint CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader) {
//     GLuint programObject = glCreateProgram();

//     GLuint myVertexShader = CompileShader(GL_VERTEX_SHADER, vertexShader);
//     GLuint myFragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

//     glAttachShader(programObject, myVertexShader);
//     glAttachShader(programObject, myFragmentShader);
//     glLinkProgram(programObject);

//     //Validate our program
//     glValidateProgram(programObject);
//     //glDetachShader, glDeletShader
//     glDetachShader(programObject, myVertexShader);
//     glDetachShader(programObject, myFragmentShader);

//     glDeleteShader(myVertexShader);
//     glDeleteShader(myFragmentShader);

//     return programObject;
// }

// void CreateGraphicsPipeline() {
//     gGraphicsPipelineShaderProgram = CreateShaderProgram(gVertexShaderSource, gFragmentShaderSource);

//     GLuint rotationMatrixLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "rotationMatrix");
//     GLuint rotationXLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "rotationX");
//     GLuint rotationYLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "rotationY");

//     glUseProgram(gGraphicsPipelineShaderProgram);
//     glUniformMatrix4fv(rotationMatrixLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
//     glUniform1f(rotationXLocation, gRotationAngleX);
//     glUniform1f(rotationYLocation, gRotationAngleY);
// }


// void GetOpenGLVersionInfo() {
//     std::cout << "Vendor: " <<glGetString(GL_VENDOR) << std::endl;
//     std::cout << "Renderer: " <<glGetString(GL_RENDERER) << std::endl;
//     std::cout << "Version: " <<glGetString(GL_VERSION ) << std::endl;
//     std::cout << "Shading Language: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
// }

// void VertexSpecification() {

//     //Lives on the CPU
//     //For Triangle
//     // const std::vector<GLfloat> vertexPositions {
//     //     -0.8f, -0.8f, 0.0f, // Left Vertex Position
//     //     0.8f, -0.8f, 0.0f,  // Right Vertex Position
//     //     0.0f, 0.8f, 0.0f   // Top Vertex Position
//     // };
//     //For Square
//     const std::vector<GLfloat> vertexPositions {
//         -0.5f, -0.5f, 0.0f, // Bottom-left vertex
//         0.5f, -0.5f, 0.0f, // Bottom-right vertex
//         0.5f,  0.5f, 0.0f, // Top-right vertex
//         -0.5f,  0.5f, 0.0f  // Top-left vertex
//     };


//     // Start to set things up
//     // on the GPU
//     glGenVertexArrays(1, &gVertexArrayObject);
//     glBindVertexArray(gVertexArrayObject);

//     //Start generating our VBO
//     //Use the & because of a c base api
//     glGenBuffers(1, &gVertexBufferObject);
//     glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);
//     glBufferData(GL_ARRAY_BUFFER, vertexPositions.size() * sizeof(GLfloat), vertexPositions.data(), GL_STATIC_DRAW);

//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

//     glBindVertexArray(0);
//     glDisableVertexAttribArray(0);

// }

// //Open up a window + openGL context
// void InitializeProgram() {
//     //Setup SDL
//     if(SDL_Init(SDL_INIT_VIDEO) < 0) {
//         std::cout << "SDL2 could not initialize! SDL Error: " << SDL_GetError() << "\n";
//         exit(1);
//     }

//     //Using OpenGL version 4.1
//     //Set up OpenGL attributes
//     SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
//     SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
//     SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

//     //Request double buffer for smooth updating
//     SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
//     SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

//     //Making the window
//     gGraphicsAppWindow = SDL_CreateWindow("Renderer", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gScreenWidth, gScreenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
//     //Check if window did not create
//     if(gGraphicsAppWindow == nullptr) {
//         std::cout << "SDL Window was not able to be created! SDL Error: " << SDL_GetError() << "\n";
//         exit(1);
//     }

//     //Create an OpenGL graphics Context
//     gOpenGLContext = SDL_GL_CreateContext(gGraphicsAppWindow);
//     if(gOpenGLContext == nullptr) {
//         std::cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << "\n";
//         exit(1);
//     }

//     //Initialize the Glad Library
//     if(!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
//         std::cout << "Glad was not initialized properly" << std::endl;
//         exit(1);
//     }

// }

// void Input() {
//     SDL_Event e;

//     while(SDL_PollEvent(&e) != 0) {
//         if(e.type == SDL_QUIT) {
//             std::cout << "See ya" << std::endl;
//             gQuit = true;
//         } else if(e.type == SDL_KEYDOWN) {
//             if(e.key.keysym.sym == SDLK_ESCAPE) {
//                 std::cout << "See ya" << std::endl;
//                 gQuit = true;
//             }
//         }
//     }
// }

// void PreDraw() {
//    //Responsible for setting openGL's state
//    glDisable(GL_DEPTH_TEST);  
//    glDisable(GL_CULL_FACE);

//    glViewport(0, 0, gScreenWidth, gScreenHeight);
//    glClearColor(1.f, 1.f, 0.f, 1.f);

//    glUseProgram(gGraphicsPipelineShaderProgram);
// }

// void Draw() {
//     glBindVertexArray(gVertexArrayObject);
//     glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferObject);

//     //Actual drawing
//     //glDrawArrays(GL_TRIANGLES, 0, 3);
//     glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

// }

// void MainLoop() {
//     while(!gQuit) {
//         Input();

//         gRotationAngleX += 0.01f; // Adjust the speed of rotation around the x-axis as needed
//         gRotationAngleY += 0.01f; // Adjust the speed of rotation around the y-axis as needed

//         // Update the rotation uniforms
//         GLuint rotationXLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "rotationX");
//         GLuint rotationYLocation = glGetUniformLocation(gGraphicsPipelineShaderProgram, "rotationY");
//         glUseProgram(gGraphicsPipelineShaderProgram);
//         glUniform1f(rotationXLocation, gRotationAngleX);
//         glUniform1f(rotationYLocation, gRotationAngleY);

//         PreDraw();

//         Draw();

//         SDL_GL_SwapWindow(gGraphicsAppWindow);
//     }
// }


// void CleanUp() {
//     SDL_DestroyWindow(gGraphicsAppWindow); // Also delete the window
//     SDL_Quit();
// }

// //Main entry point for program 
// int main() {

//     //Setup graphics program
//     InitializeProgram();

//     //Setup geometry
//     VertexSpecification();

//     //Create vertex + graphics pipeline
//     //Min = vertex + fragment shader
//     CreateGraphicsPipeline();

//     //Call the main application loop
//     MainLoop();

//     //Call the cleanup function when our program ends 
//     CleanUp();

//     return 0;
// }


#include <SDL2/SDL.h>
#include "glad/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    void main()
    {
        gl_Position = projection * view * model * vec4(aPos, 1.0f);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main()
    {
        FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
)";

int main() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_Window* window = SDL_CreateWindow("Spinning Cube", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    if (!window) {
        std::cerr << "Failed to create SDL window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

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
        -0.5f, -0.5f, -0.5f,
         0.5f, -0.5f, -0.5f,
         0.5f,  0.5f, -0.5f,
        -0.5f,  0.5f, -0.5f,
        -0.5f, -0.5f,  0.5f,
         0.5f, -0.5f,  0.5f,
         0.5f,  0.5f,  0.5f,
        -0.5f,  0.5f,  0.5f
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::lookAt(glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glUseProgram(shaderProgram);

    unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
    unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
    unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                SDL_DestroyWindow(window);
                SDL_Quit();
                return 0;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        model = glm::rotate(model, glm::radians(0.01f), glm::vec3(0.5f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

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
