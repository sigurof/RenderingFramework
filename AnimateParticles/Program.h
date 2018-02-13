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

//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//	/* Program */
//	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
//		Window::currentlyActiveWindow->close();
//	if (key == GLFW_KEY_R && action == GLFW_RELEASE)
//		
//
//	/* Window */
//	else if (key == GLFW_KEY_1 && action == GLFW_RELEASE)
//		Window::currentlyActiveWindow->captureMouse();
//	else if (key == GLFW_KEY_2 && action == GLFW_RELEASE)
//		Window::currentlyActiveWindow->releaseMouse();
//
//}

class Program
{
public:

	Program() : Program("OpenGL") {}

	Program(const std::string& title)
	{
		//scene = std::shared_ptr<Scene>(new Scene);
		Program::currentlyActiveProgram = this;
		//openViewPort(title, width, height);
	}

	~Program() {
#ifdef VERBOSEDESTRUCTORS
		std::cout << "Program::Destructor\n";
#endif // VERBOSEDESTRUCTORS
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
#ifndef NOUPLOADING
		loadUpScene();
		prepareRenderer();
#endif // NOUPLOADING
	}
	
	void setKeyCallback(void(*f)(GLFWwindow*, int, int, int, int)) 
	{
		glfwSetKeyCallback(window->getGlfwWindow(), f);
	}

	void setFrameRate(int fps) {
		playback->setFrameRate(fps);
	}

	void makeSceneFromPlaybackData(const std::string& path) 
	{
		playback = sptr<Animator>(new Animator(path));
		playback->setFrameRate(25);
#ifndef NOUPLOADING
		this->scene = playback->makeScene(loader);
#else
		this->scene = playback->makeScene(loader);

#endif
	}
#ifdef NOUPLOADING
#else
	void loadUpScene()
	{
		for (std::map<MeshEnum, std::shared_ptr<VAOHandle>>::iterator it = scene->getMeshes().begin();
			it != scene->getMeshes().end(); it++)
		{
			switch (it->first)
			{
			case Sphere:
				*(it->second) = VAOHandle(loader.loadOBJFile(OBJECTPATH + "sphere.obj"));
				break;
			case Cube:
				*(it->second) = VAOHandle(loader.loadOBJFile(OBJECTPATH + "cube.obj"));
				break;

			default:
				break;
			}
		}
	}

#endif // NOUPLOADING

	
#ifdef NOUPLOADING
	void addParticleTraceToScene(unsigned int index) 
	{
		playback->addParticleTraceToScene(scene, index);
	}
#endif // NOUPLOADING

	void toggleAlpha(bool on) {
		if (on)
		{
			window->enableAlpha();
		}
		else
		{
			window->disableAlpha();
		}
	}

	void setMouseSensitivity(float sensitivity)
	{
		window->setMouseSensitivity(sensitivity);
	}

	void shouldPlay(bool play)
	{
		playback->setRunning(play);
	}

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

	void setSpeed(float speed) {
		Camera::currentlyActiveCamera->setSpeed(speed);
	}

	void run() {
		try
		{
#ifdef NOUPLOADING
			prepareRenderer();
#endif // NOUPLOADING
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

	void restartPlayback() {
		if (playback.operator bool())
		{
			playback->restart();
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

	const std::shared_ptr<Animator> getPlayback() { return playback; }

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
			if (playback->isRunning())
			{
				if (playback->getInternalClock().elapsedS() > playback->getNextFrameInterval())
				{
					playback->updateScene();
				}
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
		renderer->setSurfaceShader(std::shared_ptr<Shader>(new Shader(SHADERPATH + "shader.vs", SHADERPATH + "shader.fs")));
		renderer->setLightShader(std::shared_ptr<Shader>(new Shader(SHADERPATH + "light.vs", SHADERPATH + "light.fs")));
		renderer->prepareSurfaceShader();
#ifdef NOUPLOADING
		renderer->setLineShader(std::shared_ptr<Shader>(new Shader(SHADERPATH + "line.vs", SHADERPATH + "line.fs")));
		renderer->prepareLineShader();
#endif // NOUPLOADING

		renderer->prepareLightShader();
	}

	void setWindowCallbackFunctions() 
	{
		glfwSetFramebufferSizeCallback(window->getGlfwWindow(), framebuffer_size_callback);
		glfwSetCursorPosCallback(window->getGlfwWindow(), cursor_move_callback);
		glfwSetMouseButtonCallback(window->getGlfwWindow(), mouse_button_callback);
		glfwSetWindowCloseCallback(window->getGlfwWindow(), window_close_callback);
		//glfwSetKeyCallback(window->getGlfwWindow(), key_callback);
	}

	void loadPlaybackData(const std::string& path) {}


};



#endif // !PROGRAM_H
