#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include<gtc/type_ptr.hpp>
#include<ext/matrix_projection.hpp>





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


class Circle  {

public :
	int id = -1;
	glm::vec2 center;
	float range=0.10f;
	bool mouseOver = false;

};

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


Circle c[4];

glm::vec2 currentMousePosition;

int screenWidth = 800;
int screenHeight = 800;
glm::vec3 realpos;

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	

	glm::vec3 win(xpos, ypos, 0);

	glm::vec4 viewport(0, 0, screenWidth, screenHeight);


	realpos = glm::unProject(win, glm::mat4(1.0f), glm::mat4(1.0f), viewport);

	realpos.y = -1*realpos.y;
	std::cout <<"real point x: " <<realpos.x<< " realpoint y: "<< realpos.y << std::endl;
}

int main() {


	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Deneme 1 2 1 2", NULL, NULL);
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


	glfwSetCursorPosCallback(window, cursor_position_callback);

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


	GLuint vertexShaderCircle = loadShader(GL_VERTEX_SHADER, "cVertex.glsl");

	GLuint fragmentShaderCircle = loadShader(GL_FRAGMENT_SHADER, "cFragment.glsl");

	GLuint circlePrograms = glCreateProgram();
	glAttachShader(circlePrograms, vertexShaderCircle);
	glAttachShader(circlePrograms, fragmentShaderCircle);
	glLinkProgram(circlePrograms);



	float controlPoints[] = {
		-1.0f, -1.0f,
		-1.0f, 1.0f,
		1.0f, -1.0f,
		1.0f, 1.0f
		

	};

	int index = 0;
	for (size_t i = 0; i < 8; i=i+2)
	{
		c[index].center = glm::vec2(controlPoints[i],controlPoints[i+1]);
		c[index].range = 0.10f;
		index++;
	}




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






	glLineWidth(1.0f);



	//glViewport(0, 0, 800, 800);
	glm::mat4 ortho = glm::ortho(-2.0, 2.0, -2.0, 2.0, -1.0, 1.0);
	glm::mat4 curveCenter(1);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));


	glClearColor(0, 0, 0, 1);
	while (!glfwWindowShouldClose(window)) {
		// Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// draw Circles 




		// Use shader program
		glUseProgram(shaderProgram);
		GLuint uniformLocation = glGetUniformLocation(shaderProgram, "gWVP");//
		glm::mat4 gWVP = ortho * view;
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(gWVP));

		// Draw Bezier curve
		glBindVertexArray(VAO);
		glDrawArrays(GL_PATCHES, 0, 4);
		glBindVertexArray(0);
		glUseProgram(0);


		glUseProgram(circlePrograms);

		GLuint pointGWVP = glGetUniformLocation(circlePrograms, "gWVP");//
		glUniformMatrix4fv(pointGWVP, 1, GL_FALSE, glm::value_ptr(gWVP));


		GLuint mausePosition = glGetUniformLocation(circlePrograms, "mouseP");//

		glUniform3fv(mausePosition, 1, glm::value_ptr(realpos));

		glBindVertexArray(VAO);
		glPointSize(10.f);
		glDrawArrays(GL_POINTS, 0, 4);
		glBindVertexArray(0);
		glUseProgram(0);

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





