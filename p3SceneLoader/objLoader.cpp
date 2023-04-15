#include "objLoader.h"

#include <random>


#include "structs.h"
#include "stb_image.h"


objLoader::objLoader( int id, std::vector<std::pair<ObjData*, std::string>>& scene, Semaphores* mutexSem)
{
	this->id = id;
	this->scene = scene;
	this->mutexSem = mutexSem;
}

void objLoader::run()
{
	load();

	while(true)
	{
		if (LoadBtn == true)
		{
			load();
			LoadBtn = false;

		}
		else if (UnloadBtn == true)
		{
			unload();
			UnloadBtn = false;
		}

	}

}

std::string objLoader::GetBaseDir_(const std::string& filepath)
{
	if (filepath.find_last_of("/\\") != std::string::npos)
		return filepath.substr(0, filepath.find_last_of("/\\"));
	return "";
}

void objLoader::LoadObjFile_(ObjData* objData, std::string filename)
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

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> orderedVertices;
	std::vector<GLuint> indices;
	std::vector<VertexData> vertexList;


	for (int i = 0; i < objData->attrib.vertices.size() / 3; i++) {
		vertices.push_back({ objData->attrib.vertices[i * 3] * 5 + bunnyOffset[0],
							objData->attrib.vertices[i * 3 + 1] * 5 + bunnyOffset[1],
							objData->attrib.vertices[i * 3 + 2] * 5 + bunnyOffset[2]
			});
	}

	for (int i = 0; i < objData->shapes.size(); i++) {
		tinyobj::shape_t shape = objData->shapes[i];
		for (int j = 0; j < shape.mesh.indices.size(); j++) {
			tinyobj::index_t idx = shape.mesh.indices[j];

			VertexData vertexData;

			vertexData.position = {
				objData->attrib.vertices[size_t(idx.vertex_index) * 3] * 5 + bunnyOffset[0],// x
				objData->attrib.vertices[size_t(idx.vertex_index) * 3 + 1] * 5 + bunnyOffset[1],// y
				objData->attrib.vertices[size_t(idx.vertex_index) * 3 + 2] * 5 + bunnyOffset[2]// z
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

	objData->indices = indices;
	objData->vertexList = vertexList;

	objData->transform = randomTransfom();

	objData->loaded = true;

	std::cout << "loaded " << filename << std::endl;
}


void objLoader::unload()
{
	//loading->acquire();
	if (finishLoad == false)
	{

		return;
	}
	for (int i = 0; i < scene.size(); i++) {
		std::cout <<"delete: "<< scene[i].first->baseDir << std::endl;
		//glDeleteVertexArrays(1, &scene[i].first->vaoId);
		//unloadVAO();

		scene[i].first->attrib.vertices.clear();
		scene[i].first->attrib.normals.clear();
		scene[i].first->attrib.texcoords.clear();
		scene[i].first->shapes.clear();
		scene[i].first->materials.clear();
		scene[i].first->indices.clear();
		scene[i].first->vertexList.clear();

		for (auto& texture : scene[i].first->textures) {
			glDeleteTextures(1, &texture.second);
		}
		scene[i].first->textures.clear();
		//UNLOADING DATA
		scene[i].first->loaded = false;
		scene[i].first->loadedToMem = false;

	}
	this->progressPercentage = 0;
	this->finishLoad = false;
	//loading->release();

}

void objLoader::unloadVAO()
{
	if (finishLoad == false)
	{

		return;
	}
	for (int i = 0; i < scene.size(); i++) {
		std::cout << "delete VAO: " << scene[i].first->baseDir << std::endl;

		glDeleteVertexArrays(1, &scene[i].first->vaoId);

	}
}


void objLoader::load()
{
	if (finishLoad == true)
	{
		return;
	}

	for (int i = 0; i < scene.size(); i++) {
		this->sleep(5000);

		mutexSem->acquire();
		LoadObjFile_(scene[i].first, scene[i].second);
		progressPercentage = (float)(i + 1) / (float)scene.size() * 100.0f;
		mutexSem->release();

	}
	std::cout << id << std::endl;

	finishLoad = true;

}

glm::mat4 objLoader::randomTransfom()
{
	static std::mt19937 rng(std::random_device{}());
	static uniform_real_distribution<float> dist(0.0f, 4.0f);
	static uniform_real_distribution<float> transdist(-10.0f, 10.0f);
	static uniform_real_distribution<float> scaledist(-10.0f, 10.0f);

	glm::vec3 translation(transdist(rng), transdist(rng), transdist(rng));
	glm::vec3 rotation(dist(rng), dist(rng), dist(rng));
	glm::vec3 scale(scaledist(rng), scaledist(rng), scaledist(rng));

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, translation);
	model = glm::rotate(model, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, scale);

	return model;
}

void objLoader::render()
{
}

void objLoader::processEvents()
{
}

void objLoader::update()
{
}