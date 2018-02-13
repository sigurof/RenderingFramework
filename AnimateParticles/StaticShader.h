#ifndef STATICSHADER_H
#define STATICSHADER_H
#pragma once





namespace ML {

	class StaticShader : public ShaderProgram
	{
	public:

		StaticShader(const std::string& vpath, const std::string& fpath) : ShaderProgram(vpath, fpath) {
			linkProgram();
			getAllUniformLocations();
		}

		~StaticShader() {
			cleanUp();
		}

		StaticShader(const StaticShader&) = delete;

		StaticShader& operator=(const StaticShader&) = delete;

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
			loadFloat(location_reflectivity, surface.getReflectivity());
			loadFloat(location_shineDamper, surface.getShineDamper());
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
			location_cameraPosition = getUniformLocation("cameraPosition");


		}


	private:

		void linkProgram() {
			int success;
			char infoLog[512];
			std::string errMsg = "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n";

			bindAttributes();
			glLinkProgram(ID);
			glValidateProgram(ID);
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
		unsigned int location_cameraPosition;

	};

}



#endif // STATICSHADER