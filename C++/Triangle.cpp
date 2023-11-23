// //Compile with: g++ -std=c++11 -o Triangle Triangle.cpp ./src/glad.c -I./include/ -I./glfw-3.3.8/include -L./glfw-3.3.8/src -lglfw3 -ldl -lpthread -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    void main()
    {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main()
    {
        FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);
    }
)";

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

    //Create vertex shader object + get reference 
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //Attach vertex shader source to vertex shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    //Compile vertex shader to machine code
    glCompileShader(vertexShader);

    //Create fragment shader object + get reference 
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //Attach fragment shader source to fragment shader object
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    //Compile fragment shader to machine code
    glCompileShader(fragmentShader);

    //Create shader program Object + get reference
    GLuint shaderProgram = glCreateProgram();
    //Attach Vertex + Fragment Shaders to Shader Program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    //Wrap up + Link all shaders into Shader Program
    glLinkProgram(shaderProgram);

    //Delete now useless Vertex + Fragment Shader Objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //Vertices coordinates
    GLfloat vertices[] = {
        -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, //Lower left
        0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, //Lower right
        0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f //Upper corner
    };

    //Create reference containers for Vertex Array Object + Vertex Buffer Object 
    GLuint VAO, VBO;

    //Generate VAO + VBO with only 1 object each
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //Make VAO current VAO current Vertex Array Object by binding it
    glBindVertexArray(VAO);

    //Bind VBO specifying it's a GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //Introduce Vertices into VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 
    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VAO and VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Set clear color
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        //Tell OpenGL what shader program we are using
        glUseProgram(shaderProgram);
        // Bind the VAO
        glBindVertexArray(VAO); 
        // Draw the triangle
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap the front and back buffers
        glfwSwapBuffers(window);

        //Take care of GLFW events
        glfwPollEvents();
    }

    // Delete VAO and VBO
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // Delete shader program
    glDeleteProgram(shaderProgram);

    // Terminate GLFW
    glfwTerminate();

    return 0;
}