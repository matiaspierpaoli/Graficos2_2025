#pragma once
#include <string>
#include <fstream>
#include <glm/glm.hpp>

class Program
{
private:
	unsigned int id;
	int GetUniformLocation(const std::string& varName);

public:
	Program();
	~Program();

	std::string ReadFile(std::string filePath);
	unsigned int CreateShader(const std::string vertexShader, const std::string fragmentShader);
	unsigned int CompileShader(const std::string& source, unsigned int type);
	void SetUniformMat4F(const std::string& varName, const glm::mat4& matrix);
	void SetUniform4f(const std::string& varName, float v1, float v2, float v3, float v4);
	void SetUniform1i(const std::string& varName, int value);
};