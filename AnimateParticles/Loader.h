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
		
		MeshIdentifier loadToVAO(std::vector<float>& vertices, std::vector<float>& textureCoords, std::vector<float>& normals, std::vector<unsigned int>& indices) {
			unsigned int* vaoID = createVAO();
			vaos.push_back(vaoID);	
			bindIndicesBuffer(indices);
			storeDataInAttributeList(0, 3, vertices);
			storeDataInAttributeList(1, 2, textureCoords);
			storeDataInAttributeList(2, 3, normals);
			unbindVAO();
			return MeshIdentifier(*vaoID, (unsigned int)indices.size());
		}

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

		void bindIndicesBuffer(std::vector<unsigned int>& indices) {
			unsigned int* eboID = new unsigned int;
			vbos.push_back(eboID);
			glGenBuffers(1, eboID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *eboID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
		}

		unsigned int* createVAO() {
			unsigned int* vaoID = new unsigned int;
			glGenVertexArrays(1, vaoID);
			glBindVertexArray(*vaoID);
			return vaoID;
		}

		template <class Type>
		void storeDataInAttributeList(unsigned int attribIndex, unsigned int coordSize, std::vector<Type>& data) {
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
	};
}



#endif // !LOADER_H

