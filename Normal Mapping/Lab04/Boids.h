#pragma once
#include "model.h"

class Boids {
private:

public:

	// Macro for indexing vertex buffer
	#define BUFFER_OFFSET(i) ((char *)NULL + (i))

	glm::vec3 position;
	glm::vec3 velocity;
	glm::mat4 rotation;

};