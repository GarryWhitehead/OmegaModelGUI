#pragma once
#include "ObjUtility.h"
#include "objector.h"
#include "OMFConverter/OMFConverter.h"
#include <string>
#include <vector>
#include <fstream>
#include "glm.hpp"

struct ModelInfo;

enum class objTextureType
{
	DIFFUSE_TEXTURE,
	AMBIENT_TEXTURE,
	SPECULAR_TEXTURE,
	BUMP_TEXTURE
};

struct objMaterial
{
	objMaterial() : index(0) {}

	struct MatColor
	{
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float roughness;
		float metallic;
	} Color;

	struct TexMap
	{
		std::string ambient;
		std::string diffuse;
		std::string specular;
		std::string bump;
		std::string alpha;
		std::string displacement;
		std::string highlight;
		std::string stencil;
	} Map;

	float opacity;
	uint32_t illumination;

	std::string name;
	uint32_t index;

	// helper functions
	std::string GetTextureTypeFilename(objTextureType type);
	bool hasTextureType(objTextureType type);
};

class MaterialParser
{
public:

	MaterialParser();
	~MaterialParser();

	void ImportMtlFile(std::string fileDir, std::string filename, OMFConverter::ModelInfo *model);
	
private:

	std::string ProcessNewMaterial(std::string fileDir, OMFConverter::Material& mat, std::fstream& file);

	ObjUtility objUtility;

	uint32_t flags;

};

