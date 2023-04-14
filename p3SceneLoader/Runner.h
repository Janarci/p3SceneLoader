#pragma once
#include <iostream>
#include <vector>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


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
	GLuint textures2[8];
	GLuint textures3[8];
	GLuint textures4[8];
	GLuint textures5[8];
	GLuint multiTexture[8];

	bool once = false;
	std::string GetBaseDir_(const std::string& filepath);
	bool FileExists_(const std::string& absFilename);
	void LoadTextureData_(ObjData* objData, GLuint* texture, GLuint* texture2, int num);
	void LoadObjFile_(ObjData* objData, std::string filename);
	void LoadObjToMemory_(ObjData* objData, GLfloat scaleFactor, GLfloat tOffset[], GLuint* texture, GLuint* texture2, int num);

	std::vector<std::pair<ObjData*, std::string>> scene1;
	std::vector<std::pair<ObjData*, std::string>> scene2;
	std::vector<std::pair<ObjData*, std::string>> scene3;
	std::vector<std::pair<ObjData*, std::string>> scene4;
	std::vector<std::pair<ObjData*, std::string>> scene5;

	ObjData GaneshaObjData;
	ObjData SwordObjData;
	ObjData PedestalObjData;
	ObjData FloorObjData;
	ObjData EarthObjData;
	ObjData MoonObjData;
	ObjData SunObjData;
	ObjData SkullObjData;

	Semaphores* mutexS1 = new Semaphores(1);
	Semaphores* mutexS2= new Semaphores(1);
	Semaphores* mutexS3 = new Semaphores(1);
	Semaphores* mutexS4 = new Semaphores(1);
	Semaphores* unloading = new Semaphores(1);
	Semaphores* running = new Semaphores(3);

private:
	bool view1;
	void render();
	void processEvents();
	void update();

	void load();
	void unload();
	void view();
	void renderAll(objLoader scene1Loader);
	

};



