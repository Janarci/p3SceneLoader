#pragma once

#include "stb_image.h"
#include "structs.h"



std::string GetBaseDir(const std::string& filepath) {
	if (filepath.find_last_of("/\\") != std::string::npos)
		return filepath.substr(0, filepath.find_last_of("/\\"));
	return "";
}

bool FileExists(const std::string& absFilename) {
	struct stat buffer;
	return (stat(absFilename.c_str(), &buffer) == 0);
}


void LoadTextureData(ObjData* objData, GLuint* texture, GLuint* texture2, int num) {
	int width, height;
	std::string baseDir = objData->baseDir;

	for (size_t m = 0; m < objData->materials.size(); m++) {
		tinyobj::material_t* mp = &objData->materials[m];
		if (mp->diffuse_texname.length() > 0) {
			std::cout << baseDir << "\n";
			if (objData->textures.find(mp->diffuse_texname) == objData->textures.end()) {
				GLuint textureId;
				int comp;

				//std::string textureFileName = baseDir + "/" + mp->diffuse_texname + ".tga";
				std::string texname = mp->diffuse_texname;
				std::string textureFileName = baseDir + "/" + texname;
				if (!FileExists(textureFileName)) {
					std::cerr << "Unable to find file:" << textureFileName << std::endl;
					exit(1);
				}
				unsigned char* image = stbi_load(textureFileName.c_str(),
					&width,
					&height,
					&comp,
					STBI_default
				);
				if (!image) {
					std::cerr << "Unable to load texture: " << textureFileName << std::endl;
					exit(1);

				}
				//std::cerr << "Loaded texture: " << textureFileName << std::endl;
				//std::cout << image;

				//glGenTextures(1, &textureId);
				//glActiveTexture(GL_TEXTURE0);
				if (m == 0)
					glBindTexture(GL_TEXTURE_2D, texture[num]);
				else
					glBindTexture(GL_TEXTURE_2D, texture2[num]);


				GLenum format = GL_RGBA;

				if (comp == 3) {
					format = GL_RGB;
				}
				else if (comp == 4) {
					format = GL_RGBA;
				}
				glTexImage2D(
					GL_TEXTURE_2D,
					0,
					format,
					width,
					height,
					0,
					format,
					GL_UNSIGNED_BYTE,
					image
				);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				stbi_image_free(image);

				objData->textures.insert(std::make_pair(texname, textureId));

			}

		}
	}

}

//load obj models
void LoadObjFile(ObjData* objData, std::string filename) {
	std::string warn;
	std::string err;

	std::string basepath = "Assets/";

	std::string inputfile = basepath + filename;

	std::string baseDir = GetBaseDir(inputfile);
	objData->baseDir = baseDir;
	/*
	bool isSuccess = tinyobj::LoadObj(&objData->attrib,
		&objData->shapes,
		NULL,
		&warn,
		&err,
		inputfile.c_str()
	);*/
	bool isSuccess = tinyobj::LoadObj(&objData->attrib,
		&objData->shapes,
		&objData->materials,
		&warn,
		&err,
		inputfile.c_str(),
		baseDir.c_str()
	);

	if (!err.empty()) {
		std::cerr << err << std::endl;

	}

	std::cout << "loaded " << filename << std::endl;

}

void LoadObjToMemory2(ObjData* objData, GLfloat scaleFactor, GLfloat tOffset[]) {

	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;
	std::vector<VertexData> vertexList;

	/*for (int i = 0; i < objData->attrib.vertices.size() / 3; i++) {
		VertexData vertexData;
		vertexData.position = {
			objData->attrib.vertices[i * 3] * scaleFactor + tOffset[0],// x
			objData->attrib.vertices[i * 3 + 1] * scaleFactor + tOffset[1],// y
			objData->attrib.vertices[i * 3 + 2] * scaleFactor + tOffset[2]// z
		};


	}*/

	for (int i = 0; i < objData->shapes.size(); i++) {
		tinyobj::shape_t shape = objData->shapes[i];
		for (int j = 0; j < shape.mesh.indices.size(); j++) {
			tinyobj::index_t idx = shape.mesh.indices[j];

			VertexData vertexData;

			vertexData.position = {
				objData->attrib.vertices[size_t(idx.vertex_index) * 3] * scaleFactor + tOffset[0],// x
				objData->attrib.vertices[size_t(idx.vertex_index) * 3 + 1] * scaleFactor + tOffset[1],// y
				objData->attrib.vertices[size_t(idx.vertex_index) * 3 + 2] * scaleFactor + tOffset[2]// z
			};

			if (idx.texcoord_index >= 0) {
				vertexData.UV = {
					objData->attrib.texcoords[size_t(idx.texcoord_index) * 2 + 0],
					objData->attrib.texcoords[size_t(idx.texcoord_index) * 2 + 1]
				};
			}

			vertexList.push_back(vertexData);
			indices.push_back(indices.size());
		}
	}

	objData->numFaces = indices.size();

	// generate VAO
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	std::cout << "Vertex Array ID: " << VAO << std::endl;

	// generate VBO (vertices)
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	std::cout << "Vertex Buffer ID: " << VBO << std::endl;
	glBufferData(GL_ARRAY_BUFFER, vertexList.size() * sizeof(VertexData), &vertexList[0], GL_STATIC_DRAW);

	//set attributes for vertice buffer
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(VertexData),  //stride, we can give 0 to let opengl handle it, we can also give 3 * sizeof(GLfloat).
		(void*)0
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(VertexData),
		(void*)offsetof(VertexData, UV)
	);

	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(GLuint),
		&indices[0],
		GL_STATIC_DRAW
	);

	//LoadTextureData(objData);

	objData->vaoId = VAO;
}

