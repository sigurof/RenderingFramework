#ifndef SCENE_H
#define SCENE_H
#pragma once

const std::string OBJECTPATH = "C:\\Users\\Sigurd\\Documents\\GitHub\\OpenGL\\learnopengl\\learnopengl\\objects\\";


namespace ML {



	class Scene
	{
	public:
#ifdef NOUPLOADING
		Scene()
		{
			cameras.push_back(std::shared_ptr<Camera>(new Camera()));
			addLight(MeshInfo(OBJECTPATH + "sphere.obj"), Color(YELLOW), glm::vec3(100, 6000, 100), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.05f, 0.05f, 0.05f));
			currentCameraIndex = 0;
		}
#else
		Scene()
		{
			cameras.push_back(std::shared_ptr<Camera>(new Camera()));
			addLight(MeshEnum::Sphere, Color(YELLOW), glm::vec3(0, 0.6, 0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.05f, 0.05f, 0.05f));
			currentCameraIndex = 0;
		}
#endif // NOUPLOADING

		

		Scene(const Scene&) = delete;

		Scene& operator=(const Scene&) = delete;

#ifdef NOUPLOADING
		void addLight(MeshInfo qualifier, const Color& color, glm::vec3& pos, const glm::vec3&  rot, const glm::vec3& scale)
		{
			try
			{
				if (!meshes[qualifier])
				{
					switch (qualifier.getStorage())
					{
					case Disc:
						meshes[qualifier] = std::shared_ptr<VAOHandle>(new VAOHandle(loader.loadOBJFile(qualifier.getDiscLocation())));
						break;
					case Memory:
						meshes[qualifier] = std::shared_ptr<VAOHandle>(new VAOHandle(loader.loadToVAO(qualifier.getMemoryData())));
						break;
					default:
						throw(std::exception("ERROR! Could not load mesh\n"));
						break;
					}
				}
				Model model(meshes[qualifier], color, GL_TRIANGLES, 1.0f, 1.0f);
				lights.push_back(std::shared_ptr<Light>(new Light(model, pos, rot, scale, color)));
			}
			catch (const std::exception& err)
			{
				std::cout << "In Scene::addLight\n";
				throw err;
			}
	}

#else
		void addLight(MeshEnum type, const Color& color, glm::vec3& pos, const glm::vec3&  rot, const glm::vec3& scale)
		{
			try
			{
				if (!meshes[type])
				{
					meshes[type] = std::shared_ptr<VAOHandle>(new VAOHandle());
				}
				Model model(meshes[type], color, GL_TRIANGLES, 1.0f, 1.0f);
				lights.push_back(std::shared_ptr<Light>(new Light(model, pos, rot, scale, color)));
			}
			catch (const std::exception& err)
			{
				std::cout << "In Scene::addLight\n";
				throw err;
			}
		}

#endif

#ifdef NOUPLOADING
		void addSurface(const MeshInfo& qualifier, const Color& color, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale, bool isDynamicEntity)
		{
			try
			{
				if (!meshes[qualifier])
				{
					switch (qualifier.getStorage())
					{
					case Disc:
						meshes[qualifier] = std::shared_ptr<VAOHandle>(new VAOHandle(loader.loadOBJFile(qualifier.getDiscLocation())));
						break;
					case Memory:
						meshes[qualifier] = std::shared_ptr<VAOHandle>(new VAOHandle(loader.loadToVAO(qualifier.getMemoryData())));
						break;
					default:
						throw(std::exception("ERROR! Could not load mesh\n"));
						break;
					}
				}
				Model model(meshes[qualifier], color, GL_TRIANGLES, 0.5f, 0.5f);
				surfaces.push_back(std::shared_ptr<Entity>(new Entity(model, pos, rot, scale)));
			}
			catch (const std::exception& err)
			{
				std::cout << "In Scene::addSurface():\n";
				throw err;
			}
		}
#else
		void addSurface(MeshEnum type, const Color& color, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale, bool isDynamicEntity)
		{
			try
			{
				if (!meshes[type])
				{
					meshes[type] = std::shared_ptr<VAOHandle>(new VAOHandle());
				}
				Model model(meshes[type], color, GL_TRIANGLES, 0.5f, 0.5f);
				surfaces.push_back(std::shared_ptr<Entity>(new Entity(model, pos, rot, scale)));
			}
			catch (const std::exception& err)
			{
				std::cout << "In addSurface():\n";
				throw err;
			}
		}
#endif

#ifdef NOUPLOADING
		void addLine(const MeshInfo& qualifier, const Color& color, const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale, bool isDynamicEntity) {
			try
			{
				glLineWidth(5);
				if (!meshes[qualifier])
				{
					switch (qualifier.getStorage())
					{
					case Disc:
						meshes[qualifier] = std::shared_ptr<VAOHandle>(new VAOHandle(loader.loadOBJFile(qualifier.getDiscLocation())));
						break;
					case Memory:
						meshes[qualifier] = std::shared_ptr<VAOHandle>(new VAOHandle(loader.loadToVAO(qualifier.getMemoryData())));
						break;
					default:
						throw(std::exception("ERROR! Could not load mesh\n"));
						break;
					}
				}
				Model model(meshes[qualifier], color, GL_LINES, 1.0f, 1.0f);
				std::shared_ptr<Line> line(new Line(model, pos, rot, scale, color));
				line->setEndElement(line->getEntity()->getModel().getRawModel()->getVertexCount() - 1);
				line->setStartElement(0);
				line->setUseGlDrawElementRange(true);
				lines.push_back(line);
			}
			catch (const std::exception& err)
			{
				std::cout << "In Scene::addLine\n";
				throw err;
			}
		}
#endif // NOUPLOADING

		

