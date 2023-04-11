#include "Runner.h"
#include "objLoader.h"


#include <chrono>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

//#include "tiny_obj_loader.h"
#include "glm/glm.hpp"
//#include "Obj_mesh.h"
#include "shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include "skybox.h"
#include "structs.h"


//
typedef std::chrono::high_resolution_clock Clock;

bool firstmouse = true;
GLfloat lastX = 0;
GLfloat lastY = 0;
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
glm::vec3 camFront;
glm::vec3 camSide;
glm::vec3 camUp;
//
GLfloat bunnyOffset[] = { 0.0f, -0.5f, 0.0f };

//ObjData GaneshaObjData;


SkyboxData skybox;

GLuint shaderProgram;
GLuint skyboxShaderProgram;
GLuint projectionLoc;
GLuint viewLoc;
GLuint modelTransformLoc;
GLuint normalTransLoc;
GLuint cameraPosLoc;
GLuint ambientColorLoc;
GLuint lightLoc;
GLuint diffuseLoc;
GLuint diffuse2Loc;
GLuint multiTex;

glm::mat4 projection;
glm::mat4 transEarth, transMoon, transSun, transGanesha, transSword, transSkull, transPedestal, transFloor;

static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	if (firstmouse) {
		lastX = xpos;
		lastY = ypos;
		firstmouse = false;
	}

	GLfloat xOffset = xpos - lastX;
	GLfloat yOffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.4f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;


	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camFront = glm::normalize(front);



}

Runner::Runner()
{
	//SceneManager::getInstance();


#pragma region initialize


	//initialize glfw  
	if (glfwInit() != GLFW_TRUE) {
		fprintf(stderr, "Failed to initialized! \n");
		return;
	}

	// set opengl version to 3.3  
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// create window   
	window = glfwCreateWindow(1024, 768, "Khalil Jan Arcilla", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to load window! \n");
		return;
	}

	glfwMakeContextCurrent(window);

	//initialize glew  
	glewExperimental = true;
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return;
	}
#pragma endregion


	glGenTextures(8, texture);
	glGenTextures(8, multiTexture);


#pragma region object

	//SceneManager::getInstance()->loadModel(modelTransformLoc);


	//testThread* test = new testThread(texture, multiTexture, bunnyOffset, GaneshaObjData, "Ganesha/Ganesha.obj", 3);
	//LoadObjFile_(&GaneshaObjData, "Ganesha/Ganesha.obj");
	//LoadObjToMemory_(&GaneshaObjData, 5.0f, bunnyOffset, texture, multiTexture, 3);
	//LoadTextureData(&GaneshaObjData, "Ganesha1.png", texture, 3);

	//std::cout << GaneshaObjData.baseDir<<"\n";

	//LoadObjFile_(&SkullObjData, "Skull/Skull.obj");
	//LoadObjToMemory_(&SkullObjData, 5.0f, bunnyOffset, texture, multiTexture, 4);
	////LoadTextureData(&SkullObjData, "skull_diffuse.jpg", texture, 4);

	//LoadObjFile_(&SwordObjData, "Sword/moonlight.obj");
	//LoadObjToMemory_(&SwordObjData, 5.0f, bunnyOffset, texture, multiTexture, 5);
	//LoadTextureData(&SwordObjData, "Tex_0014_0.png", texture, 5);

	//LoadObjFile_(&PedestalObjData, "pedestal/10421_square_pedastal_iterations-2.obj");
	//LoadObjToMemory_(&PedestalObjData, 5.0f, bunnyOffset, texture, multiTexture, 6);
	////LoadTextureData(&PedestalObjData, "10421_square_pedastal_diffuse.jpg", texture, 6);

	//LoadObjFile_(&FloorObjData, "Floor/CobbleStones2.obj");
	//LoadObjToMemory_(&FloorObjData, 5.0f, bunnyOffset, texture, multiTexture, 7);
	////LoadTextureData(&FloorObjData, "BrickRound0108_5_S.jpg", texture, 7);

	//LoadObjFile_(&EarthObjData, "earth/Earth.obj");
	//LoadObjToMemory_(&EarthObjData, 5.0f, bunnyOffset, texture, multiTexture, 0);
	////LoadTextureData(&EarthObjData, "Earth.tga", texture, 0);

	//LoadObjFile_(&MoonObjData, "earth/Earth.obj");
	//LoadObjToMemory_(&MoonObjData, 5.0f, bunnyOffset, texture, multiTexture, 1);
	////LoadTextureData(&MoonObjData, "Moon.tga", texture, 1);

	//LoadObjFile_(&SunObjData, "earth/Earth.obj");
	//LoadObjToMemory_(&SunObjData, 5.0f, bunnyOffset, texture, multiTexture, 2);
	////LoadTextureData(&SunObjData, "Sun.tga", texture, 2);

