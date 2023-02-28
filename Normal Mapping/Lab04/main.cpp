#pragma warning(disable : 4305)
#pragma warning(disable : 4244)

//References:       https://learnopengl.com/Advanced-Lighting/Normal-Mapping
//                  http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/
//                  https://www.youtube.com/watch?v=JNj1A1bl7gg&t=204s&ab_channel=VictorGordan

#include "model.h"
#include <iostream>
using namespace std;
// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))


#define MONKEY "monkey3.dae"
#define SPHERE "sphere.dae"
#define BOX "Box.dae"

#define LIGHTTEXTURE "light_texture.jpg"

#define BRICKNORMAL "Brick_normal.jpg"
#define STONENORMAL "Stone_normal.jpg"
#define STONE2NORMAL "Stone2_normal.jpg"

#define RIGHT "w_right.jpg"
#define LEFT "w_left.jpg"
#define TOP "w_top.jpg"
#define BOTTOM "w_bottom.jpg"
#define FRONT "w_front.jpg"
#define BACK "w_back.jpg"

#define BOIDNUM 100


using namespace std;

int width = 1536.0;
int height = 960.0;
int tol = 100;

bool orthopersp;
bool keyStates[256];

float lastX = width / 2;
float lastY = height / 2;
float yaw = -90.0f;
float pitch = 0.0f;
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//lightsource location
GLfloat x = 0.0f;
GLfloat y = 12.0f;
GLfloat z = 0.0f;

//Matrices
mat4 movement_matrix = identity_mat4();
mat4 camera_matrix = identity_mat4();
mat4 temp_matrix = identity_mat4();
mat4 scale_matrix = identity_mat4();

//for animation of the head and tail
mat4 animation_matrix = identity_mat4();

//animation for going around the scene
mat4 animation_matrix2 = identity_mat4(); //translate(identity_mat4(), vec3(15.0f, 1.0f, 0.0f));
mat4 temp_anim = identity_mat4();
mat4 temp_anim2 = identity_mat4();

//scaling matrices
vec3 scale_vec = vec3(1.2f, 1.2f, 1.2f); //scale vector
vec3 scale_vec_down = vec3(0.7f, 0.7f, 0.7f); //scale vector
vec3 scale_vec_non_uni = vec3(1.0f, 1.5f, 1.0f); //scale vector
vec3 scale_vec_non_uni_down = vec3(1.0f, 0.7f, 1.0f); //scale vector

//initialise the shader object
Shader shader_object;
Shader shader_object2;
Shader shader_object3;

Shader shader_skybox;
Shader shader_light;
int shaderSpecifier;

//initialise the model objects
Model light1;

Model monkeymodel;
Model teapotmodel;
Model teamugmodel;
Model monkeymodel2;
Model teapotmodel2;
Model teamugmodel2;
Model monkeymodel3;
Model teapotmodel3;
Model teamugmodel3;

//genereate random xz coordinates
float posarray[400];

//for materials
glm::vec3 ambient;
glm::vec3 diffuse;
glm::vec3 specular;
float shininess;
float k_val;
float roughness;

glm::vec3 Lambient;
glm::vec3 Ldiffuse;
glm::vec3 Lspecular;

glm::vec3 Lambient2;
glm::vec3 Ldiffuse2;
glm::vec3 Lspecular2;

glm::vec3 lightColor;
glm::vec3 objectColor;

//skybox
//Skybox skybox;
unsigned int skyboxVAO, skyboxVBO;
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
vector<std::string> faces{

	RIGHT,
	LEFT,
	TOP,
	BOTTOM,
	FRONT,
	BACK
};

void setupgeneric(GLuint& shaderProgramID, glm::mat4 model, glm::mat4 view, glm::mat4 persp_proj, GLfloat spec_coef, int& matrix_location) {
	//sets up some basic values in a generic shader passed in

	matrix_location = glGetUniformLocation(shaderProgramID, "model");
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "model"), 1, GL_FALSE, &model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgramID, "proj"), 1, GL_FALSE, &persp_proj[0][0]);

	//setting location of the lightsource 
	GLint change1 = glGetUniformLocation(shaderProgramID, "light_position");
	GLfloat light1[3] = { x, y, z };
	glUniform3fv(change1, 1, &light1[0]);

	//change the specular coefficient
	GLint change3 = glGetUniformLocation(shaderProgramID, "specular_coef");
	glUniform1f(change3, spec_coef);

	GLint change4 = glGetUniformLocation(shaderProgramID, "cameraPos");
	glUniform3fv(change4, 1, &cameraPos[0]);


}

