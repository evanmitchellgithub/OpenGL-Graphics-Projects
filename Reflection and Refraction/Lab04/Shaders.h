#pragma once
#include <windows.h>
#include <mmsystem.h>

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "maths_funcs.h" //Anton's math class
#include <string> 
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>

using namespace std;

class Shader {

private:

public:
	GLuint ShaderObj;
	GLuint shaderProgramID;
	std::string readShaderSource(const std::string& fileName);
	void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);
	GLuint CompileShaders(int num);
};