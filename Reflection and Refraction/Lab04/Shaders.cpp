#include "Shaders.h"
using namespace std;

// Shader Functions- click on + to expand
#pragma region SHADER_FUNCTIONS

std::string Shader::readShaderSource(const std::string& fileName)
{
    std::ifstream file(fileName.c_str());
    if (file.fail()) {
        cout << "error loading shader called " << fileName;
        exit(1);
    }

    std::stringstream stream;
    stream << file.rdbuf();
    file.close();

    return stream.str();
}

void Shader::AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType)
{
    // create a shader object
    ShaderObj = glCreateShader(ShaderType);

    if (ShaderObj == 0) {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(0);
    }
    std::string outShader = readShaderSource(pShaderText);
    const char* pShaderSource = outShader.c_str();

    // Bind the source code to the shader, this happens before compilation
    glShaderSource(ShaderObj, 1, (const GLchar**)&pShaderSource, NULL);
    // compile the shader and check for errors
    glCompileShader(ShaderObj);
    GLint success;
    // check for shader related errors using glGetShaderiv
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }
    // Attach the compiled shader object to the program object
    glAttachShader(ShaderProgram, ShaderObj);
}

GLuint Shader::CompileShaders(int num)
{

    shaderProgramID = glCreateProgram();
    if (shaderProgramID == 0) {
        fprintf(stderr, "Error creating shader program\n");
        exit(1);
    }
    if (num == 1) {
        AddShader(shaderProgramID, "Shaders\\Assignment 1\\skyboxvertex.txt", GL_VERTEX_SHADER);
        AddShader(shaderProgramID, "Shaders\\Assignment 1\\skyboxfragment.txt", GL_FRAGMENT_SHADER);
    }
    if (num == 2) {
        AddShader(shaderProgramID, "Shaders\\Assignment 1\\lightvertex.txt", GL_VERTEX_SHADER);
        AddShader(shaderProgramID, "Shaders\\Assignment 1\\lightfragment.txt", GL_FRAGMENT_SHADER);
    }
    if (num == 3) {
        AddShader(shaderProgramID, "Shaders\\Assignment 2\\VertexTransmittance.txt", GL_VERTEX_SHADER);
        AddShader(shaderProgramID, "Shaders\\Assignment 2\\FragmentTransmittance.txt", GL_FRAGMENT_SHADER);
    }
    GLint Success = 0;
    GLchar ErrorLog[1024] = { 0 };


    // After compiling all shader objects and attaching them to the program, we can finally link it
    glLinkProgram(shaderProgramID);
    // check for program related errors using glGetProgramiv
    glGetProgramiv(shaderProgramID, GL_LINK_STATUS, &Success);
    if (Success == 0) {
        glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    // program has been successfully linked but needs to be validated to check whether the program can execute given the current pipeline state
    glValidateProgram(shaderProgramID);
    // check for program related errors using glGetProgramiv
    glGetProgramiv(shaderProgramID, GL_VALIDATE_STATUS, &Success);
    if (!Success) {
        glGetProgramInfoLog(shaderProgramID, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }
    // Finally, use the linked shader program
    // Note: this program will stay in effect for all draw calls until you replace it with another or explicitly disable its use
    glUseProgram(shaderProgramID);

    return shaderProgramID;
}

#pragma endregion SHADER_FUNCTIONS
