#pragma once
#include <vector>
#include "glm.hpp"

class Archiver;

enum class MatType
{
	MAT_DIFFUSE,
	MAT_SPECULAR,
	MAT_NORMAL,
	MAT_NONE
};

class OMFConverter
{

public:

	// generic vertex data structs for all file formats
	struct FaceInfo
	{
		std::string material;
		std::vector<uint32_t> indices;

		// helper functions
		uint32_t numIndices() const { return indices.size(); }
	};

	struct Mesh
	{
		std::string meshName;
		std::string material;
		
		std::vector<glm::vec3> posData;
		std::vector<glm::vec3> normData;
		std::vector<glm::vec2> uvData;
		std::vector<glm::vec3> colorData;
		std::vector<FaceInfo> faceData;
	};

	struct Material
	{
		Material() : index(0) {}

		struct MatColor
		{
			glm::vec3 ambient;
			glm::vec3 diffuse;
			glm::vec3 specular;
			float shininess;
			float transparency;
		} Color;

		struct TexMap
		{
			std::string albedo;
			std::string specular;
			std::string normal;
			std::string ao;
			std::string metallic;
			std::string roughness;
		} Map;

		float opacity;
		uint32_t illumination;

		std::string name;
		uint32_t index;

		// helper functions
		std::string GetMaterialType(MatType type) const;
		bool hasTexture(MatType type) const;
		bool hasTexture() const;
	};

	// skinning data for files that contain animation info
	struct WeightInfo
	{
		uint32_t vertexId;
		float weight;
	};

	struct Node
	{
		Node() :
			numChildren(0)
		{}

		std::string name;
		std::string sid;
		glm::mat4 transform;
		int32_t parentIndex;
		std::vector<uint32_t> childrenInd;
		std::vector<uint32_t> numChildren;

		std::vector<WeightInfo> weights;

		//helper functions
		uint32_t numWeights() const { return weights.size(); }
	};

	struct SkeletonTree
	{
		std::vector<Node> bones;

		//helper functions
		glm::mat4 GetLocalTransform(std::string name)
		{
			for (uint32_t c = 0; c < bones.size(); ++c) {

				if (bones[c].sid == name) {

					return bones[c].transform;
				}
			}
		}

	} skeletonData;

	struct ModelInfo
	{
		ModelInfo() :
			numMaterials(0),
			numMeshes(0)
		{}

		std::vector<Mesh> meshData;
		std::vector<Material> materials;

		uint32_t numMaterials;
		uint32_t numMeshes;
	};

	struct LibraryAnimationFloatInfo
	{
		std::vector<float> time;
		std::vector<float> transform;
		std::vector<std::string> interpolation;
	};

	struct LibraryAnimation4X4Info
	{
		std::vector<float> time;
		std::vector<glm::mat4> transform;
		std::vector<std::string> interpolation;
	};

	OMFConverter();
	~OMFConverter();

	void SetDestinationFilename(std::string filename);
	void ConvertFileToOMF(std::string source);
	void ConvertObjFile(std::string filename);
	void ConvertColladaFile(std::string filename);
	void SerialiseModelData();

private:

	std::vector<ModelInfo> m_model;

	Archiver *p_archive;

};

