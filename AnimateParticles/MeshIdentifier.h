#ifndef MESHIDENTIFIER_H
#define MESHIDENTIFIER_H
#pragma once
//#include <iostream>
//#include <assert.h>
//#include <glad\glad.h>
//#include <GLFW\glfw3.h>
//#include <vector>

namespace ML {

	enum MeshEnum
	{
		Ball, Cube
	};

	struct Ball
	{
		Ball() : radius(1) {}
		Ball(float radius) : radius(radius) {}
		Ball(const Ball& ball) : Ball(ball.radius) {}
		float radius;
	};

	struct Cube
	{
		Cube() : sideLength(1) {}
		Cube(float sidelength) : sideLength(sidelength) {}
		Cube(const Cube& cube) : Cube(cube.sideLength) {}
		float sideLength;
	};
/*
	struct Cuboid
	{
		Cuboid() : dx(1), dy(1), dz(2) {}
		Cuboid(float dx, float dy, float dz) : dx(dx), dy(dy), dz(dz) {}
		Cuboid(const Cuboid& cuboid) : Cuboid(dx, dy, dz) {}
		float dx, dy, dz;
	};*/

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

#endif // !MESHIDENTIFIER_H

