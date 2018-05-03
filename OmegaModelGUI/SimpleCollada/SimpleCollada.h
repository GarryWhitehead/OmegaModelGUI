#pragma once
#include "XMLparse.h"
#include "OMFConverter/OMFConverter.h"
#include <vector>
#include <string>
#include "SimpleCollada/ColladaSkeleton.h"
#include "SimpleCollada/ColladaVertices.h"
#include "SimpleCollada/ColladaMaterials.h"
#include "SimpleCollada/ColladaAnimation.h"

class SimpleCollada
{
public:

	struct SceneData
	{
		ColladaAnimation* animation;
		ColladaSkeleton *skeleton;
	};

	SimpleCollada();
	~SimpleCollada();

	bool OpenColladaFile(std::string filename);
	void ImportColladaData(OMFConverter::ModelInfo *model);
	
private:

	XMLparse *m_xmlParse;

	SceneData *m_sceneData;
};



