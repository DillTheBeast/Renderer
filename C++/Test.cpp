// //Compile with: g++ -std=c++11 -o ./Test Test.cpp ./src/glad.c EBO.cpp VAO.cpp VBO.cpp ShaderClass.cpp -I./include/ -I./glfw-3.3.8/include -L./glfw-3.3.8/src -lglfw3 -ldl -lpthread -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "ShaderClass.h"
#include "VBO.h"
#include "VAO.h"
#include "EBO.h"

 //Vertices coordinates
GLfloat vertices[] = {
    -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, //Lower left
    0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, //Lower right
    0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, //Upper corner
    -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, //Inner left 
    0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, //Inner right
    0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f, //Inner down
};

GLuint indices[] = {
        0, 3, 5, //Lower left triangle
        3, 2, 4, //Lower right triangle
        5, 4, 1 // Upper triangle
};

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configure GLFW to use OpenGL 3.2 Core Profile with forward compatibility
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 800, "Renderer", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the OpenGL context of the window current
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Print the OpenGL version
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

    Shader shaderProgram("Shaders/default.vert", "Shaders/default.frag"); 

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkVBO(VBO1, 0);
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //Tell OpenGL what shader program we are using
        shaderProgram.Activate();
        // Bind the VAO
        VAO1.Bind();
        // Draw the triangle
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

        // Swap the front and back buffers
        glfwSwapBuffers(window);

        //Take care of GLFW events
        glfwPollEvents();
    }

    // Delete VAO and VBO
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();
    // Delete shader program
    glfwDestroyWindow(window);
    // Terminate GLFW
    glfwTerminate();
    return 0;
}