#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#define numVAOs 1

using namespace std;
using namespace glm;

GLuint renderingProgram;
GLuint vao[numVAOs];

void printShaderLog(GLuint shader) {
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, &chWrittn, log);
		cout << "Shader Info Log: " << log << endl;
		free(log);
	}
};

void printProgramLog(int prog) {
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
	if (len > 0) {
		log = (char*)malloc(len);
		glGetProgramInfoLog(prog, len, &chWrittn, log);
		cout << "Program Info Log: " << log << endl;
		free(log);
	}
};

bool checkOpenGlError() {
	bool foundError = false;
	int glErr = glGetError();
	while (glErr != GL_NO_ERROR) {
		cout << "glError: " << glErr << endl;
		foundError = true;
		glErr = glGetError();
	}
	return foundError;
};

string readShaderSource(const char* filePath) {
	string content;
	ifstream fileStream(filePath, ios::in);
	string line = "";
	while (!fileStream.eof()) {
		getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
};

GLuint createShaderProgram() {
	GLint vertCompiled;
	GLint fragCompiled;
	GLint linked;

	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

	string vertShaderStr = readShaderSource("shaders/vertShaderEx3_3.glsl");
	string fragShaderStr = readShaderSource("shaders/fragShaderEx2_2.glsl");

	const char* vertShaderSrc = vertShaderStr.c_str();
	const char* fragShaderSrc = fragShaderStr.c_str();

	glShaderSource(vShader, 1, &vertShaderSrc, NULL);
	glShaderSource(fShader, 1, &fragShaderSrc, NULL);

	// catch errors while compiling shaders

	glCompileShader(vShader);
	checkOpenGlError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertCompiled);
	if (vertCompiled != 1) {
		cout << "vertex compilation failed" << endl;
		printShaderLog(vShader);
	}

	glCompileShader(fShader);
	checkOpenGlError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragCompiled);
	if (fragCompiled != 1) {
		cout << "fragment compilation failed" << endl;
		printShaderLog(fShader);
	}

	GLuint vfProgram = glCreateProgram();

	// catch errors while linking shaders
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);

	glLinkProgram(vfProgram);
	checkOpenGlError();
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1) {
		cout << "linking failed" << endl;
		printProgramLog(vfProgram);
	}

	// Validate the program
	glValidateProgram(vfProgram);
	GLint validated;
	glGetProgramiv(vfProgram, GL_VALIDATE_STATUS, &validated);
	if (validated != GL_TRUE) {
		cout << "Program validation failed" << endl;
		printProgramLog(vfProgram);
	}

	return vfProgram;
}

void init(GLFWwindow* window) {
	renderingProgram = createShaderProgram();
	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);
}

void display(GLFWwindow* window, double currentTime) {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glUseProgram(renderingProgram);

	float angleX = radians(45.0f);
	float angleY = radians(45.0f);
	float angleZ = radians(45.0f);

	// Rotationsmatrix mit Euler-Winkeln
	mat4 rotationMatrix = rotate(mat4(1.0f), angleX, vec3(1.0f, 0.0f, 0.0f)) *
		rotate(mat4(1.0f), angleY, vec3(0.0f, 1.0f, 0.0f)) *
		rotate(mat4(1.0f), angleZ, vec3(0.0f, 0.0f, 1.0f));

	// Rotationsmatrix mit Quaternions
	quat qX = angleAxis(angleX, vec3(1.0f, 0.0f, 0.0f));
	quat qY = angleAxis(angleY, vec3(0.0f, 1.0f, 0.0f));
	quat qZ = angleAxis(angleZ, vec3(0.0f, 0.0f, 1.0f));

	// Reihenfolge beachten
	quat qRotation = qZ * qY * qX;

	// Konvertieren das Quaternion in eine 4x4-Matrix für OpenGL
	mat4 quatRotationMatrix = mat4_cast(qRotation);

	// Hole die Speicheradresse der uniform-Variable im Shader
	GLuint rotationLoc = glGetUniformLocation(renderingProgram, "rotationMatrix");

	GLuint quatRotationLoc = glGetUniformLocation(renderingProgram, "quatRotationMatrix");

	// Schicke die Matrix an den Shader
	glUniformMatrix4fv(rotationLoc, 1, GL_FALSE, &rotationMatrix[0][0]);

	glUniformMatrix4fv(quatRotationLoc, 1, GL_FALSE, &quatRotationMatrix[0][0]);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter3 - Exercise 3.3", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);

	init(window);

	while (!glfwWindowShouldClose(window)) {
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}