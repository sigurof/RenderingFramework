#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H
#pragma once
//#include <iostream>
//#include <string>
//#include <vector>
//#include <fstream>
//#include <sstream>
//#include <assert.h>
//#include "Entity.h"
//#include "Window.h"
//#include "Renderer.h"

//#include "Window.h"
//#include "Loader.h"
//#include "Model.h"
//#include "Renderer.h"
//#include "ShaderProgram.h"
//#include "StaticShader.h"
//#include "Texture.h"
//#include "ModelTexture.h"
//#include "Maths.h"
//#include "OBJLoader.h"
//#include "Light.h"
//#include "MasterRenderer.h"

/* not necessary*/
#include <iostream>
#include <glm-0.9.8.5\glm/glm.hpp>
#include <glm-0.9.8.5\glm/gtc/matrix_transform.hpp>
#include <glm-0.9.8.5\glm/gtc/type_ptr.hpp>
#include "Light.h"
#include "Color.h"
/*not necessary*/


#include "Window.h"
#include "Entity.h"
#include <map>
#include "OBJLoader.h"
#include "Model.h"
#include "Loader.h"

using namespace ML;

const std::string OBJECTPATH = "C:\\Users\\Sigurd\\Documents\\GitHub\\OpenGL\\learnopengl\\learnopengl\\objects\\";
const std::string TEXTUREPATH = "C:\\Users\\Sigurd\\Documents\\GitHub\\OpenGL\\learnopengl\\learnopengl\\texture_files\\";

float lastTime = 0.0f;
float currentTime = 0.0f;

class GraphicsEngine
{
public:

	GraphicsEngine() : window(Window("OpenGL")), tex(TEXTUREPATH, ImageFormatEnum::PNG) {
		light = Light(glm::vec3(0, 10, 0), glm::vec3(1,1,1));	
	}

	GraphicsEngine(const GraphicsEngine& ge) : window(ge.window), light(ge.light), instancesOfMesh(ge.instancesOfMesh), meshes(ge.meshes), loader(ge.loader), tex(TEXTUREPATH, ImageFormatEnum::PNG)
	{
		for each (Entity* e in ge.dynamicEntities)
		{
			this->dynamicEntities.push_back(new Entity(e->getModel(), e->getPosition(), e->getAngles(), e->getScaleFactors()));
		}
	}

	~GraphicsEngine()
	{
		window.~Window();
		dynamicEntities.clear();
		staticEntities.clear();
	}

	void setLightPosition(const glm::vec3& pos) {
		light.setPosition(pos);
	}

	void shiftEntities(const std::vector<glm::vec3>& newCenters) {
		//DEBUG
		//dynamicEntities[0]->setPosition(glm::vec3(0, 0, 0));
		//for (unsigned int i = 1; i < dynamicEntities.size(); i++)
		//{
		//	dynamicEntities[i]->setPosition(newCenters[i]);
		//}
		//ENDDEBUG
		for (unsigned int i = 0; i < dynamicEntities.size(); i++)
		{
			dynamicEntities[i]->setPosition(newCenters[i]);
		}
	}

	void setCameraPosition(const glm::vec3& pos)
	{
		window.setCameraPosition(pos);
	}

	void addEntity(MeshEnum type, const Color& color, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale, bool isDynamicEntity)
	{
		try
		{
			if (instancesOfMesh[type] == 0)
			{
				switch (type)
				{
				case ML::Ball:
					meshes[type]=OBJLoader::loadObjModel(OBJECTPATH + "sphere.obj", loader);
					break;
				case ML::Cube:
					meshes[type]=OBJLoader::loadObjModel(OBJECTPATH + "cube.obj", loader);
					break;
				default:
					throw(std::exception("ERROR! Not valid mesh type!\n"));
					break;
				}
				instancesOfMesh[type] += 1;
			}
			Model model(meshes[type], color, GL_TRIANGLES, 1 / 2, 1 / 2);
			if (isDynamicEntity)
			{
				dynamicEntities.push_back(new Entity(model, pos, rot, scale));
			}
			else
			{
				model.setDrawMode(GL_LINES);
				model.setSurfaceShineVariables(0, -111);
				staticEntities.push_back(new Entity(model, pos, rot, scale));
			}
		}
		catch (const std::exception& err)
		{
			std::cout << "In addEntity():\n";
			throw err;
		}	
	}
	
	void hideDynamicEntity(unsigned int index) 
	{
		dynamicEntities[index]->setDraw(false);

	}

	void hideStaticEntity(unsigned int index)
	{
		staticEntities[index]->setDraw(false);

	}

	/*void loadEntities(std::vector<Entity*>& ents) {
		for (unsigned int i = 0; i < ents.size(); i++)
		{
			loadEntity(ents[i]);
		}
	}*/

	void openWindow(int width, int height) {
		window.open(width, height);
		for (unsigned int i = 0; i < dynamicEntities.size(); i++)
		{
			if (dynamicEntities[i]->getModel().getSurface().isTextureActive())
			{
				tex.activate();
			}
		}
	}

	bool windowIsOpen() {
		return window.isOpen();
	}


	void drawScene()
	{
		try
		{
			currentTime = (float)glfwGetTime(); window.setDt(currentTime - lastTime); lastTime = currentTime;
			window.getRenderer()->render(light, dynamicEntities, staticEntities);
			window.getRenderer()->getShader()->stop();
			window.swapBuffers();//This function used to come right after window.processInput();

		}
		catch (const std::exception& err)
		{
			std::cout << "In Animator::play():\n";
			throw err;
		}
	}

	void pollUserInput() {
		window.pollEvent();
		window.processInput();
	}

	void cleanUp() {
		//renderer->cleanUp();
		window.getRenderer()->cleanUp();
		loader.cleanUp();
		glfwTerminate();
	}

	const std::vector<Entity*>& getDynamicEntities()  //std::map<Model, std::vector<Entity*>>& getEntities() {
	{
		return dynamicEntities;
	}

private:
	Window window;

	std::vector<Entity*> dynamicEntities;
	std::vector<Entity*> staticEntities;
	Light light;

	Loader loader;
	
	std::map <MeshEnum, unsigned int> instancesOfMesh;
	std::map<MeshEnum, MeshIdentifier> meshes;

	//Debug
	Texture tex;

};

#endif // !GRAPHICSENGINE_H
