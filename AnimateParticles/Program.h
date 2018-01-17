#ifndef PROGRAM_H
#define PROGRAM_H
#pragma once

float yaw(90.0f); float pitch(0.0f); bool firstMouseMovement = false;

template <class T>
using sptr = std::shared_ptr<T>;

void cursor_move_callback(GLFWwindow* window, double xpos, double ypos)
{
	float dx = (float)xpos - Window::currentlyActiveWindow->getLastMouseXpos();
	float dy = (float)ypos - Window::currentlyActiveWindow->getLastMouseYpos();
	Window::currentlyActiveWindow->setLastMouseXpos((float)xpos);
	Window::currentlyActiveWindow->setLastMouseYpos((float)ypos);
	if (Window::currentlyActiveWindow->isMouseCaptured())
	{
		dx *= Window::currentlyActiveWindow->getMouseSensitivity();
		dy *= Window::currentlyActiveWindow->getMouseSensitivity();
		yaw -= dx; pitch -= dy;
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
		Camera::currentlyActiveCamera->changeViewDirection(yaw, pitch);
	}
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		std::cout << "Left press\n";
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		std::cout << "Left release\n";

}

void window_close_callback(GLFWwindow* window)
{
	ML::Window::currentlyActiveWindow->close();
}
	
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	if (ML::Renderer::currentlyActiveRenderer)
	{
		ML::Renderer::currentlyActiveRenderer->setAspectRatio((float)width / (float)height);
	}
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/* Program */
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
		Window::currentlyActiveWindow->close();
	
	/* Window */
	else if (key == GLFW_KEY_1 && action == GLFW_RELEASE)
		Window::currentlyActiveWindow->captureMouse();
	else if (key == GLFW_KEY_2 && action == GLFW_RELEASE)
		Window::currentlyActiveWindow->releaseMouse();
	

}

class Program
{
public:

	Program() {}

	Program(const std::string& title, unsigned int width, unsigned int height) 
	{
		openViewPort(title, width, height);
	}

	~Program() {
		std::cout << "Program::Destructor\n";
		if (currentlyActiveProgram == this)
		{
			currentlyActiveProgram = nullptr;
		}
		if (playback.operator bool())
		{
			playback->stop();
		}
		else if (physicsEngine.operator bool())
		{
			physicsEngine->stop();
		}
	}
	
	Program(const Program&) = delete;
	
	Program& operator=(const Program&) = delete;
	
	void openViewPort(const std::string& title, unsigned int width, unsigned int height) 
	{
		openWindow(title, width, height);
		window->captureMouse();
		loadUpScene();
		prepareRenderer();
	}
	
	void makeSceneFromPlaybackData(const std::string& path) 
	{
		playback = sptr<Animator>(new Animator(path));
		playback->setFrameRate(25);
		this->scene = playback->initializeScene();
	}

	void loadUpScene() 
	{	
		for (std::map<MeshEnum, std::shared_ptr<MeshIdentifier>>::iterator it = scene->getMeshes().begin();
			it != scene->getMeshes().end(); it++)
		{
			switch (it->first)
			{
			case Ball:
				*(it->second) = MeshIdentifier(OBJLoader::loadObjModel(OBJECTPATH + "sphere.obj", loader));
				break;
			case Cube:
				*(it->second) = MeshIdentifier(OBJLoader::loadObjModel(OBJECTPATH + "cube.obj", loader));
				break;
			default:
				break;
			}
			
		}
	}

	void shouldPlay(){}

	void pollUserInput() {
		glfwPollEvents();
	}

	void processUserInput() 
	{
		if (Window::currentlyActiveWindow->isMouseCaptured())
		{
			if (glfwGetKey(window->getGlfwWindow(), GLFW_KEY_W) == GLFW_PRESS) { Camera::currentlyActiveCamera->move(DirectionEnum::Forwards); }
			if (glfwGetKey(window->getGlfwWindow(), GLFW_KEY_S) == GLFW_PRESS) { Camera::currentlyActiveCamera->move(DirectionEnum::Backwards); }
			if (glfwGetKey(window->getGlfwWindow(), GLFW_KEY_D) == GLFW_PRESS) { Camera::currentlyActiveCamera->move(DirectionEnum::Left); }
			if (glfwGetKey(window->getGlfwWindow(), GLFW_KEY_A) == GLFW_PRESS) { Camera::currentlyActiveCamera->move(DirectionEnum::Right); }
			if (glfwGetKey(window->getGlfwWindow(), GLFW_KEY_E) == GLFW_PRESS) { Camera::currentlyActiveCamera->move(DirectionEnum::Up); }
			if (glfwGetKey(window->getGlfwWindow(), GLFW_KEY_Q) == GLFW_PRESS) { Camera::currentlyActiveCamera->move(DirectionEnum::Down); }
		}

	}

	void run() {
		try
		{
			if (playback.operator bool()) { playback->start(0); }
			while (window->isOpen())
			{
				currentCameraTime = (float)glfwGetTime(); 
				scene->getCurrentCamera()->setDt(currentCameraTime - lastCameraTime);
				lastCameraTime = currentCameraTime;


				drawSceneToWindowUsingRenderer();

				pollUserInput();
				processUserInput();
				updateScene();
				glfwSwapBuffers(window->getGlfwWindow());
			}
		}
		catch (const std::exception& err)
		{
			std::cout << "In Animator::play():\n";
			throw err;
		}
	}

	void terminate() {
		window->close();
		if (playback.operator bool())
		{
			playback->stop();
		}
		else if (physicsEngine.operator bool())
		{
			physicsEngine->stop();
		}
	}

	static Program* currentlyActiveProgram;

private:
	std::shared_ptr<Window> window;
	std::shared_ptr<Scene> scene;
	std::shared_ptr<Renderer> renderer;
	std::shared_ptr<Animator> playback;
	std::shared_ptr<RigidBodySimulator> physicsEngine;

	float lastCameraTime;
	float currentCameraTime;

	Loader loader;


	void updateScene() {
		if (playback.operator bool())
		{
			if (playback->getInternalClock().elapsedS() > playback->getNextFrameInterval())
			{
				playback->updateScene();
			}
		}
		else if (physicsEngine.operator bool())
		{
			updateSceneUsingSimulator();
		}
	}

	void drawSceneToWindowUsingRenderer() 
	{
		if (renderer->getScene())
		{
			renderer->render();
		}
	}

	void updateSceneUsingSimulator() {}

	void openWindow(const std::string& name, unsigned int width, unsigned int height)
	{
		window = std::shared_ptr<Window>(new Window("OpenGL", width, height));
		setWindowCallbackFunctions();
	}

	void prepareRenderer() 
	{
		renderer = std::shared_ptr<Renderer>(new Renderer);
		renderer->setScene(scene);
		renderer->setShader(std::shared_ptr<StaticShader>(new StaticShader));
		renderer->prepareShader();
	}

	void setWindowCallbackFunctions() 
	{
		glfwSetFramebufferSizeCallback(window->getGlfwWindow(), framebuffer_size_callback);
		glfwSetCursorPosCallback(window->getGlfwWindow(), cursor_move_callback);
		glfwSetMouseButtonCallback(window->getGlfwWindow(), mouse_button_callback);
		glfwSetWindowCloseCallback(window->getGlfwWindow(), window_close_callback);
		glfwSetKeyCallback(window->getGlfwWindow(), key_callback);
	}

	

	void cleanUp(){
			
	}

	void loadPlaybackData(const std::string& path) {}


};



#endif // !PROGRAM_H
