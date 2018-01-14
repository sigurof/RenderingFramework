#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H
#pragma once
#include <glad\glad.h>
#include <GLFW\glfw3.h>
//#include <glm-0.9.8.5\glm\glm.hpp>
//#include <string>
//#include <glm-0.9.8.5\glm/glm.hpp>
//#include <glm-0.9.8.5\glm/gtc/matrix_transform.hpp>
#include <glm-0.9.8.5\glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

namespace ML {
	//enum SHADER_TYPE { VERTEXSHADER = GL_VERTEX_SHADER, FRAGMENTSHADER = GL_FRAGMENT_SHADER };


	class ShaderProgram
	{
	public:
		ShaderProgram(){}

		ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath)//, std::function<void(void)> bindAttributes)
		{
			std::string vertexCode;
			std::string fragmentCode;
			try
			{
				vShaderID = glCreateShader(GL_VERTEX_SHADER);
				fShaderID = glCreateShader(GL_FRAGMENT_SHADER);

				vertexCode = readShaderSource(vertexPath);
				fragmentCode = readShaderSource(fragmentPath);

				compileShader(vShaderID, vertexCode);
				compileShader(fShaderID, fragmentCode);

				/* CREATE AND LINK PROGRAM */
				ID = glCreateProgram();
				glAttachShader(ID, vShaderID);
				glAttachShader(ID, fShaderID);

				//makeProgram(vShaderID, fShaderID);//, bindAttributes);

				//glDeleteShader(vShaderID);
				//glDeleteShader(fShaderID);
			}
			catch (const std::exception& err)
			{
				std::cout << "in ShaderProgram::Constructor:\n";
				throw err;
			}
		}

		void start() const 
		{
			glUseProgram(ID);
		}

		void stop() const 
		{
			/* TODO: Check if is currently active shader?*/
			glUseProgram(0);
		}

		void cleanUp() const
		{
			stop();
			glDetachShader(ID, vShaderID);
			glDetachShader(ID, fShaderID);
			glDeleteShader(vShaderID);
			glDeleteShader(fShaderID);
			glDeleteProgram(ID);
		}

		unsigned int getID() { return ID; }

		unsigned int getvShaderID() { return vShaderID; }

		unsigned int getfShaderID() { return fShaderID; }
		
	protected:

		unsigned int getUniformLocation(std::string uniformName) {
			return glGetUniformLocation(ID, uniformName.c_str());
		}

		virtual void getAllUniformLocations() = 0;

		virtual void bindAttributes() = 0;

		void loadFloat(int location, float value) const 
		{
			glUniform1f(location, value);
		}
		
		void load4Vector(int location, glm::vec4 vec) const 
		{
			glUniform4f(location, vec.x, vec.y, vec.z, vec.a);
			//TODO: check that vec.a == vec[3]
		}

		void load3Vector(int location, const glm::vec3& vec) const
		{
			glUniform3f(location, vec.x, vec.y, vec.z);
		}

		void loadBool(int location, bool val) {
			glUniform1i(location, (int)val);
		}

