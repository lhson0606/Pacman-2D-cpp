#pragma once

#include "dy/dyutils.h"
#include <iostream>
#include <map>
#include <glm/mat4x4.hpp>

class Shader
{
public:
	static const int SHADER_ERROR_LOG_SIZE = 512;
	Shader() = default;
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	/*
	* initialize shader program
	* Note: this function must be called after gl context is created
	*/
	void Init();
	void Use();
	void Stop();
	void SetBool(const std::string& name, bool value);
	void SetInt(const std::string& name, int value);
	void SetFloat(const std::string& name, float value);
	void SetMat4(const std::string& name, const glm::mat4& value);
	void SetVec3(const std::string& name, const glm::vec3& value);
	void SetVec3(const std::string& name, float x, float y, float z);
	void ResetCache();
	int GetUniLocation(const std::string& name);

	uint getId() const { return id; }
private:
	uint id = 0;
	std::string vertCode;
	std::string fragPath;
	std::map<std::string, int> uniformLocationCache;
};