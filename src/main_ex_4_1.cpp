#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Utils.hpp"

using namespace std;

#define numVAOs 1
#define numVBOs 2

float cameraX, cameraY, cameraZ;
//float cubeLocX, cubeLocY, cubeLocZ;
GLuint renderinProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];

// allocate variables used in display() function, so that they won't need to be allocated during rendering
GLuint mvLoc, projLoc;
int width, height;
float aspect;
glm::mat4 pMat, vMat, mMat, mvMat, tMat, rMat;
std::vector<GLuint> indices;

void setupVertices(void) {
    float phi = (1.0f + sqrt(5.0f)) / 2.0f;
    float a = 1.0f;
    float b = 1.0f / phi;
    float c = phi;

    std::vector<glm::vec3> vertices = {

        {a, a, a}, // Vertex (1, 1, 1), Indices 0
        {a, a, -a}, // Vertex (1, 1, -1), Indices 1
        {a, -a, a}, // Vertex (1, -1, 1), Indices 2
        {-a, a, a}, // Vertex (-1, 1, 1), Indices 3
        {a, -a, -a}, // Vertex (1, -1, -1), Indices 4
        {-a, a, -a}, // Vertex (-1, 1, -1), Indices 5
        {-a, -a, a}, // Vertex (-1, -1, 1), Indices 6
        {-a, -a, -a}, // Vertex (-1, -1, -1), Indices 7

        {0, c, b}, // Vertex (0, phi, 1/phi), Indices 8
        {0, c, -b}, // Vertex (0, phi, -1/phi), Indices 9
        {0, -c, b}, // Vertex (0, -phi, 1/phi), Indices 10
        {0, -c, -b}, // Vertex (0, -phi, -1/phi), Indices 11

        {b, 0, c}, // Vertex (1/phi, 0, phi), Indices 12
        {-b, 0, c}, // Vertex (-1/phi, 0, phi), Indices 13
        {b, 0, -c}, // Vertex (1/phi, 0, -phi), Indices 14
        {-b, 0, -c}, // Vertex (-1/phi, 0, -phi), Indices 15

        {c, b, 0}, // Vertex (phi, 1/phi, 0), Indices 16
        {c, -b, 0}, // Vertex (phi, -1/phi, 0), Indices 17
        {-c, b, 0}, // Vertex (-phi, 1/phi, 0), Indices 18
        {-c, -b, 0} // Vertex (-phi, -1/phi, 0), Indices 19
    };

    // Building the pentagons out of the vertices
    std::vector<GLuint> pentagon1 = { 8, 3, 13, 12, 0 };
    std::vector<GLuint> pentagon2 = { 8, 0, 16, 1, 9 };
    std::vector<GLuint> pentagon3 = { 8, 9, 5, 18, 3 };
    std::vector<GLuint> pentagon4 = { 3, 18, 19, 6, 13 };
    std::vector<GLuint> pentagon5 = { 13, 6, 10, 2, 12 };
    std::vector<GLuint> pentagon6 = { 12, 2, 17, 16, 0 };
    std::vector<GLuint> pentagon7 = { 5, 15, 7, 19, 18 };
    std::vector<GLuint> pentagon8 = { 9, 1, 14, 15, 5 };
    std::vector<GLuint> pentagon9 = { 1, 16, 17, 4, 14 };
    std::vector<GLuint> pentagon10 = { 11, 7, 15, 14, 4 };
    std::vector<GLuint> pentagon11 = { 7, 11, 10, 6, 19 };
    std::vector<GLuint> pentagon12 = { 17, 2, 10, 11, 4 };

    // Building the three triangles for each pentagon
    std::vector<GLuint> pentagon1_triangulation {
        8, 3, 13,
        8, 13, 12,
        8, 12, 0
    };

    std::vector<GLuint> pentagon2_triangulation {
        8, 0, 16,
        8, 16, 1,
        8, 1, 9
    };

    std::vector<GLuint> pentagon3_triangulation {
        8, 9, 5,
        8, 5, 18,
        8, 18, 3
    };

    std::vector<GLuint> pentagon4_triangulation {
        3, 18, 19,
        3, 19, 6,
        3, 6, 13
    };

    std::vector<GLuint> pentagon5_triangulation {
        13, 6, 10,
        13, 10, 2,
        13, 2, 12
    };

    std::vector<GLuint> pentagon6_triangulation {
        12, 2, 17,
        12, 17, 16,
        12, 16, 0
    };

    std::vector<GLuint> pentagon7_triangulation {
        5, 15, 7,
        5, 7, 19,
        5, 19, 18
    };

    std::vector<GLuint> pentagon8_triangulation {
        9, 1, 14,
        9, 14, 15,
        9, 15, 5
    };

    std::vector<GLuint> pentagon9_triangulation {
        1, 16, 17,
        1, 17, 4,
        1, 4, 14
    };

    std::vector<GLuint> pentagon10_triangulation {
        11, 7, 15,
        11, 15, 14,
        11, 14, 4
    };

    std::vector<GLuint> pentagon11_triangulation {
        7, 11, 10,
        7, 10, 6,
        7, 6, 19
    };

    std::vector<GLuint> pentagon12_triangulation {
        17, 2, 10,
        17, 10, 11,
        17, 11, 4
    };

    indices.insert(indices.end(), pentagon1_triangulation.begin(), pentagon1_triangulation.end());
    indices.insert(indices.end(), pentagon2_triangulation.begin(), pentagon2_triangulation.end());
    indices.insert(indices.end(), pentagon3_triangulation.begin(), pentagon3_triangulation.end());
    indices.insert(indices.end(), pentagon4_triangulation.begin(), pentagon4_triangulation.end());
    indices.insert(indices.end(), pentagon5_triangulation.begin(), pentagon5_triangulation.end());
    indices.insert(indices.end(), pentagon6_triangulation.begin(), pentagon6_triangulation.end());
    indices.insert(indices.end(), pentagon7_triangulation.begin(), pentagon7_triangulation.end());
    indices.insert(indices.end(), pentagon8_triangulation.begin(), pentagon8_triangulation.end());
    indices.insert(indices.end(), pentagon9_triangulation.begin(), pentagon9_triangulation.end());
    indices.insert(indices.end(), pentagon10_triangulation.begin(), pentagon10_triangulation.end());
    indices.insert(indices.end(), pentagon11_triangulation.begin(), pentagon11_triangulation.end());
    indices.insert(indices.end(), pentagon12_triangulation.begin(), pentagon12_triangulation.end());

    glGenVertexArrays(1, vao);
    glBindVertexArray(vao[0]);
    glGenBuffers(numVBOs, vbo);

    // VBO 0 = Positionen
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    // VBO 1 = Indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

void init(GLFWwindow* window) {
    renderinProgram = Utils::createShaderProgram("shaders/vertShaderProg4_1.glsl", "shaders/fragShaderProg4_1.glsl");

    //build perspective matrix
    glfwGetFramebufferSize(window, &width, &height);
    aspect = (float)width / (float)height;
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f); // 1.0472 radians = 60 degrees

    cameraX = 0.0f; cameraY = 0.0f; cameraZ = 8.0f;
    setupVertices();
}