void materialproperties(GLuint& shaderProgramID) {
	//changes material properties.
	GLint change1 = glGetUniformLocation(shaderProgramID, "ambient");
	GLfloat vec1[3] = { ambient.x, ambient.y, ambient.z };
	glUniform3fv(change1, 1, &vec1[0]);

	GLint change2 = glGetUniformLocation(shaderProgramID, "diffuse");
	GLfloat vec2[3] = { diffuse.x, diffuse.y, diffuse.z };
	glUniform3fv(change2, 1, &vec2[0]);

	GLint change3 = glGetUniformLocation(shaderProgramID, "specular");
	GLfloat vec3[3] = { specular.x, specular.y, specular.z };
	glUniform3fv(change3, 1, &vec3[0]);

	GLint change4 = glGetUniformLocation(shaderProgramID, "shininess");
	glUniform1f(change4, shininess);

	GLint change5 = glGetUniformLocation(shaderProgramID, "Lambient");
	GLfloat vec5[3] = { Lambient.x, Lambient.y, Lambient.z };
	glUniform3fv(change5, 1, &vec5[0]);

	GLint change6 = glGetUniformLocation(shaderProgramID, "Ldiffuse");
	GLfloat vec6[3] = { Ldiffuse.x, Ldiffuse.y, Ldiffuse.z };
	glUniform3fv(change6, 1, &vec6[0]);

	GLint change7 = glGetUniformLocation(shaderProgramID, "Lspecular");
	GLfloat vec7[3] = { Lspecular.x, Lspecular.y, Lspecular.z };
	glUniform3fv(change7, 1, &vec7[0]);

	GLint change8 = glGetUniformLocation(shaderProgramID, "Lambient2");
	GLfloat vec8[3] = { Lambient2.x, Lambient2.y, Lambient2.z };
	glUniform3fv(change8, 1, &vec8[0]);

	GLint change9 = glGetUniformLocation(shaderProgramID, "Ldiffuse2");
	GLfloat vec9[3] = { Ldiffuse2.x, Ldiffuse2.y, Ldiffuse2.z };
	glUniform3fv(change9, 1, &vec9[0]);

	GLint change10 = glGetUniformLocation(shaderProgramID, "Lspecular2");
	GLfloat vec10[3] = { Lspecular2.x, Lspecular2.y, Lspecular2.z };
	glUniform3fv(change10, 1, &vec10[0]);

	GLint change11 = glGetUniformLocation(shaderProgramID, "lightColor");
	GLfloat vec11[3] = { lightColor.x, lightColor.y, lightColor.z };
	glUniform3fv(change11, 1, &vec11[0]);

	GLint change12 = glGetUniformLocation(shaderProgramID, "objectColor");
	GLfloat vec12[3] = { objectColor.x, objectColor.y, objectColor.z };
	glUniform3fv(change12, 1, &vec12[0]);

	GLint change13 = glGetUniformLocation(shaderProgramID, "k");
	glUniform1f(change13, k_val);

	GLint change14 = glGetUniformLocation(shaderProgramID, "roughness");
	glUniform1f(change14, roughness);

}

void display_object(Shader shader_obj, Model model_object, int matrix_location, mat4 model) {

	materialproperties(shader_obj.shaderProgramID);
	glBindTexture(GL_TEXTURE_2D, model_object.texture);
	glUseProgram(shader_obj.shaderProgramID);
	glBindVertexArray(model_object.modelvao);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, model.m);
	glDrawArrays(GL_TRIANGLES, 0, model_object.mesh_data.mPointCount);
	glBindVertexArray(0);
}

