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
			std::cout << "Renderer::Destructor\n"; 
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
					recalculateProjectionMatrix();
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
			shader->cleanUp();
		}

		void prepare(float r, float g, float b, float alpha) const
		{
			glEnable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(r, g, b, alpha);
		}

		void renderEntities() const
		{
			/* For each Model */
			for (std::vector<std::shared_ptr<Entity>>::const_iterator it = scene->getEntities().begin(); it != scene->getEntities().end(); it++)//std::map<Model,std::vector<Entity*>>::const_iterator it = entities.begin(); it!=entities.end(); it++)
			{
				if ((*it)->getDraw())
				{
					prepareModel((*it)->getModel());
					prepareInstance(*(it));
					glDrawElements((*it)->getModel().getDrawMode(), (*it)->getModel().getRawModel()->getVertexCount(), GL_UNSIGNED_INT, (void*)0);
					unbindModel();
				}
			}
		}

		void prepareModel(const Model& model) const 
		{
			glBindVertexArray(model.getRawModel()->getVaoID());
			glEnableVertexAttribArray(0);//Mulig disse kan kommenteres ut
			glEnableVertexAttribArray(1);//Mulig disse kan kommenteres ut
			glEnableVertexAttribArray(2);//Mulig disse kan kommenteres ut
			shader->loadSurfaceVariables(model.getSurface());// .getTexture());
			//if (model.getSurface().getColor()->getAlpha() == 1)
			//{
			//	glEnable(GL_CULL_FACE);
			//	glCullFace(GL_BACK);
			//}
			//else
			//{
			//	glDisable(GL_CULL_FACE);
			//}
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

		void prepareInstance(std::shared_ptr<Entity> entity) const
		{
			glm::mat4 transformationMatrix = Maths::createTransformationMatrix(entity->getPosition(), entity->getAngles(), entity->getScaleFactors());
			shader->loadTransformationMatrix(transformationMatrix);
		}

		void render() const 
		{
			/* Renders all the entities in the scene */
			prepare(0.2f, 0.2f, 0.2f, 1.0f);
			shader->start();
			shader->loadLight(scene->getLights());
			shader->loadViewMatrix(scene->getCurrentCamera());
			renderEntities();
			shader->stop();
		}
	
		void changeFOV(float fov) { FOV = fov; recalculateProjectionMatrix(); }
		void changeNEAR_PLANE(float near_plane) { NEAR_PLANE = near_plane; recalculateProjectionMatrix(); }
		void changeFAR_PLANE(float far_plane) { FAR_PLANE = far_plane; recalculateProjectionMatrix(); }
		void changeProjectionMatrix(float fov, float near_plane, float far_plane) { FOV = fov; NEAR_PLANE = near_plane; FAR_PLANE = far_plane; recalculateProjectionMatrix(); }
		void setProjectionMatrix(const glm::mat4& projMat) { projectionMatrix = projMat; }
		void setAspectRatio(float aspRat) { aspectRatio = aspRat; }
		void setShader(const std::shared_ptr<StaticShader> shadr) { this->shader = shadr; }
		void setScene(const std::shared_ptr<Scene> scn)
		{
			this->scene = scn;
		}


		std::shared_ptr<StaticShader> getShader() const { return shader; }
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



		void prepareShader() {
			shader->start();
			shader->loadProjectionMatrix(projectionMatrix);
			shader->stop();
		}

	private:

		glm::mat4 projectionMatrix;
		float FOV;
		float NEAR_PLANE;
		float FAR_PLANE;
		float aspectRatio;
		std::shared_ptr<StaticShader> shader;
		std::shared_ptr<Scene> scene;

		void recalculateProjectionMatrix()
		{
			float x_scale = (1.0f / glm::tan(glm::radians(FOV / 2.0f)));
			float y_scale = x_scale*aspectRatio;
			float frustum_length = FAR_PLANE - NEAR_PLANE;

			projectionMatrix[0][0] = x_scale;
			projectionMatrix[1][1] = y_scale;
			projectionMatrix[2][2] = -(FAR_PLANE + NEAR_PLANE) / frustum_length;
			projectionMatrix[2][3] = -1;
			projectionMatrix[3][2] = -((2 * NEAR_PLANE*FAR_PLANE) / frustum_length);
			projectionMatrix[3][3] = 0;

		}
	};
}


#endif // !RENDERER_H
