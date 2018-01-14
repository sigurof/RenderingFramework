#ifndef MATHS_H
#define MATHS_H
#pragma once
//#include <glm-0.9.8.5\glm\glm.hpp>
//#include "Camera.h"

//#include <glm-0.9.8.5\glm/glm.hpp>
#include <glm-0.9.8.5\glm/gtc/matrix_transform.hpp>
//#include <glm-0.9.8.5\glm/gtc/type_ptr.hpp>


namespace ML {
	class Maths
	{
	public:
		static glm::mat4 createTransformationMatrix(glm::vec3 translation, glm::vec3 angles, glm::vec3 scaleFactors) {
			glm::mat4 matrix;
			matrix = glm::translate(matrix, translation);
			matrix = glm::rotate(matrix, angles.x, glm::vec3(1, 0, 0));
			matrix = glm::rotate(matrix, angles.y, glm::vec3(0, 1, 0));
			matrix = glm::rotate(matrix, angles.z, glm::vec3(0, 0, 1));
			matrix = glm::scale(matrix, scaleFactors);
			return matrix;
		}

		static glm::mat4 createViewMatrix(const Camera& camera)
		{
			glm::mat4 viewMatrix = glm::lookAt(camera.getPosition(), camera.getTarget(), camera.getUp());
			//glm::mat4 viewMatrix;
			//viewMatrix = glm::rotate(viewMatrix, (float)glm::radians(camera.getPitch()), glm::vec3(1, 0, 0));
			//viewMatrix = glm::rotate(viewMatrix, (float)glm::radians(camera.getYaw()), glm::vec3(0, 1, 0));
			//viewMatrix = glm::rotate(viewMatrix, (float)glm::radians(camera.getRoll()), glm::vec3(0, 0, 1));
			//viewMatrix = glm::translate(viewMatrix, -camera.getPosition());
			/*TODO: ADD ROLL AS WELL*/
			return viewMatrix;
		}


	};
}



#endif // MATHS_H