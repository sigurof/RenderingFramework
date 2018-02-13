#ifndef VERTEXDATA
#define VERTEXDATA
#pragma once
namespace ML {
	class VertexData
	{
	public:

		const std::vector<float>& getVertices() const { return vertices; }
		const std::vector<float>& getTextureCoords() const { return textureCoords; }
		const std::vector<float>& getNormals() const { return normals; }
		const std::vector<unsigned int>& getIndices() const { return indices; }

		void setVertices(const std::vector<float>& verts)
		{
			this->vertices = verts;
			//this->vertices = std::vector<float>(verts.size(), 0);
			//std::copy(verts.begin(), verts.end(), vertices.begin());
		}

		void setTextureCoords(const std::vector<float>& texs) { textureCoords = texs; }
		void setNormals(const std::vector<float>& norms) { normals = norms; }
		void setIndices(const std::vector<unsigned int>& inds)
		{
			indices = inds; 
		}

	private:
		std::vector<float> vertices;
		std::vector<float> textureCoords;
		std::vector<float> normals;
		std::vector<unsigned int> indices;
	};

}
#endif // !VERTEXDATA
