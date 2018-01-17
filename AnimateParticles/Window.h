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
		
		void swapBuffers() 
		{
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
					if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
					{
						throw std::exception("Failed to initialize GLAD");
					}
					enableAlpha();
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
		bool isMouseCaptured() const { return mouseIsCaptured; }
		float getLastMouseXpos() const { return lastMouseXpos; }
		float getLastMouseYpos() const { return lastMouseYpos; }
		float getMouseSensitivity() const { return mouseSensitivity; }

		void setWidth(int width) { glfwSetWindowSize(window, width, getHeight()); }
		void setHeight(int height) { glfwSetWindowSize(window, getWidth(), height); }
		void setWindow(GLFWwindow* win) { window = win; }
		void setTitle(const std::string& tit) { title = tit; }
		//void setDt(float dt) { 
		//	//renderer->setDt(dt); 
		//}
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

#endif // !WINDOW_H
