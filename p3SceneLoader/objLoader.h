﻿#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "Semaphores.h"
#include "tiny_obj_loader.h"
#include "threading/IETThread.h"

struct VertexData {
	glm::vec3 position;
	glm::vec2 UV;
	glm::vec3 normal;
};

struct ObjData;
using namespace std;

struct ObjData {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	GLulong numFaces;
	GLuint vaoId;
	std::string baseDir;
	std::vector<tinyobj::material_t> materials;
	std::map<std::string, GLuint> textures;
	std::vector<GLuint> indices;
	std::vector<VertexData> vertexList;
	glm::mat4 transform;
	bool loaded = false;
	bool loadedToMem = false;
};

class objLoader : public IETThread
{
public:

	objLoader(int id, std::vector<std::pair<ObjData*, std::string>>& scene, Semaphores* limitSem, Semaphores* mutexSem);
	void run() override;

	void unload();
	void unloadVAO();
	void load();

	std::string GetBaseDir_(const std::string& filepath);
	void LoadObjFile_(ObjData* objData, std::string filename);


	ObjData* Model;
	std::string filename;
	std::vector<std::pair<ObjData*, std::string>> scene;

	GLfloat bunnyOffset[3] = { 0.0f, -0.5f, 0.0f };
	GLuint texture[8];
	GLuint multiTexture[8];

	bool finishLoad = false;
	bool LoadBtn = false;
	bool UnloadBtn = false;
	float progressPercentage;

private:
	int id;
	Semaphores* limitSem;
	Semaphores* mutexSem;

	glm::mat4 randomTransfom();
	void render();
	void processEvents();
	void update();

public:

};
