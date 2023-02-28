#pragma once
#include "Model.h"

class Skybox {
private:

public:

	// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#define RIGHT "C:\\Users\\Ev123\\Desktop\\4th Year\\CSU44052- Computer Graphics\\Lab 8 - Final Project\\skybox\\right.jpg"
#define LEFT "C:\\Users\\Ev123\\Desktop\\4th Year\\CSU44052- Computer Graphics\\Lab 8 - Final Project\\skybox\\left.jpg"
#define TOP "C:\\Users\\Ev123\\Desktop\\4th Year\\CSU44052- Computer Graphics\\Lab 8 - Final Project\\skybox\\top.jpg"
#define BOTTOM "C:\\Users\\Ev123\\Desktop\\4th Year\\CSU44052- Computer Graphics\\Lab 8 - Final Project\\skybox\\bottom.jpg"
#define FRONT "C:\\Users\\Ev123\\Desktop\\4th Year\\CSU44052- Computer Graphics\\Lab 8 - Final Project\\skybox\\front.jpg"
#define BACK "C:\\Users\\Ev123\\Desktop\\4th Year\\CSU44052- Computer Graphics\\Lab 8 - Final Project\\skybox\\back.jpg"

	GLuint shaderProgramID;
	ModelData mesh_data;
    unsigned int skyboxVAO, skyboxVBO;
	GLuint texture;
    unsigned int cubemapTexture;
    
    float skyboxVertices[108] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };


	void setup(GLuint shader);
    unsigned int loadCubemap(vector<std::string> faces);

};