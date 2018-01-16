#ifndef WINDOW_H
#define WINDOW_H
#pragma once


namespace ML
{
	class Window
	{
	public:
		
		Window() : Window("OpenGL") {}

		Window(const std::string& title) : Window(title, 800, 600) {}

		Window(const std::string& title, unsigned int width, unsigned int height) : 
		Window(title, width, height, 0.05f) {}

		Window(const std::string& title, unsigned int width, unsigned int height, float sensitivity) :
			title(title), window(nullptr), isopen(false), mouseSensitivity(sensitivity)
		{
			try
			{
				if (!glfwInit())
				{
					throw std::exception("Could not initiate GLFW\n");
				}

				initializeOpenGL(3, 3); //OpenGL v3.3 (argument 3, 3 is only a dummy at this point)
				open(width, height);
			}
			catch (const std::exception& err)
			{
				std::cout << "In Window::Constructor:\n";
				throw err;
			}

		}

		Window& operator=(const Window&) = delete;

		~Window() {
			std::cout << "Window::Destructor\n";
			if (isOpen())
			{
				close();
			}
			if (currentlyActiveWindow == this)
			{
				currentlyActiveWindow = nullptr;
			}
		}

		void clear(float r, float g, float b, float alpha) {
			glClearColor(r, g, b, alpha);
			glClear(GL_COLOR_BUFFER_BIT);
		}
		
		void swapBuffers() {

			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glfwSwapBuffers(window);
		}

		bool isOpen() const { return isopen; }
		
		void pollEvent() { glfwPollEvents(); }

		void open(int width, int height)
		{
			try
			{
				if (!this->isOpen())
				{
					window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
					if (window == NULL)
					{
						glfwTerminate();
						throw std::exception("Failed to create GLFW window\n");
					}
					isopen = true;
					setThisToCurrentlyActive();
					/*Callbacks*/
					if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
					{
						throw std::exception("Failed to initialize GLAD");
					}
					enableAlpha();
					//renderer = new Renderer(this->getAspectRatio());
					//renderer->makeShader();
				}
				else
				{
					throw(std::exception("ERROR! Cannot open window which is already open!\n"));
				}
			}
			catch (const std::exception& err)
			{
				std::cout << "In Window::open():\n";
				throw err;
			}
		}

		void close() {
			isopen = false;
			glfwSetWindowShouldClose(window, true);
		}

		void setCameraPosition(const glm::vec3& pos)
		{
			//renderer->setCameraPosition(pos);
		}

		void moveCamera(const DirectionEnum dir) { 
			/*renderer->moveCamera(dir);*/
		 }

		void enableAlpha() {
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		int getWidth() const 
		{
			try
			{
				if (!this->isOpen())
				{
					throw(std::exception("ERROR! Window is not open; cannot access its width!\n"));
				}
				int width, height;
				glfwGetWindowSize(window, &width, &height);
				return width;
			}
			catch (const std::exception& err)
			{
				std::cout << "In Window::getWidth():\n";
				throw err;
			}
		}
		int getHeight() const 
		{
			try
			{
				if (!this->isOpen())
				{
					throw(std::exception("ERROR! Window is not open; cannot access its height!\n"));
				}
				int width, height;
				glfwGetWindowSize(window, &width, &height);
				return height;
			}
			catch (const std::exception& err)
			{
				std::cout << "In Window::getHeight():\n";
				throw err;
			}
		}
		float getAspectRatio() const 
		{
			try
			{
				if (!this->isOpen())
				{
					throw(std::exception("ERROR! Window is not open; cannot access its aspect ratio!\n"));
				}
				int width, height;
				glfwGetWindowSize(window, &width, &height);
				return (float)width / (float)height;
			}
			catch (const std::exception& err)
			{
				std::cout << "In Window::getAspectRatio():\n";
				throw err;
			}
		}

		void captureMouse()
		{
			this->lastMouseXpos = 0;
			this->lastMouseYpos = 0;
			glfwSetCursorPos(window, 0, 0);
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			this->mouseIsCaptured = true;
		}

		void releaseMouse()
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			this->mouseIsCaptured = false;
		}


		GLFWwindow* getGlfwWindow() const { return window; }
		const std::string& getTitle() const { return title; }
		//const Renderer* getRenderer() const { return renderer; }
		bool isMouseCaptured() const { return mouseIsCaptured; }
		float getLastMouseXpos() const { return lastMouseXpos; }
		float getLastMouseYpos() const { return lastMouseYpos; }
		float getMouseSensitivity() const { return mouseSensitivity; }

		void setWidth(int width) { glfwSetWindowSize(window, width, getHeight()); }
		void setHeight(int height) { glfwSetWindowSize(window, getWidth(), height); }
		void setWindow(GLFWwindow* win) { window = win; }
		//void setRenderer(const Renderer& ren) {  }
		void setTitle(const std::string& tit) { title = tit; }
		void setDt(float dt) { 
			//renderer->setDt(dt); 
		}
		void setLastMouseXpos(const float x) { lastMouseXpos = x; }
		void setLastMouseYpos(const float y) { lastMouseYpos = y; }
		void setMouseSensitivity(const float val) { mouseSensitivity = val; }

		void setThisToCurrentlyActive() {
			Window::currentlyActiveWindow = this;
			glfwMakeContextCurrent(window);
		}

		static Window* currentlyActiveWindow;




	private:
		GLFWwindow* window;
		/*!!!!!!!!!!!Renderer should not be a member of Window!!!!!!*/
		//Renderer* renderer;
		//Camera camera;
		std::string title;
		bool isopen;

		bool mouseIsCaptured;
		float mouseSensitivity;

		float lastMouseXpos;
		float lastMouseYpos;

		void initializeOpenGL(unsigned int major, unsigned int minor) 
		{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//Sets opengl v3
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//Sets opengl vX.3
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//A smaller set of functions
			 //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE); //needed for mac
		}
	};
}



//void Window::mouse_capture() {
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//	this->mouse_is_captured = true;
//}
//
//void Window::mouse_release() {
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//	this->mouse_is_captured = false;
//}
//float last_mouse_xpos = 400;
//float last_mouse_ypos = 300;
//
//void mouse_move_callback_helper(GLFWwindow* window, double xpos, double ypos) {
//	float dx = xpos - last_mouse_xpos;
//	float dy = ypos - last_mouse_ypos;
//	last_mouse_xpos = xpos;
//	last_mouse_ypos = ypos;
//	if (Window::context
//		&& Window::context->mouse_is_captured) {
//		for (auto fun : Window::context->mouse_move_events) {
//			fun(dx, dy);
//		}
//	}
//}
//glfwSetCursorPosCallback(window, mouse_move_callback_helper);
//app.register_mouse_move_callback([&](double dx, double dy) {
//	const float angle = 5.0f * Window::context->frame_time;
//	float c = cos(angle / 2);
//	float s = sin(angle / 2);
//
//	glm::quat r = glm::quat(c, dy * s, dx * s, 0) * Window::context->get_scene()->camera.rotation;
//
//	Window::context->get_scene()->camera.rotation = glm::normalize(r);
//
//
//});
//void register_key_event(int key, int key_action, std::function<void()> fun);
//void register_mouse_move_callback(std::function<void(double, double)> fun);
#endif // !WINDOW_H
