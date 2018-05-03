#pragma once
#include "ObjUtility.h"
#include "OMFConverter/OMFConverter.h"
#include <string>
#include <vector>
#include <fstream>
#include <tuple>
#include <set>
#include <map>
#include "glm.hpp"

class MaterialParser;

class Objector
{
public:

	Objector();
	~Objector();

	Objector(Objector&) = delete;
	Objector& operator=(Objector& obj) = delete;
	Objector& operator=(Objector&& obj) = delete;

	// core functions
	void ImportObjFile(std::string filename, OMFConverter::ModelInfo *model);

	// helper functions
	bool isObjImported() const { return m_objFileImported; }
	
private:

	// private member functions
	void CreateMesh(std::string name, OMFConverter::ModelInfo *model, std::fstream& file);
	void ProcessGroup(OMFConverter::Mesh& mesh, OMFConverter::ModelInfo *model, std::fstream& file);
	void ProcessIndicies(std::string str, OMFConverter::FaceInfo& face, OMFConverter::Mesh& mesh);

	MaterialParser *materialParser;
	ObjUtility objUtility;

	// temporary vectors for storing data pulled from file
	std::vector<glm::vec3> m_tempPos;
	std::vector<glm::vec2> m_tempUv;
	std::vector<glm::vec3> m_tempNorm;
	std::vector<glm::vec3> m_tempColor;
	std::map<std::tuple<int, int, int>, uint32_t> m_indexRef;		// tuple - pos, uv, norm index
	uint32_t m_nextIndex;

	bool m_objFileImported;

};

