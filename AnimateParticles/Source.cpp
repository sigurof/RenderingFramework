// learnopengl.cpp : Defines the entry point for the console application.
//


//#include <string>
//#include "StaticShader.h"
//#include "Animator.h"


#include <iostream>
#include <vector>
#include <string>

#include "StaticShader.h"
#include "Animator.h"


using namespace ML;

const std::string SHADERPATH = "C:\\Users\\Sigurd\\Documents\\GitHub\\OpenGL\\learnopengl\\learnopengl\\shader_source\\";
const std::string StaticShader::VERTEXPATH =   "C:\\Users\\Sigurd\\Documents\\GitHub\\OpenGL\\learnopengl\\learnopengl\\shader_source\\static.vs";
const std::string StaticShader::FRAGMENTPATH = "C:\\Users\\Sigurd\\Documents\\GitHub\\OpenGL\\learnopengl\\learnopengl\\shader_source\\static.fs";




int main()
{
	try
	{
		std::string simulationFile = "C:\\Users\\Sigurd\\Documents\\GitHub\\ParticlesInBox\\RigidBody\\ParticlesInBox\\newtwo.pos";
		Animator anim(simulationFile);
		//anim.play();
		anim.setCameraPosition(glm::vec3(0, 0, -3));
		anim.setLightPosition(glm::vec3(0, 200, 0));
		anim.setFrameRate(24);
		//anim.hideBox();
		anim.play();
		//anim.drawSingleFrame(0);
		//
		//Window window(1920, 1080, "OpenGL");
		//Loader loader;
		////RenderObject model = loader.loadToVAO(cubeVertices, cubeTextureCoords, cubeIndices);
		//Light light(glm::vec3(200, 200, -200), glm::vec3(1, 1, 1));
		//Camera camera(&window);
		//RenderObject model = OBJLoader::loadObjModel(OBJECTPATH + "sphere.obj", loader);
		//ModelTexture texture(loader.loadTexture(TEXTUREPATH + "gold.png", ImageFormat::PNG), 1, 100);
		//Model staticModel(model, texture);
		//Entity* entity = new Entity(staticModel, glm::vec3(0, 0, -50), glm::vec3(0, 0, 0), glm::vec3(1, 1, 1));
		//MasterRenderer renderer(&window);
		//renderer.processEntity(entity);
		//while (window.isOpen())
		//{
		//	currentTime = (float)glfwGetTime(); camera.setDt(currentTime-lastTime); lastTime = currentTime;
		//	camera.move();
		//	renderer.render(light, camera);
		//	window.processInput();
		//	window.draw();
		//	window.pollEvent();
		//}
		//renderer.cleanUp();
		//loader.cleanUp();
		//
		//glfwTerminate();
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