void LoadObjToMemory(ObjData* objData, GLfloat scaleFactor, GLfloat tOffset[], GLuint* texture, GLuint* texture2, int num) {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> orderedVertices;
	std::vector<GLuint> indices;
	std::vector<VertexData> vertexList;


	for (int i = 0; i < objData->attrib.vertices.size() / 3; i++) {
		vertices.push_back({ objData->attrib.vertices[i * 3] * scaleFactor + tOffset[0],
							objData->attrib.vertices[i * 3 + 1] * scaleFactor + tOffset[1],
							objData->attrib.vertices[i * 3 + 2] * scaleFactor + tOffset[2]
			});
	}

	for (int i = 0; i < objData->shapes.size(); i++) {
		tinyobj::shape_t shape = objData->shapes[i];
		for (int j = 0; j < shape.mesh.indices.size(); j++) {
			tinyobj::index_t idx = shape.mesh.indices[j];

			VertexData vertexData;

			vertexData.position = {
				objData->attrib.vertices[size_t(idx.vertex_index) * 3] * scaleFactor + tOffset[0],// x
				objData->attrib.vertices[size_t(idx.vertex_index) * 3 + 1] * scaleFactor + tOffset[1],// y
				objData->attrib.vertices[size_t(idx.vertex_index) * 3 + 2] * scaleFactor + tOffset[2]// z
			};

			if (idx.texcoord_index >= 0) {
				vertexData.UV = {
					objData->attrib.texcoords[size_t(idx.texcoord_index) * 2 + 0],
					objData->attrib.texcoords[size_t(idx.texcoord_index) * 2 + 1]
				};
				vertexData.UV.y = 1 - vertexData.UV.y;
			}

			if (idx.normal_index >= 0) {
				vertexData.normal = {
				objData->attrib.normals[size_t(idx.normal_index) * 3 + 0],
				objData->attrib.normals[size_t(idx.normal_index) * 3 + 1],
				objData->attrib.normals[size_t(idx.normal_index) * 3 + 2],
				};
			}
			vertexList.push_back(vertexData);
			indices.push_back(indices.size());

			/*
			GLuint idx = shape.mesh.indices[j].vertex_index;
			indices.push_back(idx);*/
			//orderedVertices.push_back(vertices[idx]);
		}
	}
	objData->numFaces = indices.size();
	/*
	GLfloat vertexBufferData[] = {
		-0.9f, -0.6f, 0.0f,
		-0.6f, -0.9f, 0.0f,
		0.0f, 0.0f, 0.0f,  //1
		0.6f, -0.9f, 0.0f,
		0.9f, -0.6f, 0.0f,
		0.9f, 0.6f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.6f, 0.9f, 0.0f,
		-0.6f, 0.9f, 0.0f,
		-0.9f, 0.6f, 0.0f,
		0.0f, 0.0f, 0.0f,
		-0.9f, -0.6f, 0.0f,

	};
	/*	GLfloat vertexBufferData[] = {
			-0.9f, -0.6f, 0.0f,
			-0.6f, -0.9f, 0.0f,
			0.0f, 0.0f, 0.0f,  //1
			-0.6f, -0.9f, 0.0f,
			0.6f, -0.9f, 0.0f,
			0.0f, 0.0f, 0.0f,//2
			0.9f, -0.6f, 0.0f,
			0.6f, -0.9f, 0.0f,
			0.0f, 0.0f, 0.0f,//3
			0.9f, -0.6f, 0.0f,
			0.9f, 0.6f, 0.0f,
			0.0f, 0.0f, 0.0f,//4
			0.9f, 0.6f, 0.0f,
			0.6f, 0.9f, 0.0f,
			0.0f, 0.0f, 0.0f,//5
			0.6f, 0.9f, 0.0f,
			-0.6f, 0.9f, 0.0f,
			0.0f, 0.0f, 0.0f,//6
			-0.9f, 0.6f, 0.0f,
			0.0f, 0.0f, 0.0f,
			-0.6f, 0.9f, 0.0f,//7
			-0.9f, 0.6f, 0.0f,
			-0.9f, -0.6f, 0.0f,
			0.0f, 0.0f, 0.0f,//8
		};
		*/
		/*
		GLfloat colorBufferData[] = {
			.85f, .72f, .25f,
			.85f, .72f, .25f,
			.85f, .72f, .25f,
			.85f, .72f, .25f,
			.85f, .72f, .25f,
			.85f, .72f, .25f,
			.85f, .72f, .25f,
			.85f, .72f, .25f,
			.85f, .72f, .25f,
			.85f, .72f, .25f,
			.85f, .72f, .25f,
			.85f, .72f, .25f,
			.85f, .72f, .25f,
			.85f, .72f, .25f,
			.85f, .72f, .25f,
			.85f, .72f, .25f,
			.85f, .72f, .25f,
			.85f, .72f, .25f,
			.85f, .72f, .25f,
			.85f, .72f, .25f,
			.85f, .72f, .25f,
			.85f, .72f, .25f,
			.85f, .72f, .25f,
			.85f, .72f, .25f,

		};

		GLfloat vertexBufferDataFeatures[] = {
			-0.5f, 0.3f, 0.0f,
			-0.2f, 0.3f, 0.0f,
			-0.5f, 0.5f, 0.0f,
			0.2f, 0.3f, 0.0f,
			0.5f, 0.3f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.1f, -0.1f, 0.0f,
			0.1f, -0.1f, 0.0f,
			0.0f, 0.0f, 0.0f,
			-0.3f, -0.4f, 0.0f,
			-0.2f, -0.55f, 0.0f,
			0.3f, -0.4f, 0.0f,
			-0.2f, -0.55f, 0.0f,
			0.2f, -0.55f, 0.0f,
			0.3f, -0.4f, 0.0f,


		};
		GLfloat colorBufferDataFeatures[] = {

			0.0f,0.0f,0.0f,
			8.0f,0.0f,2.0f,
			0.0f,0.0f,0.0f,
			8.0f,0.0f,2.0f,
			0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,
			0.8f,0.3f,0.4f,
			0.8f,0.3f,0.4f,
			0.8f,0.3f,0.4f,
			0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,
			0.0f,0.0f,0.0f,

		};
		*/

		//for flat shading if it doesnt have normals or smthn
	if (objData->attrib.normals.size() == 0) {
		for (int i = 0; i < vertexList.size() / 3; i++) {
			int idx = i * 3;
			glm::vec3 normal = glm::normalize(glm::cross(vertexList[idx + 1].position - vertexList[idx].position, vertexList[idx + 2].position - vertexList[idx].position));
			vertexList[idx].normal = normal;
			vertexList[idx + 1].normal = normal;
			vertexList[idx + 2].normal = normal;
		}
	}


	//BUNNY
	GLuint BVAO, BVBO, BEBO;
	glGenVertexArrays(1, &BVAO);
	glBindVertexArray(BVAO);

	glGenBuffers(1, &BVBO);
	glBindBuffer(GL_ARRAY_BUFFER, BVBO);
	glBufferData(GL_ARRAY_BUFFER, vertexList.size() * sizeof(VertexData), &vertexList[0], GL_STATIC_DRAW);

	glGenBuffers(1, &BEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		indices.size() * sizeof(GLuint),
		&indices[0],
		GL_STATIC_DRAW
	);

	//set attributes for vertice buffer
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(
		0,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(VertexData),  //stride, we can give 0 to let opengl handle it, we can also give 3 * sizeof(GLfloat).
		(void*)0
	);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(VertexData),
		(void*)offsetof(VertexData, UV)
	);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(VertexData),
		(void*)offsetof(VertexData, normal)
	);


	LoadTextureData(objData, texture, texture2, num);
	objData->vaoId = BVAO;
}

