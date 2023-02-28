#include "Model.h"

#include "stb_image.h"

#pragma region MESH LOADING
/*----------------------------------------------------------------------------
MESH LOADING FUNCTION
----------------------------------------------------------------------------*/

ModelData Model::LoadMesh(const char* file_name) {
	ModelData modelData;

	/* Use assimp to read the model file, forcing it to be read as    */
	/* triangles. The second flag (aiProcess_PreTransformVertices) is */
	/* relevant if there are multiple meshes in the model file that   */
	/* are offset from the origin. This is pre-transform them so      */
	/* they're in the right position.                                 */
	const aiScene* scene = aiImportFile(
		file_name,
		//aiProcess_Triangulate | aiProcess_PreTransformVertices
		aiProcess_Triangulate | aiProcess_PreTransformVertices | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace
	);

	if (!scene) {
		fprintf(stderr, "ERROR: reading mesh %s\n", file_name);
		return modelData;
	}

	printf("  %i materials\n", scene->mNumMaterials);
	printf("  %i meshes\n", scene->mNumMeshes);
	printf("  %i textures\n", scene->mNumTextures);

	for (unsigned int m_i = 0; m_i < scene->mNumMeshes; m_i++) {
		const aiMesh* mesh = scene->mMeshes[m_i];
		printf("    %i vertices in mesh\n", mesh->mNumVertices);
		modelData.mPointCount += mesh->mNumVertices;
		//walk through each mesh vertices
		for (unsigned int v_i = 0; v_i < mesh->mNumVertices; v_i++) {
			if (mesh->HasPositions()) {
				const aiVector3D* vp = &(mesh->mVertices[v_i]);
				modelData.mVertices.push_back(glm::vec3(vp->x, vp->y, vp->z));
			}
			if (mesh->HasNormals()) {
				const aiVector3D* vn = &(mesh->mNormals[v_i]);
				modelData.mNormals.push_back(glm::vec3(vn->x, vn->y, vn->z));
			}
			if (mesh->HasTextureCoords(0)) {
				const aiVector3D* vt = &(mesh->mTextureCoords[0][v_i]);
				modelData.mTextureCoords.push_back(glm::vec2(vt->x, vt->y));
			}
			if (mesh->HasTangentsAndBitangents()) {
				/* You can extract tangents and bitangents here              */
				/* Note that you might need to make Assimp generate this     */
				/* data for you. Take a look at the flags that aiImportFile  */
				/* can take.                                                 */
				const aiVector3D* vTn = &(mesh->mTangents[v_i]);
				modelData.mTangents.push_back(glm::vec3(vTn->x, vTn->y, vTn->z));

				const aiVector3D* vb = &(mesh->mBitangents[v_i]);
				modelData.mBitangents.push_back(glm::vec3(vb->x, vb->y, vb->z));
			}
		}
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			// retrieve all indices of the face and store them in the indices vector
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				modelData.indices.push_back(face.mIndices[j]);
		}
	}

	aiReleaseImport(scene);
	return modelData;
}

#pragma endregion MESH LOADING

// VBO Functions - click on + to expand
#pragma region VBO_FUNCTIONS
void Model::GenerateObjectBufferMesh(const char* mesh_name, const char* texture_name, const char* normal_name) {
	/*----------------------------------------------------------------------------
	LOAD MESH HERE AND COPY INTO BUFFERS
	----------------------------------------------------------------------------*/

	//Note: you may get an error "vector subscript out of range" if you are using this code for a mesh that doesnt have positions and normals
	//Might be an idea to do a check for that before generating and binding the buffer.

	mesh_data = LoadMesh(mesh_name);
	unsigned int vp_vbo = 0;
	loc1 = glGetAttribLocation(shaderProgramID, "vertex_position");
	loc2 = glGetAttribLocation(shaderProgramID, "vertex_normal");
	loc3 = glGetAttribLocation(shaderProgramID, "vertex_texture");
	loc4 = glGetAttribLocation(shaderProgramID, "tangent_data");
	loc5 = glGetAttribLocation(shaderProgramID, "bitangent_data");

	glGenBuffers(1, &vp_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mVertices[0], GL_STATIC_DRAW);

	unsigned int vn_vbo = 0;
	glGenBuffers(1, &vn_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mNormals[0], GL_STATIC_DRAW);

	//This is for texture coordinates
	unsigned int vt_vbo = 0;
	//unsigned int text;
	glGenBuffers (1, &vt_vbo);
	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
	glBufferData (GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof (vec2), &mesh_data.mTextureCoords[0], GL_STATIC_DRAW);

	//This is for tangents
	unsigned int vtn_vbo = 0;
	//unsigned int text;
	glGenBuffers(1, &vtn_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vtn_vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mTangents[0], GL_STATIC_DRAW);

	//This is for bitangents
	unsigned int vb_vbo = 0;
	//unsigned int text;
	glGenBuffers(1, &vb_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vb_vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh_data.mPointCount * sizeof(vec3), &mesh_data.mBitangents[0], GL_STATIC_DRAW);

	//unsigned int vao = 0;
	glGenVertexArrays(1, &modelvao);
	glBindVertexArray(modelvao);

	glEnableVertexAttribArray(loc1);
	glBindBuffer(GL_ARRAY_BUFFER, vp_vbo);
	glVertexAttribPointer(loc1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(loc2);
	glBindBuffer(GL_ARRAY_BUFFER, vn_vbo);
	glVertexAttribPointer(loc2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//Texture stuff
	unsigned int text;
	glGenTextures(1, &text);
	glBindTexture(GL_TEXTURE_2D, text);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;
	//unsigned char* data = stbi_load("bin_texture.jpg", &width, &height, &nrChannels, 0);
	unsigned char* data = stbi_load(texture_name, &width, &height, &nrChannels, 0);
	if (data)
	{
		//for png files
		GLenum type = GL_RGBA;
		if (nrChannels == 3) {
			type = GL_RGB;
		}
		else if (nrChannels == 4) {
			type = GL_RGBA;
		}


		glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	//glVertexAttribPointer(loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	//glEnableVertexAttribArray(loc3);
	glEnableVertexAttribArray (loc3);
	glBindBuffer (GL_ARRAY_BUFFER, vt_vbo);
	glVertexAttribPointer (loc3, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindTexture(text, 0);

	texture = text;

	//Texture stuff
	unsigned int normalm;
	glGenTextures(1, &normalm);
	glBindTexture(GL_TEXTURE_2D, normalm);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//normal maps
	unsigned char* normaldata = stbi_load(normal_name, &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB, width, height,0,GL_RGB,GL_UNSIGNED_BYTE, normaldata);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(normaldata);
	glBindTexture(GL_TEXTURE_2D, 1); // Unbinds the OpenGL Texture object so that it can't accidentally be modified

	glEnableVertexAttribArray(loc4);
	glBindBuffer(GL_ARRAY_BUFFER, vtn_vbo);
	glVertexAttribPointer(loc4, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(loc5);
	glBindBuffer(GL_ARRAY_BUFFER, vb_vbo);
	glVertexAttribPointer(loc5, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindTexture(normalm, 0);

	normalmap = normalm;

}
#pragma endregion VBO_FUNCTIONS

void Model::setup(const char* mesh_name, const char* texture_name, const char* normal_name, GLuint shader) {
	shaderProgramID = shader;
	GenerateObjectBufferMesh(mesh_name, texture_name, normal_name);
}