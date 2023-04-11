#pragma once
#include <iostream>
#include <vector>



#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "objLoader.h"


using namespace std;
class Runner
{
public:

	Runner();
	void run();
	
	GLFWwindow* window;
	GLuint texture[8];
	GLuint multiTexture[8];
	bool once = false;
	std::string GetBaseDir_(const std::string& filepath);
	bool FileExists_(const std::string& absFilename);
	void LoadTextureData_(ObjData* objData, GLuint* texture, GLuint* texture2, int num);
	void LoadObjFile_(ObjData* objData, std::string filename);
	void LoadObjToMemory_(ObjData* objData, GLfloat scaleFactor, GLfloat tOffset[], GLuint* texture, GLuint* texture2, int num);

	std::vector<std::pair<ObjData*, std::string>> scene1;
	std::vector<std::pair<ObjData*, std::string>> scene2;

	ObjData GaneshaObjData;
	ObjData SwordObjData;
	ObjData PedestalObjData;
	ObjData FloorObjData;
	ObjData EarthObjData;
	ObjData MoonObjData;
	ObjData SunObjData;
	ObjData SkullObjData;


private:

	void render();
	void processEvents();
	void update();
	

};



