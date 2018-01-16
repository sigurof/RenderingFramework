#ifndef MATHS_H
#define MATHS_H
#pragma once



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

		static glm::mat4 createViewMatrix(const std::shared_ptr<Camera> camera)
		{
			glm::mat4 viewMatrix = glm::lookAt(camera->getPosition(), camera->getTarget(), camera->getUpDirection());
			return viewMatrix;
			
			//glm::mat4 viewMatrix = glm::lookAt(Camera::currentlyActiveCamera->getPosition(), Camera::currentlyActiveCamera->getTarget(), Camera::currentlyActiveCamera->getUpDirection());
			return viewMatrix;
		}


	};
}



#endif // MATHS_H