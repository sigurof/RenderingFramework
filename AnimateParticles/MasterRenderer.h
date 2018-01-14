#ifndef MASTERRENDERER_H
#define MASTERRENDERER_H
#pragma once
#include "Model.h"
#include <map>

#include "Entity.h"
#include "Renderer.h"
#include "StaticShader.h"
#include "Light.h"
#include "Camera.h"

namespace ML {

	class MasterRenderer
	{
	public:
		MasterRenderer(){}
		MasterRenderer(Window* window) : renderer(Renderer(shader, window)) {}
		~MasterRenderer() 
		{
			entities.clear();
		}
		
		void cleanUp() 
		{
			shader.cleanUp();
		}

		void render(Light& sun, Camera& camera)
		{
			/* Renders all the entities in the scene */
			renderer.prepare(0.1f,0.1f,1.0f,1.0f);
			shader.start();
			shader.loadLight(sun);
			shader.loadViewMatrix(camera);
			renderer.render(entities, camera);
			shader.stop();
			//entities.clear();
		}

		void processEntity(Entity* entity) {
			if (entities[entity->getModel()].size() != 0)
			{
				entities[entity->getModel()].push_back(entity);
			}
			else
			{
				const std::vector<Entity*> batch = { entity };
				entities[entity->getModel()] = batch;
			}
		}

	private:
		StaticShader shader;
		Renderer renderer;
		std::map<Model, std::vector<Entity*>> entities;
	};
}


#endif MASTERRENDERER_H