void display() {

	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//matrices
	mat4 model = identity_mat4();
	mat4 seabed_matrix = identity_mat4();
	mat4 light_matrix = identity_mat4();
	light_matrix = translate(light_matrix, vec3(x, y, z));
	seabed_matrix = translate(seabed_matrix, vec3(0.0f, -2.0f, 0.0f));


	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	glm::mat4 persp_proj = glm::perspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	if (orthopersp) {
		persp_proj = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, -100.0f, 100.0f);
	};


	//setting location of the lightsource
	GLfloat light_vec[3] = { x, y, z };
	int matrix_location;
	//Lightsources
	glActiveTexture(GL_TEXTURE0);
	glUseProgram(shader_light.shaderProgramID);
	glm::mat4 model_light = glm::mat4(1.0f);
	model_light = glm::translate(model_light, glm::vec3(x, y, z));
	matrix_location = glGetUniformLocation(shader_light.shaderProgramID, "model");
	glUniformMatrix4fv(glGetUniformLocation(shader_light.shaderProgramID, "model"), 1, GL_FALSE, &model_light[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader_light.shaderProgramID, "view"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader_light.shaderProgramID, "proj"), 1, GL_FALSE, &persp_proj[0][0]);
	GLfloat light_vec3[3] = { 1.0, 1.0, 1.0 }; //colour
	glUniform3fv(glGetUniformLocation(shader_light.shaderProgramID, "lightcolour"), 1, &light_vec3[0]);
	glBindVertexArray(light1.modelvao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, light1.texture);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, light_matrix.m);
	glDrawArrays(GL_TRIANGLES, 0, light1.mesh_data.mPointCount);
	glBindVertexArray(0);

	//setting up
	temp_anim2 = model;
	model = model * movement_matrix * scale_matrix;
	model = model * animation_matrix2;
	model = model * temp_anim;
	model = translate(model, vec3(2.0f, 0.0f, 2.0f));
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::mat4 model_shader = glm::mat4(1.0f);

	glm::vec3 greyish = glm::vec3(0.45f, 0.32f, 0.29f);
	glm::vec3 red = glm::vec3(0.9f, 0.32f, 0.29f);
	glm::vec3 orangeish = glm::vec3(0.55f, 0.33f, 0.17f);

	if (shaderSpecifier == 7) {
		glActiveTexture(GL_TEXTURE0);
		glUseProgram(shader_object2.shaderProgramID);
		// MONKEY HEAD==================================================================================================================
		objectColor = greyish;
		setupgeneric(shader_object2.shaderProgramID, model_shader, view, persp_proj, 90.0, matrix_location);
		display_object(shader_object2, monkeymodel, matrix_location, model);
		// MODEL TEAPOT==================================================================================================================
		model = translate(model, vec3(-8.0f, 0.0f, 0.0f));
		objectColor = red;
		display_object(shader_object2, teamugmodel, matrix_location, model);
		// MODEL TEAMUG==================================================================================================================
		model = translate(model, vec3(0.0f, 0.0f, -8.0f));
		objectColor = orangeish;
		display_object(shader_object2, teapotmodel, matrix_location, model);
	}
	if (shaderSpecifier == 8) {
		glActiveTexture(GL_TEXTURE0);
		glUseProgram(shader_object.shaderProgramID);
		// MONKEY HEAD==================================================================================================================
		objectColor = greyish;
		setupgeneric(shader_object.shaderProgramID, model_shader, view, persp_proj, 90.0, matrix_location);
		display_object(shader_object, monkeymodel, matrix_location, model);
		// MODEL TEAPOT==================================================================================================================
		model = translate(model, vec3(-8.0f, 0.0f, 0.0f));
		objectColor = red;
		display_object(shader_object, teamugmodel, matrix_location, model);
		// MODEL TEAMUG==================================================================================================================
		model = translate(model, vec3(0.0f, 0.0f, -8.0f));
		objectColor = orangeish;
		display_object(shader_object, teapotmodel, matrix_location, model);
	}
	if (shaderSpecifier == 9) {
		glActiveTexture(GL_TEXTURE0);
		glUseProgram(shader_object.shaderProgramID);
		// MONKEY HEAD==================================================================================================================
		objectColor = greyish;
		setupgeneric(shader_object.shaderProgramID, model_shader, view, persp_proj, 90.0, matrix_location);
		display_object(shader_object, monkeymodel3, matrix_location, model);
		// MODEL TEAPOT==================================================================================================================
		model = translate(model, vec3(-8.0f, 0.0f, 0.0f));
		objectColor = red;
		display_object(shader_object, teamugmodel3, matrix_location, model);
		// MODEL TEAMUG==================================================================================================================
		model = translate(model, vec3(0.0f, 0.0f, -8.0f));
		objectColor = orangeish;
		display_object(shader_object, teapotmodel3, matrix_location, model);
	}

	//resetting the matrices for the next fish
	temp_anim = identity_mat4();
	model = temp_anim2;





	//Skybox
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content

	glUseProgram(shader_skybox.shaderProgramID);//use the skybox shader
	view = glm::mat4(glm::mat3(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp))); // undo translation from view matrix (otherwise its just a box around origin)
	glUniformMatrix4fv(glGetUniformLocation(shader_skybox.shaderProgramID, "view"), 1, GL_FALSE, &view[0][0]); //similar to the uniformmatrix4fv above, but taking into account the view and projection
	glUniformMatrix4fv(glGetUniformLocation(shader_skybox.shaderProgramID, "projection"), 1, GL_FALSE, &persp_proj[0][0]);


	// This bit is the part that actually renders the skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0); //breaks existing vertex array binding

	glDepthFunc(GL_LESS); // set depth function back to default

	glutSwapBuffers();
}

