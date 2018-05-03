#include "ColladaVertices.h"

ColladaVertices::ColladaVertices(XMLparse *parse) :
	m_xmlParse(parse),
	useSetTwoForUv(true)
{
}

ColladaVertices::~ColladaVertices()
{
}

void ColladaVertices::ImportVertices(OMFConverter::ModelInfo *model)
{
	// begin by reading geomtry data into a buffer
	XMLPbuffer geomBuffer;
	geomBuffer = m_xmlParse->ReadTreeIntoBuffer("library_geometries");
	if (m_xmlParse->ReportErrors() == ErrorFlags::XMLP_INCORRECT_FILE_FORMAT) {
		return;
	}

	// and then the skinning data
	XMLPbuffer conBuffer;
	conBuffer = m_xmlParse->ReadTreeIntoBuffer("library_controllers");
	if (m_xmlParse->ReportErrors() == ErrorFlags::XMLP_INCORRECT_FILE_FORMAT) {
		return;
	}

	std::string src;
	uint32_t geomIndex = 0;
	uint32_t nextGeomIndex = 0;
	uint32_t prevIndex = 0;
	uint32_t meshIndex = 0;

	while (geomIndex < geomBuffer.size()) {

		// for some reason, some exporters interchange the word vertices and position for ids, so check whether this is the case
		geomIndex = m_xmlParse->FindElementInBuffer("vertices", geomBuffer, geomIndex);
		if (geomIndex != UINT32_MAX) {
		
			++geomIndex;
			std::string semantic = m_xmlParse->ReadElementDataString("semantic", geomBuffer, geomIndex);
			if (semantic == "POSITION") {

				src = m_xmlParse->ReadElementDataString("source", geomBuffer, geomIndex);

				// first check whether the triangle or polylist is using the same set of vertices as the last set
				// if so, then treat this data as a face instead of a mesh
				if (m_geometryInfo.vertex.id == src) {
					m_generateFace = true;
				}
				else {
					
					m_geometryInfo.vertex.id = src;
					m_generateFace = false;
				}
			}
		}
		// if there are no more vertices elements, there could still be potentially faces to process
		else if (m_geometryInfo.vertex.id == src) {
			m_generateFace = true;
		}
	

		// Get vertex information before importing data
		geomIndex = m_xmlParse->FindElementInBuffer("triangles", geomBuffer, geomIndex);			// files either contain triangles
		if (geomIndex != UINT32_MAX) {

			geomIndex = ProcessGeometryData(geomBuffer, geomIndex, MeshType::MESH_TRIANGLE, meshIndex, model);
			nextGeomIndex = geomIndex + 1;
		}
		else {
			// or polylists - though we are only allowing for triangulated data so the vcount must be 3
			geomIndex = prevIndex;
			geomIndex = m_xmlParse->FindElementInBuffer("polylist", geomBuffer, geomIndex);

			if (geomIndex != UINT32_MAX) {

				geomIndex = ProcessGeometryData(geomBuffer, geomIndex, MeshType::MESH_POLYLIST, meshIndex, model);
				prevIndex = geomIndex;
				nextGeomIndex = geomIndex + 1;
			}
			else {
				return;
			}
		}

		// now process the library controller for this mesh
		uint32_t conIndex = 0;
		conIndex = m_xmlParse->FindElementInBuffer("controller", conBuffer, conIndex++);			// find next controller
		if (conIndex == UINT32_MAX) {
			break;
		}

		//conIndex = ProcessControllers(conBuffer, conIndex, p_skeleton);
		
		// prepare the data for the user - condense indices into one uinqiue index for graphic API use and chnage bone weight vertex id accordingly
		ProcessVertices(meshIndex, model);
		geomIndex = nextGeomIndex;
	}
}