#pragma endregion

#pragma region skybox
	std::vector<std::string> faces{
		"right.png",
		"left.png",
		"top.png",
		"bottom.png",
		"front.png",
		"back.png"

	};
	skybox = loadSkyBox("Assets/Skybox", faces);
#pragma endregion

	

#pragma region shader
	//skybox shader
	skyboxShaderProgram = loadShaders("Shaders/skybox_vertex.shader", "Shaders/skybox_fragment.shader");

	shaderProgram = loadShaders("Shaders/phong_vertex.shader", "Shaders/phong_fragment.shader");
	glUseProgram(shaderProgram);
	GLfloat red = 1.0f;
	GLfloat green = 0.0f;
	GLfloat blue = 0.0f;
	GLuint colorLoc = glGetUniformLocation(shaderProgram, "u_color");
	glUniform3f(colorLoc, 0.3f, 0.3f, 0.3f);

	modelTransformLoc = glGetUniformLocation(shaderProgram, "u_model");
	viewLoc = glGetUniformLocation(shaderProgram, "u_view");
	projectionLoc = glGetUniformLocation(shaderProgram, "u_projection");

	//normal transformations
	normalTransLoc = glGetUniformLocation(shaderProgram, "u_normal");
	cameraPosLoc = glGetUniformLocation(shaderProgram, "u_camera_pos");
	ambientColorLoc = glGetUniformLocation(shaderProgram, "u_ambient_color");
	glUniform3f(ambientColorLoc, 0.8f, 0.8f, 0.8f);

	//light shading
	lightLoc = glGetUniformLocation(shaderProgram, "u_light_pos");
	diffuseLoc = glGetUniformLocation(shaderProgram, "texture_diffuse");
	diffuse2Loc = glGetUniformLocation(shaderProgram, "texture2_diffuse");
	glUniform3f(lightLoc, 3.0f, 3.0f, 2.0f);

	//set textures
	glUniform1i(diffuseLoc, 0);
	glUniform1i(diffuse2Loc, 1);

	//bool check
	//GLuint isSun = glGetUniformLocation(shaderProgram, "sun");
	multiTex = glGetUniformLocation(shaderProgram, "multi");

	//transformation matrix
	glm::mat4 trans = glm::mat4(1.0f); // identity
	trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, 0.0f));//change this for x y z positions
	//trans = glm::rotate(trans, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.0f));// rotation by degrees on glm::radians
	trans = glm::scale(trans, glm::vec3(1.0f, 1.0f, 1.0f));//scale
	//glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(trans));

	//define projection matrix
	projection = glm::mat4(1.0f);// = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, -1.0f);
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	
	transMoon =
		transSun =
		transGanesha =
		transSword =
		transPedestal =
		transFloor =
		transSkull = glm::mat4(1.0f);

	transSun = glm::scale(transSun, glm::vec3(1.0f, 1.0f, 1.0f));
	transSun = glm::translate(transSun, glm::vec3(30.0f, 0.0f, -40.0f));

	transPedestal = glm::translate(transPedestal, glm::vec3(50.0f, 0.0f, -40.0f));

	transFloor = glm::translate(transFloor, glm::vec3(0.0f, 0.0f, -15.0f));

	transGanesha = glm::translate(transGanesha, glm::vec3(30.0f, 0.0f, -40.0f));
	transGanesha = glm::translate(transGanesha, glm::vec3(120.0f, 0.0f, 0.0f));
	transGanesha = glm::scale(transGanesha, glm::vec3(2.0f, 2.0f, 2.0f));//scale

	transGanesha = glm::rotate(transGanesha, glm::radians(220.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//transGanesha = glm::translate(transGanesha, glm::vec3(10.0f, 0.0f, 0.0f));

	transSword = glm::translate(transSword, glm::vec3(30.0f, 0.0f, -40.0f));
	transSword = glm::translate(transSword, glm::vec3(200.0f, 15.0f, 0.0f));
	transSword = glm::scale(transSword, glm::vec3(0.2f, 0.2f, 0.2f));//scale
	transSword = glm::scale(transSword, glm::vec3(0.5f, 0.5f, 0.5f));//scale
	transSword = glm::rotate(transSword, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));

	//transSkull = glm::translate(transSkull, glm::vec3(40.0f, 0.0f, -20.0f));
	transSkull = glm::translate(transSkull, glm::vec3(50.0f, 0.0f, -40.0f));
	transSkull = glm::scale(transSkull, glm::vec3(4.0f, 4.0f, 4.0f));//scale



	GLfloat x = 0.0f, y = 0.0f, z = 0.0f;
	GLfloat xScale = 0.0f, yScale = 0.0f, zScale = 0.0f;
	GLfloat xRotate = 0.0f, YRotate = 0.0f, ZRotate = 0.0f;
	double prevxpos = 0.0f, prevypos = 0.0f;
	double xpos = 0.0f, ypos = 0.0f;
#pragma endregion


	// set bg color to green  
	glClearColor(0.3f, 0.3f, 0.3f, 0.3f);

	float max_far = 2000.0f;
	float walkspeed = 0.5f;

	glm::vec3 position = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 viewDirections = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Up = glm::vec3(0.0f, 0.0f, -1.0f);
	//projection = glm::ortho(-40.0f, 120.0f, -40.0f, 120.0f, 0.1f, max_far);

	position = glm::vec3(30.0f, 20.0f, 40.0f);
	viewDirections = glm::vec3(30.0f, 0.0f, 0.0f);
	Up = glm::vec3(0.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);

	std::string str1 = "models/obj1/";
	std::string str2 = "models/obj2/";

	// add pairs to the vector
	scene1.push_back(std::make_pair(&SwordObjData, "Sword/moonlight.obj"));

	scene1.push_back(std::make_pair(&GaneshaObjData, "Ganesha/Ganesha.obj"));




	/*scene1.push_back(std::make_pair(obj2, "models/obj2/"));
	for (int i = 0; i < scene1.size(); i++) {
		std::cout << "Pair " << i << ":" << std::endl;
		std::cout << "  ObjData: [do something with myVector[" << i << "].first]" << std::endl;
		std::cout << "  String: " << scene1[i].second << std::endl;
	}*/
}

