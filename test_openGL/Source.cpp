#include <iostream> 
#include <glad/glad.h> 
#include <GLFW/glfw3.h> 
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp> 
#define _USE_MATH_DEFINES 
#include <cmath> 
#include "M_PI.h" 
#include "M_PI_2.h" 
#include "cameraControl.h"
#include "textureLoader.h"
#include "VertexShader.h"
#include "FragmentShader.h"
#include "vertexShaderSourceLight.h"
#include "fragmentShaderSourceLight.h"

using namespace std;

// Function prototypes
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xpos, double ypos);

// Cube Light vertices 
GLfloat cubeVerticesL[] = {
    // Positions          
    -0.25f, -0.25f, -0.25f,
     0.25f, -0.25f, -0.25f,
     0.25f,  0.25f, -0.25f,
     0.25f,  0.25f, -0.25f,
    -0.25f,  0.25f, -0.25f,
    -0.25f, -0.25f, -0.25f,

    -0.25f, -0.25f,  0.25f,
     0.25f, -0.25f,  0.25f,
     0.25f,  0.25f,  0.25f,
     0.25f,  0.25f,  0.25f,
    -0.25f,  0.25f,  0.25f,
    -0.25f, -0.25f,  0.25f,

    -0.25f,  0.25f,  0.25f,
    -0.25f,  0.25f, -0.25f,
    -0.25f, -0.25f, -0.25f,
    -0.25f, -0.25f, -0.25f,
    -0.25f, -0.25f,  0.25f,
    -0.25f,  0.25f,  0.25f,

     0.25f,  0.25f,  0.25f,
     0.25f,  0.25f, -0.25f,
     0.25f, -0.25f, -0.25f,
     0.25f, -0.25f, -0.25f,
     0.25f, -0.25f,  0.25f,
     0.25f,  0.25f,  0.25f,

    -0.25f, -0.25f, -0.25f,
     0.25f, -0.25f, -0.25f,
     0.25f, -0.25f,  0.25f,
     0.25f, -0.25f,  0.25f,
    -0.25f, -0.25f,  0.25f,
    -0.25f, -0.25f, -0.25f,

    -0.25f,  0.25f, -0.25f,
     0.25f,  0.25f, -0.25f,
     0.25f,  0.25f,  0.25f,
     0.25f,  0.25f,  0.25f,
    -0.25f,  0.25f,  0.25f,
    -0.25f,  0.25f, -0.25f
};

// Vertices for the plane with texture coordinates
GLfloat planeVertices[] = {
    // Positions           // Texture coordinates
    -5.0f, -0.25f, -5.0f,    0.0f, 0.0f,
     5.0f, -0.25f, -5.0f,    1.0f, 0.0f,
     5.0f, -0.25f,  5.0f,    1.0f, 1.0f,
     5.0f, -0.25f,  5.0f,    1.0f, 1.0f,
    -5.0f, -0.25f,  5.0f,    0.0f, 1.0f,
    -5.0f, -0.25f, -5.0f,    0.0f, 0.0f,
};

// Vertices for the cube with texture coordinates
GLfloat cubeVertices[] = {
    // Positions           // Texture coordinates
    -0.5f, -0.249f, -0.5f,    0.0f, 0.0f,
     0.5f, -0.249f, -0.5f,    1.0f, 0.0f,
     0.5f,  0.6f, -0.5f,    1.0f, 1.0f,
    -0.5f,  0.6f, -0.5f,    0.0f, 1.0f,

    -0.5f, -0.249f,  0.5f,    0.0f, 0.0f,
     0.5f, -0.249f,  0.5f,    1.0f, 0.0f,
     0.5f,  0.6f,  0.5f,    1.0f, 1.0f,
    -0.5f,  0.6f,  0.5f,    0.0f, 1.0f
};

// Indices for cube
GLuint cubeIndices[] = {
    0, 1, 2,
    2, 3, 0,
    4, 5, 6,
    6, 7, 4,
    0, 3, 7,
    7, 4, 0,
    1, 2, 6,
    6, 5, 1,
    3, 2, 6,
    6, 7, 3,
    0, 1, 5,
    5, 4, 0
};

GLfloat recY = 0.28f;
GLfloat recZ = 2.0f;

GLfloat rectangleWallet1[] = {
    // Positions           // Texture coordinates
    // Front face
    -1.0f,  recY, -0.25,  0.0f, 0.0f,  // Bottom-left  0
    -2.0f,  recY, -0.25f,  1.0f, 0.0f,  // Bottom-right 1
    -2.0f, 0.30f, -0.25f,  1.0f, 1.0f,  // Top-right    2
    -1.0f, 0.30f, -0.25f,  0.0f, 1.0f,  // Top-left     3

    // Back face
    -1.0f,  recY, recZ,   0.0f, 0.0f,  // Bottom-left  4
    -2.0f,  recY, recZ,   1.0f, 0.0f,  // Bottom-right 5
    -2.0f, 0.30f, recZ,   1.0f, 1.0f,  // Top-right    6
    -1.0f, 0.30f, recZ,   0.0f, 1.0f,  // Top-left     7

    // Right face
    -2.0f,  recY, -0.25f,  0.0f, 0.0f,  // Bottom-left  8
    -2.0f, 0.30f, -0.25f,  1.0f, 0.0f,  // Bottom-right 9
    -2.0f, 0.30f,  recZ,  1.0f, 1.0f,  // Top-right    10
    -2.0f,  recY,  recZ,  0.0f, 1.0f,  // Top-left     11

    // Left face
    -1.0f,  recY, -0.25f,  0.0f, 0.0f,  // Bottom-left  12
    -1.0f, 0.30f, -0.25f,  1.0f, 0.0f,  // Bottom-right 13
    -1.0f, 0.30f,  recZ,  1.0f, 1.0f,  // Top-right    14
    -1.0f,  recY,  recZ,  0.0f, 1.0f,  // Top-left     15

    // Top face
    -1.0f,  recY, -0.25f,  0.0f, 0.0f,  // Bottom-left  16
    -2.0f,  recY, -0.25f,  1.0f, 0.0f,  // Bottom-right 17
    -2.0f,  recY,  recZ,  1.0f, 1.0f,  // Top-right    18
    -1.0f,  recY,  recZ,  0.0f, 1.0f,  // Top-left     19

    // Bottom face
    -1.0f, 0.30f, -0.25f,  0.0f, 0.0f,  // Bottom-left  20
    -2.0f, 0.30f, -0.25f,  1.0f, 0.0f,  // Bottom-right 21
    -2.0f, 0.30f,  recZ,  1.0f, 1.0f,  // Top-right    22
    -1.0f, 0.30f,  recZ,  0.0f, 1.0f   // Top-left     23
};


