#ifndef UTILS_HPP
#define UTILS_HPP

#include <GL/glew.h>
#include <string>

class Utils {
public:
    static std::string readShaderFile(const char* filePath);
    static bool checkOpenGlError();
    static void printShaderLog(GLuint shader);
    static void printProgramLog(GLuint prog);
    static GLuint prepareShader(int shaderType, const char* shaderPath);
    static int finalizeShaderProgram(GLuint program);
    static GLuint createShaderProgram(const char* vp, const char* fp);
};

#endif // UTILS_HPP
