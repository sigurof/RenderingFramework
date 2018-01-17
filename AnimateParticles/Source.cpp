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
