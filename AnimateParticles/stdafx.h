#include <iostream>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <glm-0.9.8.5\glm/glm.hpp>
#include <glm-0.9.8.5\glm/gtc/matrix_transform.hpp>
#include <glm-0.9.8.5\glm/gtc/type_ptr.hpp>

#include <chrono>
#include "Clock.h"
#include "MeshIdentifier.h"
#include <string>
#include <fstream>
#include <sstream>
#include <map>


#include "Loader.h"
#include "Model.h"
#include "ShaderProgram.h"
#include "StaticShader.h"
#include "Color.h"
#include "Texture.h"
#include "Surface.h"
#include "ModelTexture.h"
#include "Maths.h"
#include "OBJLoader.h"
#include "Light.h"
#include "Renderer.h"
#include "Window.h"


#include "GraphicsEngine.h"
#include "Animator.h"