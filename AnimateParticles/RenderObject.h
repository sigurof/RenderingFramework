#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H
#pragma once
#include <iostream>
#include <assert.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <vector>

namespace ML {



	class RenderObject
	{
	public:
		RenderObject(){}
		RenderObject(unsigned int vaoID, unsigned int vertexCount) : vaoID(vaoID), vertexCount(vertexCount) { };

		unsigned int getVaoID() { return vaoID; };
		unsigned int getVertexCount() { return vertexCount; };

	private:
		unsigned int vaoID;
		unsigned int vertexCount;
	};
}


/*	class VertexBuffer
{
public:
VertexBuffer(std::vector<float>& vertices) : vertices(vertices)
{
unsigned int VBO;
glGenBuffers(1, &VBO);
};

void bind() {
isBound = true;
// make all calls to GL_ARRAY_BUFFER operate on this buffer
glBindBuffer(GL_ARRAY_BUFFER, VBO);
}
void unbind() {
try
{
if (!isBound)
{
throw std::exception("ERROR! ML::VertexBuffer::unbind() called for unbound instance");
}
glBindBuffer(GL_ARRAY_BUFFER, 0);
}
catch (const std::exception& err)
{
std::cout << "In ML::VertexBuffer::unbind()\n";
throw err;
}
}
void buffer() {
// copies the previously defined vertex data into the buffer's memory:
glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
}

private:
unsigned int VBO;
bool isBound = true;
std::vector<float> vertices;


};
*/

#endif // !RENDEROBJECT_H

