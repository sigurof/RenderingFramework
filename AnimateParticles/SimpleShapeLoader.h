#ifndef SIMPLESHAPELOADER_H
#define SIMPLESHAPELOADER_H
#pragma once


namespace ML {

	class SimpleShapeLoader
	{
	public:
		SimpleShapeLoader() {};

		static VAOHandle loadSimpleShapeModel(Loader loader) {
			
		}


	private:

		static void processFaces(std::vector<std::vector<std::string>>& faceInfos,
			std::vector<unsigned int>& indices, std::vector<glm::vec2>& textures,
			std::vector<glm::vec3>& normals,
			std::vector<float>& texturesArray,
			std::vector<float>& normalsArray)
		{

			for each (std::vector<std::string> faceInfo in faceInfos)// Face
			{
				for (int i = 1; i < faceInfo.size(); i++)
				{
					std::istringstream vertexStream(faceInfo[i]);
					std::string vtn_string;
					std::vector<unsigned int> vtn; /* vertex texture normal */
					int j(0);
					while (std::getline(vertexStream, vtn_string, '/')) {
						j++; vtn.push_back(std::stoi(vtn_string));
					}
					unsigned int currentVertexIndex = vtn[0] - 1; indices.push_back(currentVertexIndex);

					glm::vec2 currentTex = textures[vtn[1] - 1];
					texturesArray[currentVertexIndex * 2 + 0] = currentTex.x;
					texturesArray[currentVertexIndex * 2 + 1] = 1 - currentTex.y;

					glm::vec3 currentNorm = normals[vtn[2] - 1];
					normalsArray[currentVertexIndex * 3 + 0] = currentNorm.x;
					normalsArray[currentVertexIndex * 3 + 1] = currentNorm.y;
					normalsArray[currentVertexIndex * 3 + 2] = currentNorm.z;

				}
			}
		}




	};

}
#endif // SIMPLESHAPELOADER_H
