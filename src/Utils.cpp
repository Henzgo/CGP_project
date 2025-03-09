#include "Utils.hpp"
#include <iostream>
#include <fstream>

using namespace std;

string Utils::readShaderFile(const char* filePath) {
    string content;
    ifstream fileStream(filePath, ios::in);
    if (!fileStream.is_open()) {
        cerr << "Error opening shader file: " << filePath << endl;
        return "";
    }
    string line = "";
    while (getline(fileStream, line)) {
        content.append(line + "\n");
    }
    fileStream.close();
    return content;
}

bool Utils::checkOpenGlError() {
    bool foundError = false;
    int glErr = glGetError();
    while (glErr != GL_NO_ERROR) {
        cout << "glError: " << glErr << endl;
        foundError = true;
        glErr = glGetError();
    }
    return foundError;
}

void Utils::printShaderLog(GLuint shader) {
    int len = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        char* log = (char*)malloc(len);
        glGetShaderInfoLog(shader, len, nullptr, log);
        cout << "Shader Info Log: " << log << endl;
        free(log);
    }
}

void Utils::printProgramLog(GLuint prog) {
    int len = 0;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    if (len > 0) {
        char* log = (char*)malloc(len);
        glGetProgramInfoLog(prog, len, nullptr, log);
        cout << "Program Info Log: " << log << endl;
        free(log);
    }
}

GLuint Utils::prepareShader(int shaderType, const char* shaderPath) {
    GLint shaderCompiled;
    string shaderStr = readShaderFile(shaderPath);
    const char* shaderSrc = shaderStr.c_str();
    GLuint shaderRef = glCreateShader(shaderType);
    glShaderSource(shaderRef, 1, &shaderSrc, NULL);
    glCompileShader(shaderRef);

    checkOpenGlError();
    glGetShaderiv(shaderRef, GL_COMPILE_STATUS, &shaderCompiled);
    if (shaderCompiled != 1) {
        if (shaderType == GL_VERTEX_SHADER) cout << "Vertex ";
        if (shaderType == GL_FRAGMENT_SHADER) cout << "Fragment ";
        cout << "shader compilation failed." << endl;
        printShaderLog(shaderRef);
    }
    return shaderRef;
}

int Utils::finalizeShaderProgram(GLuint program) {
    GLint linked;
    glLinkProgram(program);
    checkOpenGlError();
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked != 1) {
        cout << "Shader linking failed!" << endl;
        printProgramLog(program);
    }
    return program;
}

GLuint Utils::createShaderProgram(const char* vp, const char* fp) {
    GLuint vShader = prepareShader(GL_VERTEX_SHADER, vp);
    GLuint fShader = prepareShader(GL_FRAGMENT_SHADER, fp);
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vShader);
    glAttachShader(shaderProgram, fShader);
    finalizeShaderProgram(shaderProgram);
    return shaderProgram;
}