void Runner::run()
{
	ObjData obj1, obj2;
	scene2.push_back(std::make_pair(&obj1, "Skull/Skull.obj"));


	//SceneManager::getInstance()->loadModel(modelTransformLoc, this);
	objLoader scene1Loader(&GaneshaObjData, "Ganesha/Ganesha.obj", scene1);
	scene1Loader.start();

	objLoader scene2Loader(&GaneshaObjData, "Ganesha/Ganesha.obj", scene2);
	scene2Loader.start();

	



	// set bg color to green  
	glClearColor(0.3f, 0.3f, 0.3f, 0.3f);

	float max_far = 2000.0f;
	float walkspeed = 0.5f;

	glm::vec3 position = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 viewDirections = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 Up = glm::vec3(0.0f, 0.0f, -1.0f);
	//projection = glm::ortho(-40.0f, 120.0f, -40.0f, 120.0f, 0.1f, max_far);

	position = glm::vec3(30.0f, 20.0f, 40.0f);
	viewDirections = glm::vec3(30.0f, 0.0f, 0.0f);
	Up = glm::vec3(0.0f, 1.0f, 0.0f);

	glEnable(GL_DEPTH_TEST);




	auto t1 = Clock::now();
	while (!glfwWindowShouldClose(window)) {
		glfwSetCursorPosCallback(window, mouse_callback);


		//for FPS
		if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) {
			walkspeed = 1.0f;
		}
		else {
			walkspeed = 0.5f;

		}
		if (true) {
			glm::vec3 strafeDir = glm::cross(camFront, Up);
			camUp = glm::cross(strafeDir, camFront);
			strafeDir = glm::cross(camFront, camUp);
			if (glfwGetKey(window, GLFW_KEY_D)) {
				position += walkspeed * strafeDir;
				//std::cout << glm::to_string(position) << std::endl;

			}
			if (glfwGetKey(window, GLFW_KEY_A)) {
				position -= walkspeed * strafeDir;
				//std::cout << glm::to_string(position) << std::endl;

			}
			if (glfwGetKey(window, GLFW_KEY_W)) {
				position += walkspeed * camFront;
				//std::cout << glm::to_string(position) << std::endl;

				//viewDirections.z += 0.1f;
			}
			if (glfwGetKey(window, GLFW_KEY_S)) {
				position -= walkspeed * camFront;
				//std::cout << glm::to_string(position) << std::endl;

				//viewDirections.z -= 0.1f;
			}
			if (glfwGetKey(window, GLFW_KEY_E)) {
				position += walkspeed * Up;
				//std::cout << glm::to_string(position) << std::endl;
			}
			if (glfwGetKey(window, GLFW_KEY_Q)) {
				position -= walkspeed * Up;
				//std::cout << glm::to_string(position) << std::endl;
			}
			if (glfwGetKey(window, GLFW_KEY_P)) {
				GaneshaObjData.attrib.vertices.clear();
				GaneshaObjData.attrib.normals.clear();
				GaneshaObjData.attrib.texcoords.clear();
				GaneshaObjData.shapes.clear();
				GaneshaObjData.materials.clear();
				for (auto& texture : GaneshaObjData.textures) {
					glDeleteTextures(1, &texture.second);
				}
				GaneshaObjData.textures.clear();
				glDeleteVertexArrays(1, &GaneshaObjData.vaoId);
				//UNLOADING DATA
				scene1Loader.finishLoad = false;
				once = false;
				//SceneManager::getInstance()->run();

			}
			if (position.y < 40.0f || position.y > 40.0f) {
				position.y = 40.0f;
			}
		}

		float ratio;
		int width, height;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);

		if (glfwGetKey(window, GLFW_KEY_R)) {
			position = glm::vec3(30.0f, 20.0f, 40.0f);
			viewDirections = glm::vec3(30.0f, 0.0f, 0.0f);
			Up = glm::vec3(0.0f, 1.0f, 0.0f);
			//projection = glm::mat4(1.0f);
			projection = glm::perspective(glm::radians(60.0f), ratio, 0.1f, max_far);
		}
		if (glfwGetKey(window, GLFW_KEY_T)) {
			position = glm::vec3(0.0f, 1.0f, 0.0f);
			viewDirections = glm::vec3(0.0f, 0.0f, 0.0f);
			Up = glm::vec3(0.0f, 0.0f, -1.0f);
			//projection = glm::mat4(1.0f);
			projection = glm::ortho(-40.0f, 120.0f, -40.0f, 120.0f, 0.1f, max_far);
		}
		projection = glm::perspective(glm::radians(60.0f), ratio, 0.1f, max_far);

		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