GLfloat recIY = 0.28f;
GLfloat recIZ = 2.0f;

GLfloat rectangleWalletIsIn[] = {
    // Positions           // Texture coordinates
    // Front face
    -1.0f,  recIY, -0.25,  0.0f, 0.0f,  // Bottom-left  0
    -2.0f,  recIY, -0.25f,  1.0f, 0.0f,  // Bottom-right 1
    -2.0f, -0.249f, -0.25f,  1.0f, 1.0f,  // Top-right    2
    -1.0f, -0.249f, -0.25f,  0.0f, 1.0f,  // Top-left     3

    // Back face
    -1.0f,  recIY, recIZ,   0.0f, 0.0f,  // Bottom-left  4
    -2.0f,  recIY, recIZ,   1.0f, 0.0f,  // Bottom-right 5
    -2.0f, -0.249f, recIZ,   1.0f, 1.0f,  // Top-right    6
    -1.0f, -0.249f, recIZ,   0.0f, 1.0f,  // Top-left     7

    // Right face
    -2.0f,  recIY, -0.25f,  0.0f, 0.0f,  // Bottom-left  8
    -2.0f, -0.249f, -0.25f,  1.0f, 0.0f,  // Bottom-right 9
    -2.0f, -0.249f,  recIZ,  1.0f, 1.0f,  // Top-right    10
    -2.0f,  recIY,  recIZ,  0.0f, 1.0f,  // Top-left     11

    // Left face
    -1.0f,  recIY, -0.25f,  0.0f, 0.0f,  // Bottom-left  12
    -1.0f, -0.249f, -0.25f,  1.0f, 0.0f,  // Bottom-right 13
    -1.0f, -0.249f,  recIZ,  1.0f, 1.0f,  // Top-right    14
    -1.0f,  recIY,  recIZ,  0.0f, 1.0f,  // Top-left     15

    // Top face
    -1.0f,  recIY, -0.25f,  0.0f, 0.0f,  // Bottom-left  16
    -2.0f,  recIY, -0.25f,  1.0f, 0.0f,  // Bottom-right 17
    -2.0f,  recIY,  recIZ,  1.0f, 1.0f,  // Top-right    18
    -1.0f,  recIY,  recIZ,  0.0f, 1.0f,  // Top-left     19

    // Bottom face
    -1.0f, -0.249f, -0.25f,  0.0f, 0.0f,  // Bottom-left  20
    -2.0f, -0.249f, -0.25f,  1.0f, 0.0f,  // Bottom-right 21
    -2.0f, -0.249f,  recIZ,  1.0f, 1.0f,  // Top-right    22
    -1.0f, -0.249f,  recIZ,  0.0f, 1.0f   // Top-left     23
};

// Vertecise for the Cleaner
GLfloat cleaningVer[] = {
    // Front face
    2.0f,  -0.249f, -0.25f,  0.0f, 0.0f,  // Bottom-left  0
    2.5f, -0.249f, -0.25f,  1.0f, 0.0f,  // Bottom-right 1
    2.5f,  2.0f, -0.25f,  1.0f, 1.0f,  // Top-right    2
    2.0f,  2.0f, -0.25f,  0.0f, 1.0f,  // Top-left     3

    // Back face
    2.0f,  -0.249f, 0.25f,  0.0f, 0.0f,  // Bottom-left  4
    2.5f, -0.249f, 0.25f,  1.0f, 0.0f,  // Bottom-right 5
    2.5f,  2.0f, 0.25f,  1.0f, 1.0f,  // Top-right    6
    2.0f,  2.0f, 0.25f,  0.0f, 1.0f,  // Top-left     7

    // Right face
    2.5f, -0.249f, -0.25f,  0.0f, 0.0f,  // Bottom-left  8
    2.5f,  2.0f, -0.25f,  1.0f, 0.0f,  // Bottom-right 9
    2.5f,  2.0f,  0.25f,  1.0f, 1.0f,  // Top-right    10
    2.5f, -0.249f,  0.25f,  0.0f, 1.0f,  // Top-left     11

    // Left face
    2.0f, -0.249f, -0.25f,  0.0f, 0.0f,  // Bottom-left  12
    2.0f,  2.0f, -0.25f,  1.0f, 0.0f,  // Bottom-right 13
    2.0f,  2.0f,  0.25f,  1.0f, 1.0f,  // Top-right    14
    2.0f, -0.249f,  0.25f,  0.0f, 1.0f,  // Top-left     15

    // Top face
    2.0f,  2.0f, -0.25f,  0.0f, 0.0f,  // Bottom-left  16
    2.5f,  2.0f, -0.25f,  1.0f, 0.0f,  // Bottom-right 17
    2.5f,  2.0f,  0.25f,  1.0f, 1.0f,  // Top-right    18
    2.0f,  2.0f,  0.25f,  0.0f, 1.0f,  // Top-left     19

    // Bottom face
    2.0f, -0.249f, -0.25f,  0.0f, 0.0f,  // Bottom-left  20
    2.5f, -0.249f, -0.25f,  1.0f, 0.0f,  // Bottom-right 21
    2.5f, -0.249f,  0.25f,  1.0f, 1.0f,  // Top-right    22
    2.0f, -0.249f,  0.25f,  0.0f, 1.0f   // Top-left     23
};