uint32_t ColladaVertices::ProcessGeometryData(XMLPbuffer& buffer, uint32_t bufferIndex, MeshType type, uint32_t& meshIndex, OMFConverter::ModelInfo *model)
{
	std::string semantic;
	uint32_t uvSetCount = 1;

	uint32_t index = bufferIndex;
	++index;

	for (int c = 0; c < 3; ++c) {

		semantic = m_xmlParse->ReadElementDataString("semantic", buffer, index);
		if (!semantic.empty()) {

			if (semantic == "VERTEX") {

				if (m_geometryInfo.vertex.id.empty()) {
					m_geometryInfo.vertex.id = m_xmlParse->ReadElementDataString("source", buffer, index);
				}
				m_geometryInfo.vertex.offset = m_xmlParse->ReadElementDataInt("offset", buffer, index++);
			}
			else if (semantic == "NORMAL") {
				m_geometryInfo.normal.id = m_xmlParse->ReadElementDataString("source", buffer, index);
				m_geometryInfo.normal.offset = m_xmlParse->ReadElementDataInt("offset", buffer, index++);
			}
			else if (semantic == "TEXCOORD") {
				if (useSetTwoForUv) {
					if (m_xmlParse->CheckChildElement("source", buffer, index + 1)) {		// check that a second UV set exsists
						uvSetCount = 2;
						++index;
					}
				}
				m_geometryInfo.texcoord.id = m_xmlParse->ReadElementDataString("source", buffer, index);
				m_geometryInfo.texcoord.offset = m_xmlParse->ReadElementDataInt("offset", buffer, index++);
			}
		}
	}

	index = bufferIndex;
	uint32_t count = 0;

	if (!m_generateFace) {

		// Read data into buffers - TODO : check counts and stride values of each data set
		index = m_xmlParse->FindElementInBufferIterateBack("mesh", buffer, index);
		if (index == UINT32_MAX) {
			return index;
		}

		// if the set is being treated as a face and using the vertices from the previous read, don't clear the temp buffers
		// if this is a new set of vertices and being treated as a mesh, then clear buffers and generate new vertex data
		ClearTempBuffers();

		for (int c = 0; c < 3; ++c) {

			index = m_xmlParse->FindElementInBuffer("source", buffer, index);
			++index;

			std::string sourceId = m_xmlParse->ReadElementDataString("id", buffer, index);
			if (sourceId == m_geometryInfo.vertex.id + "-array") {

				count = m_xmlParse->ReadElementDataInt("count", buffer, index);
				index = m_xmlParse->ReadElementArrayVec<glm::vec3>(buffer, m_tempPosition, index, count, VectorType::VEC3_TYPE);
				assert(!m_tempPosition.empty());
			}
			else if (sourceId == m_geometryInfo.normal.id + "-array") {

				count = m_xmlParse->ReadElementDataInt("count", buffer, index);
				index = m_xmlParse->ReadElementArrayVec<glm::vec3>(buffer, m_tempNormal, index, count, VectorType::VEC3_TYPE);
				assert(!m_tempNormal.empty());
			}
			else if (sourceId == m_geometryInfo.texcoord.id + "-array") {

				count = m_xmlParse->ReadElementDataInt("count", buffer, index);
				index = m_xmlParse->ReadElementArrayVec<glm::vec2>(buffer, m_tempTexcoord, index, count, VectorType::VEC2_TYPE);		// TODO:: texcoords can be found as either vec3 or vec2. Need to check the stride and parse as appropiate
				assert(!m_tempTexcoord.empty());
			}
			else if (useSetTwoForUv && uvSetCount > 1) {
	
				index = m_xmlParse->FindElementInBuffer("source", buffer, index);
				++index;
				
				sourceId = m_xmlParse->ReadElementDataString("id", buffer, index);
				if(sourceId == m_geometryInfo.texcoord.id + "-array") {

					count = m_xmlParse->ReadElementDataInt("count", buffer, index);
					index = m_xmlParse->ReadElementArrayVec<glm::vec2>(buffer, m_tempTexcoord, index, count, VectorType::VEC2_TYPE);		// TODO:: texcoords can be found as either vec3 or vec2. Need to check the stride and parse as appropiate
					assert(!m_tempTexcoord.empty());
				}
			}
		}
	}

	// And finally the indices data - start by checking whether triangle mesh data is present
	index = bufferIndex;

	if (type == MeshType::MESH_TRIANGLE) {

		OMFConverter::FaceInfo face;
		//face.material = m_xmlParse->ReadElementDataString("material", buffer, index);
		uint32_t count = m_xmlParse->ReadElementDataInt("count", buffer, index);
		count *= 9;			// dealing with triangles - so 3 vertices per count * stride of 3

		index += 4;
		std::vector<uint32_t> indices;
		index = m_xmlParse->ReadElementArray<uint32_t>(buffer, indices, index, count);
		m_tempIndices.push_back(indices);

		// if not generating a face, create a new mesh
		if(!m_generateFace) {

			OMFConverter::Mesh mesh;
			mesh.faceData.push_back(face);
			model->meshData.push_back(mesh);
			++meshIndex;
		}
		else {

			// otherwise add indices to exsiting mesh
			model->meshData[meshIndex - 1].faceData.push_back(face);
		}
	}

	else if (type == MeshType::MESH_POLYLIST) {
		// then check for polylists - these contain a vcount (number of indices to count) and p datasets

		OMFConverter::FaceInfo face;
		std::string matName = m_xmlParse->ReadElementDataString("material", buffer, index);
		uint32_t count = m_xmlParse->ReadElementDataInt("count", buffer, index++);

		index = m_xmlParse->FindElementInBuffer("vcount", buffer, index);
		std::vector<uint32_t> vcount;																	// not actually using this as only trianglulated data allowed so 3 assumed
		index = m_xmlParse->ReadElementArray<uint32_t>(buffer, vcount, index, XMLparse::WHOLE_ARRAY);
		
		++index;
		std::vector<uint32_t> indices;
		index = m_xmlParse->ReadElementArray<uint32_t>(buffer, indices, index, XMLparse::WHOLE_ARRAY);
		m_tempIndices.push_back(indices);

		if (!m_generateFace) {

			OMFConverter::Mesh mesh;
			mesh.faceData.push_back(face);
			model->meshData.push_back(mesh);
			++meshIndex;
		}
		else {

			// otherwise add indices to exsiting mesh
			model->meshData[meshIndex - 1].faceData.push_back(face);
		}
	}
	return index;
}