		void addSurface(std::shared_ptr<Entity> entity) {
			surfaces.push_back(entity);
		}


		const std::vector<std::shared_ptr<Camera>>& getCameras() const { return cameras;}
		const std::vector<std::shared_ptr<Light>>&  getLights() const { return lights;}
		const std::vector<std::shared_ptr<Entity>>& getSurfaces() const { return surfaces;}
		const std::vector<std::shared_ptr<Line>>& getLines() const { return lines; }
#ifdef NOUPLOADING
#ifdef UNORDEREDMAP
		std::unordered_map<MeshInfo, std::shared_ptr<VAOHandle>, MeshInfoHasher>& getMeshes() { return meshes; }
#else
		std::map<MeshInfo, std::shared_ptr<VAOHandle>>& getMeshes() { return meshes; }

#endif // UNORDEREDMAP



#else
		std::map<MeshEnum, std::shared_ptr<VAOHandle>>& getMeshes() { return meshes; }
#endif // NOUPLOADING


		std::shared_ptr<Camera> getCurrentCamera() {
			return cameras[currentCameraIndex];
		}

	private:
		std::vector<std::shared_ptr<Camera>> cameras;
		std::vector<std::shared_ptr<Light>>  lights;
		std::vector<std::shared_ptr<Entity>> surfaces;
		std::vector<std::shared_ptr<Line>> lines;
		/* 
		Note on entities: All entities that should belong to a scene should be created from within that scene instance.
		This is to ensure the correct creation of VAOHandles and Model maps so that no excessive information is uploaded
		to the graphics card or just regular memory for that matter.
		*/
		unsigned int currentCameraIndex;

#ifdef NOUPLOADING
		Loader loader;
#ifdef UNORDEREDMAP
		std::unordered_map<MeshInfo, std::shared_ptr<VAOHandle>, MeshInfoHasher> meshes;
#else
		/*
		the map comparator must have strict ordering. 
		if cmp(MeshInfo1, MeshInfo2) == false 
		then cmp(MeshInfo2, MeshInfo1) == true
		So equal_to is not a valid comparator for map
		*/
		std::map<MeshInfo, std::shared_ptr<VAOHandle>> meshes;

#endif // UNORDEREDMAP

#else
		std::map<MeshEnum, std::shared_ptr<VAOHandle>> meshes;

#endif // NOUPLOADING

		//std::vector<std::shared_ptr<VertexData>> 

	};
}


#endif // !SCENE_H

