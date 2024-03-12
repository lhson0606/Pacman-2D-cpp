#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <exception>
#include <fstream>
#include <sstream>
#include "STB/stb_image.h"

std::string static readShader(const std::string& path)
{
	std::string shaderCode;
	std::ifstream shaderFile;

	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		shaderFile.open(path);
		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		throw std::exception(("failed to read shader file: " + path).c_str());
	}

	return shaderCode;
}

inline unsigned int static loadTex2D(std::string path)
{
	unsigned int texId = 0;
	int w, h, channels;
	unsigned char* data;
	
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(path.c_str(), &w, &h, &channels, 0);

	if (!data)
	{
		std::string errMsg = "failed to load texture " + path;
		throw std::exception(errMsg.c_str());
	}

	glGenTextures(1, &texId);

	glBindTexture(GL_TEXTURE_2D, texId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	if (path.ends_with(".png") )
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else if(path.ends_with(".jpg"))
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		throw std::exception("unsupported image format");
	}
	
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);

	return texId;
}

inline unsigned int static loadPixelStyleTexture(std::string path)
{
	unsigned int texId = 0;
	int w, h, channels;
	unsigned char* data;

	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(path.c_str(), &w, &h, &channels, 0);

	if (!data)
	{
		std::string errMsg = "failed to load texture " + path;
		throw std::exception(errMsg.c_str());
	}

	glGenTextures(1, &texId);

	glBindTexture(GL_TEXTURE_2D, texId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	if (path.ends_with(".png"))
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else if (path.ends_with(".jpg"))
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		throw std::exception("unsupported image format");
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	stbi_image_free(data);

	return texId;
}

inline static unsigned int genVAO()
{
	unsigned int vao;
	glGenVertexArrays(1, &vao);
	return vao;
}

inline static unsigned int genVBO()
{
	unsigned int vbo;
	glGenBuffers(1, &vbo);
	return vbo;
}
