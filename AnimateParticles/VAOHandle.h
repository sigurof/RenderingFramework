#ifndef VAOHANDLE_H
#define VAOHANDLE_H
#pragma once


namespace ML {

	enum MeshEnum
	{
		Sphere, Cube, Dragon
	};

	class VAOHandle
	{
	public:
		VAOHandle() {}
		VAOHandle(const VAOHandle& m) : vaoID(m.getVaoID()), vertexCount(m.getVertexCount()) {}
		VAOHandle(unsigned int vaoID, unsigned int vertexCount) : vaoID(vaoID), vertexCount(vertexCount) { };

		unsigned int getVaoID() const { return vaoID; };
		unsigned int getVertexCount() const
		{
			try
			{
				if (this == nullptr)
				{
					throw std::exception("ERROR! Trying to access nullptr\n");
				}
				return vertexCount;
			}
			catch (const std::exception& err)
			{
				std::cout << "In VAOHandle::getVertexCount\n";
				throw err;
			}
		};

	private:
		unsigned int vaoID;
		unsigned int vertexCount;

	};
}
#endif // !VAOHANDLE_H

