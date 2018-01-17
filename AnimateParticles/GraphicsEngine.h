//#ifndef GRAPHICSENGINE_H
//#define GRAPHICSENGINE_H
//#pragma once
//
//
//using namespace ML;
//
////const std::string OBJECTPATH = "C:\\Users\\Sigurd\\Documents\\GitHub\\OpenGL\\learnopengl\\learnopengl\\objects\\";
//const std::string TEXTUREPATH = "C:\\Users\\Sigurd\\Documents\\GitHub\\OpenGL\\learnopengl\\learnopengl\\texture_files\\";
//
//float lastTime = 0.0f;
//float currentTime = 0.0f;
//
//class GraphicsEngine
//{
//public:
//
//	GraphicsEngine()
//	{
//		window = new Window("OpenGL");
//		light = Light(glm::vec3(0, 10, 0), glm::vec3(1,1,1));	
//	}
//
//	GraphicsEngine(const GraphicsEngine& ge) : window(ge.window), light(ge.light), instancesOfMesh(ge.instancesOfMesh), meshes(ge.meshes), loader(ge.loader)
//	{
//		for each (Entity* e in ge.dynamicEntities)
//		{
//			this->dynamicEntities.push_back(new Entity(e->getModel(), e->getPosition(), e->getAngles(), e->getScaleFactors()));
//		}
//	}
//
//	~GraphicsEngine()
//	{
//		window->~Window();
//		dynamicEntities.clear();
//		staticEntities.clear();
//	}
//
//	void setLightPosition(const glm::vec3& pos) {
//		light.setPosition(pos);
//	}
//
//	void shiftEntities(const std::vector<glm::vec3>& newCenters) {
//		for (unsigned int i = 0; i < dynamicEntities.size(); i++)
//		{
//			dynamicEntities[i]->setPosition(newCenters[i]);
//		}
//	}
//
//	//void setCameraPosition(const glm::vec3& pos)
//	//{
//	//	window->setCameraPosition(pos);
//	//}
//	
//	void hideDynamicEntity(unsigned int index) 
//	{
//		dynamicEntities[index]->setDraw(false);
//
//	}
//
//	void hideStaticEntity(unsigned int index)
//	{
//		staticEntities[index]->setDraw(false);
//
//	}
//
//	//void openWindow(int width, int height) {
//	//	window->open(width, height);
//	//	for (unsigned int i = 0; i < dynamicEntities.size(); i++)
//	//	{
//	//		if (dynamicEntities[i]->getModel().getSurface().isTextureActive())
//	//		{
//	//			tex.activate();
//	//		}
//	//	}
//	//}
//
//	bool windowIsOpen() {
//		return window->isOpen();
//	}
//
//	//void drawScene()
//	//{
//	//	try
//	//	{
//	//		currentTime = (float)glfwGetTime(); window->setDt(currentTime - lastTime); lastTime = currentTime;
//	//		window->swapBuffers();
//	//
//	//	}
//	//	catch (const std::exception& err)
//	//	{
//	//		std::cout << "In Animator::play():\n";
//	//		throw err;
//	//	}
//	//}
//
//	void cleanUp() {
//		loader.cleanUp();
//		glfwTerminate();
//	}
//
//	const std::vector<Entity*>& getDynamicEntities()
//	{
//		return dynamicEntities;
//	}
//
//private:
//	Window* window;
//
//	std::vector<Entity*> dynamicEntities;
//	std::vector<Entity*> staticEntities;
//	Light light;
//
//	Loader loader;
//
//	std::map <MeshEnum, unsigned int> instancesOfMesh;
//	std::map<MeshEnum, MeshIdentifier> meshes;
//
//};
//
//#endif // !GRAPHICSENGINE_H
