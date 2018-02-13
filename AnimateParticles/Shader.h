#ifndef SHADER_H
#define SHADER_H
#pragma once

namespace ML {

	class Shader
	{
	public:

		Shader(const std::string& vpath, const std::string& fpath) 
		{
			try
			{
				std::string vertexCode;
				std::string fragmentCode;
				vShaderID = glCreateShader(GL_VERTEX_SHADER);
				fShaderID = glCreateShader(GL_FRAGMENT_SHADER);

				vertexCode = readShaderSource(vpath);
				fragmentCode = readShaderSource(fpath);
				try { compileShader(vShaderID, vertexCode); }
				catch (const std::exception& err)
				{
					std::cout << "While compiling vertex shader\n";
					throw err;
				}
				try { compileShader(fShaderID, fragmentCode); }
				catch (const std::exception& err)
				{
					std::cout << "While compiling fragment shader:\n";
					throw err;
				}
				/* CREATE AND LINK PROGRAM */
				ID = glCreateProgram();
				glAttachShader(ID, vShaderID);
				glAttachShader(ID, fShaderID);
				linkProgram();
				getAllUniformLocations();
			}
			catch (const std::exception& err)
			{
				std::cout << "in Shader::Constructor:\n";
				throw err;
			}
		}

		~Shader() {
			cleanUp();
		}

		Shader(const Shader&) = delete;

		Shader& operator=(const Shader&) = delete;

		void start()
		{
			currentlyActiveShader = this;
			glUseProgram(ID);
		}

		void stop() const
		{
			currentlyActiveShader = nullptr;
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

		void loadTransformationMatrix(const glm::mat4& matrix) const
		{
			loadMatrix(location_transformationMatrix, matrix);
		}

		void loadProjectionMatrix(glm::mat4 projection) {
			loadMatrix(location_projectionMatrix, projection);
		}

		void loadViewMatrix(const std::shared_ptr<Camera> camera) const
		{
			loadMatrix(location_viewMatrix, Maths::createViewMatrix(camera));
		}

		void loadLight(const std::vector<std::shared_ptr<Light>>& lights) const
		{
			for (unsigned int i = 0; i < lights.size(); i++)
			{
				load3Vector(location_lightPosition, lights[i]->getPosition());
				load3Vector(location_lightColor, lights[i]->getColor().getRgb());
			}
		}

		void loadSurfaceVariables(const Surface& surface) const
		{
			//loadFloat(location_reflectivity, surface.getReflectivity());
			//loadFloat(location_shineDamper, surface.getShineDamper());
			loadFloat(location_textureFraction, surface.getTextureFraction());
		}

		void loadSurfaceColor(const Color& color) const
		{
			load4Vector(location_color, glm::vec4(color.getRgb(), color.getAlpha()));
		}

		void loadCameraPosition(const std::shared_ptr<Camera> camera)
		{
			load3Vector(location_cameraPosition, camera->getPosition());
		}

		static Shader* currentlyActiveShader;


	protected:


		void bindAttributes() {
			bindAttribute(0, std::string("position"));
			bindAttribute(1, std::string("textureCoordinates"));
			bindAttribute(2, std::string("normal"));
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
			location_cameraPosition = getUniformLocation("cameraPosition");


		}



		unsigned int getUniformLocation(std::string uniformName) {
			return glGetUniformLocation(ID, uniformName.c_str());
		}

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


		void linkProgram() 
		{
			try
			{
				int success;
				char infoLog[512];
				bindAttributes();
				glLinkProgram(ID);
				glValidateProgram(ID);
				/* CHECK FOR LINKING ERRORS: */
				glGetProgramiv(ID, GL_LINK_STATUS, &success);
				if (!success)
				{
					glGetProgramInfoLog(ID, 512, NULL, infoLog);
					throw (std::exception(infoLog));
				}
				glDeleteShader(vShaderID);
				glDeleteShader(fShaderID);
			}
			catch (const std::exception& err)
			{
				std::cout << "In Shader::linkProgram\n";
				throw err;
			}
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
		unsigned int location_cameraPosition;


		static std::string readShaderSource(const std::string& path)
		{
			try
			{
				std::ifstream shaderFile;
				shaderFile.open(path);
				if (!shaderFile) { throw (std::exception(("ERROR:COULD NOT OPEN " + path).c_str())); }
				std::stringstream shaderStream;
				shaderStream << shaderFile.rdbuf();
				return shaderStream.str();
			}
			catch (const std::exception& err)
			{
				std::cout << "In Shader::readShaderSource\n";
				throw err;
			}
		}

		static void compileShader(unsigned int& shaderHandle, std::string& shaderCode)
		{
			try
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
					throw (std::exception(infoLog));
				}
			}
			catch (const std::exception& err)
			{
				std::cout << "In Shader::compileShader()\n";
				throw err;
			}
		}

		void makeProgram(unsigned int& shaderHandle1, unsigned int& shaderHandle2) {//, std::function<void(void)> bindAttributes) {
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

#endif // !SHADER_H
