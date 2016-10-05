#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <GL/glew.h>; // Include glew to get all the required OpenGL headers

class Shader
{
public:
	// The program ID
	GLuint Program;
	// Constructor reads and builds the shader
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	// Use the program
	void Use();
};