uint32_t ColladaVertices::ProcessControllers(XMLPbuffer& buffer, uint32_t bufferIndex, ColladaSkeleton *p_skeleton)
{
	uint32_t index = bufferIndex;
	std::string invBindSource, jointSource, weightSource;

	index = m_xmlParse->FindElementInBuffer("joints", buffer, index);
	index += 2;

	invBindSource = m_xmlParse->ReadElementDataString("source", buffer, index);

	index = m_xmlParse->FindElementInBuffer("vertex_weights", buffer, index);
	uint32_t vCount = m_xmlParse->ReadElementDataInt("count", buffer, index++);

	for (int c = 0; c < 2; ++c) {
		std::string semantic = m_xmlParse->ReadElementDataString("semantic", buffer, index);

		if (semantic == "JOINT") {
			jointSource = m_xmlParse->ReadElementDataString("source", buffer, index++);
		}
		else if (semantic == "WEIGHT") {
			weightSource = m_xmlParse->ReadElementDataString("source", buffer, index++);
		}
	}

	// now import vcount (vertex weight count) and v (vertex weight indicies)
	std::vector<uint32_t> m_vertCounts;
	std::vector<uint32_t> m_vertIndices;
	index = m_xmlParse->ReadElementArray<uint32_t>(buffer, m_vertCounts, index, vCount);
	index = m_xmlParse->ReadElementArray<uint32_t>(buffer, m_vertIndices, index + 1, XMLparse::WHOLE_ARRAY);

	// import the joints, inverse binding and weights arrays
	index = bufferIndex;
	std::string id;
	uint32_t arrayCount = 0;
	SkinningInfo skin;

	for (int c = 0; c < 3; ++c) {

		index = m_xmlParse->FindElementInBuffer("source", buffer, index);
		id = m_xmlParse->ReadElementDataString("id", buffer, index++);
		if (id == jointSource) {

			arrayCount = m_xmlParse->ReadElementDataInt("count", buffer, index);
			index = m_xmlParse->ReadElementArray<std::string>(buffer, skin.joints, index, arrayCount);
		}
		else if (id == invBindSource) {

			arrayCount = m_xmlParse->ReadElementDataInt("count", buffer, index);
			index = m_xmlParse->ReadElementArrayMatrix(buffer, skin.invBind, index, arrayCount);
		}
		else if (id == weightSource) {

			arrayCount = m_xmlParse->ReadElementDataInt("count", buffer, index);
			index = m_xmlParse->ReadElementArray<float>(buffer, m_weights, index, arrayCount);
		}
	}

	// now sort the data so it is more user friendly. Add bone wights to corresponding bones and vertex Id
	// load vertex bone data into a temporary buffer as the vertex Ids will be changing when we prepare the indices and vertices positions at the final stage
	uint32_t vertexId = 0;
	uint32_t indexCount = 0;
	for (uint32_t c = 0; c < m_vertCounts.size(); ++c) {

		std::vector<TempWeightInfo> weightInfo;

		for (uint32_t i = 0; i < m_vertCounts[c]; ++i) {

			TempWeightInfo weight;
			assert(indexCount < m_vertIndices.size());
			weight.weight = m_weights[m_vertIndices[indexCount + 1]];
			std::string boneId = skin.joints[m_vertIndices[indexCount]];		// joint index - offset = 0

			size_t pos = boneId.find('<');				// check whther the boneID contains the XML element end designator - TODO: actually implement this check in the array reader
			if (pos != std::string::npos) {
				boneId = boneId.substr(0, pos - 1);
			}

			uint32_t boneIndex = p_skeleton->FindBoneId(boneId);
			if (boneIndex == UINT32_MAX) {

				break;
			}
			weight.boneIndex = boneIndex;

			weightInfo.push_back(weight);
			indexCount += 2;					// stride of 2
		}

		m_tempWeights.insert(std::make_pair(vertexId, weightInfo));
		++vertexId;
	}
	skinData.push_back(skin);

	return index;
}

