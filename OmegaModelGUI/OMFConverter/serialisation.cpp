#include "serialisation.h"
#include "file_log.h"
#include <sstream>
#include <iostream>

Archiver::Archiver()
{
}

void Archiver::OpenFile(const std::string filename, SaveMode saveMode, FileMode fileMode)
{
	mode = fileMode;

	std::ios_base::openmode flags;
	
	if (fileMode == FileMode::FILE_IN) {
		flags = std::fstream::in;
	}
	else {
		flags = std::fstream::out;
	}

	if (saveMode == SaveMode::SAVE_BINARY) {
		flags |= std::fstream::binary;
	}

	file.open(filename, flags);

	if (!file.is_open()) {
		std::cout << "Critical error!! Unable to open" << filename << " during serialisation.";
		exit(EXIT_FAILURE);
	}
}

// custom variable type functions
void Archiver::Serialise(char& value, const var_info& info)
{
	std::string line;
	SerialiseText<int32_t>(value, info, line);
	if (!line.empty())
		value = line[0];
}

void Archiver::Serialise(float& value, const var_info& info)
{
	std::string line;
	SerialiseText<float>(value, info, line);
	if (!line.empty())
		value = std::stof(line);
}

void Archiver::Serialise(uint16_t& value, const var_info& info)
{
	std::string line;
	SerialiseText<int16_t>(value, info, line);
	if (!line.empty())
		value = std::stoi(line);
}

void Archiver::Serialise(int& value, const var_info& info)
{
	std::string line;
	SerialiseText<int>(value, info, line);
	if (!line.empty())
		value = std::stoi(line);
}

void Archiver::Serialise(uint32_t& value, const var_info& info)
{
	std::string line;
	SerialiseText<uint32_t>(value, info, line);
	if (!line.empty())
		value = std::stoi(line);
}

void Archiver::Serialise(long& value, const var_info& info)
{
	std::string line;
	SerialiseText<long>(value, info, line);
	if (!line.empty())
		value = std::stol(line);
}

void Archiver::Serialise(bool& value, const var_info& info)
{
	std::string line;
	SerialiseText<bool>(value, info, line);
	if (!line.empty())
		value = (std::stoi(line) == 0) ? false : true;
}

void Archiver::Serialise(double& value, const var_info& info)
{
	std::string line;
	SerialiseText<double>(value, info, line);
	if (!line.empty())
		value = std::stod(line);
}

void Archiver::Serialise(std::string& str, const var_info& info)
{
	std::string line;
	SerialiseText<std::string>(str, info, line);
}

// glm functions
void Archiver::Serialise(glm::vec2& vec, const var_info& info)
{
	Serialise(vec.x, var_info(info.name + ".x"));
	Serialise(vec.y, var_info(info.name + ".y"));
}

void Archiver::Serialise(glm::vec3& vec, const var_info& info)
{
	Serialise(vec.x, var_info(info.name + ".x"));
	Serialise(vec.y, var_info(info.name + ".y"));
	Serialise(vec.z, var_info(info.name + ".z"));
}

void Archiver::Serialise(glm::vec4& vec, const var_info& info)
{
	Serialise(vec.x, var_info(info.name + ".x"));
	Serialise(vec.y, var_info(info.name + ".y"));
	Serialise(vec.z, var_info(info.name + ".z"));
	Serialise(vec.w, var_info(info.name + ".w"));
}

void Archiver::Serialise(glm::mat4& vec, const var_info& info)
{
	Serialise(vec[0], var_info(info.name + "[0].mat4"));
	Serialise(vec[1], var_info(info.name + "[1].mat4"));
	Serialise(vec[2], var_info(info.name + "[2].mat4"));
	Serialise(vec[3], var_info(info.name + "[3].mat4"));
}

// serialisation functions for model data - mesh and materials

// material serialisation functions
void Archiver::Serialise(OMFConverter::Material& material, const Archiver::var_info& info)
{
	// materials colour
	Serialise(material.Color.ambient, info.name + "ambient");
	Serialise(material.Color.diffuse, info.name + "diffuse");
	Serialise(material.Color.specular, info.name + "specular");
	Serialise(material.Color.roughness, info.name + "roughness");
	Serialise(material.Color.metallic, info.name + "metallic");

	// materials texture maps
	Serialise(material.Map.albedo, info.name + "albedo");
	Serialise(material.Map.ao, info.name + "ao");
	Serialise(material.Map.metallic, info.name + "metallic");
	Serialise(material.Map.normal, info.name + "normal");
	Serialise(material.Map.roughness, info.name + "roughness");
	Serialise(material.Map.specular, info.name + "specular");

	Serialise(material.illumination, info.name + "illumination");
	Serialise(material.opacity, info.name + "opacity");
	Serialise(material.name, info.name + "name");
}

// mesh serialisation fucntions
void Archiver::Serialise(OMFConverter::FaceInfo& face, const Archiver::var_info& info)
{
	Serialise<uint32_t>(face.indices, info.name + "indices");
}

void Archiver::Serialise(OMFConverter::Mesh& mesh, const Archiver::var_info& info)
{
	Serialise(mesh.meshName, info.name + "name");
	Serialise(mesh.material, info.name + "material");
	Serialise<glm::vec3>(mesh.posData, info.name + "position");
	Serialise<glm::vec2>(mesh.uvData, info.name + "uv");
	Serialise<glm::vec3>(mesh.normData, info.name + "normal");
	Serialise<glm::vec3>(mesh.colorData, info.name + "color");
	Serialise<OMFConverter::FaceInfo>(mesh.faceData, info.name + "face");
}

// model serialisation fucntions
void Archiver::Serialise(OMFConverter::ModelInfo& model, const Archiver::var_info& info)
{
	Serialise<OMFConverter::Mesh>(model.meshData, info.name + "meshData");
	*g_filelog << "Finished serialising mesh data....\n";
	Serialise<OMFConverter::Material>(model.materials, info.name + "materials");
	*g_filelog << "Finished serialising material data......\n";
}