#pragma region View

//for fps

		glm::mat4 view = glm::lookAt(
			position,
			position + camFront,
			Up
		);
		glUniform3f(cameraPosLoc, position.x, position.y, position.z);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

#pragma endregion
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#pragma region Draw
		//--- start drawing here ---

		drawSkyBox(skybox, skyboxShaderProgram, view, projection);
		glUseProgram(shaderProgram);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(multiTex, false);
		//std::this_thread::sleep_for(std::chrono::milliseconds(9000));

		//if (scene1Loader.finishLoad == true && once == false)
		//{
		//	once = true;
		//	/*LoadObjToMemory_(&GaneshaObjData, 5.0f, bunnyOffset, texture, multiTexture, 3);
		//	LoadObjToMemory_(&SwordObjData, 5.0f, bunnyOffset, texture, multiTexture, 5);*/
		//	std::cout << "checl" << std::endl;
		//	for (int i = 0; i < scene1.size(); i++) {
		//		LoadObjToMemory_(scene1[i].first, 5.0f, bunnyOffset, texture, multiTexture, i);

		//	}

		//}

		for (int i = 0; i < scene2.size(); i++) {
			if (scene2[i].first->finish && scene2[i].first->loadedToMem == false)
			{
				LoadObjToMemory_(scene2[i].first, 5.0f, bunnyOffset, texture, multiTexture, i);
				scene2[i].first->loadedToMem = true;
			}

		}

		for (int i = 0; i < scene2.size(); i++) {
			glBindVertexArray(scene2[i].first->vaoId);
			glBindTexture(GL_TEXTURE_2D, texture[i]);
			glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(transGanesha));
			glDrawElements(GL_TRIANGLES, scene2[i].first->numFaces, GL_UNSIGNED_INT, (void*)0);
		}


		/*glBindVertexArray(GaneshaObjData.vaoId);
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(transGanesha));
		glDrawElements(GL_TRIANGLES, GaneshaObjData.numFaces, GL_UNSIGNED_INT, (void*)0);



		glBindVertexArray(SwordObjData.vaoId);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(transSword));
		glDrawElements(GL_TRIANGLES, SwordObjData.numFaces, GL_UNSIGNED_INT, (void*)0);*/

		//glBindVertexArray(SkullObjData.vaoId);
		//glBindTexture(GL_TEXTURE_2D, texture[4]);
		//glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(transSkull));
		//glDrawElements(GL_TRIANGLES, SkullObjData.numFaces, GL_UNSIGNED_INT, (void*)0);


		//glBindVertexArray(PedestalObjData.vaoId);
		//glBindTexture(GL_TEXTURE_2D, texture[6]);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, multiTexture[7]);


		//for (int i = 0; i < 3; i++) {
		//	glm::mat4 dupe = transPedestal;
		//	dupe = glm::translate(dupe, glm::vec3(i * 100.0f, 0.0f, 0.0f));
		//	dupe = glm::scale(dupe, glm::vec3(0.2f, 0.2f, 0.2f));//scale
		//	dupe = glm::rotate(dupe, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//	if (i == 1)
		//		glUniform1i(multiTex, false);
		//	else
		//		glUniform1i(multiTex, true);
		//	glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(dupe));
		//	glDrawElements(GL_TRIANGLES, PedestalObjData.numFaces, GL_UNSIGNED_INT, (void*)0);
		//}


		//glUniform1i(multiTex, true);

		//glBindVertexArray(FloorObjData.vaoId);
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, texture[7]);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, multiTexture[7]);


		//for (int i = 0; i < 6; i++) {
		//	for (int j = 0; j < 6; j++) {

		//		glm::mat4 dupe = transFloor;
		//		dupe = glm::translate(dupe, glm::vec3(i * 100.0f, -85.0f, j * 100.0f));
		//		dupe = glm::translate(dupe, glm::vec3(-100.0f, 0.0f, 0.0f));
		//		dupe = glm::scale(dupe, glm::vec3(2.2f, 2.2f, 2.2f));//scale
		//		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(dupe));
		//		glDrawElements(GL_TRIANGLES, FloorObjData.numFaces, GL_UNSIGNED_INT, (void*)0);
		//	}

		//}
		//glUniform1i(multiTex, false);

		//for (int i = 0; i < 4; i++) {
		//	for (int j = 0; j < 4; j++) {//ceiling

		//		glm::mat4 dupe = transFloor;
		//		dupe = glm::translate(dupe, glm::vec3(i * 100.0f, 105.0f, j * 100.0f));
		//		dupe = glm::scale(dupe, glm::vec3(2.2f, 2.2f, 2.2f));//scale
		//		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(dupe));
		//		glDrawElements(GL_TRIANGLES, FloorObjData.numFaces, GL_UNSIGNED_INT, (void*)0);
		//	}
		//}
		//for (int i = 0; i < 4; i++) {
		//	for (int j = 0; j < 2; j++) {//wall

		//		glm::mat4 dupe = transFloor;
		//		dupe = glm::translate(dupe, glm::vec3(0.0f, -85.0f, i * 100.0f));
		//		dupe = glm::translate(dupe, glm::vec3(-48.0f, 48.0f, -2.0f));
		//		dupe = glm::translate(dupe, glm::vec3(0.0f, j * 100.0f, 0.0f));
		//		dupe = glm::scale(dupe, glm::vec3(2.2f, 2.2f, 2.2f));//scale
		//		dupe = glm::rotate(dupe, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(dupe));
		//		glDrawElements(GL_TRIANGLES, FloorObjData.numFaces, GL_UNSIGNED_INT, (void*)0);
		//	}
		//	for (int j = 0; j < 2; j++) {//wall

		//		glm::mat4 dupe = transFloor;
		//		dupe = glm::translate(dupe, glm::vec3(0.0f, -85.0f, i * 100.0f));
		//		dupe = glm::translate(dupe, glm::vec3(348.0f, 48.0f, -2.0f));
		//		dupe = glm::translate(dupe, glm::vec3(0.0f, j * 100.0f, 0.0f));
		//		dupe = glm::scale(dupe, glm::vec3(2.2f, 2.2f, 2.2f));//scale
		//		dupe = glm::rotate(dupe, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		//		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(dupe));
		//		glDrawElements(GL_TRIANGLES, FloorObjData.numFaces, GL_UNSIGNED_INT, (void*)0);
		//	}
		//	for (int j = 0; j < 2; j++) {//wall

		//		glm::mat4 dupe = transFloor;
		//		dupe = glm::translate(dupe, glm::vec3(i * 100.0f, -85.0f, 0.0f));
		//		dupe = glm::translate(dupe, glm::vec3(0.0f, 44.0f, -48.0f));
		//		dupe = glm::translate(dupe, glm::vec3(0.0f, j * 100.0f, 0.0f));
		//		dupe = glm::scale(dupe, glm::vec3(2.2f, 2.2f, 2.2f));//scale
		//		dupe = glm::rotate(dupe, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//		glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(dupe));
		//		glDrawElements(GL_TRIANGLES, FloorObjData.numFaces, GL_UNSIGNED_INT, (void*)0);
		//	}
		//	for (int j = 0; j < 2; j++) {//wall

		//		glm::mat4 dupe = transFloor;
		//		if (i != 1) {
		//			dupe = glm::translate(dupe, glm::vec3(i * 100.0f, -85.0f, 0.0f));
		//			dupe = glm::translate(dupe, glm::vec3(0.0f, 44.0f, 350.0f));
		//			dupe = glm::translate(dupe, glm::vec3(0.0f, j * 100.0f, 0.0f));
		//			dupe = glm::scale(dupe, glm::vec3(2.2f, 2.2f, 2.2f));//scale
		//			dupe = glm::rotate(dupe, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//			glUniformMatrix4fv(modelTransformLoc, 1, GL_FALSE, glm::value_ptr(dupe));
		//			glDrawElements(GL_TRIANGLES, FloorObjData.numFaces, GL_UNSIGNED_INT, (void*)0);
		//		}
		//	}

		//}



		//threads[0]->run();
		//SceneManager::getInstance()->loader->run();
		//loader->run();


		//SceneManager::getInstance()->loader->run();
		/*if (loader->finishLoad)
		{
			loader->run();
		}*/
		
			//--- stop drawing here ---