void ColladaVertices::ProcessVertices(uint32_t meshIndex, OMFConverter::ModelInfo *model)
{
	// Because openGL and Vulkan (and probably DirectX) only allow one set of indices and the data in the collada file is 
	// stored as two or three (assuming three for now) independent indices, we need to generate vertices for each unique set of indices
	// TODO : only dealing with <triangles>. also add support for polylists,etc.
	uint32_t faceIndex = 0;
	
	for (auto& indices : m_tempIndices) {

		uint32_t posIndex = 0, uvIndex = 0, normIndex = 0;

		for (uint32_t i = 0; i < indices.size(); i += 3) {

			// an assumption is made that offet 0 = position; offset 1 = normal; offest 2 = texcoord
			// TODO: actually check the layout of the indices and check the stride of the texcoord data
			posIndex = indices[i];
			normIndex = indices[i + 1];
			uvIndex = indices[i + 2];

			auto key = std::make_tuple(posIndex, normIndex, uvIndex);

			auto iter = m_indexRef.find(key);
			if (iter != m_indexRef.end()) {

				model->meshData[meshIndex - 1].faceData[faceIndex].indices.push_back(m_indexRef[key]);			// if key already exsists, no need to create a new vertex,
			}
			else {

				model->meshData[meshIndex - 1].posData.push_back(m_tempPosition[posIndex]);		// if unique key, add new vertex data
				model->meshData[meshIndex - 1].normData.push_back(m_tempNormal[normIndex]);
				model->meshData[meshIndex - 1].uvData.push_back(m_tempTexcoord[uvIndex]);

				model->meshData[meshIndex - 1].faceData[faceIndex].indices.push_back(m_nextFreeIndex);				// index this unique key in the indices buffer

				m_indexRef.insert(std::make_pair(key, m_nextFreeIndex));	// and add to index reference map 

																				// generate the bone weight map using the newly derived vertex id
				/*for (auto& temp : m_tempWeights[posIndex]) {

					ColladaSkeleton::WeightInfo info;
					info.vertexId = m_nextFreeIndex;
					info.weight = temp.weight;
					skeleton.bones[temp.boneIndex].weights.push_back(info);
				}*/
				++m_nextFreeIndex;
			}
		}
		++faceIndex;
	}
}

void ColladaVertices::ClearTempBuffers()
{
	// free temp buffers to save memory
	m_tempIndices.clear();
	m_tempPosition.clear();
	m_tempNormal.clear();
	m_tempTexcoord.clear();
	m_tempWeights.clear();
	m_indexRef.clear();
}


