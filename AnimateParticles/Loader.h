#ifndef LOADER_H
#define LOADER_H
#pragma once


namespace ML {

	class Loader
	{
	public:

		~Loader() {
			for (unsigned int i = 0; i < vaos.size(); i++)
			{
				delete vaos[i];
			}
			for (unsigned int i = 0; i < vbos.size(); i++)
			{
				delete vbos[i];
			}
			for (unsigned int i = 0; i < textures.size(); i++)
			{
				delete textures[i];
			}
		};

		VAOHandle loadOBJFile(const std::string& filePath)
		{
			try
			{
				Clock timer;
				timer.start();
				std::vector<float> verticesArray;
				std::vector<float> texturesArray;
				std::vector<float> normalsArray;

				std::ifstream file(filePath);
				if (!file.good()) { throw(std::exception("ERROR: Could not open file\n")); }
				std::string line;
				std::vector<glm::vec3> vertices;
				std::vector<glm::vec2> textures;
				std::vector<glm::vec3> normals;
				std::vector<unsigned int> indices;
				std::vector<std::vector<std::string>> faceInfo;

				while (std::getline(file, line))	/* Parse lines */
				{
					std::istringstream iss(line);
					std::vector<std::string> words; std::string word; int i = 0;
					while (iss >> word) { words.push_back(word); }
					if (words[0] == "v")//vertex
					{
						vertices.push_back(glm::vec3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3])));
					}
					else if (words[0] == "vt")//vertex texture
					{
						textures.push_back(glm::vec2(std::stof(words[1]), std::stof(words[2])));
					}
					else if (words[0] == "vn")//vertex normal
					{
						normals.push_back(glm::vec3(std::stof(words[1]), std::stof(words[2]), std::stof(words[3])));
					}
					else if (words[0] == "f")//face
					{
						faceInfo.push_back(words);
					}
				}
				file.close();

				processFaces(faceInfo, vertices, textures, normals, indices, verticesArray, texturesArray, normalsArray);

