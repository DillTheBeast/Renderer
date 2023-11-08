#include <SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>

const int WIDTH = 1440;
const int HEIGHT = 845;

bool paused = false;
bool forward = false;
bool triangle = false;
bool pyramid = false;
bool cube = true;
bool hexPrism = false;
bool sphere = false;
bool moveUp = false;
bool moveDown = false;
bool moveLeft = false;
bool moveRight = false;

int scale = 96;
int circle_pos[2] = { WIDTH / 2, HEIGHT / 2 };
float angle = 0;
float angleAddition = 0.01;
int wall_thickness = 10;
float gravity = 0.5;
float bounceStop = 0.3;

GLuint vertexArrayObject;
GLuint vertexBufferObject;

class Cube {
public:
    std::vector<glm::vec3> cubePoints;
    std::vector<std::vector<int>> cubeFaces;

    Cube() {
        appendCubePoints();
        cubeFaces = {
            {0, 1, 2, 3},  // front face
            {4, 5, 6, 7},  // back face
            {0, 1, 5, 4},  // bottom face
            {3, 2, 6, 7},  // top face
            {0, 3, 7, 4},  // left face
            {1, 2, 6, 5}   // right face
        };
    }

    void appendCubePoints() {
        cubePoints.push_back(glm::vec3(-1, -1, 1));
        cubePoints.push_back(glm::vec3(1, -1, 1));
        cubePoints.push_back(glm::vec3(1, 1, 1));
        cubePoints.push_back(glm::vec3(-1, 1, 1));
        cubePoints.push_back(glm::vec3(-1, -1, -1));
        cubePoints.push_back(glm::vec3(1, -1, -1));
        cubePoints.push_back(glm::vec3(1, 1, -1));
        cubePoints.push_back(glm::vec3(-1, 1, -1));
    }

    void setupBuffers() {
        glGenVertexArrays(1, &vertexArrayObject);
        glGenBuffers(1, &vertexBufferObject);

        glBindVertexArray(vertexArrayObject);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);

        glBufferData(GL_ARRAY_BUFFER, cubePoints.size() * sizeof(glm::vec3), &cubePoints[0], GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void draw(GLuint shaderProgram, glm::mat4 model) {
        glUseProgram(shaderProgram);
        glBindVertexArray(vertexArrayObject);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

        glDrawArrays(GL_POINTS, 0, cubePoints.size());
        glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
    }

    void connectCubePoints(glm::mat4 model, GLuint shaderProgram) {
        GLuint uColorLocation = glGetUniformLocation(shaderProgram, "uColor");
        glUniform3f(uColorLocation, 0.5f, 0.0f, 0.0f);

        for (const auto& face : cubeFaces) {
            glBegin(GL_LINE_LOOP);
            for (const auto& index : face) {
                glVertex3f(cubePoints[index].x, cubePoints[index].y, cubePoints[index].z);
            }
            glEnd();
        }
    }
};

void createShaderProgram() {
    const char* vertexShaderSource = R"(
        #version 330 core
        layout (location = 0) in vec3 aPos;
        uniform mat4 model;
        void main()
        {
            gl_Position = model * vec4(aPos, 1.0);
            gl_PointSize = 5.0;
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 330 core
        out vec4 FragColor;
        uniform vec3 uColor;
        void main()
        {
            FragColor = vec4(uColor, 1.0);
        }
    )";

    GLuint vertexShader, fragmentShader;
    GLint success;
    GLchar infoLog[512];

    // Vertex Shader
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Fragment Shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Shader Program
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);
}

void render(Cube& cube, GLuint shaderProgram, glm::mat4 model) {
    cube.draw(shaderProgram, model);
    cube.connectCubePoints(model, shaderProgram);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    SDL_Window* window = SDL_CreateWindow("3D Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);
    if (!window) {
        return -1;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    createShaderProgram();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    Cube cube;
    cube.setupBuffers();

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(WIDTH), static_cast<float>(HEIGHT), 0.0f, -1.0f, 1.0f);

    while (true) {
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(circle_pos[0], circle_pos[1], 0.0f));
        model = glm::rotate(model, angle, glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(scale, scale, 1.0f));

        render(cube, shaderProgram, model);

        SDL_GL_SwapWindow(window);

        angle += angleAddition;
    }

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
