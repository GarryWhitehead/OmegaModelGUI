#include "OMFConverter.h"
#include "serialisation.h"
#include "file_log.h"
#include "Objector/objector.h"

OMFConverter::OMFConverter()
{
	p_archive = new Archiver();
}

OMFConverter::~OMFConverter()
{
	if (p_archive != nullptr) {

		delete p_archive;
	}
}

void OMFConverter::SetDestinationFilename(std::string filename)
{
	p_archive->OpenFile(filename, SaveMode::SAVE_TEXT, FileMode::FILE_OUT);
}

void OMFConverter::ConvertFileToOMF(std::string source)
{
	// check filename extension to obtain file format
	
	size_t pos = source.find(".obj");
	if (pos != std::string::npos) {

		ConvertObjFile(source);
	}

	pos = source.find(".dae");
	if (pos != std::string::npos) {

		ConvertColladaFile(source);
	}
}

void OMFConverter::ConvertObjFile(std::string filename)
{
	*g_filelog << "importing .obj file: " << filename << "........\n";

	// import .obj file into OMF format
	Objector *p_objector = new Objector();
	OMFConverter::ModelInfo model;

	p_objector->ImportObjFile(filename, &model);

	m_model.push_back(model);

	delete p_objector;
}

void OMFConverter::SerialiseModelData()
{
	// serialise OMF converted file to file
	p_archive->Serialise<OMFConverter::ModelInfo>(m_model, Archiver::var_info(""));

	*g_filelog << "Completed serialising model data to destination.\n";
}

void OMFConverter::ConvertColladaFile(std::string filename)
{

}

std::string OMFConverter::Material::GetMaterialType(MatType type) const
{
	std::string filename;
	switch (type) {
	case MatType::MAT_DIFFUSE:
		filename = Map.albedo;
		break;
	case MatType::MAT_NORMAL:
		filename = Map.normal;
		break;
	}
	return filename;
}

bool OMFConverter::Material::hasTexture(MatType type) const
{
	switch (type) {
	case MatType::MAT_DIFFUSE:
		return !Map.albedo.empty();
		break;
	case MatType::MAT_NORMAL:
		return !Map.normal.empty();
		break;
	}
}

bool OMFConverter::Material::hasTexture() const
{
	// all materials must have a diffuse texture
	if (Map.albedo.empty()) {

		return false;
	}

	return true;
}

