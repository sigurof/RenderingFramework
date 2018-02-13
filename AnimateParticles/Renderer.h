#ifndef RENDERER_H
#define RENDERER_H
#pragma once



namespace ML {
	class Renderer
	{
	public:

		Renderer() : Renderer(70, 0.1f, 1000.0f) {}
							 
		Renderer(float fov, float near_plane, float far_plane)
		{
			FOV = fov; 
			NEAR_PLANE = near_plane; 
			FAR_PLANE = far_plane; 
			if (Window::currentlyActiveWindow)
			{
				activate();
			}
		}

		Renderer(const Renderer&) = delete;

		~Renderer() { 
#ifdef VERBOSEDESTRUCTORS
			std::cout << "Renderer::Destructor\n";
#endif // VERBOSEDESTRUCTORS
			if (currentlyActiveRenderer == this)
			{
				currentlyActiveRenderer = nullptr;
			}
		}

		Renderer& operator=(const Renderer&) = delete;

		void activate() 
		{
			try
			{
				if (Window::currentlyActiveWindow)
				{
					aspectRatio = Window::currentlyActiveWindow->getAspectRatio();
					windowWidth = Window::currentlyActiveWindow->getWidth();
					windowHeight = Window::currentlyActiveWindow->getHeight();
					recalculateProjectionMatrix(false);
					setThisToCurrentlyActive();
					glEnable(GL_CULL_FACE);
					glCullFace(GL_BACK);
				}
				else
				{
					throw(std::exception("ERROR! Renderer was not activated\n"));
				}

			}
			catch (const std::exception& err)
			{
				std::cout << "In Renderer::activate():\n";
				throw err;
			}		
		}
		
		void cleanUp() const
		{
			surfaceShader->cleanUp();
			lineShader->cleanUp();
			lightShader->cleanUp();

		}

		void prepare(float r, float g, float b, float alpha) const
		{
			glEnable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(r, g, b, alpha);
		}

		void renderSurfaces() const
		{
			/* For each Model */
			for (std::vector<std::shared_ptr<Entity>>::const_iterator it = scene->getSurfaces().begin(); it != scene->getSurfaces().end(); it++)//std::map<Model,std::vector<Entity*>>::const_iterator it = entities.begin(); it!=entities.end(); it++)
			{
				if ((*it)->shouldDraw())
				{
					prepareModel((*it)->getModel());
					prepareInstance(*(it));
					glDrawElements((*it)->getModel().getDrawMode(), (*it)->getModel().getRawModel()->getVertexCount(), GL_UNSIGNED_INT, (void*)0);
					unbindModel();
				}
			}
		}

		void renderLines() const
		{
			try
			{

				/* For each Model */
				for (std::vector<std::shared_ptr<Line>>::const_iterator it = scene->getLines().begin(); it != scene->getLines().end(); it++)
				{
					if ((*it)->shouldDraw())
					{
						prepareLine((*it)->getEntity()->getModel());
						prepareInstance((*it)->getEntity());
						if ((*it)->shouldUseGlDrawElementRange())
						{
							//glDrawElements(GL_TRIANGLES, (*it)->getEntity()->getModel().getRawModel()->getVertexCount(), GL_UNSIGNED_INT, (void*)0);
							glDrawRangeElements((*it)->getEntity()->getModel().getDrawMode(), (*it)->getStartElement(), (*it)->getEndElement(), 2*(*it)->getEntity()->getModel().getRawModel()->getVertexCount(), GL_UNSIGNED_INT, (void*)0);
						}
						else
						{
							glDrawElements((*it)->getEntity()->getModel().getDrawMode(), (*it)->getEntity()->getModel().getRawModel()->getVertexCount(), GL_UNSIGNED_INT, (void*)0);
						}
						unbindLine();
					}
				}
			}
			catch (const std::exception& err)
			{
				std::cout << "In Renderer::renderLines\n";
				throw err;
			}
		}

		void renderLights() const
		{
			for (std::vector<std::shared_ptr<Light>>::const_iterator it = scene->getLights().begin(); it != scene->getLights().end(); it++)
			{
				if ((*it)->getEntity()->shouldDraw())
				{
					//glBindVertexArray((*it)->getEntity()->getModel().getRawModel()->getVaoID());
					//glEnableVertexAttribArray(0);//Mulig disse kan kommenteres ut
					//glEnableVertexAttribArray(1);//Mulig disse kan kommenteres ut
					//glEnableVertexAttribArray(2);//Mulig disse kan kommenteres ut
					//lightShader->loadSurfaceColor((*it)->getColor());
					prepareModel((*it)->getEntity()->getModel());
					prepareInstance((*it)->getEntity());
					glDrawElements((*it)->getEntity()->getModel().getDrawMode(), (*it)->getEntity()->getModel().getRawModel()->getVertexCount(), GL_UNSIGNED_INT, (void*)0);
					unbindModel();
				}
			}
		}

		void prepareLine(const Model& model) const
		{
			glBindVertexArray(model.getRawModel()->getVaoID());
			glEnableVertexAttribArray(0);
			Shader::currentlyActiveShader->loadSurfaceVariables(model.getSurface());// .getTexture());
			if (model.getSurface().isColorActive())
			{
				Shader::currentlyActiveShader->loadSurfaceColor(*model.getSurface().getColor());
			}
		}

		void unbindLine() const
		{
			glDisableVertexAttribArray(0);
			glBindVertexArray(0);
		}

