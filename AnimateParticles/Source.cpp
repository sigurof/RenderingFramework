// learnopengl.cpp : Defines the entry point for the console application.

#define BOOST_CONFIG_SUPPRESS_OUTDATED_MESSAGE
#define NOUPLOADING
#define UNORDEREDMAP
//#define VERBOSEDESTRUCTORS

#include "stdafx.h"
const std::string SHADERPATH = "C:\\Users\\Sigurd\\Documents\\GitHub\\OpenGL\\learnopengl\\learnopengl\\shader_source\\";
const std::string simulationFile = "C:\\Users\\Sigurd\\Documents\\GitHub\\ParticlesInBox\\RigidBody\\ParticlesInBox\\newtwo.pos";
#include "includes.h"



using namespace ML;


Shader* Shader::currentlyActiveShader = nullptr;
Camera* Camera::currentlyActiveCamera = nullptr;
Window* Window::currentlyActiveWindow = nullptr;
Renderer* Renderer::currentlyActiveRenderer = nullptr;
Program* Program::currentlyActiveProgram = nullptr;
//unsigned int MeshInfo::nInstances = 0;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/* Program */
	if (key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
		Program::currentlyActiveProgram->terminate();	
	if (key == GLFW_KEY_R && action == GLFW_RELEASE)
		Program::currentlyActiveProgram->restartPlayback();
	if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
		Program::currentlyActiveProgram->shouldPlay(!Program::currentlyActiveProgram->getPlayback()->isRunning());

		/* Window */
	else if (key == GLFW_KEY_1 && action == GLFW_RELEASE)
		Window::currentlyActiveWindow->captureMouse();
	else if (key == GLFW_KEY_2 && action == GLFW_RELEASE)
		Window::currentlyActiveWindow->releaseMouse();

}

bool runProgram()
{
	Program p;
#ifndef NOUPLOADING
	p.makeSceneFromPlaybackData(simulationFile);
	p.shouldPlay(true);
#endif
	p.openViewPort("OpenGL", 1920, 1080);
#ifdef NOUPLOADING
	p.makeSceneFromPlaybackData(simulationFile);
	p.shouldPlay(true);
	p.addParticleTraceToScene(0);
#endif // NOUPLOADING

	p.setKeyCallback(key_callback);
	p.setFrameRate(25);
	p.toggleAlpha(true);
	p.setMouseSensitivity(0.03f);
	p.setSpeed(2.0);
	p.run();
	return true;
}

class A
{
public:
	A(int a, const std::string& str) : a(a), str(str) {}
	int getA() const { return a; }
	const std::string& getStr() const { return str; }
	
private:
	int a;
	std::string str;

};

bool operator<(const A& lhs, const A& rhs)
{
	if (lhs.getA() == rhs.getA())
	{
		return lhs.getStr() < rhs.getStr();
	}
	return lhs.getA() < rhs.getA();
}

class B
{
public:
	B() : b(0), valid(false) {}
	B(int b) : b(b), valid(true) {}
	int getB() const { return b; }
	
	explicit operator bool() const
	{
		return this->valid;
	}

private:
	int b;
	bool valid;
};

int main()
{
	try
	{
		//using storage_t = std::vector<std::pair<A, B>>;
		//uto a = std::declval<storage_t&>();
		runProgram();
		//std::map<A, B> u;
		//for (int i = 0; i < 10; i++)
		//{
		//	A a(i,"a"); B b(i*i);
		//	u[a] = b;
		//}
		//std::cout << "u[A(2,\"b\")].operator bool() = " << u[A(2, "b")].operator bool();
		
	}
	catch (const std::exception& err)
	{
		std::cout << "in main:\n" << err.what() << std::endl;
		return 1; /* Fail code */
	}
	return 0;/* Success */
}


//std::vector<float> vertices =
//{
//	-0.5f, -0.5f, -0.5f,
//	-0.5f,  0.5f, -0.5f,
//	-0.5f, -0.5f,  0.5f,
//	-0.5f,  0.5f,  0.5f,
//	0.5f, -0.5f, -0.5f,
//	0.5f,  0.5f, -0.5f,
//	0.5f, -0.5f,  0.5f,
//	0.5f,  0.5f,  0.5f
//};
//
//std::vector<float> indices =
//{
//	0,4,5,
//	5,1,0,
//	2,6,7,
//	7,3,2,
//	3,1,0,
//	0,2,3,
//	7,5,4,
//	4,6,7,
//	0,4,6, 
//	6,2,0,
//	1,5,7, 
//	7,3,1
//};