// Vertices for the front of the Cleaner
GLfloat cleaningfr[] = {
    // Front face
    2.0f,  -0.249f, 0.0f,  0.0f, 0.0f,  // Bottom-left  0
    2.5f, -0.249f, 0.0f,  1.0f, 0.0f,  // Bottom-right 1
    2.5f,  2.0f, 0.0f,  1.0f, 1.0f,  // Top-right    2
    2.0f,  2.0f, 0.0f,  0.0f, 1.0f,  // Top-left     3

    // Back face
    2.0f,  -0.249f, 0.35f,  0.0f, 0.0f,  // Bottom-left  4
    2.5f, -0.249f, 0.35f,  1.0f, 0.0f,  // Bottom-right 5
    2.5f,  2.0f, 0.35f,  1.0f, 1.0f,  // Top-right    6
    2.0f,  2.0f, 0.35f,  0.0f, 1.0f,  // Top-left     7

    // Right face
    2.5f, -0.249f, 0.0f,  0.0f, 0.0f,  // Bottom-left  8
    2.5f,  2.0f,  0.0f,  1.0f, 0.0f,  // Bottom-right 9
    2.5f,  2.0f,  0.35f,  1.0f, 1.0f,  // Top-right    10
    2.5f, -0.249f,  0.35f,  0.0f, 1.0f,  // Top-left     11

    // Left face
    2.0f, -0.249f, 0.0f,  0.0f, 0.0f,  // Bottom-left  12
    2.0f,  2.0f,  0.0f,  1.0f, 0.0f,  // Bottom-right 13
    2.0f,  2.0f,  0.35f,  1.0f, 1.0f,  // Top-right    14
    2.0f, -0.249f,  0.35f,  0.0f, 1.0f,  // Top-left     15

    // Top face
    2.0f,  2.0f,  0.0f,  0.0f, 0.0f,  // Bottom-left  16
    2.5f,  2.0f,  0.0f,  1.0f, 0.0f,  // Bottom-right 17
    2.5f,  2.0f,  0.35f,  1.0f, 1.0f,  // Top-right    18
    2.0f,  2.0f,  0.35f,  0.0f, 1.0f,  // Top-left     19

    // Bottom face
    2.0f, -0.249f,  0.0f,  0.0f, 0.0f,  // Bottom-left  20
    2.5f, -0.249f,  0.0f,  1.0f, 0.0f,  // Bottom-right 21
    2.5f, -0.249f,  0.35f,  1.0f, 1.0f,  // Top-right    22
    2.0f, -0.249f,  0.35f,  0.0f, 1.0f   // Top-left     23
};

// vertices for black top for cleaner
GLfloat blackTop[] = {
    // Front face
    2.0f,  2.0f, -0.25f,  0.0f, 0.0f,  // Bottom-left  0
    2.5f,  2.0f, -0.25f,  1.0f, 0.0f,  // Bottom-right 1
    2.5f,  2.1f, -0.25f,  1.0f, 1.0f,  // Top-right    2
    2.0f,  2.1f, -0.25f,  0.0f, 1.0f,  // Top-left     3

    // Back face
    2.0f,  2.0f, 0.25f,  0.0f, 0.0f,  // Bottom-left  4
    2.5f,  2.0f, 0.25f,  1.0f, 0.0f,  // Bottom-right 5
    2.5f,  2.1f, 0.25f,  1.0f, 1.0f,  // Top-right    6
    2.0f,  2.1f, 0.25f,  0.0f, 1.0f,  // Top-left     7

    // Right face
    2.5f,  2.0f, -0.25f,  0.0f, 0.0f,  // Bottom-left  8
    2.5f,  2.1f, -0.25f,  1.0f, 0.0f,  // Bottom-right 9
    2.5f,  2.1f,  0.25f,  1.0f, 1.0f,  // Top-right    10
    2.5f,  2.0f,  0.25f,  0.0f, 1.0f,  // Top-left     11

    // Left face
    2.0f,  2.0f, -0.25f,  0.0f, 0.0f,  // Bottom-left  12
    2.0f,  2.1f, -0.25f,  1.0f, 0.0f,  // Bottom-right 13
    2.0f,  2.1f,  0.25f,  1.0f, 1.0f,  // Top-right    14
    2.0f,  2.0f,  0.25f,  0.0f, 1.0f,  // Top-left     15

    // Top face
    2.0f,  2.1f, -0.25f,  0.0f, 0.0f,  // Bottom-left  16
    2.5f,  2.1f, -0.25f,  1.0f, 0.0f,  // Bottom-right 17
    2.5f,  2.1f,  0.25f,  1.0f, 1.0f,  // Top-right    18
    2.0f,  2.1f,  0.25f,  0.0f, 1.0f,  // Top-left     19

    // Bottom face
    2.0f,  2.0f, -0.25f,  0.0f, 0.0f,  // Bottom-left  20
    2.5f,  2.0f, -0.25f,  1.0f, 0.0f,  // Bottom-right 21
    2.5f,  2.0f,  0.25f,  1.0f, 1.0f,  // Top-right    22
    2.0f,  2.0f,  0.25f,  0.0f, 1.0f   // Top-left     23
};

// vertices for black top for pencil0
GLfloat pencilBody0[] = {
    // Front face
    3.1f,  -0.249f, -0.25f,  0.0f, 0.0f,  // Bottom-left  0
    3.4f, -0.249f, -0.25f,  1.0f, 0.0f,  // Bottom-right 1
    3.4f, 0.1f, -0.25f,  1.0f, 1.0f,  // Top-right    2
    3.1f, 0.1f, -0.25f,  0.0f, 1.0f,  // Top-left     3

    // Back face
    3.1f, -0.249f, 3.0f,  0.0f, 0.0f,  // Bottom-left  4
    3.4f, -0.249f, 3.0f,  1.0f, 0.0f,  // Bottom-right 5
    3.4f, 0.1f, 3.0f,  1.0f, 1.0f,  // Top-right    6
    3.1f, 0.1f, 3.0f,  0.0f, 1.0f,  // Top-left     7

    // Right face
    3.4f, -0.249f, -0.25f,  0.0f, 0.0f,  // Bottom-left  8
    3.4f, 0.1f, -0.25f,  1.0f, 0.0f,  // Bottom-right 9
    3.4f, 0.1f, 3.0f,  1.0f, 1.0f,  // Top-right    10
    3.4f, -0.249f, 3.0f,  0.0f, 1.0f,  // Top-left     11

    // Left face
    3.1f, -0.249f, -0.25f,  0.0f, 0.0f,  // Bottom-left  12
    3.1f, 0.1f, -0.25f,  1.0f, 0.0f,  // Bottom-right 13
    3.1f, 0.1f, 3.0f,  1.0f, 1.0f,  // Top-right    14
    3.1f, -0.249f, 3.0f,  0.0f, 1.0f,  // Top-left     15

    // Top face
    3.1f, 0.1f, -0.25f,  0.0f, 0.0f,  // Bottom-left  16
    3.4f, 0.1f, -0.25f,  1.0f, 0.0f,  // Bottom-right 17
    3.4f, 0.1f, 3.0f,  1.0f, 1.0f,  // Top-right    18
    3.1f, 0.1f, 3.0f,  0.0f, 1.0f,  // Top-left     19

    // Bottom face
    3.1f, -0.249f, -0.25f,  0.0f, 0.0f,  // Bottom-left  20
    3.4f, -0.249f, -0.25f,  1.0f, 0.0f,  // Bottom-right 21
    3.4f, -0.249f, 3.0f,  1.0f, 1.0f,  // Top-right    22
    3.1f, -0.249f, 3.0f,  0.0f, 1.0f   // Top-left     23
};

