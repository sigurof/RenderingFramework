#ifndef RENDERER_H
#define RENDERER_H
#pragma once

//#include <iostream>
//#include <assert.h>
//#include <vector>
//#include <math.h>
//#include "MeshIdentifier.h"
//#include "Loader.h"
//#include "Model.h"

//#include "Entity.h"
//#include "StaticShader.h"
//#include "Window.h"
//#include "StaticShader.h"
//#include "Camera.h"
//#include "Maths.h"
//#include <map>


#include "Camera.h"
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <vector>
#include "Entity.h"

//1
//namespace ML {
//	class Window;/* {
//	public:
//		const Camera& getCamera();
//		float getAspectRatio();
//	};*//* NB! THIS LINE FIXES CIRCULAR REFERENCE PROBLEM*/
//}

//2
//#include "Window.h"



namespace ML {
	//class Window;
	class Renderer
	{
	public:

		//Renderer() : Renderer(StaticShader()) { }
							 
		Renderer(float aspRat) : aspectRatio(aspRat)//const StaticShader& shad)
		{
			FOV = 70;
			NEAR_PLANE = 0.1f;
			FAR_PLANE = 1000;
			createProjectionMatrix();
		}
		
		Renderer(const Renderer& rend) : 
			projectionMatrix(rend.projectionMatrix), FOV(rend.FOV), 
			NEAR_PLANE(rend.NEAR_PLANE), FAR_PLANE(rend.FAR_PLANE),
			aspectRatio(rend.aspectRatio), shader(rend.shader), camera(rend.camera)
		{
		
		}

		//Renderer(const Renderer& rend) : Renderer(rend) {}
		//Renderer(const Window* par) : Renderer(par, StaticShader()) { }
		//
		//Renderer(const Window* par, const StaticShader& shad) : parentWindow(par), shader(shad)
		//{
		//	FOV = 70;
		//	NEAR_PLANE = 0.1f;
		//	FAR_PLANE = 1000;
		//	glEnable(GL_CULL_FACE);
		//	glCullFace(GL_BACK);
		//	createProjectionMatrix();
		//	shader->start();
		//	shader->loadProjectionMatrix(projectionMatrix);
		//	shader->stop();
		//}
		//
		//Renderer(const Renderer& rend, const Window* parent) :
		//	projectionMatrix(rend.getProjectionMatrix()), FOV(rend.getFOV()),
		//	NEAR_PLANE(rend.getNEAR_PLANE()), FAR_PLANE(rend.getFAR_PLANE()),
		//	aspectRatio(rend.getAspectRatio()), shader(rend.getShader()), parentWindow(parent)
		//{}
		//
		//Renderer(const Renderer& rend) : Renderer(rend, nullptr) {}

		void setCameraPosition(const glm::vec3& pos)
		{
			camera.setPosition(pos);
		}
		
		void cleanUp() const
		{
			shader->cleanUp();
		}

		void prepare(float r, float g, float b, float alpha) const
		{
			glEnable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(r, g, b, alpha);
		}

		void renderEntities(const std::vector<Entity*>& dynamicEntities, const std::vector<Entity*>& staticEntities) const //const std::map<Model, std::vector<Entity*>>& entities) const
		{
			/* For each Model */
			for (std::vector<Entity*>::const_iterator it = dynamicEntities.begin(); it != dynamicEntities.end(); it++)//std::map<Model,std::vector<Entity*>>::const_iterator it = entities.begin(); it!=entities.end(); it++)
			{
				if ((*it)->getDraw())
				{
					prepareModel((*it)->getModel());
					prepareInstance(*(it));
					glDrawElements((*it)->getModel().getDrawMode(), (*it)->getModel().getRawModel().getVertexCount(), GL_UNSIGNED_INT, (void*)0);
					unbindModel();
				}
			}
			for (std::vector<Entity*>::const_iterator it = staticEntities.begin(); it != staticEntities.end(); it++)//std::map<Model,std::vector<Entity*>>::const_iterator it = entities.begin(); it!=entities.end(); it++)
			{
				if ((*it)->getDraw())
				{
					prepareModel((*it)->getModel());
					prepareInstance(*(it));
					glDrawElements((*it)->getModel().getDrawMode(), (*it)->getModel().getRawModel().getVertexCount(), GL_UNSIGNED_INT, (void*)0);
					unbindModel();
				}
			}
		}

