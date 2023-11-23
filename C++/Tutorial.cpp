//Compile with: g++ -std=c++11 -o Tutorial Tutorial.cpp ./src/glad.c -I ./include/ -I ./glfw-3.3.8/include -L ./glfw-3.3.8/src -lglfw3 -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main() {
    glfwInit();

    //Telling code version of glfw being used
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //Data type of window object          Width, Height, Name, Fullscreen?, Not Important 
    GLFWwindow* window = glfwCreateWindow(800, 800, "Renderer", NULL, NULL);
    if(window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    while(!glfwWindowShouldClose(window)) {
        
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
