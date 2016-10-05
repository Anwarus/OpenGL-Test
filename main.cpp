//Simple I/O
#include <stdio.h>
// GLEW
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
//SOIL
#include <SOIL.h>
//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

//Window dimenstions
const GLuint WIDTH = 800, HEIGHT = 600;
//Frammate
const GLuint FPS = 30;

//Declarations///////////////////////////////////////
GLFWwindow* window;

GLchar* vertexShaderSource;
GLchar* fragmentShaderSource;

Shader* shader;
GLuint vao, vbo;
GLuint texture, texture2;

float offset = 0;

bool init();
void loop(GLfloat elapsedTime, GLfloat lastTime);

void input();
void update();
void draw();

void setupBuffers();
/////////////////////////////////////////////////////

//Stuff to draw//////////////////////////////////////
GLfloat vertices[] =
{
	//Position         //TextCoords
	-0.5f, 0.5f, 1.f,    0.f, 1.f,
	0.5f, 0.5f, 1.f,     1.f, 1.f,
	0.5f, -0.5f, 1.f,    1.f, 0.f,
	-0.5f, -0.5f, 1.f,   0.f, 0.f
};
/////////////////////////////////////////////////////

int main()
{
	if (!init())
		return -1;

	GLfloat elapsed_time = 0, lastTime = glfwGetTime();
	loop(elapsed_time, lastTime);

	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);

	glfwTerminate();

	delete shader;

	return 0;
}

bool init()
{
	if (!glfwInit())
	{
		printf("Failed to initialize GLFW!\n");
		return false;
	}

	window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL", nullptr, nullptr);
	if (!window)
	{
		printf("Failed to create windod!\n");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();

	setupBuffers();

	shader = new Shader("vertexShader.shader", "fragmentShader.shader");

	glClearColor(0.f, 0.4f, 0.4f, 1.f);

	//Load texture
	int width, height;
	//Tex1
	unsigned char* image = SOIL_load_image("container.jpg", &width, &height, 0, SOIL_LOAD_RGB);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	//Tex2
	image = SOIL_load_image("awesomeface.png", &width, &height, 0, SOIL_LOAD_RGB);
	
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
	// Set our texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
}

void loop(GLfloat elapsedTime, GLfloat lastTime)
{
	while (!glfwWindowShouldClose(window))
	{
		elapsedTime = glfwGetTime() - lastTime;

		if (elapsedTime >= 1.0 / FPS)
		{
			input();
			update();
			draw();

			lastTime = glfwGetTime();
		}
	}
}

void input()
{
	glfwPollEvents();
}

void update()
{
	offset += 0.05f;

	if (offset >= 6.278f)
		offset = 0;
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(shader->Program, "ourTexture"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(shader->Program, "ourTexture2"), 1);

	shader->Use();

	// Create transformations
	glm::mat4 transform;
	transform = glm::translate(transform, glm::vec3(0.5f, -0.5f, 0.f));
	transform = glm::rotate(transform, (GLfloat)(0.5f * 2.f * 3.14), glm::vec3(0.f, 0.f, 1.f));

	// Get matrix's uniform location and set matrix
	GLint transformLoc = glGetUniformLocation(shader->Program, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	glBindVertexArray(vao);
	glDrawArrays(GL_QUADS, 0, 4);
	glBindVertexArray(0);

	glfwSwapBuffers(window);
}

void setupBuffers()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
}