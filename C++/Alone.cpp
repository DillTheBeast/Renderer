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

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
)";

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        return -1;
    }

    // Set GLFW to not create an OpenGL context (we'll use GLAD for that)
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(800, 600, "Spinning Cube", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return -1;
    }

    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Link shaders into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Set up vertex data and buffers
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
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4,
        0, 1, 5, 5, 4, 0,
        2, 3, 7, 7, 6, 2,
        0, 3, 7, 7, 4, 0,
        1, 2, 6, 6, 5, 1
    };

    GLuint VAO, VBO, EBO;
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

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Set the callback function for window resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Process input
        processInput(window);

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader program
        glUseProgram(shaderProgram);

        // Set up transformations
        float time = glfwGetTime();
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(45.0f) * time, glm::vec3(0.5f, 1.0f, 0.0f));
        glm::mat4 view = glm::lookAt(glm::vec3(3.0f, 3.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

        GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Draw the cube
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }

    // Clean up
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
