#pragma once

#include <math.h>
#include <string> 
#include <vector> // STL dynamic memory.

// Assimp includes
#include <assimp/cimport.h> // scene importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations

#include "Shaders.h"

#include "stb_image.h"

using namespace std;

#pragma region SimpleTypes
struct ModelData {
	size_t mPointCount = 0;
	std::vector<glm::vec3> mVertices;
	std::vector<glm::vec3> mNormals;
	std::vector<glm::vec2> mTextureCoords;
};


class Model {
private:

public:

	// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

	GLuint loc1, loc2, loc3;
	ModelData LoadMesh(const char* file_name);
	void GenerateObjectBufferMesh(const char* mesh_name, const char* texture_name);

	GLuint shaderProgramID;
	ModelData mesh_data;
	//GLuint VAO;
	GLuint modelvao;
	GLuint texture;

	void setup(const char* mesh_name, const char* texture_name, GLuint shader);

};