// vertices for black top for pencil1
GLfloat pencilBody1[] = {
    // Front face
    3.1f,  -0.249f, -1.00f,  0.0f, 0.0f,  // Bottom-left  0
    3.4f, -0.249f, -1.00f,  1.0f, 0.0f,  // Bottom-right 1
    3.4f, 0.1f, -1.00f,  1.0f, 1.0f,  // Top-right    2
    3.1f, 0.1f, -1.00f,  0.0f, 1.0f,  // Top-left     3

    // Back face
    3.1f, -0.249f, -0.25f,  0.0f, 0.0f,  // Bottom-left  4
    3.4f, -0.249f, -0.25f,  1.0f, 0.0f,  // Bottom-right 5
    3.4f, 0.1f, -0.25,  1.0f, 1.0f,  // Top-right    6
    3.1f, 0.1f, -0.25,  0.0f, 1.0f,  // Top-left     7

    // Right face
    3.4f, -0.249f, -1.00f,  0.0f, 0.0f,  // Bottom-left  8
    3.4f, 0.1f, -1.00f,  1.0f, 0.0f,  // Bottom-right 9
    3.4f, 0.1f, -0.25f,  1.0f, 1.0f,  // Top-right    10
    3.4f, -0.249f, -0.25f,  0.0f, 1.0f,  // Top-left     11

    // Left face
    3.1f, -0.249f, -1.00f,  0.0f, 0.0f,  // Bottom-left  12
    3.1f, 0.1f, -1.00f,  1.0f, 0.0f,  // Bottom-right 13
    3.1f, 0.1f, -0.25,  1.0f, 1.0f,  // Top-right    14
    3.1f, -0.249f, -0.25f,  0.0f, 1.0f,  // Top-left     15

    // Top face
    3.1f, 0.1f, -1.00f,  0.0f, 0.0f,  // Bottom-left  16
    3.4f, 0.1f, -1.00f,  1.0f, 0.0f,  // Bottom-right 17
    3.4f, 0.1f, -0.25f,  1.0f, 1.0f,  // Top-right    18
    3.1f, 0.1f, -0.25f,  0.0f, 1.0f,  // Top-left     19

    // Bottom face
    3.1f, -0.249f, -1.00f,  0.0f, 0.0f,  // Bottom-left  20
    3.4f, -0.249f, -1.00f,  1.0f, 0.0f,  // Bottom-right 21
    3.4f, -0.249f, -0.25f,  1.0f, 1.0f,  // Top-right    22
    3.1f, -0.249f, -0.25f,  0.0f, 1.0f   // Top-left     23
};

// vertices for black top for pencil2
GLfloat pencilBody2[] = {
    // Front face
    3.1f,  -0.249f, 3.00f,  0.0f, 0.0f,  // Bottom-left  0
    3.4f, -0.249f, 3.00f,  1.0f, 0.0f,  // Bottom-right 1
    3.4f, 0.1f, 3.00f,  1.0f, 1.0f,  // Top-right    2
    3.1f, 0.1f, 3.00f,  0.0f, 1.0f,  // Top-left     3

    // Back face
    3.25f, -0.05f, 3.5f,  0.0f, 0.0f,  // Bottom-left  4
    3.25f, -0.05f, 3.5f,  1.0f, 0.0f,  // Bottom-right 5
    3.25f, -0.05f, 3.5,  1.0f, 1.0f,  // Top-right    6
    3.25f, -0.05f, 3.5,  0.0f, 1.0f,  // Top-left     7

    // Right face
    3.4f, -0.249f, 3.00f,  0.0f, 0.0f,  // Bottom-left  8
    3.4f, 0.1f, 3.00f,  1.0f, 0.0f,  // Bottom-right 9
    3.25f, -0.05f, 3.5f,  1.0f, 1.0f,  // Top-right    10
    3.25f, -0.05f, 3.5f,  0.0f, 1.0f,  // Top-left     11

    // Left face
    3.1f,  -0.249f, 3.00f,  0.0f, 0.0f,  // Bottom-left  12
    3.1f,   0.1f,   3.00f,  1.0f, 0.0f,  // Bottom-right 13
    3.25f, -0.05f,  3.5,  1.0f, 1.0f,  // Top-right    14
    3.25f, -0.05f,  3.5f,  0.0f, 1.0f,  // Top-left     15

    // Top face
    3.1f, 0.1f, 3.00f,  0.0f, 0.0f,  // Bottom-left  16
    3.4f, 0.1f, 3.00f,  1.0f, 0.0f,  // Bottom-right 17
    3.25f, -0.05f, 3.5f,  1.0f, 1.0f,  // Top-right    18
    3.25f, -0.05f, 3.5f,  0.0f, 1.0f,  // Top-left     19

    // Bottom face
    3.1f, -0.249f, 3.00f,  0.0f, 0.0f,  // Bottom-left  20
    3.4f, -0.249f, 3.00f,  1.0f, 0.0f,  // Bottom-right 21
    3.25f, -0.05f, 3.5f,  1.0f, 1.0f,  // Top-right    22
    3.25f, -0.05f, 3.5f,  0.0f, 1.0f   // Top-left     23
};

