// learnopengl.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace ML;

const std::string SHADERPATH = "C:\\Users\\Sigurd\\Documents\\GitHub\\OpenGL\\learnopengl\\learnopengl\\shader_source\\";
const std::string StaticShader::VERTEXPATH =   "C:\\Users\\Sigurd\\Documents\\GitHub\\OpenGL\\learnopengl\\learnopengl\\shader_source\\static.vs";
const std::string StaticShader::FRAGMENTPATH = "C:\\Users\\Sigurd\\Documents\\GitHub\\OpenGL\\learnopengl\\learnopengl\\shader_source\\static.fs";
const std::string simulationFile = "C:\\Users\\Sigurd\\Documents\\GitHub\\ParticlesInBox\\RigidBody\\ParticlesInBox\\newtwo.pos";

Camera* Camera::currentlyActiveCamera = nullptr;
Window* Window::currentlyActiveWindow = nullptr;
Renderer* Renderer::currentlyActiveRenderer = nullptr;
Program* Program::currentlyActiveProgram = nullptr;




int main()
{
	try
	{
		Program p;
		p.makeSceneFromPlaybackData(simulationFile);
		p.shouldPlay();
		p.openViewPort("OpenGL", 1920, 1080);
		p.run();

	}
	catch (const std::exception& err)
	{
		std::cout << "in main:\n" << err.what() << std::endl;
	}
	return 0;
}









std::vector<float> vertices = {
	// lower right triangle
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.5f,  0.5f, 0.0f,
	//upper left triangle
	-0.5f, 0.5f, 0.0f,
};

std::vector<unsigned int> indices = {
	0,1,2,
	0,3,2
};

std::vector<float> textureCoords = {
	0,0,
	0,1,
	1,1,
	1,0
};

std::vector<float> cubeVertices = {
	-0.5f,0.5f,-0.5f,
	-0.5f,-0.5f,-0.5f,
	0.5f,-0.5f,-0.5f,
	0.5f,0.5f,-0.5f,

	-0.5f,0.5f,0.5f,
	-0.5f,-0.5f,0.5f,
	0.5f,-0.5f,0.5f,
	0.5f,0.5f,0.5f,

	0.5f,0.5f,-0.5f,
	0.5f,-0.5f,-0.5f,
	0.5f,-0.5f,0.5f,
	0.5f,0.5f,0.5f,

	-0.5f,0.5f,-0.5f,
	-0.5f,-0.5f,-0.5f,
	-0.5f,-0.5f,0.5f,
	-0.5f,0.5f,0.5f,

	-0.5f,0.5f,0.5f,
	-0.5f,0.5f,-0.5f,
	0.5f,0.5f,-0.5f,
	0.5f,0.5f,0.5f,

	-0.5f,-0.5f,0.5f,
	-0.5f,-0.5f,-0.5f,
	0.5f,-0.5f,-0.5f,
	0.5f,-0.5f,0.5f

};

std::vector<float> cubeTextureCoords = {

	0,0,
	0,1,
	1,1,
	1,0,
	0,0,
	0,1,
	1,1,
	1,0,
	0,0,
	0,1,
	1,1,
	1,0,
	0,0,
	0,1,
	1,1,
	1,0,
	0,0,
	0,1,
	1,1,
	1,0,
	0,0,
	0,1,
	1,1,
	1,0


};

std::vector<unsigned int> cubeIndices = {
	0,1,3,
	3,1,2,
	4,5,7,
	7,5,6,
	8,9,11,
	11,9,10,
	12,13,15,
	15,13,14,
	16,17,19,
	19,17,18,
	20,21,23,
	23,21,22
};
