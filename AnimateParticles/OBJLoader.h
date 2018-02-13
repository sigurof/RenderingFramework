//#ifndef OBJLOADER_H
//#define OBJLOADER_H
//#pragma once
//
//
//namespace ML {
//
//	class OBJLoader
//	{
//	public:
//		OBJLoader() {};
//
//		static VAOHandle loadObjModel(const std::string& filePath, Loader& loader)
//		{
//			try
//			{
//				return loadFromFile(filePath, loader);
//			}
//			catch (const std::exception& err)
//			{
//				std::cout << "In OBJLoader::loadObjModel\n";
//				throw err;
//			}
//		}
//
//
//	private:
//
//		//static VAOHandle loadSphere() {
//		//
//		//}
//
//
//		static VAOHandle loadFromFile(const std::string& filePath, Loader& loader)
//		{
//			try
//			{
//				Clock timer;
//				timer.start();
//				std::vector<float> verticesArray;
//				std::vector<float> texturesArray;
//				std::vector<float> normalsArray;
//
//				std::ifstream file(filePath);
//				if (!file.good()) { throw(std::exception("ERROR: Could not open file\n")); }
//				std::string line;
//				std::vector<glm::vec3> vertices;
//				std::vector<glm::vec2> textures;
//				std::vector<glm::vec3> normals;
//				std::vector<unsigned int> indices;
//				std::vector<std::vector<std::string>> faceInfo;
//
//				while (std::getline(file, line))	/* Parse lines */
//				{
//					std::istringstream iss(line);
//					std::vector<std::string> words; std::string word; int i = 0;
//					while (iss >> word) { words.push_back(word); }
//					if (words[0] == "v")//vertex
//					{
//						vertices.push_back(glm::vec3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3])));
//					}
//					else if (words[0] == "vt")//vertex texture
//					{
//						textures.push_back(glm::vec2(std::stof(words[1]), std::stof(words[2])));
//					}
//					else if (words[0] == "vn")//vertex normal
//					{
//						normals.push_back(glm::vec3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3])));
//					}
//					else if (words[0] == "f")//face
//					{
//						faceInfo.push_back(words);
//					}
//				}
//				file.close();
//
//				processFaces(faceInfo, vertices, textures, normals, indices, verticesArray, texturesArray, normalsArray);
//
//				std::cout << "Object loaded in " << timer.elapsedMs() << "ms.\n";
//				return loader.loadToVAO(verticesArray, texturesArray, normalsArray, indices);
//			}
//			catch (const std::exception& err)
//			{
//				std::cout << "In loadFromFile\n";
//				throw err;
//			}
//		}
//
//			static void processFaces(std::vector<std::vector<std::string>>& faceInfos,
//				std::vector<glm::vec3>& vertices,
//				std::vector<glm::vec2>& textures,
//				std::vector<glm::vec3>& normals,
//				std::vector<unsigned int>& indices,
//				std::vector<float>& verticesArray,
//				std::vector<float>& texturesArray,
//				std::vector<float>& normalsArray)
//			{
//				try
//				{
//					unsigned int index = 0;
//					//TripleMap<int, int, int, int> vtn_vs_index;
//					std::map<std::vector<int>, int> vtn_vs_index;
//					for each (std::vector<std::string> faceInfo in faceInfos)// Face
//					{
//						for (int i = 1; i < faceInfo.size(); i++)
//						{
//							std::istringstream vertexStream(faceInfo[i]);
//							std::string vtn_string;
//							std::vector<int> vtn; /* vertex texture normal */
//							int j(0);
//							while (std::getline(vertexStream, vtn_string, '/')) { j++; vtn.push_back(std::stoi(vtn_string)); }
//							if (!vtn_vs_index[vtn])
//							{
//								vtn_vs_index[vtn] = index;
//								index++;
//
//								verticesArray.push_back((vertices[vtn[0] - 1]).x);
//								verticesArray.push_back((vertices[vtn[0] - 1]).y);
//								verticesArray.push_back((vertices[vtn[0] - 1]).z);
//
//								texturesArray.push_back(textures[vtn[1] - 1].x);
//								texturesArray.push_back(1 - textures[vtn[1] - 1].y);
//
//								normalsArray.push_back(normals[vtn[2] - 1].x);
//								normalsArray.push_back(normals[vtn[2] - 1].y);
//								normalsArray.push_back(normals[vtn[2] - 1].z);
//							}
//							indices.push_back(vtn_vs_index[vtn]);
//
//
//						}
//					}
//				}
//				catch (const std::exception& err)
//				{
//					std::cout << "In processFaces\n";
//					throw err;
//				}
//				
//			}
//			
//
//		
//
//		//	static void processFaces2(std::vector<std::vector<std::string>>& faceInfos,
//		//		std::vector<unsigned int>& indices, std::vector<glm::vec2>& textures,
//		//		std::vector<glm::vec3>& normals,
//		//		std::vector<float>& texturesArray,
//		//		std::vector<float>& normalsArray)
//		//	{
//		//		for each (std::vector<std::string> faceInfo in faceInfos)// Face
//		//		{
//		//			for (int i = 1; i < faceInfo.size(); i++)
//		//			{
//		//				std::istringstream vertexStream(faceInfo[i]);
//		//				std::string vtn_string;
//		//				std::vector<unsigned int> vtn; /* vertex texture normal */
//		//				int j(0);
//		//				while (std::getline(vertexStream, vtn_string, '/')) {
//		//					j++; vtn.push_back(std::stoi(vtn_string));
//		//				}
//		//				unsigned int currentVertexIndex = vtn[0] - 1; indices.push_back(currentVertexIndex);
//		//				glm::vec2 currentTex = textures[vtn[1] - 1];
//		//				texturesArray[currentVertexIndex * 2 + 0] = currentTex.x;
//		//				texturesArray[currentVertexIndex * 2 + 1] = 1 - currentTex.y;
//		//				glm::vec3 currentNorm = normals[vtn[2] - 1];
//		//				normalsArray[currentVertexIndex * 3 + 0] = currentNorm.x;
//		//				normalsArray[currentVertexIndex * 3 + 1] = currentNorm.y;
//		//				normalsArray[currentVertexIndex * 3 + 2] = currentNorm.z;
//		//			}
//		//		}
//		//	}		
//		//}
//		//static VAOHandle loadFromFile2(const std::string& filePath, Loader& loader) 
//		//{
//		//	Clock timer;
//		//	timer.start();
//		//	std::vector<float> verticesArray;
//		//	std::vector<float> texturesArray;
//		//	std::vector<float> normalsArray;
//		//	std::ifstream file;
//		//	try
//		//	{
//		//		file.open(filePath);
//		//		if (!file)
//		//			throw(std::exception("ERROR: Could not open file\n"));
//		//	}
//		//	catch (std::ifstream::failure err2)
//		//	{
//		//		std::cout << "In loadObjModel (" << filePath << "):\n";
//		//		throw std::exception(err2.what());
//		//	}
//		//	catch (const std::exception& err1)
//		//	{
//		//		std::cout << "In loadObjModel::Constructor:\n";
//		//		throw(err1);
//		//	}
//		//	std::string line;
//		//	std::vector<glm::vec3> vertices;
//		//	std::vector<glm::vec2> textures;
//		//	std::vector<glm::vec3> normals;
//		//	std::vector<unsigned int> indices;
//		//	std::vector<std::vector<std::string>> faceInfo;
//		//	try
//		//	{
//		//		while (std::getline(file, line))	/* Parse lines */
//		//		{
//		//			std::istringstream iss(line);
//		//			std::vector<std::string> words; std::string word; int i = 0;
//		//			while (iss >> word) { words.push_back(word); }
//		//			if (words[0] == "v")//vertex
//		//			{
//		//				vertices.push_back(glm::vec3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3])));
//		//			}
//		//			else if (words[0] == "vt")//vertex texture
//		//			{
//		//				textures.push_back(glm::vec2(std::stof(words[1]), std::stof(words[2])));
//		//			}
//		//			else if (words[0] == "vn")//vertex normal
//		//			{
//		//				normals.push_back(glm::vec3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3])));
//		//			}
//		//			else if (words[0] == "f")//face
//		//			{
//		//				faceInfo.push_back(words);
//		//			}
//		//		}
//		//		file.close();
//		//		texturesArray = std::vector<float>(vertices.size() * 2, 0.0f);
//		//		normalsArray = std::vector<float>(vertices.size() * 3, 0.0f);
//		//		verticesArray = std::vector<float>(vertices.size() * 3, 0.0f);
//		//		processFaces(faceInfo, indices, textures, normals, texturesArray, normalsArray);
//		//	}
//		//	catch (const std::exception& err)
//		//	{
//		//		std::cout << "In loadObjModel (" << filePath << "):\n";
//		//		throw err;
//		//	}
//		//	int i = 0;
//		//	for each (glm::vec3 vertex in vertices)
//		//	{
//		//		verticesArray[i++] = vertex.x;
//		//		verticesArray[i++] = vertex.y;
//		//		verticesArray[i++] = vertex.z;
//		//	}
//		//	std::cout << "Object loaded in " << timer.elapsedMs() << "ms.\n";
//		//	return loader.loadToVAO(verticesArray, texturesArray, normalsArray, indices);
//		//}
//		//static void processFaces(std::vector<std::vector<std::string>>& faceInfos, 
//		//	std::vector<unsigned int>& indices, std::vector<glm::vec2>& textures, 
//		//	std::vector<glm::vec3>& normals,
//		//	std::vector<float>& texturesArray,
//		//	std::vector<float>& normalsArray) 
//		//{ 
//		//	for each (std::vector<std::string> faceInfo in faceInfos)// Face
//		//	{
//		//		for (int i = 1; i < faceInfo.size(); i++)
//		//		{
//		//			std::istringstream vertexStream(faceInfo[i]);
//		//			std::string vtn_string;
//		//			std::vector<unsigned int> vtn; /* vertex texture normal */
//		//			int j(0);
//		//			while (std::getline(vertexStream, vtn_string, '/')) {
//		//				j++; vtn.push_back(std::stoi(vtn_string));
//		//			}
//		//			unsigned int currentVertexIndex = vtn[0] - 1; indices.push_back(currentVertexIndex);
//		//			glm::vec2 currentTex = textures[vtn[1] - 1];
//		//			texturesArray[currentVertexIndex * 2 + 0] = currentTex.x;
//		//			texturesArray[currentVertexIndex * 2 + 1] = 1 - currentTex.y;
//		//			glm::vec3 currentNorm = normals[vtn[2] - 1];
//		//			normalsArray[currentVertexIndex * 3 + 0] = currentNorm.x;
//		//			normalsArray[currentVertexIndex * 3 + 1] = currentNorm.y;
//		//			normalsArray[currentVertexIndex * 3 + 2] = currentNorm.z;
//		//		}
//		//	}
//		//}
//	
//	
//	
//	
//	};
//
//}
//#endif // OBJLOADER_H
