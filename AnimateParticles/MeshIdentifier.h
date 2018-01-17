#ifndef MESHIDENTIFIER_H
#define MESHIDENTIFIER_H
#pragma once


namespace ML {

	enum MeshEnum
	{
		Ball, Cube
	};

	class MeshIdentifier
	{
	public:
		MeshIdentifier() {}
		MeshIdentifier(const MeshIdentifier& m) : vaoID(m.getVaoID()), vertexCount(m.getVertexCount()) {}
		MeshIdentifier(unsigned int vaoID, unsigned int vertexCount) : vaoID(vaoID), vertexCount(vertexCount) { };

		unsigned int getVaoID() const { return vaoID; };
		unsigned int getVertexCount() const { return vertexCount; };

	private:
		unsigned int vaoID;
		unsigned int vertexCount;
	};
}
#endif // !MESHIDENTIFIER_H

