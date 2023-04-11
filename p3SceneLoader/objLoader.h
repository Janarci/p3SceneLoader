#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "tiny_obj_loader.h"
#include "threading/IETThread.h"

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
	bool finish = false;
	bool loadedToMem = false;
};

class objLoader : public IETThread
{
public:

	objLoader(ObjData* model, std::string filename, std::vector<std::pair<ObjData*, std::string>>& scene);
	void run() override;


	std::string GetBaseDir_(const std::string& filepath);
	void LoadObjFile_(ObjData* objData, std::string filename);

	ObjData* Model;
	std::string filename;
	std::vector<std::pair<ObjData*, std::string>> scene;

	bool finishLoad = false;
private:
	
	void render();
	void processEvents();
	void update();

public:

};