GLuint indicesForRect[] = {
    // Front face
     0, 1, 2, // First Triangle
     2, 3, 0, // Second Triangle

     // Back face
     4, 5, 6, // First Triangle
     6, 7, 4, // Second Triangle

     // Right face
     8, 9, 10, // First Triangle
     10, 11, 8, // Second Triangle

     // Left face
     12, 13, 14, // First Triangle
     14, 15, 12, // Second Triangle

     // Top face
     16, 17, 18, // First Triangle
     18, 19, 16, // Second Triangle

     // Bottom face
     20, 21, 22, // First Triangle
     22, 23, 20  // Second Triangle
};

// Function to generate vertices and indices for a sphere with texture coordinates
void createSphereWithOffset(GLfloat radius, GLuint rings, GLuint sectors, GLfloat offsetY, GLfloat** vertices, GLuint** indices, GLfloat** texCoords) {
    *vertices = new GLfloat[6 * rings * sectors];  // 6 components per vertex 
    *indices = new GLuint[6 * (rings - 1) * (sectors - 1)];
    *texCoords = new GLfloat[2 * rings * sectors];
    int r, s;
    float const R = 1.0f / static_cast<float>(rings - 1);
    float const S = 1.0f / static_cast<float>(sectors - 1);

    GLfloat* v = *vertices;
    GLuint* i = *indices;
    GLfloat* t = *texCoords;
    for (r = 0; r < rings; ++r)
        for (s = 0; s < sectors; ++s) {
            float const y = sin(-M_PI_2 + M_PI * r * R);
            float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
            float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

            // Position with offset
            v[0] = x * radius;
            v[1] = y * radius + offsetY; // Add offset here
            v[2] = z * radius;

            // Texture coordinates
            t[0] = s * S; // u
            t[1] = r * R; // v

            v += 6;
            t += 2;
        }

    for (r = 0; r < rings - 1; ++r)
        for (s = 0; s < sectors - 1; ++s) {
            i[0] = r * sectors + s;
            i[1] = r * sectors + (s + 1);
            i[2] = (r + 1) * sectors + (s + 1);
            i[3] = r * sectors + s;
            i[4] = (r + 1) * sectors + (s + 1);
            i[5] = (r + 1) * sectors + s;

            i += 6;
        }
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        cout << "Failed to open GLFW" << endl;
        return -1;
    }

    // Set GLFW to use OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // Create a GLFW window
    GLFWwindow* window = glfwCreateWindow(800, 500, "Mohamed", NULL, NULL);
    if (window == NULL) {
        cout << "Failed to open!" << endl;
        glfwTerminate();
        return -1;
    }

    glfwSetCursorPosCallback(window, mouse_callback); // Set mouse movement callback
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetScrollCallback(window, scroll_callback); // Set scroll wheel callback

    // Make the window's context the current one
    glfwMakeContextCurrent(window);

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        cout << "Failed to open GLAD" << endl;
        return -1;
    }


    // Compile and link shaders
    GLuint vertexShaderLight, fragmentShaderLight, shaderProgramLight;
    vertexShaderLight = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShaderLight, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShaderLight);

    fragmentShaderLight = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderLight, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShaderLight);

    shaderProgramLight = glCreateProgram();
    glAttachShader(shaderProgramLight, vertexShaderLight);
    glAttachShader(shaderProgramLight, fragmentShaderLight);
    glLinkProgram(shaderProgramLight);

    glDeleteShader(vertexShaderLight);
    glDeleteShader(fragmentShaderLight);
    glUseProgram(shaderProgramLight);

    // Define the cube's light VAO and VBO
    GLuint VAOL, VBOL;
    glGenVertexArrays(1, &VAOL);
    glGenBuffers(1, &VBOL);

    glBindVertexArray(VAOL);

    glBindBuffer(GL_ARRAY_BUFFER, VBOL);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVerticesL), cubeVerticesL, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Compile and link shaders
    GLuint vertexShader, fragmentShader, shaderProgram;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Define and use the shader program
    glUseProgram(shaderProgram);

    // Vertex Array Objects (VAOs) and Vertex Buffer Objects (VBOs) and Element Buffer Objects (EBOs) for cube
    GLuint VAOCube, VBOCube, EBOCube;
    glGenVertexArrays(1, &VAOCube);
    glGenBuffers(1, &VBOCube);
    glGenBuffers(1, &EBOCube);

    glBindVertexArray(VAOCube);

    glBindBuffer(GL_ARRAY_BUFFER, VBOCube);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOCube);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinates attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
 