void MoveCamera(float delta) {
	float speed = 5.0f * delta;

	if (keyStates['i']) {
		cameraPos += speed * cameraFront;
	}
	if (keyStates['j']) {
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
	}
	if (keyStates['k']) {
		cameraPos -= speed * cameraFront;
	}
	if (keyStates['l']) {
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
	}
}

void movement() {

	//model controls
	if (keyStates['w']) {
		movement_matrix = translate(movement_matrix, vec3(0.0f, 0.0f, -0.5f));
	}
	if (keyStates['a']) {
		movement_matrix = translate(movement_matrix, vec3(-0.5f, 0.0f, 0.0f));
	}
	if (keyStates['s']) {
		movement_matrix = translate(movement_matrix, vec3(0.0f, 0.0f, 0.5f));
	}
	if (keyStates['d']) {
		movement_matrix = translate(movement_matrix, vec3(0.5f, 0.0f, 0.0f));
	}
	if (keyStates['q']) {
		movement_matrix = translate(movement_matrix, vec3(0.0f, -0.5f, 0.0f));
	}
	if (keyStates['e']) {
		movement_matrix = translate(movement_matrix, vec3(0.0f, 0.5f, 0.0f));
	}

	//object rotation
	if (keyStates['x']) {
		temp_matrix = movement_matrix;
		movement_matrix = translate(identity_mat4(), vec3(0.0f, 0.0f, 0.0f));
		movement_matrix = rotate_x_deg(movement_matrix, 10.0f);
		movement_matrix = temp_matrix * movement_matrix;
	}
	if (keyStates['y']) {
		temp_matrix = movement_matrix;
		movement_matrix = translate(identity_mat4(), vec3(0.0f, 0.0f, 0.0f));
		movement_matrix = rotate_y_deg(movement_matrix, 10.0f);
		movement_matrix = temp_matrix * movement_matrix;
	}
	if (keyStates['z']) {
		temp_matrix = movement_matrix;
		movement_matrix = translate(identity_mat4(), vec3(0.0f, 0.0f, 0.0f));
		movement_matrix = rotate_z_deg(movement_matrix, 10.0f);
		movement_matrix = temp_matrix * movement_matrix;
	}

	//light control
	if (keyStates['=']) {
		x = x + 0.5f;
	}
	if (keyStates['-']) {
		x = x - 0.5f;
	}

	//scaling
	if (keyStates['3']) {
		orthopersp = false;
	}
	if (keyStates['4']) {
		orthopersp = true;
	}

	//Shaders
	if (keyStates['7']) {
		shaderSpecifier = 7;
	}
	if (keyStates['8']) {
		shaderSpecifier = 8;
	}
	if (keyStates['9']) {
		shaderSpecifier = 9;
	}
	if (keyStates['5']) {
		roughness = 0.1;
		shininess = 120;
	}
	if (keyStates['6']) {
		roughness = 0.3;
		shininess = 22;
	}
	if (keyStates['1']) {
		k_val = 0.5;
	}
	if (keyStates['2']) {
		k_val = 0.25;
	}

}

