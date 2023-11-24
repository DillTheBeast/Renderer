#include "ShaderClass.h"

std::string get_file_contents(const char* filename) {
    std::ifstream in(filename, std::ios::binary);
    if(in) {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return (contents);
    }
    throw(errno);
}

// Shader::Shader(const char* vertexFile, const char* fragmentFile) {
//     std::string vertexCode = get_file_contents(vertexFile);
//     std::string fragmentCode = get_file_contents(fragmentFile);

//     const char* vertexSource = vertexCode.c_str();
//     const char* fragmentSource = fragmentCode.c_str();

//     //Create vertex shader object + get reference 
//     GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
//     //Attach vertex shader source to vertex shader object
//     glShaderSource(vertexShader, 1, &vertexSource, NULL);
//     //Compile vertex shader to machine code
//     glCompileShader(vertexShader);

//     //Create fragment shader object + get reference 
//     GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//     //Attach fragment shader source to fragment shader object
//     glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
//     //Compile fragment shader to machine code
//     glCompileShader(fragmentShader);

//     //Create shader program Object + get reference
//     ID = glCreateProgram();
//     //Attach Vertex + Fragment Shaders to Shader Program
//     glAttachShader(ID, vertexShader);
//     glAttachShader(ID, fragmentShader);
//     //Wrap up + Link all shaders into Shader Program
//     glLinkProgram(ID);

//     //Delete now useless Vertex + Fragment Shader Objects
//     glDeleteShader(vertexShader);
//     glDeleteShader(fragmentShader);
// }

// void Shader::Activate() {
//     glUseProgram(ID);
// }

// void Shader::Delete() {
//     glDeleteProgram(ID);
// }

Shader::Shader(const char* vertexFile, const char* fragmentFile) {
    std::string vertexCode = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    // Create vertex shader object + get reference 
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attach vertex shader source to vertex shader object
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    // Compile vertex shader to machine code
    glCompileShader(vertexShader);

    // Create fragment shader object + get reference 
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Attach fragment shader source to fragment shader object
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    // Compile fragment shader to machine code
    glCompileShader(fragmentShader);

    // Create shader program Object + get reference
    ID = glCreateProgram();
    // Attach Vertex + Fragment Shaders to Shader Program
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    // Wrap up + Link all shaders into Shader Program
    glLinkProgram(ID);

    // Delete now useless Vertex + Fragment Shader Objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Activate() {
    glUseProgram(ID);
}

void Shader::Delete() {
    glDeleteProgram(ID);
}