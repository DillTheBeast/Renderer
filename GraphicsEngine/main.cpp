// main.cpp
//g++ -o GraphicsEngine main.cpp -I/opt/homebrew/Cellar/glfw/3.3.9/include -I/opt/homebrew/Cellar/glew/2.2.0_1/include -I/opt/homebrew/Cellar/glm/0.9.9.8/include -L/opt/homebrew/Cellar/glfw/3.3.9/lib -L/opt/homebrew/Cellar/glew/2.2.0_1/lib -lglfw -lGLEW -framework OpenGL

#define GLFW_INCLUDE_NONE
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Rest of the code...

// Function to load a shader from a file
GLuint loadShader(GLenum type, const char *source) {
    // Create shader object
    GLuint shader = glCreateShader(type);
    if (shader == 0) {
        std::cerr << "Failed to create shader" << std::endl;
        return 0;
    }

    // Set shader source
    glShaderSource(shader, 1, &source, NULL);

    // Compile shader
    glCompileShader(shader);

    // Check for compilation errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Shader compilation failed:\n" << infoLog << std::endl;

        // Delete shader and return 0 on failure
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Set OpenGL context version (adjust as needed)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    // Create a GLFW windowed mode window and its OpenGL context
    GLFWwindow *window = glfwCreateWindow(800, 600, "Graphics Engine", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW initialization failed!" << std::endl;
        return -1;
    }

    // Vertex Shader
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, "vertexShader.glsl");
    if (!vertexShader) {
        return -1;
    }

    // Fragment Shader
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, "fragmentShader.glsl");
    if (!fragmentShader) {
        glDeleteShader(vertexShader);
        return -1;
    }

    // Shader Program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Check for shader program linking errors
    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        return -1;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Vertex data for a simple triangle
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glUseProgram(shaderProgram);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        // Transformation: Rotate the triangle
        float angle = static_cast<float>(glfwGetTime()) * 50.0f;
        glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));

        GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
