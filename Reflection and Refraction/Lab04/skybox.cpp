#include "skybox.h"

void Skybox::setup(GLuint shader) {

    shaderProgramID = shader;

    // skybox VAO
    //unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // load textures
    // -------------

    vector<std::string> faces
    {
        "C:\\Users\\Ev123\\Desktop\\4th Year\\CSU44052- Computer Graphics\\Lab 8 - Final Project\\skybox\\right.jpg",
        "C:\\Users\\Ev123\\Desktop\\4th Year\\CSU44052- Computer Graphics\\Lab 8 - Final Project\\skybox\\left.jpg",
        "C:\\Users\\Ev123\\Desktop\\4th Year\\CSU44052- Computer Graphics\\Lab 8 - Final Project\\skybox\\top.jpg",
        "C:\\Users\\Ev123\\Desktop\\4th Year\\CSU44052- Computer Graphics\\Lab 8 - Final Project\\skybox\\bottom.jpg",
        "C:\\Users\\Ev123\\Desktop\\4th Year\\CSU44052- Computer Graphics\\Lab 8 - Final Project\\skybox\\front.jpg",
        "C:\\Users\\Ev123\\Desktop\\4th Year\\CSU44052- Computer Graphics\\Lab 8 - Final Project\\skybox\\back.jpg"
    };

    cubemapTexture = loadCubemap(faces);

}


//from::https://github.com/JoeyDeVries/LearnOpenGL/blob/master/src/4.advanced_opengl/6.1.cubemaps_skybox/cubemaps_skybox.cpp
unsigned int Skybox::loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    //for (unsigned int i = 0; i < faces.size(); i++)
    for (unsigned int i = 0; i < 6; i++)
    {
        unsigned char* data = NULL;
        if (i == 0) {
            data = stbi_load(RIGHT, &width, &height, &nrChannels, 0);
        }
        if (i == 1) {
            data = stbi_load(LEFT, &width, &height, &nrChannels, 0);
        }
        if (i == 2) {
            data = stbi_load(FRONT, &width, &height, &nrChannels, 0);
        }
        if (i == 3) {
            data = stbi_load(BACK, &width, &height, &nrChannels, 0);
        }
        if (i == 4) {
            data = stbi_load(TOP, &width, &height, &nrChannels, 0);
        }
        if (i == 5) {
            data = stbi_load(BOTTOM, &width, &height, &nrChannels, 0);
        }
        //unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
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
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindTexture(textureID, 0);

    return textureID;
}