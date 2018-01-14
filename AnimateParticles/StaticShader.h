#ifndef STATICSHADER_H
#define STATICSHADER_H
#pragma once
//#include <glad\glad.h>
//#include <GLFW\glfw3.h>
//#include "Window.h"
//#include "Renderer.h"
//#include "Camera.h"
//
//#include "ShaderProgram.h"
//#include <string>
//#include "Camera.h"
//#include "Maths.h"
//#include "Light.h"

#include "ShaderProgram.h"
#include <glm-0.9.8.5\glm\glm.hpp>
#include "Camera.h"
#include "Maths.h"
#include "Light.h"
#include "Surface.h"
#include "Color.h"
#include <glad\glad.h>
#include <GLFW\glfw3.h>



namespace ML {

	class StaticShader : public ShaderProgram
	{
	public:

		StaticShader(const StaticShader& stat) : 
			location_transformationMatrix(stat.location_transformationMatrix), location_projectionMatrix(stat.location_projectionMatrix), 
			location_viewMatrix(stat.location_viewMatrix), location_lightPosition(stat.location_lightPosition),
			location_lightColor(stat.location_lightColor), location_shineDamper(stat.location_shineDamper),
			location_reflectivity(stat.location_reflectivity),location_color(stat.location_color),
			location_textureFraction(stat.location_textureFraction), ShaderProgram(VERTEXPATH, FRAGMENTPATH)
		{
			linkProgram();
			getAllUniformLocations();
		}

		~StaticShader() {
			cleanUp();
		}

		StaticShader() : ShaderProgram(VERTEXPATH, FRAGMENTPATH) {
			linkProgram();
			getAllUniformLocations();
		}

		void loadTransformationMatrix(const glm::mat4& matrix) const
		{
			loadMatrix(location_transformationMatrix, matrix);
		}

		void loadProjectionMatrix(glm::mat4 projection) {
			loadMatrix(location_projectionMatrix, projection);
		}

		//void loadViewMatrix(const Window* parent) const {
		//	loadViewMatrix(location_viewMatrix, Maths::createViewMatrix(parent->getCamera()));
		//}

		void loadViewMatrix(const Camera& camera) const 
		{
			loadMatrix(location_viewMatrix, Maths::createViewMatrix(camera));
		}

		void loadLight(const Light& light) const
		{
			load3Vector(location_lightPosition, light.getPosition());
			load3Vector(location_lightColor, light.getColor());
		}

		void loadSurfaceVariables(const Surface& surface) const
		{
			loadFloat(location_reflectivity, surface.getReflectivity());
			loadFloat(location_shineDamper, surface.getShineDamper());
			loadFloat(location_textureFraction, surface.getTextureFraction());
		}

		void loadSurfaceColor(const Color& color) const
		{
			load4Vector(location_color, glm::vec4(color.getRgb(), color.getAlpha()));
		}

	protected:
		void bindAttributes() {
			ShaderProgram::bindAttribute(0, std::string("position"));
			ShaderProgram::bindAttribute(1, std::string("textureCoordinates"));
			ShaderProgram::bindAttribute(2, std::string("normal"));
		}
		
		void getAllUniformLocations() {
			location_transformationMatrix = getUniformLocation("transformationMatrix");
			location_projectionMatrix = getUniformLocation("projectionMatrix");
			location_viewMatrix = getUniformLocation("viewMatrix");
			location_lightColor = getUniformLocation("lightColor");
			location_lightPosition = getUniformLocation("lightPosition");
			location_reflectivity = getUniformLocation("reflectivity");
			location_shineDamper = getUniformLocation("shineDamper");
			location_color = getUniformLocation("color");
			location_textureFraction = getUniformLocation("textureFraction");


		}

	private:
		static const std::string VERTEXPATH;
		static const std::string FRAGMENTPATH;

		void linkProgram() {
			int success;
			char infoLog[512];
			std::string errMsg = "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";

			bindAttributes();// TODO: This function is not implemented!
			glLinkProgram(ID);
			//glValidateProgram(ID);
			/* CHECK FOR LINKING ERRORS: */
			glGetProgramiv(ID, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(ID, 512, NULL, infoLog);
				errMsg += std::string(infoLog);
				std::cout << "In makeProgram:\n";
				throw (std::exception(errMsg.c_str()));
			}
			glDeleteShader(vShaderID);
			glDeleteShader(fShaderID);
		}

		unsigned int location_transformationMatrix;
		unsigned int location_projectionMatrix;
		unsigned int location_viewMatrix;
		unsigned int location_lightPosition;
		unsigned int location_lightColor;
		unsigned int location_shineDamper;
		unsigned int location_reflectivity;
		unsigned int location_color;
		unsigned int location_textureFraction;



	};

}



#endif // STATICSHADER