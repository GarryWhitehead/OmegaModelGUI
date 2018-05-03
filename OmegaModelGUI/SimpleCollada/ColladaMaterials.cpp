#include "ColladaMaterials.h"
#include "SimpleCollada/XMLparse.h"
#include "SimpleCollada/SimpleCollada.h"


ColladaMaterials::ColladaMaterials(XMLparse *parse) :
	m_xmlParse(parse)
{
}


ColladaMaterials::~ColladaMaterials()
{
}

void ColladaMaterials::ImportMaterials(OMFConverter::ModelInfo *model)
{
	ImportLibraryMaterials(model);
	ImportLibraryImages();
}

void ColladaMaterials::ImportLibraryImages()
{
	libraryImageData.clear();
	XMLPbuffer buffer;

	// begin by reading geomtry data into a buffer
	buffer = m_xmlParse->ReadTreeIntoBuffer("library_images");
	if (m_xmlParse->ReportErrors() == ErrorFlags::XMLP_INCORRECT_FILE_FORMAT) {

		return;
	}

	uint32_t index = 0;
	while (index < buffer.size()) {

		index = m_xmlParse->FindElementInBuffer("image", buffer, index);
		if (index == UINT32_MAX) {
			break;
		}
		std::string filename = m_xmlParse->ReadElementDataString("id", buffer, index++);
		libraryImageData.push_back(filename);
	}

	if (libraryImageData.empty()) {

		// let the user know that no image file was found

	}
}

void ColladaMaterials::ImportLibraryMaterials(OMFConverter::ModelInfo *model)
{
	// first link materail id with face and get a count of the number of materials

	XMLPbuffer buffer;

	buffer = m_xmlParse->ReadTreeIntoBuffer("library_materials");
	if (m_xmlParse->ReportErrors() == ErrorFlags::XMLP_INCORRECT_FILE_FORMAT) {

		return;
	}

	// TODO: not using a lot of the information from the file. Will add when needed.
	// Now import the material data
	uint32_t index = 0;
	XMLPbuffer effectsBuffer;
	effectsBuffer = m_xmlParse->ReadTreeIntoBuffer("library_effects");
	if (m_xmlParse->ReportErrors() == ErrorFlags::XMLP_INCORRECT_FILE_FORMAT) {

		return;
	}

	while (index < effectsBuffer.size()) {

		OMFConverter::Material material;

		uint32_t startIndex = m_xmlParse->FindElementInBuffer("effect", effectsBuffer, index);
		if (startIndex == UINT32_MAX) {
			break;
		}
		material.name = m_xmlParse->ReadElementDataString("id", effectsBuffer, startIndex);

		// ambient
		index = m_xmlParse->FindChildElementInBuffer("effect", "ambient", effectsBuffer, startIndex);
		if (index != UINT32_MAX) {
			material.Color.ambient = m_xmlParse->ReadElementDataVec<glm::vec4>("color", effectsBuffer, index + 1);
		}
		
		// diffuse / albedo
		index = m_xmlParse->FindChildElementInBuffer("effect", "diffuse", effectsBuffer, startIndex);
		if (index != UINT32_MAX) {
			if (m_xmlParse->CheckElement("texture", effectsBuffer, index + 1)) {
				material.Map.albedo = m_xmlParse->ReadElementDataString("texture", effectsBuffer, index + 1);
			}
			else {
				material.Color.diffuse = m_xmlParse->ReadElementDataVec<glm::vec4>("color", effectsBuffer, index + 1);
			}
		}

		// normal
		index = m_xmlParse->FindChildElementInBuffer("effect", "normal", effectsBuffer, startIndex);
		if (index != UINT32_MAX) {
			if (m_xmlParse->CheckElement("texture", effectsBuffer, index + 1)) {
				material.Map.normal = m_xmlParse->ReadElementDataString("texture", effectsBuffer, index + 1);
			}
		}
		
		// specular
		index = m_xmlParse->FindChildElementInBuffer("effect", "specular", effectsBuffer, startIndex);
		if (index != UINT32_MAX) {
			material.Color.specular = m_xmlParse->ReadElementDataVec<glm::vec4>("color", effectsBuffer, index + 1);
		}
		else {
			material.Color.specular = glm::vec4(1.0f);
		}

		// shininess
		index = m_xmlParse->FindChildElementInBuffer("effect", "shininess", effectsBuffer, startIndex);
		if (index != UINT32_MAX) {
			material.Color.shininess = m_xmlParse->ReadElementDataFloat("float", effectsBuffer, index + 1);
		}
		else {
			material.Color.shininess = 0.0f;
		}

		// transparency
		index = m_xmlParse->FindChildElementInBuffer("effect", "transparency", effectsBuffer, startIndex);
		if (index != UINT32_MAX) {
			material.Color.transparency = m_xmlParse->ReadElementDataFloat("float", effectsBuffer, index + 1);
		}
		else {
			material.Color.transparency = 0.0f;
		}

		model->materials.push_back(material);
		index = startIndex + 1;
	}

	// now chnage the name of used in library effects to the material name used by the vertices
	index = 0;

	for (auto mat : model->materials) {

		index = m_xmlParse->FindElementInBuffer("material", buffer, index);
		std::string url = m_xmlParse->ReadElementDataString("url", buffer, index + 1);

		if (url == mat.name) {
			mat.name = m_xmlParse->ReadElementDataString("id", buffer, index);
		}
		++index;
	}
	
}