void display(GLFWwindow* window, double currentTime) {
    glClear(GL_DEPTH_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(renderinProgram);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // Wireframe aktiv

    // get the uniform variables for the MV and projection matrices
    mvLoc = glGetUniformLocation(renderinProgram, "mv_matrix");
    projLoc = glGetUniformLocation(renderinProgram, "proj_matrix");

    vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

    tMat = glm::translate(glm::mat4(1.0f),
        glm::vec3(sin(0.35f * currentTime) * 2.0f, cos(0.52f * currentTime) * 2.0f, sin(0.7f * currentTime) * 2.0f));

    // use current time to compute different translations in x, y, and z
    rMat = glm::rotate(glm::mat4(1.0f), 1.75f * (float)currentTime, glm::vec3(0.0f, 1.0f, 0.0f));
    rMat = glm::rotate(rMat, 1.75f * (float)currentTime, glm::vec3(1.0f, 0.0f, 0.0f));
    rMat = glm::rotate(rMat, 1.75f * (float)currentTime, glm::vec3(0.0f, 0.0f, 1.0f));

    mMat = tMat * rMat;
    //mMat = rMat * tMat; Wrong order

    mvMat = vMat * mMat;

    // copy perspective and MV maatrices to corresponding uniform variables
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

    glBindVertexArray(vao[0]);

    // associate VBO with the corresponding vertex attribute in the vertex shader
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    //adjust OpenGL settings and draw model
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void window_size_callback(GLFWwindow* win, int newWidth, int newHeight) {
    aspect = (float)newWidth / (float)newHeight;
    glViewport(0, 0, newWidth, newHeight);
    pMat = glm::perspective(1.0472f, aspect, 0.1f, 1000.0f);
}

int main(void) {
    if (!glfwInit()) { exit(EXIT_FAILURE); }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(600, 600, "Chapter4 - Program 4.1", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
    glfwSwapInterval(1);

    glfwSetWindowSizeCallback(window, window_size_callback);

    init(window);

    double lastTime = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}