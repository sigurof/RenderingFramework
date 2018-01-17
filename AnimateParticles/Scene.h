#ifndef SCENE_H
#define SCENE_H
#pragma once

const std::string OBJECTPATH = "C:\\Users\\Sigurd\\Documents\\GitHub\\OpenGL\\learnopengl\\learnopengl\\objects\\";


namespace ML {
	class Scene
	{
	public:

		Scene() 
		{
			cameras.push_back(std::shared_ptr<Camera>(new Camera()));
			lights.push_back(std::shared_ptr<Light>(new Light(glm::vec3(0, 200, 0), glm::vec3(1, 1, 1))));
			currentCameraIndex = 0;
		}

		Scene(const Scene&) = delete;

		Scene& operator=(const Scene&) = delete;
		
		void addEntity(MeshEnum type, const Color& color, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale, bool isDynamicEntity)
		{
			try
			{
				if (!meshes[type])
				{
					meshes[type] = std::shared_ptr<MeshIdentifier>(new MeshIdentifier());

				}
				Model model(meshes[type], color, GL_TRIANGLES, 0.5f, 0.5f);
				if (isDynamicEntity)
				{
					entities.push_back(std::shared_ptr<Entity>(new Entity(model, pos, rot, scale)));
				}
				else
				{
					model.setDrawMode(GL_TRIANGLES);
					model.setSurfaceShineVariables(0.5f, 0.5f);
					entities.push_back(std::shared_ptr<Entity>(new Entity(model, pos, rot, scale)));
				}
			}
			catch (const std::exception& err)
			{
				std::cout << "In addEntity():\n";
				throw err;
			}
		}
		void addEntity(std::shared_ptr<Entity> entity) {
			entities.push_back(entity);
		}


		const std::vector<std::shared_ptr<Camera>>& getCameras() const { return cameras;}
		const std::vector<std::shared_ptr<Light>>&  getLights() const { return lights;}
		const std::vector<std::shared_ptr<Entity>>& getEntities() const { return entities;}

		std::map<MeshEnum, std::shared_ptr<MeshIdentifier>>& getMeshes() { return meshes; }

		std::shared_ptr<Camera> getCurrentCamera() {
			return cameras[currentCameraIndex];
		}

	private:
		std::vector<std::shared_ptr<Camera>> cameras;
		std::vector<std::shared_ptr<Light>>  lights;
		std::vector<std::shared_ptr<Entity>> entities;
		unsigned int currentCameraIndex;

		std::map<MeshEnum, std::shared_ptr<MeshIdentifier>> meshes;


	};
}


#endif // !SCENE_H

