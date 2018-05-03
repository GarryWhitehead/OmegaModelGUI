#pragma once
#include <vector>
#include "SimpleCollada/XMLparse.h"
#include "SimpleCollada/ColladaSKeleton.h"
#include "OMFConverter/OMFConverter.h"
#include <map>


enum class MeshType
{
	MESH_TRIANGLE,
	MESH_POLYLIST
};

class ColladaVertices
{
public:

	struct FaceInfo
	{
		MeshType type;
		std::string material;
		std::vector<uint32_t> indices;	

		uint32_t numIndices() const { return indices.size(); }
	};

	struct MeshInfo
	{
		std::string id;
		uint32_t count;
		uint8_t offset;
	};

	struct GeometryInfo
	{
		MeshInfo vertex;
		MeshInfo normal;
		MeshInfo texcoord;
	};

	struct TempWeightInfo
	{
		uint32_t boneIndex;
		float weight;
	};

	struct SkinningInfo
	{
		glm::mat4 bindShape;
		std::vector<glm::mat4> invBind;
		std::vector<std::string> joints;

		// helper functions
		uint32_t FindBone(std::string name)
		{
			for (uint32_t c = 0; c < joints.size(); ++c) {
				if (joints[c] == name) {
					return c;
				}
			}
			return 0;
		}
	};

	ColladaVertices(XMLparse *parse);
	~ColladaVertices();

	void ImportVertices(OMFConverter::ModelInfo *model);
	
	// main vertices buffers open to the public
	std::vector<SkinningInfo> skinData;

private:

	uint32_t ProcessGeometryData(XMLPbuffer& buffer, uint32_t index, MeshType type, uint32_t& meshIndex, OMFConverter::ModelInfo *model);
	uint32_t ProcessControllers(XMLPbuffer& buffer, uint32_t index, ColladaSkeleton *p_skeleton);
	void ProcessVertices(uint32_t meshIndex, OMFConverter::ModelInfo *model);
	void ClearTempBuffers();

	XMLparse *m_xmlParse;
	GeometryInfo m_geometryInfo;

	//bone weights related stuff
	std::vector<float> m_weights;

	// temporary buffers to store vertex info, before creating unique indices data
	std::vector<glm::vec3> m_tempPosition;
	std::vector<glm::vec3> m_tempNormal;
	std::vector<glm::vec2> m_tempTexcoord;
	std::vector<std::vector<uint32_t> > m_tempIndices;
	std::map<uint32_t, std::vector<TempWeightInfo> > m_tempWeights;		// temp weights for each mesh
	
	// used to store unique indices
	std::map<std::tuple<int, int, int>, uint32_t> m_indexRef;		// tuple - pos, uv, norm index
	uint32_t m_nextFreeIndex;

	bool m_generateFace;
	bool useSetTwoForUv;
};

