#pragma  once
#include "glew.h"
#include "freeglut.h"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>

static char *textFileRead(const char *fn) {


	FILE *fp;
	char *content = NULL;

	int count=0;

	if (fn != NULL) {
		fp = fopen(fn,"rt");

		if (fp != NULL) {

			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);

			if (count > 0) {
				content = (char *)calloc( (count+1), sizeof(char));
				for (int i = 0 ; i < count +1; i++)
					content[i] = NULL;
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

using namespace std;

class FXZProgram;

class FXZShader
{
	friend class FXZProgram;
private:
	GLuint shader;
	bool alive;
public:
	explicit FXZShader( GLenum shaderType)
	{
		if (shaderType != GL_VERTEX_SHADER && shaderType != GL_FRAGMENT_SHADER)
		{
			cout<<"ERROR: invalid shader type! Cannot create shader";
			exit(-1);
		}
		this->shader = glCreateShader(shaderType);
		alive = true;
		cout << "Created shader "<<shader<<endl;
	}
	void setSource(const char * fileName)
	{
		char * shaderSource;
		long l,m;
		ifstream file (fileName, ios::in|ios::binary);
		l = file.tellg();
		file.seekg (0, ios::end);
		m = file.tellg();
		file.close(); 
		l = m - l;

		shaderSource = textFileRead(fileName);
		if (alive)
			glShaderSource(this->shader, 1, (const GLchar **) &shaderSource, NULL);
		delete[] shaderSource;

	}
	void compileShader()
	{
		if (alive)
			glCompileShader(this->shader);
	}
	void deleteShader()
	{
		if (alive)
			glDeleteShader(this->shader);
		alive = false;
	}

	void infoLog()
	{
		int compileStatus = 0;
		glGetShaderiv(this->shader, GL_COMPILE_STATUS, &compileStatus);
		if (compileStatus)
		{
			cout << "Shader " << shader<<" compiled successfully ..";
			int infologLength = 0;
			int charsWritten  = 0;
			char *infoLog;

			glGetShaderiv(shader, GL_INFO_LOG_LENGTH,&infologLength);

			if (infologLength > 0)
			{
				infoLog = new char[infologLength]; //(char *)malloc(infologLength);
				glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
				cout<<infoLog << endl; //printf("%s\n",infoLog);
				delete[] infoLog;
			}
		}
		else
		{
			cout << "Error compiling "<< shader <<". Fatal Error. Exiting..";
			cin.get();
			exit(-1);
		}
	}

	~FXZShader()
	{
		this->deleteShader();
	}
};


enum UNIFORMTYPE
{
	UNIFORM1 = 0,
	UNIFORM2,
	UNIFORM3,
	UNIFORM4
};

struct uniformVar 
{
	const char * name;
	UNIFORMTYPE utype;
	GLint uniform;
	void setUniform(GLfloat v0, GLfloat v1 = 0, GLfloat v2 = 0, GLfloat v3 = 0)
	{
		if (utype == UNIFORM1)
			glUniform1f(this->uniform, v0);
		if (utype == UNIFORM2)
			glUniform2f(this->uniform, v0, v1);
		if (utype == UNIFORM3)
			glUniform3f(this->uniform, v0, v1, v2);
		if (utype == UNIFORM4)
			glUniform4f(this->uniform, v0, v1, v2, v3);
	}
};
class FXZProgram
{
private:
	GLuint program;
	std::vector <int> shaders;
	vector <uniformVar> uniforms;
public:
	explicit FXZProgram()
	{
		this->program = glCreateProgram();
	}
	void attachShader(FXZShader * shaderInstance)
	{
		glAttachShader(this->program, shaderInstance->shader);
		shaders.push_back(shaderInstance->shader);
	}
	void linkProgram()
	{
		glLinkProgram(this->program);
	}
	void useProgram()
	{
		glUseProgram(this->program);
	}

	void detachShader(FXZShader * shaderInstance)
	{
		glDetachShader(this->program, shaderInstance->shader);
		for (size_t i = 0; i < shaders.size(); i++)
		{
			if (shaders[i] == shaderInstance->shader)
			{
				shaders.erase(shaders.begin() + i);
				break;
			}
		}
	}

	void infoLog()
	{
		int isLinked = 0;
		glGetProgramiv(this->program, GL_LINK_STATUS, &isLinked);
		if (isLinked)
		{
			cout<<"Program "<<program<<" successfully linked.."<<endl;
		}
		else
		{
			cout<<"Fatal error, couldn't link program "<<program<<endl;
			cin.get();
			exit(-2);
		}

		int infologLength = 0;
		int charsWritten  = 0;
		char *infoLog;

		glGetProgramiv(this->program, GL_INFO_LOG_LENGTH,&infologLength);

		if (infologLength > 0)
		{
			infoLog = new char[infologLength]; 
			glGetProgramInfoLog(this->program, infologLength, &charsWritten, infoLog);
			cout<< infoLog <<endl;
			delete[] infoLog;
		}
	}
	void addUniform(const char * uname, UNIFORMTYPE utype)
	{
		uniformVar uvar;
		uvar.name = uname;
		uvar.utype = utype;
		uvar.uniform = glGetUniformLocation(this->program, uname);
		this->uniforms.push_back(uvar);
	}

	void setUniform(const char * uname, GLfloat v0, GLfloat v1 =0, GLfloat v2 = 0, GLfloat v3 =0)
	{
		for (int i = 0; i < uniforms.size(); i++)
		{
			if (!strcmp(uniforms[i].name, uname))
			{
				switch (uniforms[i].utype)
				{
				case UNIFORM1:
					glUniform1f(uniforms[i].uniform, v0);
					break;
				case UNIFORM2:
					glUniform2f(uniforms[i].uniform, v0, v1);
					break;
				case UNIFORM3:
					glUniform3f(uniforms[i].uniform, v0 , v1, v2);
					break;
				case UNIFORM4:
					glUniform4f(uniforms[i].uniform, v0, v1, v2, v3);
					break;
				}
			}
		}
	}

	~FXZProgram()
	{
		for (int i = 0; i < shaders.size(); i++)
			glDetachShader(this->program, shaders[i]);
		glDeleteProgram(this->program);
	}
};