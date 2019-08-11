#pragma once

/*

OGL shader class

*/

class Shader
{
public:
	Shader(std::string name, std::string vertexPath, std::string fragPath);
	~Shader();

private:
	std::string m_name;

	std::vector<GLuint> m_shaders; //shaders
	GLuint m_shaderProgram; //the actual program which renders
	std::vector<GLuint> m_shaderVar; //the variables used in the shaders

	//setup methods
	void loadShader(GLenum type, const std::string& filePath);
	void createProgram();

public:
	GLuint addShaderVariable(const std::string &varName);
	void useShader(); //uses the shader program

	GLuint getShaderProgram();
	std::string& getNameRef();
	GLuint getShaderVar(int index);
};

/*

end Shader

*/