#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <vector>
#include <map>
#include <memory>
#include <glm-0.9.8.5\glm/glm.hpp>
#include <glm-0.9.8.5\glm/gtc/matrix_transform.hpp>
#include <glm-0.9.8.5\glm/gtc/type_ptr.hpp>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include "stb_image.h"
#include "Enums.h"
#include "Camera.h"
#include "Maths.h"
#include "Clock.h"
#include "RigidBodySimulator.h"
#include "Window.h"
/* Renderer */
			#include "Color.h"
			#include "ModelTexture.h"
			#include "Surface.h"
			#include "MeshIdentifier.h"
		#include "Model.h"
	#include "Entity.h"
	/* Model */
	#include "Light.h"
	/* StaticShader */
		#include "ShaderProgram.h"
	#include "StaticShader.h"
#include "Texture.h"

#include "Loader.h"
#include "OBJLoader.h"

#include "Scene.h"
#include "Renderer.h"
#include "GraphicsEngine.h"
#include "Animator.h"
#include "Program.h"