#pragma endregion
		glfwSwapBuffers(window);

		//listen for glfw input events
		glfwPollEvents();
	}

	return;
}
//
std::string Runner::GetBaseDir_(const std::string& filepath)
{
	if (filepath.find_last_of("/\\") != std::string::npos)
		return filepath.substr(0, filepath.find_last_of("/\\"));
	return "";
}

bool Runner::FileExists_(const std::string& absFilename)
{
	struct stat buffer;
	return (stat(absFilename.c_str(), &buffer) == 0);
}

void Runner::LoadTextureData_(ObjData* objData, GLuint* texture, GLuint* texture2, int num)
{
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
				if (!FileExists_(textureFileName)) {
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

void Runner::LoadObjFile_(ObjData* objData, std::string filename)
{
	std::string warn;
	std::string err;

	std::string basepath = "Assets/";

	std::string inputfile = basepath + filename;

	std::string baseDir = GetBaseDir_(inputfile);
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

void Runner::LoadObjToMemory_(ObjData* objData, GLfloat scaleFactor, GLfloat tOffset[], GLuint* texture,
	GLuint* texture2, int num)
{
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


	this->LoadTextureData_(objData, texture, texture2, num);
	objData->vaoId = BVAO;
}

void Runner::render()
{
}

void Runner::processEvents()
{
}

void Runner::update()
{
}