		void prepareModel(const Model& model) const 
		{
			glBindVertexArray(model.getRawModel().getVaoID());
			glEnableVertexAttribArray(0);//Mulig disse kan kommenteres ut
			glEnableVertexAttribArray(1);//Mulig disse kan kommenteres ut
			glEnableVertexAttribArray(2);//Mulig disse kan kommenteres ut
			shader->loadSurfaceVariables(model.getSurface());// .getTexture());
			if (model.getSurface().getColor()->getAlpha() == 1)
			{
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
			}
			else
			{
				glDisable(GL_CULL_FACE);
			}
			if (model.getSurface().isTextureActive())
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, model.getSurface().getTexture()->getID());
			}
			if (model.getSurface().isColorActive())
			{
				shader->loadSurfaceColor(*model.getSurface().getColor());
			}
		}

		void unbindModel() const
		{
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
			glBindVertexArray(0);
			
		}

		void prepareInstance(Entity* entity) const
		{
			glm::mat4 transformationMatrix = Maths::createTransformationMatrix(entity->getPosition(), entity->getAngles(), entity->getScaleFactors());
			shader->loadTransformationMatrix(transformationMatrix);
		}

		void render(const Light& sun, const std::vector<Entity*>& dynamicEntities, const std::vector<Entity*>& staticEntities) const //std::map<Model, std::vector<Entity*>>& entities) const
		{
			/* Renders all the entities in the scene */
			prepare(0.2f, 0.2f, 0.2f, 1.0f);
			shader->start();
			shader->loadLight(sun);
			shader->loadViewMatrix(camera);
			//shader->loadViewMatrix(parentWindow->getCamera());
			renderEntities(dynamicEntities, staticEntities);
			shader->stop();
			//entities.clear();
		}

		void makeShader() {
			shader = new StaticShader();
			shader->start();
			shader->loadProjectionMatrix(projectionMatrix);
			shader->stop();
		}

		//void processEntity(Entity* entity) {
		//	if (entities[entity->getModel()].size() != 0)
		//	{
		//		entities[entity->getModel()].push_back(entity);
		//	}
		//	else
		//	{
		//		const std::vector<Entity*> batch = { entity };
		//		entities[entity->getModel()] = batch;
		//	}
		//}

		void moveCamera(const DirectionEnum dir) { camera.move(dir); }

		//void setShader(const StaticShader& shad) { shader = shad; }
		void setFOV(float fov) { FOV = fov; }
		void setNEAR_PLANE(float near_plane) { NEAR_PLANE = near_plane; }
		void setFAR_PLANE(float far_plane) { FAR_PLANE = far_plane; }
		void setProjectionMatrix(const glm::mat4& projMat) { projectionMatrix = projMat; }
		void setAspectRatio(float aspRat) { aspectRatio = aspRat; }
		void setCamera(const Camera& cam) { camera = cam; }
		void setDt(float dt) { camera.setDt(dt); }

		const StaticShader* getShader() const { return shader; }
		float getFOV() const { return FOV; }
		float getNEAR_PLANE() const { return NEAR_PLANE; }
		float getFAR_PLANE() const { return FAR_PLANE; }
		const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }
		float getAspectRatio() const { return aspectRatio; }
		const Camera& getCamera() const { return camera; }

	private:

		glm::mat4 projectionMatrix;
		float FOV;
		float NEAR_PLANE;
		float FAR_PLANE;
		float aspectRatio;
		StaticShader* shader;
		Camera camera;
		//const Window* parentWindow;

		void createProjectionMatrix()
		{
			//aspectRatio = parentWindow->getAspectRatio();
			float x_scale = (1.0f / glm::tan(glm::radians(FOV / 2.0f)));
			float y_scale = x_scale*aspectRatio;
			float frustum_length = FAR_PLANE - NEAR_PLANE;

			projectionMatrix[0][0] = x_scale;
			projectionMatrix[1][1] = y_scale;
			projectionMatrix[2][2] = -(FAR_PLANE + NEAR_PLANE) / frustum_length;
			projectionMatrix[2][3] = -1;
			projectionMatrix[3][2] = -((2 * NEAR_PLANE*FAR_PLANE) / frustum_length);
			projectionMatrix[3][3] = 0;

			//return matrix;

		}
	};
}


#endif // !RENDERER_H
