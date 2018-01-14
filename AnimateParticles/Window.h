#ifndef WINDOW_H
#define WINDOW_H
#pragma once
//
//#include <iostream>
//#include <glad/glad.h>
//#include <glfw/glfw3.h>
//#include "Camera.h"
//#include "GraphicsEngine.h"

//namespace ML {
//	class Renderer;
//}

//1
#include "Renderer.h"

//2
//namespace ML {
//	class Renderer;/* NB! THIS LINE FIXES CIRCULAR REFERENCE PROBLEM*/
//}

namespace ML
{
	void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}


}
namespace ML
{
	class Window
	{
	public:
		
		Window() : Window("OpenGL") {}

		Window(const std::string& title) : title(title), window(nullptr), isopen(false)
		{
			try
			{
				if (!glfwInit())
				{
					throw std::exception("Could not initiate GLFW\n");
				}
				glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//Sets opengl v3
				glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//Sets opengl vX.3
				glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//A smaller set of functions
																			  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE); //needed for mac
			}
			catch (const std::exception& err)
			{
				std::cout << "In Window::Constructor:\n";
				throw err;
			}
		}

		Window(const Window& win) : Window(win.getTitle()) {}//, win.getRenderer()) {}

		//Window(const std::string& title) : //, const Renderer& rend) : 
		//	title(title), window(nullptr)//, renderer(rend)//, renderer(rend, *this), title(title)
		//{
		//	try
		//	{
		//		if (!glfwInit())
		//		{
		//			throw std::exception("Could not initiate GLFW\n");
		//		}
		//		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//Sets opengl v3
		//		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//Sets opengl vX.3
		//		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//A smaller set of functions
		//	  //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE); //needed for mac
		//	}
		//	catch (const std::exception& err)
		//	{
		//		std::cout << "In Window::Constructor:\n";
		//		throw err;
		//	}

		//};

		~Window() {
			if (isOpen())
			{
				close();
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
	
		void processInput()
		{
			if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				close();
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				this->moveCamera(DirectionEnum::Forwards);
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				this->moveCamera(DirectionEnum::Backwards);
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				this->moveCamera(DirectionEnum::Left);
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				this->moveCamera(DirectionEnum::Right);
			if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
				this->moveCamera(DirectionEnum::Up);
			if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
				this->moveCamera(DirectionEnum::Down);
		}

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
					glfwMakeContextCurrent(window);//Context are above my head for the moment
					glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);// Use a callback function to do this.
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
					glfwSetCursorPos(window, 0, 0);
					glfwSetCursorPosCallback(window, mouse_callback);
					if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))//Cast to GLADloadproc
					{
						throw std::exception("Failed to initialize GLAD");
					}
					isopen = true;
					renderer = new Renderer(this->getAspectRatio());
					renderer->makeShader();
					//This is necessary for alpha channel to work!
					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
					//
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
			//DEBUG
			if (window==nullptr || window == NULL)
			{
				std::cout << "window==nullptr||NULL\n";
			}
			//DEBUG
		}

		void setCameraPosition(const glm::vec3& pos)
		{
			renderer->setCameraPosition(pos);
		}

		void moveCamera(const DirectionEnum dir) { renderer->moveCamera(dir); }

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
		const GLFWwindow* getGlfwWindow() const { return window; }
		const std::string& getTitle() const { return title; }
		const Renderer* getRenderer() const { return renderer; }
		
		void setWidth(int width) { glfwSetWindowSize(window, width, getHeight()); }
		void setHeight(int height) { glfwSetWindowSize(window, getWidth(), height); }
		void setWindow(GLFWwindow* win) { window = win; }
		void setRenderer(const Renderer& ren) {  }
		void setTitle(const std::string& tit) { title = tit; }
		void setDt(float dt) { renderer->setDt(dt); }



	private:
		GLFWwindow* window;
		/*!!!!!!!!!!!Renderer should not be a member of Window!!!!!!*/
		Renderer* renderer;
		//Camera camera;
		std::string title;
		bool isopen;
	};


}
#endif // !WINDOW_H

