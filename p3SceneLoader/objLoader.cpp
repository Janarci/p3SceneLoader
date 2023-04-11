#include "objLoader.h"



#include "structs.h"
#include "stb_image.h"


objLoader::objLoader(ObjData* model, std::string filename, std::vector<std::pair<ObjData*, std::string>>& scene)
{
	this->Model = model;
	this->filename = filename;
	this->scene = scene;
}

void objLoader::run()
{

	//LoadObjFile_(Model, filename);
	for (int i = 0; i < scene.size(); i++) {
		LoadObjFile_(scene[i].first, scene[i].second);
	}

	/*LoadObjFile_(&scene[0].first, scene[0].second);*/
	finishLoad = true;
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
	objData->finish = true;
	std::cout << "loaded " << filename << std::endl;
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