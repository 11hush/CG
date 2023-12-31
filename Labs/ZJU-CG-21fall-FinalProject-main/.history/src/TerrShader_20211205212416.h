#pragma once
#include "Model.h"
#include "Camera.h"
#include "Matrix.h"
#include "Light.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace glm;

class TerrainShader {
private:
	int programID;
	int vertexShaderID;
	int fragmentShaderID;
	unsigned int transMatrixLocation;
	unsigned int projectMatrixLocation;
	unsigned int viewMatrixLocation;
	unsigned int lightPosLocation;
	unsigned int lightColorLocation;
	unsigned int shineDamperLocation;
	unsigned int reflectLocation;
	unsigned int skyColorLocation;
	unsigned int location_backgroundTexture;
	unsigned int location_rTexture;
	unsigned int location_gTexture;
	unsigned int location_bTexture;
	unsigned int location_blendMap;

	unsigned int loadShader(int type, const char* Path) {
		std::string Code;
		std::ifstream ShaderFile;
		try
		{
			ShaderFile.open(Path);
			std::stringstream ShaderStream;
			ShaderStream << ShaderFile.rdbuf();
			ShaderFile.close();
			Code = ShaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* ShaderCode = Code.c_str();

		unsigned int ShaderID;
		ShaderID = glCreateShader(type);
		glShaderSource(ShaderID, 1, &ShaderCode, NULL);
		glCompileShader(ShaderID);
		int  success;
		char infoLog[512];
		glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(ShaderID, 512, NULL, infoLog);
			if (type == GL_VERTEX_SHADER)
				std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			else
				std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		return ShaderID;
	}

public:
	TerrainShader() {

	}
	TerrainShader(const char* vertexPath, const char* fragmentPath) {
		vertexShaderID = loadShader(GL_VERTEX_SHADER, vertexPath);
		fragmentShaderID = loadShader(GL_FRAGMENT_SHADER, fragmentPath);
		programID = glCreateProgram();
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);
		BindAttrib();
		glLinkProgram(programID);

		int  success;
		char infoLog[512];
		glGetProgramiv(programID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(programID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		glValidateProgram(programID);
		getAllUniformLocations();
	}

	void Start() {
		glUseProgram(programID);
	}

	void Stop() {
		glUseProgram(0);
	}

	void Clear() {
		Stop();
		glDetachShader(programID, vertexShaderID);
		glDetachShader(programID, fragmentShaderID);
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		glDeleteProgram(programID);
	}

	void BindAttrib() {
		glBindAttribLocation(programID, 0, "position");
		glBindAttribLocation(programID, 1, "textureCoord");
		glBindAttribLocation(programID, 2, "normal");
	}

	void getAllUniformLocations() {
		transMatrixLocation = getUniformLocation("transMatrix");
		projectMatrixLocation = getUniformLocation("projectMatrix");
		viewMatrixLocation = getUniformLocation("viewMatrix");
		lightPosLocation = getUniformLocation("lightPos");
		lightColorLocation = getUniformLocation("lightColor");
		shineDamperLocation = getUniformLocation("shineDamper");
		reflectLocation = getUniformLocation("reflectivity");
		skyColorLocation = getUniformLocation("skyColor");
		//check写对没有
		location_backgroundTexture = getUniformLocation("backgroundTexture");
		location_rTexture = getUniformLocation("rTexture");
		location_gTexture = getUniformLocation("gTexture");
		location_bTexture = getUniformLocation("bTexture");
		location_blendMap = getUniformLocation("blendMap");
	}

	void connectTextureUnits() {
		glUniform1i(location_backgroundTexture, 0);
		glUniform1i(location_rTexture, 1);
		glUniform1i(location_gTexture, 2);
		glUniform1i(location_bTexture, 3);
		glUniform1i(location_blendMap, 4);
	}
	
	void loadSkyColor(float r, float g, float b){
		loadVector(skyColorLocation, vec3(r,g,b));
	}

	unsigned int getUniformLocation(std::string uniformName) {
		return glGetUniformLocation(programID, uniformName.c_str());
	}

	void loadShine(float damper, float reflectivity) {
		loadFloat(shineDamperLocation, damper);
		loadFloat(reflectLocation, reflectivity);
	}

	void loadLight(Light light) {
		loadVector(lightPosLocation, light.pos);
		loadVector(lightColorLocation, light.color);
	}

	void loadTransMatrix(mat4 matrix) {
		loadMatrix(transMatrixLocation, matrix);
	}

	void loadProjectMatrix(mat4 matrix) {
		loadMatrix(projectMatrixLocation, matrix);
	}

	void loadViewMatrix(Camera camera) {
		mat4 viewMatrix = createViewMatrix(camera);
		loadMatrix(viewMatrixLocation, viewMatrix);
	}

	void loadFloat(int location, float value) {
		glUniform1f(location, value);
	}

	void loadVector(int location, vec3 vector) {
		glUniform3f(location, vector.x, vector.y, vector.z);
	}

	void loadBoolean(int location, bool value) {
		float toLoad = 0;
		if (value) {
			toLoad = 1;
		}
		glUniform1f(location, toLoad);
	}

	void loadMatrix(int location, mat4 matrix) {
		glUniformMatrix4fv(location, 1, GL_FALSE, value_ptr(matrix));
	}

};