// Vertex Array Objects (VAOs) and Vertex Buffer Objects (VBOs) and Element Buffer Objects (EBOs) for wallet
    GLuint VAOWallet1, VBOWallet1, EBOWallet1;
    glGenVertexArrays(1, &VAOWallet1);
    glGenBuffers(1, &VBOWallet1);
    glGenBuffers(1, &EBOWallet1);

    glBindVertexArray(VAOWallet1);

    glBindBuffer(GL_ARRAY_BUFFER, VBOWallet1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleWallet1), rectangleWallet1, GL_STATIC_DRAW);

    // Element Buffer Object (EBO)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOWallet1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesForRect), indicesForRect, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture Coordinates attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);


    // Vertex Array Objects (VAOs) and Vertex Buffer Objects (VBOs) and Element Buffer Objects (EBOs) for wallet the inside
    GLuint VAOWalletIn, VBOWalletIn, EBOWalletIn;
    glGenVertexArrays(1, &VAOWalletIn);
    glGenBuffers(1, &VBOWalletIn);
    glGenBuffers(1, &EBOWalletIn);

    glBindVertexArray(VAOWalletIn);

    glBindBuffer(GL_ARRAY_BUFFER, VBOWalletIn);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleWalletIsIn), rectangleWalletIsIn, GL_STATIC_DRAW);

    // Element Buffer Object (EBO)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOWallet1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesForRect), indicesForRect, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture Coordinates attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Vertex Array Objects (VAOs) and Vertex Buffer Objects (VBOs) and Element Buffer Objects (EBOs) for Cleaning tool
    GLuint VAOClean, VBOClean, EBOClean;
    glGenVertexArrays(1, &VAOClean);
    glGenBuffers(1, &VBOClean);
    glGenBuffers(1, &EBOClean);

    glBindVertexArray(VAOClean);

    glBindBuffer(GL_ARRAY_BUFFER, VBOClean);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cleaningVer), cleaningVer, GL_STATIC_DRAW);

    // Element Buffer Object (EBO)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOClean);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesForRect), indicesForRect, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture Coordinates attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Vertex Array Objects (VAOs) and Vertex Buffer Objects (VBOs) and Element Buffer Objects (EBOs) for cleaning front
    GLuint VAOCleanfr, VBOCleanfr, EBOCleanfr;
    glGenVertexArrays(1, &VAOCleanfr);
    glGenBuffers(1, &VBOCleanfr);
    glGenBuffers(1, &EBOCleanfr);

    glBindVertexArray(VAOCleanfr);

    glBindBuffer(GL_ARRAY_BUFFER, VBOCleanfr);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cleaningfr), cleaningfr, GL_STATIC_DRAW);

    // Element Buffer Object (EBO)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOWallet1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesForRect), indicesForRect, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture Coordinates attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Vertex Array Objects (VAOs) and Vertex Buffer Objects (VBOs) and Element Buffer Objects (EBOs) for plane
    GLuint VAOPlane, VBOPlane;
    glGenVertexArrays(1, &VAOPlane);
    glGenBuffers(1, &VBOPlane);

    glBindVertexArray(VAOPlane);

    glBindBuffer(GL_ARRAY_BUFFER, VBOPlane);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture Coordinates attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Vertex Array Objects (VAOs) and Vertex Buffer Objects (VBOs) and Element Buffer Objects (EBOs) for Black top
    GLuint VAOBlackTop, VBOBlackTop, EBOBlackTop;
    glGenVertexArrays(1, &VAOBlackTop);
    glGenBuffers(1, &VBOBlackTop);
    glGenBuffers(1, &EBOBlackTop);

    glBindVertexArray(VAOBlackTop);

    glBindBuffer(GL_ARRAY_BUFFER, VBOBlackTop);
    glBufferData(GL_ARRAY_BUFFER, sizeof(blackTop), blackTop, GL_STATIC_DRAW);

    // Element Buffer Object (EBO)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOWallet1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesForRect), indicesForRect, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture Coordinates attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Vertex Array Objects (VAOs) and Vertex Buffer Objects (VBOs) and Element Buffer Objects (EBOs) for pencil body 0
    GLuint VAOPenciBody0, VBOPenciBody0, EBOPenciBody0;
    glGenVertexArrays(1, &VAOPenciBody0);
    glGenBuffers(1, &VBOPenciBody0);
    glGenBuffers(1, &EBOPenciBody0);

    glBindVertexArray(VAOPenciBody0);

    glBindBuffer(GL_ARRAY_BUFFER, VBOPenciBody0);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pencilBody0), pencilBody0, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOPenciBody0);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesForRect), indicesForRect, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinates attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Vertex Array Objects (VAOs) and Vertex Buffer Objects (VBOs) and Element Buffer Objects (EBOs) for pencil body 1
    GLuint VAOPenciBody1, VBOPenciBody1, EBOPenciBody1;
    glGenVertexArrays(1, &VAOPenciBody1);
    glGenBuffers(1, &VBOPenciBody1);
    glGenBuffers(1, &EBOPenciBody1);

    glBindVertexArray(VAOPenciBody1);

    glBindBuffer(GL_ARRAY_BUFFER, VBOPenciBody1);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pencilBody1), pencilBody1, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOPenciBody0);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesForRect), indicesForRect, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinates attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);


    // Vertex Array Objects (VAOs) and Vertex Buffer Objects (VBOs) and Element Buffer Objects (EBOs) for pencil body 2
    GLuint VAOPenciBody2, VBOPenciBody2, EBOPenciBody2;
    glGenVertexArrays(1, &VAOPenciBody2);
    glGenBuffers(1, &VBOPenciBody2);
    glGenBuffers(1, &EBOPenciBody2);

    glBindVertexArray(VAOPenciBody2);

    glBindBuffer(GL_ARRAY_BUFFER, VBOPenciBody2);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pencilBody2), pencilBody2, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOPenciBody0);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesForRect), indicesForRect, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinates attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // Create vertices and indices for a sphere
    GLfloat* sphereVertices;
    GLuint* sphereIndices;
    GLfloat* texCoords;
    createSphereWithOffset(0.9f, 30, 30, 0.8f, &sphereVertices, &sphereIndices, &texCoords);

    // Vertex Array Objects (VAOs) and Vertex Buffer Objects (VBOs) and Element Buffer Objects (EBOs) for sphere
    GLuint VAOSphere, VBOSphere, EBOSphere;
    glGenVertexArrays(1, &VAOSphere);
    glGenBuffers(1, &VBOSphere);
    glGenBuffers(1, &EBOSphere);

    glBindVertexArray(VAOSphere);

    glBindBuffer(GL_ARRAY_BUFFER, VBOSphere);
    glBufferData(GL_ARRAY_BUFFER, 6 * 30 * 30 * sizeof(GLfloat), sphereVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOSphere);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 29 * 29 * sizeof(GLuint), sphereIndices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinates attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);


    // Texture loading and setup
    GLuint texture2 = loadTexture("leather.jpg");
    GLuint texture3 = loadTexture("black.png");
    GLuint texture4 = loadTexture("soccer.jpg");
    GLuint texture5 = loadTexture("carbon.png");
    GLuint texture6 = loadTexture("walletIn.png");
    GLuint texture7 = loadTexture("cleaner.png");
    GLuint texture8 = loadTexture("cleanerfr.png");
    GLuint texture9 = loadTexture("pencil.png");
    GLuint texture10 = loadTexture("backPencil.png");
    GLuint texture11 = loadTexture("backPencil1.png");
   
    // Projection matrix
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // Model-View matrix
    glm::mat4 view = glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glEnable(GL_DEPTH_TEST);

    // Main loop
    float rotationSpeed = 0.0f; // Adjust the rotation
    float sphereOffset = 0.5f;  // Adjust how much inside the sphere the cube goes

    // Get uniform locations outside the loop
    GLuint textureUniformLoc = glGetUniformLocation(shaderProgram, "texture1");
    glUniform1i(textureUniformLoc, 0);

    // Retrieve uniform locations
    GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
    GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
    GLint projectionLoc = glGetUniformLocation(shaderProgram, "projection");

    GLuint modelLocCube = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLocCube = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLocCube = glGetUniformLocation(shaderProgram, "projection");

    // Get uniform locations for wallet1 shader
    GLuint modelLocWallet1 = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLocWallet1 = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLocWallet1 = glGetUniformLocation(shaderProgram, "projection");

    // Get uniform locations for sphere shader
    GLuint modelLocSphere = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLocSphere = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionLocSphere = glGetUniformLocation(shaderProgram, "projection");

    // Retrieve uniform locations
    GLint modelLocWalletIn = glGetUniformLocation(shaderProgram, "model");
    GLint viewLocWalletIn = glGetUniformLocation(shaderProgram, "view");
    GLint projectionLocWalletIn = glGetUniformLocation(shaderProgram, "projection");


    // Retrieve uniform locations
    GLint modelLocClean = glGetUniformLocation(shaderProgram, "model");
    GLint viewLocClean = glGetUniformLocation(shaderProgram, "view");
    GLint projectionLocClean = glGetUniformLocation(shaderProgram, "projection");

    // Retrieve uniform locations
    GLint modelLocCleanfr = glGetUniformLocation(shaderProgram, "model");
    GLint viewLocCleanfr = glGetUniformLocation(shaderProgram, "view");
    GLint projectionLocCleanfr = glGetUniformLocation(shaderProgram, "projection");

    // Retrieve uniform locations
    GLint modelLocBlackTop = glGetUniformLocation(shaderProgram, "model");
    GLint viewLocBlackTop = glGetUniformLocation(shaderProgram, "view");
    GLint projectionLocBlackTop = glGetUniformLocation(shaderProgram, "projection");

    // Retrieve uniform locations
    GLuint modelLocPencil0 = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLocPencil0 = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionPencil0 = glGetUniformLocation(shaderProgram, "projection");

    // Retrieve uniform locations
    GLuint modelLocPencil1 = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLocPencil1 = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionPencil1 = glGetUniformLocation(shaderProgram, "projection");

    // Retrieve uniform locations
    GLuint modelLocPencil2 = glGetUniformLocation(shaderProgram, "model");
    GLuint viewLocPencil2 = glGetUniformLocation(shaderProgram, "view");
    GLuint projectionPencil2 = glGetUniformLocation(shaderProgram, "projection");

    // While loop
    while (!glfwWindowShouldClose(window)) {
        // Process input
        processInput(window);

        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set view matrix
        view = glm::lookAt(glm::vec3(camX, camY, camZ), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        // Use light shader program
        glUseProgram(shaderProgramLight);

        // Set transformation matrices for light cube
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 3.0f, 0.0f)); // Translate the cube upwards
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
        GLint modelLoc = glGetUniformLocation(shaderProgramLight, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        GLint viewLoc = glGetUniformLocation(shaderProgramLight, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        GLint projectionLoc = glGetUniformLocation(shaderProgramLight, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Draw the cube Light
        glBindVertexArray(VAOL);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // Update the model matrices for rotation
        glm::mat4 modelCube = glm::mat4(1.0f);
        modelCube = glm::rotate(modelCube, (float)glfwGetTime() * rotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 modelSphere = glm::mat4(1.0f);
        modelSphere = glm::translate(modelSphere, glm::vec3(0.0f, sphereOffset, 0.0f)); // Adjust y-offset

        glm::mat4 modelWallet1 = glm::mat4(1.0f);
        modelWallet1 = glm::translate(modelWallet1, glm::vec3(0.0f, 0.0f, -3.0f)); // Adjust the position of wallet1

        glm::mat4 modelWalletIn = glm::mat4(1.0f);
        modelWalletIn = glm::translate(modelWalletIn, glm::vec3(0.0f, 0.0f, -3.0f)); // Adjust the position of wallet1

        glm::mat4 modelCleaner = glm::mat4(1.0f);
        modelCleaner = glm::translate(modelCleaner, glm::vec3(0.0f, 0.0f, -3.0f)); // Adjust the position of wallet1

        glm::mat4 modelCleanerfr = glm::mat4(1.0f);
        modelCleanerfr = glm::translate(modelCleanerfr, glm::vec3(0.0f, 0.0f, -3.0f)); // Adjust the position of wallet1
        
        glm::mat4 modelBlackTop = glm::mat4(1.0f);
        modelBlackTop = glm::translate(modelBlackTop, glm::vec3(0.0f, 0.0f, -3.0f)); // Adjust the position of wallet1
        
        glm::mat4 modelPencil = glm::mat4(1.0f);
        modelPencil = glm::translate(modelPencil, glm::vec3(0.0f, 0, 0.0f)); // Adjust y-offset

        glm::mat4 modelPencil1 = glm::mat4(1.0f);
        modelPencil1 = glm::translate(modelPencil1, glm::vec3(0.0f, 0, 0.0f)); // Adjust y-offset

        glm::mat4 modelPencil2 = glm::mat4(1.0f);
        modelPencil2 = glm::translate(modelPencil2, glm::vec3(0.0f, 0, 0.0f)); // Adjust y-offset

        // Use main shader program
        glUseProgram(shaderProgram);

        // Set transformation matrices for cube
        glBindVertexArray(VAOCube);
        glUniformMatrix4fv(modelLocCube, 1, GL_FALSE, glm::value_ptr(modelCube));
        glUniformMatrix4fv(viewLocCube, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLocCube, 1, GL_FALSE, glm::value_ptr(projection));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture3);
        glDrawElements(GL_TRIANGLES, sizeof(cubeIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

        // Draw plane
        glBindVertexArray(VAOPlane);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture2);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw sphere
        glBindVertexArray(VAOSphere);
        glUniformMatrix4fv(modelLocSphere, 1, GL_FALSE, glm::value_ptr(modelSphere));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture4);
        glDrawElements(GL_TRIANGLES, 6 * 29 * 29, GL_UNSIGNED_INT, 0);
        
        // Draw Wallet1
        glBindVertexArray(VAOWallet1);
        glUniformMatrix4fv(modelLocWallet1, 1, GL_FALSE, glm::value_ptr(modelWallet1)); // Pass model matrix to wallet shader
        glUniformMatrix4fv(viewLocWallet1, 1, GL_FALSE, glm::value_ptr(view)); // Pass view matrix to wallet shader
        glUniformMatrix4fv(projectionLocWallet1, 1, GL_FALSE, glm::value_ptr(projection)); // Pass projection matrix to wallet shader
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture5);
        glDrawArrays(GL_TRIANGLES, 0, 40);
        glDrawElements(GL_TRIANGLES, sizeof(indicesForRect) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
        // Draw Walletin
        glBindVertexArray(VAOWalletIn);
        glUniformMatrix4fv(modelLocWalletIn, 1, GL_FALSE, glm::value_ptr(modelWalletIn)); // Pass model matrix to wallet shader
        glUniformMatrix4fv(viewLocWalletIn, 1, GL_FALSE, glm::value_ptr(view)); // Pass view matrix to wallet shader
        glUniformMatrix4fv(projectionLocWalletIn, 1, GL_FALSE, glm::value_ptr(projection)); // Pass projection matrix to wallet shader
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture6);
        glDrawArrays(GL_TRIANGLES, 0, 40);
        glDrawElements(GL_TRIANGLES, sizeof(indicesForRect) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

        // Draw Cleaner
        glBindVertexArray(VAOClean);
        glUniformMatrix4fv(modelLocClean, 1, GL_FALSE, glm::value_ptr(modelCleaner)); // Pass model matrix to wallet shader
        glUniformMatrix4fv(viewLocClean, 1, GL_FALSE, glm::value_ptr(view)); // Pass view matrix to wallet shader
        glUniformMatrix4fv(projectionLocClean, 1, GL_FALSE, glm::value_ptr(projection)); // Pass projection matrix to wallet shader
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture7);
        glDrawArrays(GL_TRIANGLES, 0, 40);
        glDrawElements(GL_TRIANGLES, sizeof(indicesForRect) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

        // Draw Cleanerfr
        glBindVertexArray(VAOCleanfr);
        glUniformMatrix4fv(modelLocCleanfr, 1, GL_FALSE, glm::value_ptr(modelCleanerfr)); // Pass model matrix to wallet shader
        glUniformMatrix4fv(viewLocCleanfr, 1, GL_FALSE, glm::value_ptr(view)); // Pass view matrix to wallet shader
        glUniformMatrix4fv(projectionLocCleanfr, 1, GL_FALSE, glm::value_ptr(projection)); // Pass projection matrix to wallet shader
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture8);
        glDrawArrays(GL_TRIANGLES, 0, 40);
        glDrawElements(GL_TRIANGLES, sizeof(indicesForRect) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

        // Draw Black Top
        glBindVertexArray(VAOBlackTop);
        glUniformMatrix4fv(modelLocBlackTop, 1, GL_FALSE, glm::value_ptr(modelCleanerfr)); // Pass model matrix to wallet shader
        glUniformMatrix4fv(viewLocBlackTop, 1, GL_FALSE, glm::value_ptr(view)); // Pass view matrix to wallet shader
        glUniformMatrix4fv(projectionLocBlackTop, 1, GL_FALSE, glm::value_ptr(projection)); // Pass projection matrix to wallet shader
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture3);
        glDrawArrays(GL_TRIANGLES, 0, 40);
        glDrawElements(GL_TRIANGLES, sizeof(indicesForRect) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

        // Draw Pencil0
        glBindVertexArray(VAOPenciBody0);
        glUniformMatrix4fv(modelLocPencil0, 1, GL_FALSE, glm::value_ptr(modelPencil)); // Pass model matrix to wallet shader
        glUniformMatrix4fv(viewLocPencil0, 1, GL_FALSE, glm::value_ptr(view)); // Pass view matrix to wallet shader
        glUniformMatrix4fv(projectionPencil0, 1, GL_FALSE, glm::value_ptr(projection)); // Pass projection matrix to wallet shader
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture9);
        glDrawArrays(GL_TRIANGLES, 0, 40);
        glDrawElements(GL_TRIANGLES, sizeof(indicesForRect) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

        // Draw Pencil1
        glBindVertexArray(VAOPenciBody1);
        glUniformMatrix4fv(modelLocPencil1, 1, GL_FALSE, glm::value_ptr(modelPencil1)); // Pass model matrix to wallet shader
        glUniformMatrix4fv(viewLocPencil1, 1, GL_FALSE, glm::value_ptr(view)); // Pass view matrix to wallet shader
        glUniformMatrix4fv(projectionPencil1, 1, GL_FALSE, glm::value_ptr(projection)); // Pass projection matrix to wallet shader
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture10);
        glDrawArrays(GL_TRIANGLES, 0, 40);
        glDrawElements(GL_TRIANGLES, sizeof(indicesForRect) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

        // Draw Pencil1
        glBindVertexArray(VAOPenciBody2);
        glUniformMatrix4fv(modelLocPencil2, 1, GL_FALSE, glm::value_ptr(modelPencil2)); // Pass model matrix to wallet shader
        glUniformMatrix4fv(viewLocPencil2, 1, GL_FALSE, glm::value_ptr(view)); // Pass view matrix to wallet shader
        glUniformMatrix4fv(projectionPencil2, 1, GL_FALSE, glm::value_ptr(projection)); // Pass projection matrix to wallet shader
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture11);
        glDrawArrays(GL_TRIANGLES, 0, 40);
        glDrawElements(GL_TRIANGLES, sizeof(indicesForRect) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Clean up
    glDeleteVertexArrays(1, &VAOCube);
    glDeleteBuffers(1, &VBOCube);
    glDeleteBuffers(1, &EBOCube);
    glDeleteVertexArrays(1, &VAOSphere);
    glDeleteBuffers(1, &VBOSphere);
    glDeleteBuffers(1, &EBOSphere);
    glDeleteVertexArrays(1, &VAOWallet1);
    glDeleteVertexArrays(1, &VAOWalletIn);
    glDeleteVertexArrays(1, &VAOClean);
    glDeleteVertexArrays(1, &VAOCleanfr);
    glDeleteVertexArrays(1, &VAOBlackTop);
    glDeleteBuffers(1, &VBOBlackTop);
    glDeleteBuffers(1, &EBOBlackTop);
    glDeleteBuffers(1, &VAOPenciBody0);
    glDeleteBuffers(1, &VAOPenciBody1);
    glDeleteBuffers(1, &VAOPenciBody2);
    glDeleteProgram(shaderProgram);
    glfwTerminate();

    return 0;
}