		void loadMatrix(int location, const glm::mat4& trans) const 
		{
			glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(trans));
		}

		void bindAttribute(int attribute, std::string& variableName) {
			glBindAttribLocation(ID, attribute, variableName.c_str());
		}

		unsigned int ID;
		unsigned int vShaderID;
		unsigned int fShaderID;

	private:

		static std::string readShaderSource(const std::string& path)
		{
			std::ifstream shaderFile;
			/* Ensure ifstream objects can throw exceptions:*/
			shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
			try
			{
				/* Open files */
				shaderFile.open(path);
				if (!shaderFile)
				{
					throw (std::exception("ERROR:COULD NOT OPEN FILE"));
				}
				std::stringstream shaderStream;
				/* Read file's buffer contents into streams */
				shaderStream << shaderFile.rdbuf();
				/* Convert stream into string */
				return shaderStream.str();
			}

			catch (std::ifstream::failure a)
			{
				std::cout << "In readShader (" << path << "):\n";
				throw std::exception("ERROR::SHADER::FILLE_NOT_SUCCESFULLY_READ");
			}
			catch (std::exception& err)
			{
				std::cout << "In readShader (" << path << "):\n";
				throw err;
			}
		}

		static void compileShader(unsigned int& shaderHandle, std::string& shaderCode)
		{
			const char* code = shaderCode.c_str();

			/* COMPILE SHADER: */
			glShaderSource(shaderHandle, 1, &code, NULL);
			glCompileShader(shaderHandle);

			/* CHECK FOR COMPILE ERRORS: */
			int success;
			char infoLog[512];
			glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shaderHandle, 512, NULL, infoLog);
				std::cout << "In compileShader:\n";
				throw (std::exception(infoLog));
			}
		}

		void makeProgram(unsigned int& shaderHandle1, unsigned int& shaderHandle2){//, std::function<void(void)> bindAttributes) {
			int success;
			char infoLog[512];
			std::string errMsg = "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";
			/* CREATE AND LINK PROGRAM */
			ID = glCreateProgram();
			glAttachShader(ID, shaderHandle1);
			glAttachShader(ID, shaderHandle2);
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
				throw (errMsg.c_str());
			}
		}

	};
}

#endif // SHADERPROGRAM_H
















//static std::string readShaderSource(std::string& path)
//{
//	std::ifstream shaderFile;
//	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
//	try
//	{
//		shaderFile.open(path);
//		if (!shaderFile)
//		{
//			throw (std::exception("ERROR:COULD NOT OPEN FILE"));
//		}
//		std::stringstream shaderStream;
//		shaderStream << shaderFile.rdbuf();
//		return shaderStream.str();
//	}
//	catch (std::ifstream::failure a)
//	{
//		std::cout << "In readShader (" << path << "):\n";
//		throw std::exception("ERROR::SHADER::FILLE_NOT_SUCCESFULLY_READ");
//	}
//	catch (std::exception& err)
//	{
//		std::cout << "In readShader (" << path << "):\n";
//		throw err;
//	}
//}
//static void compileShader(unsigned int& shaderHandle, std::string& shaderCode)
//{
//	const char* code = shaderCode.c_str();
//	/* COMPILE SHADER: */
//	glShaderSource(shaderHandle, 1, &code, NULL);
//	glCompileShader(shaderHandle);
//	/* CHECK FOR COMPILE ERRORS: */
//	int success;
//	char infoLog[512];
//	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
//	if (!success)
//	{
//		glGetShaderInfoLog(shaderHandle, 512, NULL, infoLog);
//		std::cout << "In compileShader:\n";
//		throw (std::exception(infoLog));
//	}
//}
//static void makeProgram(unsigned int& shaderHandle1, unsigned int& shaderHandle2) {
//	unsigned int programID;
//	int success;
//	char infoLog[512];
//	std::string errMsg = "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";
//	/* CREATE AND LINK PROGRAM */
//	ID = glCreateProgram();
//	glAttachShader(ID, shaderHandle1);
//	glAttachShader(ID, shaderHandle2);
//	glLinkProgram(ID);
//	/* CHECK FOR LINKING ERRORS: */
//	glGetProgramiv(ID, GL_LINK_STATUS, &success);
//	if (!success)
//	{
//		glGetProgramInfoLog(ID, 512, NULL, infoLog);
//		errMsg += std::string(infoLog);
//		std::cout << "In makeProgram:\n";
//		throw (errMsg.c_str());
//	}
//}
//static unsigned int loadShader(std::string& path, GLenum type)
//{
//	std::string code;
//	try
//	{
//		unsigned int shaderID = glCreateShader(type);
//		code = readShaderSource(path);
//		compileShader(shaderID, code);
//	}
//	catch (const std::exception& err)
//	{
//		std::cout << "in loadShader constructor:\n";
//		throw err;
//	}
//}