		void prepareModel(const Model& model) const 
		{
			glBindVertexArray(model.getRawModel()->getVaoID());
			glEnableVertexAttribArray(0);//Mulig disse kan kommenteres ut
			glEnableVertexAttribArray(1);//Mulig disse kan kommenteres ut
			glEnableVertexAttribArray(2);//Mulig disse kan kommenteres ut
			Shader::currentlyActiveShader->loadSurfaceVariables(model.getSurface());// .getTexture());
			if (model.getSurface().isTextureActive())
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, model.getSurface().getTexture()->getID());
			}
			if (model.getSurface().isColorActive())
			{
				Shader::currentlyActiveShader->loadSurfaceColor(*model.getSurface().getColor());
			}
		}

		void unbindModel() const
		{
			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
			glBindVertexArray(0);
		}

		void prepareInstance(std::shared_ptr<Entity> entity) const
		{
			glm::mat4 transformationMatrix = Maths::createTransformationMatrix(entity->getPosition(), entity->getAngles(), entity->getScaleFactors());
			Shader::currentlyActiveShader->loadTransformationMatrix(transformationMatrix);
		}

		void render() const 
		{
			/* Renders all the entities in the scene */
			prepare(0.2f, 0.2f, 0.2f, 1.0f);
			surfaceShader->start(); 
			surfaceShader->loadLight(scene->getLights()); 
			surfaceShader->loadViewMatrix(scene->getCurrentCamera());
			surfaceShader->loadCameraPosition(scene->getCurrentCamera());
			renderSurfaces();
			surfaceShader->stop();
#ifdef NOUPLOADING
			//glDisable(GL_DEPTH_TEST);
			lineShader->start();
			lineShader->loadLight(scene->getLights());
			lineShader->loadViewMatrix(scene->getCurrentCamera());
			lineShader->loadCameraPosition(scene->getCurrentCamera());
			renderLines();
			lineShader->stop();
			//glEnable(GL_DEPTH_TEST);
#endif // NOUPLOADING
			
			lightShader->start();
			lightShader->loadLight(scene->getLights());
			lightShader->loadViewMatrix(scene->getCurrentCamera());
			lightShader->loadCameraPosition(scene->getCurrentCamera());
			renderLights();
			lightShader->stop();
		}
	
		void changeFOV(float fov) { FOV = fov; recalculateProjectionMatrix(false); }
		void changeNEAR_PLANE(float near_plane) { NEAR_PLANE = near_plane; recalculateProjectionMatrix(false); }
		void changeFAR_PLANE(float far_plane) { FAR_PLANE = far_plane; recalculateProjectionMatrix(false); }
		void changeProjectionMatrix(float fov, float near_plane, float far_plane) { FOV = fov; NEAR_PLANE = near_plane; FAR_PLANE = far_plane; recalculateProjectionMatrix(false); }
		void setProjectionMatrix(const glm::mat4& projMat) { projectionMatrix = projMat; }
		void setAspectRatio(float aspRat) { aspectRatio = aspRat; }

		void setScene(const std::shared_ptr<Scene> scn)
		{
			this->scene = scn;
		}
		
		std::shared_ptr<Shader> getLightShader() const { return lightShader; }
		std::shared_ptr<Shader> getSurfaceShader() const { return surfaceShader; }
		std::shared_ptr<Shader> getLineShader() const { return lineShader; }

		void setSurfaceShader(const std::shared_ptr<Shader> shadr) { this->surfaceShader = shadr; }
		void setLineShader(const std::shared_ptr<Shader> shadr) { this->lineShader = shadr; }
		void setLightShader(const std::shared_ptr<Shader> shadr) { this->lightShader = shadr; }



		float getFOV() const { return FOV; }
		float getNEAR_PLANE() const { return NEAR_PLANE; }
		float getFAR_PLANE() const { return FAR_PLANE; }
		const glm::mat4& getProjectionMatrix() const { return projectionMatrix; }
		float getAspectRatio() const { return aspectRatio; }
		std::shared_ptr<Scene> getScene() const { return scene; }


		static Renderer* currentlyActiveRenderer;
	
		void setThisToCurrentlyActive() 
		{
			Renderer::currentlyActiveRenderer = this;
		}

		void prepareLightShader() {
			lightShader->start();
			lightShader->loadProjectionMatrix(projectionMatrix);
			lightShader->stop();
		}

		void prepareLineShader() {
			lineShader->start();
			lineShader->loadProjectionMatrix(projectionMatrix);
			lineShader->stop();
		}

		void prepareSurfaceShader() {
			surfaceShader->start();
			surfaceShader->loadProjectionMatrix(projectionMatrix);
			surfaceShader->stop();
		}

	private:

		glm::mat4 projectionMatrix;
		float FOV;
		float NEAR_PLANE;
		float FAR_PLANE;
		float aspectRatio;
		int windowWidth;
		int windowHeight;

		std::shared_ptr<Shader> surfaceShader;
		std::shared_ptr<Shader> lineShader;
		std::shared_ptr<Shader> lightShader;


		std::shared_ptr<Scene> scene;

		void recalculateProjectionMatrix(bool orthographic)
		{
			if (orthographic)
			{
				projectionMatrix = glm::ortho(-1.f*aspectRatio, 1.f*aspectRatio, -1.f, 1.f, (float)NEAR_PLANE, (float)FAR_PLANE);
			}
			else
			{

				float x_scale = (1.0f / glm::tan(glm::radians(FOV / 2.0f)));
				float y_scale = x_scale * aspectRatio;
				float frustum_length = FAR_PLANE - NEAR_PLANE;

				projectionMatrix[0][0] = x_scale;
				projectionMatrix[1][1] = y_scale;
				projectionMatrix[2][2] = -(FAR_PLANE + NEAR_PLANE) / frustum_length;
				projectionMatrix[2][3] = -1;
				projectionMatrix[3][2] = -((2 * NEAR_PLANE*FAR_PLANE) / frustum_length);
				projectionMatrix[3][3] = 0;

			}
		}
	};
}


#endif // !RENDERER_H
