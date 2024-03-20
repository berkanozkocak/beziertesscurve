#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>


std::string readShaderSource(const char* filePath) {
	std::string content;
	std::ifstream fileStream(filePath, std::ios::in);

	if (!fileStream.is_open()) {
		std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
		return "";
	}

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

GLuint loadShader(GLenum shaderType, const char* filePath) {
	GLuint shader = glCreateShader(shaderType);
	std::string shaderSource = readShaderSource(filePath);
	const char* sourcePtr = shaderSource.c_str();
	glShaderSource(shader, 1, &sourcePtr, nullptr);
	glCompileShader(shader);

	// Check for compilation errors
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, infoLog);
		std::cerr << "Shader compilation error: " << infoLog << std::endl;
		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

int main() {


	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(800, 800, "Deneme 1 2 1 2", NULL, NULL);
	if (!window) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW: " << glewGetErrorString(err) << std::endl;
		return -1;
	}

	GLuint vertexShader = loadShader(GL_VERTEX_SHADER, "vertex.glsl");

	GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER, "fragment.glsl");

	GLuint beziertessShader = loadShader(GL_TESS_EVALUATION_SHADER, "beziertess.glsl");

	GLuint beziertesscShader = loadShader(GL_TESS_CONTROL_SHADER, "beziertessc.glsl");


	// Create shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, beziertesscShader);

	
	glAttachShader(shaderProgram, beziertessShader);
	glAttachShader(shaderProgram, fragmentShader);



	glLinkProgram(shaderProgram);


	// Check for linking errors
	GLint success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cerr << "Shader program linking error: " << infoLog << std::endl;
		return -1;
	}


	float controlPoints[] = {
		-1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, -1.0f,
		1.0f, 1.0f
		

	};




	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 8 * 4, &controlPoints[0], GL_STATIC_DRAW);


	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glPatchParameteri(GL_PATCH_VERTICES, 4);

	glPointSize(1.0f);
	glLineWidth(1.0f);



	//glViewport(0, 0, 800, 800);
	glm::mat4 ortho = glm::ortho(-2.0, 2.0, -2.0, 2.0, -1.0, 1.0);
	glm::mat4 curveCenter(1);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));


	glClearColor(0, 0, 0, 1);
	while (!glfwWindowShouldClose(window)) {
		// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Use shader program
		glUseProgram(shaderProgram);

		GLuint uniformLocation = glGetUniformLocation(shaderProgram, "gWVP");//
		

		glm::mat4 gWVP = ortho * view;

		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(gWVP));


		// Draw Bezier curve
		glBindVertexArray(VAO);
		glDrawArrays(GL_PATCHES, 0, 4);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Clean up resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shaderProgram);

	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}


