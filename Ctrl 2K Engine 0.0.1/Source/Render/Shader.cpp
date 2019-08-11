#include <Ctrl 2kPCH.h>

/*

OGL shader class

*/

Shader::Shader(std::string name, std::string vertexPath, std::string fragPath)
{
	this->m_name = name; //sets shader name

	//loads the vertex and fragment shaders
	this->loadShader(GL_VERTEX_SHADER, vertexPath);
	this->loadShader(GL_FRAGMENT_SHADER, fragPath);

	//creates the program
	this->createProgram();
	assert(this->m_shaderProgram, "The " + this->m_name + " Shader Was Not Compiled Properly..."); //for debugging only
}

void Shader::loadShader(GLenum type, const std::string &filePath)
{
	std::ifstream ifs; //input stream
	ifs.open(filePath);

	//if not opened properly
	if (!ifs)
	{
		std::cerr << "Cannot Open Shader File: \"" << filePath << "\"" << std::endl;
		this->m_shaders.push_back(0);
		return;
	}

	//copies shaderfile to source
	std::string source(std::istreambuf_iterator<char>(ifs), (std::istreambuf_iterator<char>()));

	ifs.close(); //file closed herre

	GLuint shader = glCreateShader(type); //shader object

	//loads shader source for each shader object and compiles
	const GLchar* sources[] = { source.c_str() };
	glShaderSource(shader, 1, sources, NULL);
	glCompileShader(shader); //compiles here

	//checks for compiler errors
	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus != GL_TRUE) //outputs log if windows, std::err if else
	{
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* infoLog = new GLchar[logLength];
		glGetShaderInfoLog(shader, logLength, NULL, infoLog);

#ifdef _WIN32
		OutputDebugString(infoLog);
#else
		std::cerr << infoLog << std::endl;
#endif
		delete infoLog;
		this->m_shaders.push_back(0);
	}
	this->m_shaders.push_back(shader);
}

void Shader::createProgram()
{
	//creates the program
	this->m_shaderProgram = glCreateProgram();

	//Attaches all the appropriate shaders
	for (GLuint shader : this->m_shaders)
	{
		glAttachShader(this->m_shaderProgram, shader);
	}

	glLinkProgram(this->m_shaderProgram); //links program

	//Check Link Status, uses std::err if os is not window or else outputs the error log
	GLint linkStatus;
	glGetProgramiv(this->m_shaderProgram, GL_LINK_STATUS, &linkStatus);
	if (linkStatus != GL_TRUE)
	{
		GLint logLength;
		glGetProgramiv(this->m_shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
		GLchar* infoLog = new GLchar[logLength];

		glGetProgramInfoLog(this->m_shaderProgram, logLength, NULL, infoLog);

#ifdef _WIN32
		OutputDebugString(infoLog);
#else
		std::cerr << infoLog << std::endl;
#endif
		delete infoLog;
		this->m_shaderProgram = 0;
	}
}

GLuint Shader::addShaderVariable(const std::string &varName) //basically adds a variable location to the list
{
	GLuint pos = glGetUniformLocation(this->m_shaderProgram, varName.c_str());
	this->m_shaderVar.push_back(pos);

	return pos;
}

void Shader::useShader()
{
	glUseProgram(this->m_shaderProgram);
}

//accessors
GLuint Shader::getShaderProgram()
{
	return this->m_shaderProgram;
}

std::string& Shader::getNameRef()
{
	return this->m_name;
}

GLuint Shader::getShaderVar(int index)
{
	return this->m_shaderVar[index];
}

Shader::~Shader()
{

}

/*

end Shader

*/