void updateScene() {

	// Wait until at least 16ms passed since start of last frame (Effectively caps framerate at ~60fps)
	static DWORD  last_time = 0;
	DWORD  curr_time = timeGetTime();
	float  delta = (curr_time - last_time) * 0.001f;
	if (delta > 0.03f)
		delta = 0.03f;
	last_time = curr_time;

	// Draw the next frame
	glutPostRedisplay();

	MoveCamera(delta);
	movement();

	animation_matrix2 = rotate_y_deg(animation_matrix2, -0.1f);
}

//from:https://github.com/JoeyDeVries/LearnOpenGL/blob/master/src/4.advanced_opengl/6.1.cubemaps_skybox/cubemaps_skybox.cpp
unsigned int loadCubemap(vector<std::string> faces) {
	//creates and binds textureID as a cubemap
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++) //loads in the vectors passed in similar to my model texture function
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	//parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void init()
{
	// Set up the shaders
	shader_skybox.shaderProgramID = shader_skybox.CompileShaders(1);
	shader_light.shaderProgramID = shader_light.CompileShaders(2);

	shader_object.shaderProgramID = shader_object.CompileShaders(0);
	shader_object2.shaderProgramID = shader_object2.CompileShaders(3);
	shader_object3.shaderProgramID = shader_object3.CompileShaders(4);

	//set up models
	//fish_body.setup(FISHBODY, FISHBODYTEXTURE, shader_object.shaderProgramID); //first bricknormal was the original texture
	monkeymodel.setup(MONKEY, STONENORMAL, BRICKNORMAL, shader_object.shaderProgramID);
	teapotmodel.setup(BOX, STONENORMAL, BRICKNORMAL, shader_object.shaderProgramID);
	teamugmodel.setup(SPHERE, STONENORMAL, BRICKNORMAL, shader_object.shaderProgramID);

	monkeymodel2.setup(MONKEY, BRICKNORMAL, BRICKNORMAL, shader_object.shaderProgramID);
	teapotmodel2.setup(BOX, BRICKNORMAL, BRICKNORMAL, shader_object.shaderProgramID);
	teamugmodel2.setup(SPHERE, BRICKNORMAL, BRICKNORMAL, shader_object.shaderProgramID);

	monkeymodel3.setup(MONKEY, STONE2NORMAL, BRICKNORMAL, shader_object.shaderProgramID);
	teapotmodel3.setup(BOX, STONE2NORMAL, BRICKNORMAL, shader_object.shaderProgramID);
	teamugmodel3.setup(SPHERE, STONE2NORMAL, BRICKNORMAL, shader_object.shaderProgramID);

	light1.setup(SPHERE, LIGHTTEXTURE, BRICKNORMAL, shader_light.shaderProgramID);

	// skybox VAO
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	cubemapTexture = loadCubemap(faces);
	shaderSpecifier = 7;
	k_val = 0.2;
	roughness = 0.1;
	shininess = 32.0f;
}

void KeyDown(unsigned char key, int x, int y) {
	keyStates[key] = true;
}
void KeyUp(unsigned char key, int x, int y) {
	keyStates[key] = false;
}

void MouseMovement(int x, int y) {

	if (x < tol || x > width - tol || y < tol || y > height - tol) {
		glutWarpPointer(width / 2, height / 2);
	}
	else {
		float xoffset = x - lastX;
		float yoffset = lastY - y;

		glutWarpPointer(lastX, lastY);

		float sensitivity = 0.1f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f) {
			pitch = 89.0f;
		}
		else if (pitch < -89.0f) {
			pitch = -89.0f;
		}

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(front);
	}
}

int main(int argc, char** argv) {

	// Set up the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutCreateWindow("Viewport Teapots");
	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);
	//glutKeyboardFunc(keypress);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);
	glutPassiveMotionFunc(MouseMovement);
	glutSetCursor(GLUT_CURSOR_NONE);

	// A call to glewInit() must be done after glut is initialized!
	glewExperimental = GL_TRUE; //for non-lab machines, this line gives better modern GL support
	GLenum res = glewInit();
	// Check for any errors
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}
	// Set up your objects and shaders
	init();
	// Begin infinite event loop
	glutMainLoop();
	return 0;
}




