#include "ShaderClass.h"
#include <fstream>
#include <iostream>

std::string get_file_contents(const char* filename) {
    // Open the file
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        throw(errno);
    }

    // Seek to the end to get the file size
    in.seekg(0, std::ios::end);
    std::streamsize size = in.tellg();

    // Check if the file is empty
    if (size <= 0) {
        std::cerr << "Error: File is empty: " << filename << std::endl;
        throw(errno);
    }

    // Allocate a buffer to hold the file content
    std::string contents(size, '\0');

    // Seek back to the beginning and read the file content
    in.seekg(0, std::ios::beg);
    if (!in.read(&contents[0], size)) {
        std::cerr << "Error reading file: " << filename << std::endl;
        throw(errno);
    }

    // Close the file
    in.close();

    std::cout << "Successfully read file: " << filename << ", Size: " << size << " bytes" << std::endl;

    return contents;
}


Shader::Shader(const char* vertexFile, const char* fragmentFile) {
    std::string vertexCode = get_file_contents(vertexFile);
    std::string fragmentCode = get_file_contents(fragmentFile);

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    //Create vertex shader object + get reference 
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //Attach vertex shader source to vertex shader object
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    //Compile vertex shader to machine code
    glCompileShader(vertexShader);

    //Create fragment shader object + get reference 
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //Attach fragment shader source to fragment shader object
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    //Compile fragment shader to machine code
    glCompileShader(fragmentShader);

    //Create shader program Object + get reference
    ID = glCreateProgram();
    //Attach Vertex + Fragment Shaders to Shader Program
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    //Wrap up + Link all shaders into Shader Program
    glLinkProgram(ID);

    //Delete now useless Vertex + Fragment Shader Objects
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Activate() {
    glUseProgram(ID);
}

void Shader::Delete() {
    glDeleteProgram(ID);
}