				std::cout << "Object loaded in " << timer.elapsedMs() << "ms.\n";
				return this->loadToVAO(verticesArray, texturesArray, normalsArray, indices);
			}
			catch (const std::exception& err)
			{
				std::cout << "In loadFromFile\n";
				throw err;
			}
		}


		//VAOHandle loadObjModel(const std::string& filePath)
		//{
		//	try
		//	{
		//		return loadFromFile(filePath);
		//	}
		//	catch (const std::exception& err)
		//	{
		//		std::cout << "In OBJLoader::loadObjModel\n";
		//		throw err;
		//	}
		//}
		
		VAOHandle loadToVAO(const std::vector<float>& vertices, const std::vector<float>& textureCoords, const std::vector<float>& normals, const std::vector<unsigned int>& indices) {
			unsigned int* vaoID = createVAO();
			vaos.push_back(vaoID);	
			bindIndicesBuffer(indices);
			storeDataInAttributeList(0, 3, vertices);
			storeDataInAttributeList(1, 2, textureCoords);
			storeDataInAttributeList(2, 3, normals);
			unbindVAO();
			return VAOHandle(*vaoID, (unsigned int)indices.size());
		}

		VAOHandle loadToVAO(const std::shared_ptr<VertexData> data) 
		{
			unsigned int index = 0;
			unsigned int* vaoID = createVAO();
			vaos.push_back(vaoID);
			bindIndicesBuffer(data->getIndices());
			if (data->getVertices().size() > 0)
			{
				storeDataInAttributeList(index, 3, data->getVertices());
				index++;
			}
			if (data->getTextureCoords().size() > 0)
			{
				storeDataInAttributeList(index, 2, data->getTextureCoords());
				index++;
			}
			if (data->getNormals().size() > 0)
			{
				storeDataInAttributeList(index, 3, data->getNormals());
				index++;
			}
			unbindVAO();
			return VAOHandle(*vaoID, (unsigned int)data->getIndices().size());
		}

		//VAOHandle load_VN_ToVAO(std::vector<float>& vertices, std::vector<float>& normals, std::vector<unsigned int>& indices) {
		//	unsigned int* vaoID = createVAO();
		//	vaos.push_back(vaoID);
		//	bindIndicesBuffer(indices);
		//	storeDataInAttributeList(0, 3, vertices);
		//	storeDataInAttributeList(1, 2, textureCoords);
		//	storeDataInAttributeList(2, 3, normals);
		//	unbindVAO();
		//	return VAOHandle(*vaoID, (unsigned int)indices.size());
		//}

		unsigned int loadTexture(std::string& path, ImageFormatEnum format) {
			Texture texture(path, format);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			unsigned int *textureID = new unsigned int(texture.getID());
			textures.push_back(textureID);

			return *textureID;
		}

		void bindIndicesBuffer(const std::vector<unsigned int>& indices) 
		{
			try
			{
				if (indices.size() == 0)
				{
					throw std::exception("ERROR! indices must have non-zero length!");
				}
				unsigned int* eboID = new unsigned int;
				vbos.push_back(eboID);
				glGenBuffers(1, eboID);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *eboID);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
			}
			catch (const std::exception& err)
			{
				std::cout << "In Loader::bindIndicesBuffer\n";
				throw err;
			}
		}

		unsigned int* createVAO() {
			unsigned int* vaoID = new unsigned int;
			glGenVertexArrays(1, vaoID);
			glBindVertexArray(*vaoID);
			return vaoID;
		}

		template <class Type>
		void storeDataInAttributeList(unsigned int attribIndex, unsigned int coordSize, const std::vector<Type>& data) {
			unsigned int* vboID = new unsigned int;
			glGenBuffers(1, vboID);
			vbos.push_back(vboID);
			glBindBuffer(GL_ARRAY_BUFFER, *vboID);
			glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(attribIndex, coordSize, GL_FLOAT, GL_FALSE, coordSize*sizeof(Type), (void*)0);//Fifth argument should be kept at 0 if only one attribute is fed in
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		void unbindVAO() {
			glBindVertexArray(0);
		}

		void cleanUp() {
			/* TODO: Perhaps store in array and use
			glDeleteXXX(size of array, array)        */
			for (int i = 0; i < vaos.size(); i++)
			{
				glDeleteVertexArrays(1, vaos[i]);
			}
			for (int i = 0; i < vbos.size(); i++)
			{
				glDeleteBuffers(1, vbos[i]);
			}
			for (int i = 0; i < textures.size(); i++)
			{
				glDeleteTextures(1, textures[i]);
			}
		}

	private:
		std::vector<unsigned int*> vaos;
		std::vector<unsigned int*> vbos;
		std::vector<unsigned int*> textures;

		
		static void processFaces(std::vector<std::vector<std::string>>& faceInfos,
			std::vector<glm::vec3>& vertices,
			std::vector<glm::vec2>& textures,
			std::vector<glm::vec3>& normals,
			std::vector<unsigned int>& indices,
			std::vector<float>& verticesArray,
			std::vector<float>& texturesArray,
			std::vector<float>& normalsArray)
		{
			try
			{
				unsigned int index = 0;
				//TripleMap<int, int, int, int> vtn_vs_index;
				std::map<std::vector<int>, int> vtn_vs_index;
				for each (std::vector<std::string> faceInfo in faceInfos)// Face
				{
					for (int i = 1; i < faceInfo.size(); i++)
					{
						std::istringstream vertexStream(faceInfo[i]);
						std::string vtn_string;
						std::vector<int> vtn; /* vertex texture normal */
						int j(0);
						while (std::getline(vertexStream, vtn_string, '/')) { j++; vtn.push_back(std::stoi(vtn_string)); }
						if (!vtn_vs_index[vtn])
						{
							vtn_vs_index[vtn] = index;
							index++;

							verticesArray.push_back((vertices[vtn[0] - 1]).x);
							verticesArray.push_back((vertices[vtn[0] - 1]).y);
							verticesArray.push_back((vertices[vtn[0] - 1]).z);

							texturesArray.push_back(textures[vtn[1] - 1].x);
							texturesArray.push_back(1 - textures[vtn[1] - 1].y);

							normalsArray.push_back(normals[vtn[2] - 1].x);
							normalsArray.push_back(normals[vtn[2] - 1].y);
							normalsArray.push_back(normals[vtn[2] - 1].z);
						}
						indices.push_back(vtn_vs_index[vtn]);


					}
				}
			}
			catch (const std::exception& err)
			{
				std::cout << "In processFaces\n";
				throw err;
			}

		}


	};
}



#endif // !LOADER_H

