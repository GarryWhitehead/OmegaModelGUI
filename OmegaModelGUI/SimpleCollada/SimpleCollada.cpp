#include "SimpleCollada.h"
#include "SimpleCollada/ColladaSkeleton.h"
#include "SimpleCollada/ColladaVertices.h"
#include "SimpleCollada/ColladaMaterials.h"
#include "SimpleCollada/ColladaAnimation.h"

SimpleCollada::SimpleCollada() 
{
}


SimpleCollada::~SimpleCollada()
{
}

bool SimpleCollada::OpenColladaFile(std::string filename)
{
	m_xmlParse = new XMLparse();
	m_xmlParse->LoadXMLDocument(filename);

	if (m_xmlParse->ReportErrors() == ErrorFlags::XMLP_ERROR_UNABLE_TO_OPEN_FILE) {

		return false;
	}

	return true;
}

void SimpleCollada::ImportColladaData(OMFConverter::ModelInfo *model)
{
	m_sceneData = new SceneData;
	
	// start with library animation data
	/*m_sceneData->animation = new ColladaAnimation(m_xmlParse);
	m_sceneData->animation->ImportLibraryAnimations();

	// skeleton data
	m_sceneData->skeleton= new ColladaSkeleton(m_xmlParse);
	m_sceneData->skeleton->ImportSkeleton();*/

	// vertex data
	ColladaVertices *p_vertices = new ColladaVertices(m_xmlParse);
	p_vertices->ImportVertices(model);

	// material data
	ColladaMaterials *p_materials = new ColladaMaterials(m_xmlParse);
	p_materials->ImportMaterials(model);

}







