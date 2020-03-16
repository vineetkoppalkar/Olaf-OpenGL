#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <list>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs

#include <glm/glm.hpp>                  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>
#include <FreeImageIO.h>                // Used to load textures

//#include "SphereModel.h"

using namespace glm;
using namespace std;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processUserInput(GLFWwindow* window);
void drawGrid(int shaderProgram, GLuint worldMatrixLocation, GLuint colorLocation);
void drawGround(int shaderProgram, GLuint worldMatrixLocation);
void drawCoordinateAxis(int shaderProgram, GLuint worldMatrixLocation, GLuint colorLocation);
void drawOlaf(int shaderProgram, GLuint worldMatrixLocation, GLuint colorLocation, float lastFrameTime);
void drawCarrotNose(int shaderProgram, GLuint worldMatrixLocation);
void updateViewAndProjection(int shaderProgram);
void renderScene(int shaderProgram, GLuint modelMatrixLocation);
void renderQuad();

unsigned int testCubeVAO = 0;
unsigned int testCubeVBO = 0;
void renderCube()
{

    // initialize (if necessary)
    if (testCubeVAO == 0)
    {
        float vertices[] = {
            // back face
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
             1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
            -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
            -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
            // front face
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
             1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
            -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
            -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
            // left face
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
            -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
            // right face
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
             1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
             1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
             1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
            // bottom face
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
             1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
             1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
            -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
            -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
            // top face
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
             1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
             1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
             1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
            -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
            -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
        };
        glGenVertexArrays(1, &testCubeVAO);
        glGenBuffers(1, &testCubeVBO);
        // fill buffer
        glBindBuffer(GL_ARRAY_BUFFER, testCubeVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // link vertex attributes
        glBindVertexArray(testCubeVAO);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    // render Cube
    glBindVertexArray(testCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
}

// Screen sizes
int SCREEN_WIDTH = 1024;
int SCREEN_HEIGHT = 768;

// Grid parameters
int sizeOfGrid = 100;
bool showGrid = false;

// Camera parameters for view transform
vec3 cameraPosition(15.0f, 5.0f, 15.0f);
vec3 cameraLookAt = normalize(vec3(-15.0f, -5.0f, -15.0f));
vec3 cameraUp(0.0f, 1.0f, 0.0f);
vec3 cameraSideVector = cross(cameraLookAt, cameraUp);

// Other camera parameters
float camera_fov = 70.0f;
float dt = 0.0f;

// Camera movement
float cameraRotateX = 0.0f;
float cameraRotateY = 0.0f;
float cameraRotationSpeed = 5.0f;
float cameraRotationAmount = 1.0f;

float cameraHorizontalAngle = 180.0f - degrees(acos(dot(normalize(vec3(1, 0, 0)), normalize(cameraPosition))));
float cameraVerticalAngle = -degrees(acos(dot(normalize(vec3(1, 0, 1)), normalize(cameraPosition))));

float cameraPanningSpeed = 1.0f;
float cameraTiltingSpeed = 1.0f;
float cameraFovChangeSpeed = 0.01f;

// Cursor position
double lastMousePosX;
double lastMousePosY;

// Mouse button states
bool isLeftMouseBtnHeld = false;
bool isRightMouseBtnHeld = false;
bool isMiddleMouseBtnHeld = false;

// Controls
bool isHoldingShift = false;
bool moveForwardAndBack = false;
bool moveLeftRight = false;

// Olaf parameters
vec3 olafPosition(0.0f, -1.2f, 0.0f);
vec3 olafLookAt(1.0f, 0.0f, 0.0f);
vec3 olafUp(0.0f, 1.0f, 0.0f);
vec3 olafSideVector = cross(olafLookAt, olafUp);
mat4 olafNoseWorldMatrix;

// Olaf other parameters
float olafAngle = 90.0f;
float olafSpeed = 5.0f;
float olafScale = 0.6f;
float olafRotationAmount = 5.0f;
float olafScaleAmount = 0.05f;

// Rendering parameters
bool drawWithReversedFaceCulling = true;
GLenum renderMode = GL_TRIANGLE_STRIP;
int numTriangles = 1297;

// Lighting position
vec3 lightPos(-2.0f, 4.0f, -1.0f);
bool isCalculatingShadows = false;

// Colors
vec3 backgroundColor(0.2f, 0.298f, 0.298f);
vec3 yellow(1.0f, 1.0f, 0.0f);
vec3 red(1.0f, 0.0f, 0.0f);
vec3 blue(0.0f, 0.0f, 1.0f);
vec3 green(0.0f, 1.0f, 0.0f);
vec3 white(1.0f, 1.0f, 1.0f);
vec3 gray(0.62f, 0.62f, 0.62f);
vec3 black(0.01f, 0.01f, 0.01f);
vec3 orange(1.0f, 0.5f, 0.31f);
vec3 brown(0.5f, 0.38f, 0.38f);
vec3 darkBlue(0.06f, 0.22f, 0.54f);

// Textures
GLuint snowTextureID;
GLuint carrotTextureID;
GLuint brickTextureID;

// VAOs
int lineVAO;
int cubeVAO;
int sphereVAO;
int testVAO;
int lampVAO;
int texturedCubeVAO;
unsigned int planeVAO;


string loadShaderFile(const char* shaderPath)
{
    string shaderCode;
    ifstream shaderFile;
    shaderFile.exceptions(ifstream::failbit | ifstream::badbit);

    try
    {
        shaderFile.open(shaderPath);
        stringstream shaderStream;

        // Read the file contents
        shaderStream << shaderFile.rdbuf();

        // Close file handlers
        shaderFile.close();

        // Convert stream to string
        shaderCode = shaderStream.str();
    }
    catch (ifstream::failure e)
    {
        cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
    }

    return shaderCode;
}
int compileAndLinkShaders(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    // compile and link shader program
    // return shader program id
    // ------------------------------------

    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //const char* vertexShaderSource = getVertexShaderSource();
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    //const char* fragmentShaderSource = getFragmentShaderSource();
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int createTestVAO()
{
    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return cubeVAO;

}
int createLineSegmentVertexArrayObject()
{
    vec3 vertexArray[] = {
        // position
        vec3(0.0f, 0.0f, 0.0f),
        vec3(1.0f, 0.0f, 0.0f),
    };

    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);


    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        sizeof(vec3),        // stride - each vertex contain 1 vec3 (position)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);

    return vertexArrayObject;
}
int createTexturedCubeVertexArrayObject()
{
    // Cube model w/ position and texture 
    float vertexArray[] = {
        -0.5f,-0.5f,-0.5f, 0.0f, 0.0f,
        -0.5f,-0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,

        -0.5f,-0.5f,-0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f,-0.5f, 1.0f, 0.0f,

        0.5f, 0.5f,-0.5f,  1.0f, 1.0f,
        -0.5f,-0.5f,-0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f,-0.5f, 0.0f, 1.0f,

        0.5f, 0.5f,-0.5f,  1.0f, 1.0f,
        0.5f,-0.5f,-0.5f,  1.0f, 0.0f,
        -0.5f,-0.5f,-0.5f, 0.0f, 0.0f,

        0.5f,-0.5f, 0.5f,  1.0f, 1.0f,
        -0.5f,-0.5f,-0.5f, 0.0f, 0.0f,
        0.5f,-0.5f,-0.5f,  1.0f, 0.0f,

        0.5f,-0.5f, 0.5f,  1.0f, 1.0f,
        -0.5f,-0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f,-0.5f,-0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f,-0.5f, 0.5f, 0.0f, 0.0f,
        0.5f,-0.5f, 0.5f,  1.0f, 0.0f,

        0.5f, 0.5f, 0.5f,  1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        0.5f,-0.5f, 0.5f,  1.0f, 0.0f,

        0.5f, 0.5f, 0.5f,  1.0f, 1.0f,
        0.5f,-0.5f,-0.5f,  0.0f, 0.0f,
        0.5f, 0.5f,-0.5f,  1.0f, 0.0f,

        0.5f,-0.5f,-0.5f,  0.0f, 0.0f,
        0.5f, 0.5f, 0.5f,  1.0f, 1.0f,
        0.5f,-0.5f, 0.5f,  0.0f, 1.0f,

        0.5f, 0.5f, 0.5f,  1.0f, 1.0f,
        0.5f, 0.5f,-0.5f,  1.0f, 0.0f,
        -0.5f, 0.5f,-0.5f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f,  1.0f, 1.0f,
        -0.5f, 0.5f,-0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f
    };

    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);


    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        5 * sizeof(float),   // stride - each vertex contain 1 vec3 (position)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,                            // attribute 0 matches aPos in Vertex Shader
        2,                            // size
        GL_FLOAT,                     // type
        GL_FALSE,                     // normalized?
        5 * sizeof(float),            // stride - each vertex contain 1 vec3 (position)
        (void*)(3 * sizeof(float))    // array buffer offset
    );
    glEnableVertexAttribArray(1);

    return vertexArrayObject;
}
int createCubeVertexArrayObject()
{
    // Cube model w/ position
    vec3 vertexArray[] = {
        // position
        vec3(-0.5f,-0.5f,-0.5f),
        vec3(-0.5f,-0.5f, 0.5f),
        vec3(-0.5f, 0.5f, 0.5f),

        vec3(-0.5f,-0.5f,-0.5f),
        vec3(-0.5f, 0.5f, 0.5f),
        vec3(-0.5f, 0.5f,-0.5f),

        vec3(0.5f, 0.5f,-0.5f),
        vec3(-0.5f,-0.5f,-0.5f),
        vec3(-0.5f, 0.5f,-0.5f),

        vec3(0.5f, 0.5f,-0.5f),
        vec3(0.5f,-0.5f,-0.5f),
        vec3(-0.5f,-0.5f,-0.5f),

        vec3(0.5f,-0.5f, 0.5f),
        vec3(-0.5f,-0.5f,-0.5f),
        vec3(0.5f,-0.5f,-0.5f),

        vec3(0.5f,-0.5f, 0.5f),
        vec3(-0.5f,-0.5f, 0.5f),
        vec3(-0.5f,-0.5f,-0.5f),

        vec3(-0.5f, 0.5f, 0.5f),
        vec3(-0.5f,-0.5f, 0.5f),
        vec3(0.5f,-0.5f, 0.5f),

        vec3(0.5f, 0.5f, 0.5f),
        vec3(-0.5f, 0.5f, 0.5f),
        vec3(0.5f,-0.5f, 0.5f),

        vec3(0.5f, 0.5f, 0.5f),
        vec3(0.5f,-0.5f,-0.5f),
        vec3(0.5f, 0.5f,-0.5f),

        vec3(0.5f,-0.5f,-0.5f),
        vec3(0.5f, 0.5f, 0.5f),
        vec3(0.5f,-0.5f, 0.5f),

        vec3(0.5f, 0.5f, 0.5f),
        vec3(0.5f, 0.5f,-0.5f),
        vec3(-0.5f, 0.5f,-0.5f),

        vec3(0.5f, 0.5f, 0.5f),
        vec3(-0.5f, 0.5f,-0.5f),
        vec3(-0.5f, 0.5f, 0.5f)
    };

    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);


    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        sizeof(vec3),        // stride - each vertex contain 1 vec3 (position)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);

    return vertexArrayObject;
}
int createSphereVertexArrayObject()
{
    // Sphere data position
    /*vec3 vertexArray[] = {
        vec3(0.000000, 0.000000, -1.000000),
        vec3(0.173648, 0.000000, -0.984808),
        vec3(0.171010, 0.030154, -0.984808),
        vec3(0.342020, 0.000000, -0.939693),
        vec3(0.336824, 0.059391, -0.939693),
        vec3(0.500000, 0.000000, -0.866025),
        vec3(0.492404, 0.086824, -0.866025),
        vec3(0.642788, 0.000000, -0.766044),
        vec3(0.633022, 0.111619, -0.766044),
        vec3(0.766044, 0.000000, -0.642788),
        vec3(0.754407, 0.133022, -0.642788),
        vec3(0.866025, 0.000000, -0.500000),
        vec3(0.852869, 0.150384, -0.500000),
        vec3(0.939693, 0.000000, -0.342020),
        vec3(0.925417, 0.163176, -0.342020),
        vec3(0.984808, 0.000000, -0.173648),
        vec3(0.969846, 0.171010, -0.173648),
        vec3(1.000000, 0.000000, 0.000000),
        vec3(0.984808, 0.173648, 0.000000),
        vec3(0.984808, 0.000000, 0.173648),
        vec3(0.969846, 0.171010, 0.173648),
        vec3(0.939693, 0.000000, 0.342020),
        vec3(0.925417, 0.163176, 0.342020),
        vec3(0.866025, 0.000000, 0.500000),
        vec3(0.852869, 0.150384, 0.500000),
        vec3(0.766044, 0.000000, 0.642788),
        vec3(0.754407, 0.133022, 0.642788),
        vec3(0.642788, 0.000000, 0.766044),
        vec3(0.633022, 0.111619, 0.766044),
        vec3(0.500000, 0.000000, 0.866025),
        vec3(0.492404, 0.086824, 0.866025),
        vec3(0.342020, 0.000000, 0.939693),
        vec3(0.336824, 0.059391, 0.939693),
        vec3(0.173648, 0.000000, 0.984808),
        vec3(0.171010, 0.030154, 0.984808),
        vec3(0.000000, 0.000000, 1.000000),
        vec3(0.171010, 0.030154, 0.984808),
        vec3(0.163176, 0.059391, 0.984808),
        vec3(0.336824, 0.059391, 0.939693),
        vec3(0.321394, 0.116978, 0.939693),
        vec3(0.492404, 0.086824, 0.866025),
        vec3(0.469846, 0.171010, 0.866025),
        vec3(0.633022, 0.111619, 0.766044),
        vec3(0.604023, 0.219846, 0.766044),
        vec3(0.754407, 0.133022, 0.642788),
        vec3(0.719846, 0.262003, 0.642788),
        vec3(0.852869, 0.150384, 0.500000),
        vec3(0.813798, 0.296198, 0.500000),
        vec3(0.925417, 0.163176, 0.342020),
        vec3(0.883022, 0.321394, 0.342020),
        vec3(0.969846, 0.171010, 0.173648),
        vec3(0.925417, 0.336824, 0.173648),
        vec3(0.984808, 0.173648, 0.000000),
        vec3(0.939693, 0.342020, 0.000000),
        vec3(0.969846, 0.171010, -0.173648),
        vec3(0.925417, 0.336824, -0.173648),
        vec3(0.925417, 0.163176, -0.342020),
        vec3(0.883022, 0.321394, -0.342020),
        vec3(0.852869, 0.150384, -0.500000),
        vec3(0.813798, 0.296198, -0.500000),
        vec3(0.754407, 0.133022, -0.642788),
        vec3(0.719846, 0.262003, -0.642788),
        vec3(0.633022, 0.111619, -0.766044),
        vec3(0.604023, 0.219846, -0.766044),
        vec3(0.492404, 0.086824, -0.866025),
        vec3(0.469846, 0.171010, -0.866025),
        vec3(0.336824, 0.059391, -0.939693),
        vec3(0.321394, 0.116978, -0.939693),
        vec3(0.171010, 0.030154, -0.984808),
        vec3(0.163176, 0.059391, -0.984808),
        vec3(0.000000, 0.000000, -1.000000),
        vec3(0.163176, 0.059391, -0.984808),
        vec3(0.150384, 0.086824, -0.984808),
        vec3(0.321394, 0.116978, -0.939693),
        vec3(0.296198, 0.171010, -0.939693),
        vec3(0.469846, 0.171010, -0.866025),
        vec3(0.433013, 0.250000, -0.866025),
        vec3(0.604023, 0.219846, -0.766044),
        vec3(0.556670, 0.321394, -0.766044),
        vec3(0.719846, 0.262003, -0.642788),
        vec3(0.663414, 0.383022, -0.642788),
        vec3(0.813798, 0.296198, -0.500000),
        vec3(0.750000, 0.433013, -0.500000),
        vec3(0.883022, 0.321394, -0.342020),
        vec3(0.813798, 0.469846, -0.342020),
        vec3(0.925417, 0.336824, -0.173648),
        vec3(0.852869, 0.492404, -0.173648),
        vec3(0.939693, 0.342020, 0.000000),
        vec3(0.866025, 0.500000, 0.000000),
        vec3(0.925417, 0.336824, 0.173648),
        vec3(0.852869, 0.492404, 0.173648),
        vec3(0.883022, 0.321394, 0.342020),
        vec3(0.813798, 0.469846, 0.342020),
        vec3(0.813798, 0.296198, 0.500000),
        vec3(0.750000, 0.433013, 0.500000),
        vec3(0.719846, 0.262003, 0.642788),
        vec3(0.663414, 0.383022, 0.642788),
        vec3(0.604023, 0.219846, 0.766044),
        vec3(0.556670, 0.321394, 0.766044),
        vec3(0.469846, 0.171010, 0.866025),
        vec3(0.433013, 0.250000, 0.866025),
        vec3(0.321394, 0.116978, 0.939693),
        vec3(0.296198, 0.171010, 0.939693),
        vec3(0.163176, 0.059391, 0.984808),
        vec3(0.150384, 0.086824, 0.984808),
        vec3(0.000000, 0.000000, 1.000000),
        vec3(0.150384, 0.086824, 0.984808),
        vec3(0.133022, 0.111619, 0.984808),
        vec3(0.296198, 0.171010, 0.939693),
        vec3(0.262003, 0.219846, 0.939693),
        vec3(0.433013, 0.250000, 0.866025),
        vec3(0.383022, 0.321394, 0.866025),
        vec3(0.556670, 0.321394, 0.766044),
        vec3(0.492404, 0.413176, 0.766044),
        vec3(0.663414, 0.383022, 0.642788),
        vec3(0.586824, 0.492404, 0.642788),
        vec3(0.750000, 0.433013, 0.500000),
        vec3(0.663414, 0.556670, 0.500000),
        vec3(0.813798, 0.469846, 0.342020),
        vec3(0.719846, 0.604023, 0.342020),
        vec3(0.852869, 0.492404, 0.173648),
        vec3(0.754407, 0.633022, 0.173648),
        vec3(0.866025, 0.500000, 0.000000),
        vec3(0.766044, 0.642788, 0.000000),
        vec3(0.852869, 0.492404, -0.173648),
        vec3(0.754407, 0.633022, -0.173648),
        vec3(0.813798, 0.469846, -0.342020),
        vec3(0.719846, 0.604023, -0.342020),
        vec3(0.750000, 0.433013, -0.500000),
        vec3(0.663414, 0.556670, -0.500000),
        vec3(0.663414, 0.383022, -0.642788),
        vec3(0.586824, 0.492404, -0.642788),
        vec3(0.556670, 0.321394, -0.766044),
        vec3(0.492404, 0.413176, -0.766044),
        vec3(0.433013, 0.250000, -0.866025),
        vec3(0.383022, 0.321394, -0.866025),
        vec3(0.296198, 0.171010, -0.939693),
        vec3(0.262003, 0.219846, -0.939693),
        vec3(0.150384, 0.086824, -0.984808),
        vec3(0.133022, 0.111619, -0.984808),
        vec3(0.000000, 0.000000, -1.000000),
        vec3(0.133022, 0.111619, -0.984808),
        vec3(0.111619, 0.133022, -0.984808),
        vec3(0.262003, 0.219846, -0.939693),
        vec3(0.219846, 0.262003, -0.939693),
        vec3(0.383022, 0.321394, -0.866025),
        vec3(0.321394, 0.383022, -0.866025),
        vec3(0.492404, 0.413176, -0.766044),
        vec3(0.413176, 0.492404, -0.766044),
        vec3(0.586824, 0.492404, -0.642788),
        vec3(0.492404, 0.586824, -0.642788),
        vec3(0.663414, 0.556670, -0.500000),
        vec3(0.556670, 0.663414, -0.500000),
        vec3(0.719846, 0.604023, -0.342020),
        vec3(0.604023, 0.719846, -0.342020),
        vec3(0.754407, 0.633022, -0.173648),
        vec3(0.633022, 0.754407, -0.173648),
        vec3(0.766044, 0.642788, 0.000000),
        vec3(0.642788, 0.766044, 0.000000),
        vec3(0.754407, 0.633022, 0.173648),
        vec3(0.633022, 0.754407, 0.173648),
        vec3(0.719846, 0.604023, 0.342020),
        vec3(0.604023, 0.719846, 0.342020),
        vec3(0.663414, 0.556670, 0.500000),
        vec3(0.556670, 0.663414, 0.500000),
        vec3(0.586824, 0.492404, 0.642788),
        vec3(0.492404, 0.586824, 0.642788),
        vec3(0.492404, 0.413176, 0.766044),
        vec3(0.413176, 0.492404, 0.766044),
        vec3(0.383022, 0.321394, 0.866025),
        vec3(0.321394, 0.383022, 0.866025),
        vec3(0.262003, 0.219846, 0.939693),
        vec3(0.219846, 0.262003, 0.939693),
        vec3(0.133022, 0.111619, 0.984808),
        vec3(0.111619, 0.133022, 0.984808),
        vec3(0.000000, 0.000000, 1.000000),
        vec3(0.111619, 0.133022, 0.984808),
        vec3(0.086824, 0.150384, 0.984808),
        vec3(0.219846, 0.262003, 0.939693),
        vec3(0.171010, 0.296198, 0.939693),
        vec3(0.321394, 0.383022, 0.866025),
        vec3(0.250000, 0.433013, 0.866025),
        vec3(0.413176, 0.492404, 0.766044),
        vec3(0.321394, 0.556670, 0.766044),
        vec3(0.492404, 0.586824, 0.642788),
        vec3(0.383022, 0.663414, 0.642788),
        vec3(0.556670, 0.663414, 0.500000),
        vec3(0.433013, 0.750000, 0.500000),
        vec3(0.604023, 0.719846, 0.342020),
        vec3(0.469846, 0.813798, 0.342020),
        vec3(0.633022, 0.754407, 0.173648),
        vec3(0.492404, 0.852869, 0.173648),
        vec3(0.642788, 0.766044, 0.000000),
        vec3(0.500000, 0.866025, 0.000000),
        vec3(0.633022, 0.754407, -0.173648),
        vec3(0.492404, 0.852869, -0.173648),
        vec3(0.604023, 0.719846, -0.342020),
        vec3(0.469846, 0.813798, -0.342020),
        vec3(0.556670, 0.663414, -0.500000),
        vec3(0.433013, 0.750000, -0.500000),
        vec3(0.492404, 0.586824, -0.642788),
        vec3(0.383022, 0.663414, -0.642788),
        vec3(0.413176, 0.492404, -0.766044),
        vec3(0.321394, 0.556670, -0.766044),
        vec3(0.321394, 0.383022, -0.866025),
        vec3(0.250000, 0.433013, -0.866025),
        vec3(0.219846, 0.262003, -0.939693),
        vec3(0.171010, 0.296198, -0.939693),
        vec3(0.111619, 0.133022, -0.984808),
        vec3(0.086824, 0.150384, -0.984808),
        vec3(0.000000, 0.000000, -1.000000),
        vec3(0.086824, 0.150384, -0.984808),
        vec3(0.059391, 0.163176, -0.984808),
        vec3(0.171010, 0.296198, -0.939693),
        vec3(0.116978, 0.321394, -0.939693),
        vec3(0.250000, 0.433013, -0.866025),
        vec3(0.171010, 0.469846, -0.866025),
        vec3(0.321394, 0.556670, -0.766044),
        vec3(0.219846, 0.604023, -0.766044),
        vec3(0.383022, 0.663414, -0.642788),
        vec3(0.262003, 0.719846, -0.642788),
        vec3(0.433013, 0.750000, -0.500000),
        vec3(0.296198, 0.813798, -0.500000),
        vec3(0.469846, 0.813798, -0.342020),
        vec3(0.321394, 0.883022, -0.342020),
        vec3(0.492404, 0.852869, -0.173648),
        vec3(0.336824, 0.925417, -0.173648),
        vec3(0.500000, 0.866025, 0.000000),
        vec3(0.342020, 0.939693, 0.000000),
        vec3(0.492404, 0.852869, 0.173648),
        vec3(0.336824, 0.925417, 0.173648),
        vec3(0.469846, 0.813798, 0.342020),
        vec3(0.321394, 0.883022, 0.342020),
        vec3(0.433013, 0.750000, 0.500000),
        vec3(0.296198, 0.813798, 0.500000),
        vec3(0.383022, 0.663414, 0.642788),
        vec3(0.262003, 0.719846, 0.642788),
        vec3(0.321394, 0.556670, 0.766044),
        vec3(0.219846, 0.604023, 0.766044),
        vec3(0.250000, 0.433013, 0.866025),
        vec3(0.171010, 0.469846, 0.866025),
        vec3(0.171010, 0.296198, 0.939693),
        vec3(0.116978, 0.321394, 0.939693),
        vec3(0.086824, 0.150384, 0.984808),
        vec3(0.059391, 0.163176, 0.984808),
        vec3(0.000000, 0.000000, 1.000000),
        vec3(0.059391, 0.163176, 0.984808),
        vec3(0.030154, 0.171010, 0.984808),
        vec3(0.116978, 0.321394, 0.939693),
        vec3(0.059391, 0.336824, 0.939693),
        vec3(0.171010, 0.469846, 0.866025),
        vec3(0.086824, 0.492404, 0.866025),
        vec3(0.219846, 0.604023, 0.766044),
        vec3(0.111619, 0.633022, 0.766044),
        vec3(0.262003, 0.719846, 0.642788),
        vec3(0.133022, 0.754407, 0.642788),
        vec3(0.296198, 0.813798, 0.500000),
        vec3(0.150384, 0.852869, 0.500000),
        vec3(0.321394, 0.883022, 0.342020),
        vec3(0.163176, 0.925417, 0.342020),
        vec3(0.336824, 0.925417, 0.173648),
        vec3(0.171010, 0.969846, 0.173648),
        vec3(0.342020, 0.939693, 0.000000),
        vec3(0.173648, 0.984808, 0.000000),
        vec3(0.336824, 0.925417, -0.173648),
        vec3(0.171010, 0.969846, -0.173648),
        vec3(0.321394, 0.883022, -0.342020),
        vec3(0.163176, 0.925417, -0.342020),
        vec3(0.296198, 0.813798, -0.500000),
        vec3(0.150384, 0.852869, -0.500000),
        vec3(0.262003, 0.719846, -0.642788),
        vec3(0.133022, 0.754407, -0.642788),
        vec3(0.219846, 0.604023, -0.766044),
        vec3(0.111619, 0.633022, -0.766044),
        vec3(0.171010, 0.469846, -0.866025),
        vec3(0.086824, 0.492404, -0.866025),
        vec3(0.116978, 0.321394, -0.939693),
        vec3(0.059391, 0.336824, -0.939693),
        vec3(0.059391, 0.163176, -0.984808),
        vec3(0.030154, 0.171010, -0.984808),
        vec3(0.000000, 0.000000, -1.000000),
        vec3(0.030154, 0.171010, -0.984808),
        vec3(0.000000, 0.173648, -0.984808),
        vec3(0.059391, 0.336824, -0.939693),
        vec3(0.000000, 0.342020, -0.939693),
        vec3(0.086824, 0.492404, -0.866025),
        vec3(0.000000, 0.500000, -0.866025),
        vec3(0.111619, 0.633022, -0.766044),
        vec3(0.000000, 0.642788, -0.766044),
        vec3(0.133022, 0.754407, -0.642788),
        vec3(0.000000, 0.766044, -0.642788),
        vec3(0.150384, 0.852869, -0.500000),
        vec3(0.000000, 0.866025, -0.500000),
        vec3(0.163176, 0.925417, -0.342020),
        vec3(0.000000, 0.939693, -0.342020),
        vec3(0.171010, 0.969846, -0.173648),
        vec3(0.000000, 0.984808, -0.173648),
        vec3(0.173648, 0.984808, 0.000000),
        vec3(0.000000, 1.000000, 0.000000),
        vec3(0.171010, 0.969846, 0.173648),
        vec3(0.000000, 0.984808, 0.173648),
        vec3(0.163176, 0.925417, 0.342020),
        vec3(0.000000, 0.939693, 0.342020),
        vec3(0.150384, 0.852869, 0.500000),
        vec3(0.000000, 0.866025, 0.500000),
        vec3(0.133022, 0.754407, 0.642788),
        vec3(0.000000, 0.766044, 0.642788),
        vec3(0.111619, 0.633022, 0.766044),
        vec3(0.000000, 0.642788, 0.766044),
        vec3(0.086824, 0.492404, 0.866025),
        vec3(0.000000, 0.500000, 0.866025),
        vec3(0.059391, 0.336824, 0.939693),
        vec3(0.000000, 0.342020, 0.939693),
        vec3(0.030154, 0.171010, 0.984808),
        vec3(0.000000, 0.173648, 0.984808),
        vec3(0.000000, 0.000000, 1.000000),
        vec3(0.000000, 0.173648, 0.984808),
        vec3(-0.030154, 0.171010, 0.984808),
        vec3(0.000000, 0.342020, 0.939693),
        vec3(-0.059391, 0.336824, 0.939693),
        vec3(0.000000, 0.500000, 0.866025),
        vec3(-0.086824, 0.492404, 0.866025),
        vec3(0.000000, 0.642788, 0.766044),
        vec3(-0.111619, 0.633022, 0.766044),
        vec3(0.000000, 0.766044, 0.642788),
        vec3(-0.133022, 0.754407, 0.642788),
        vec3(0.000000, 0.866025, 0.500000),
        vec3(-0.150384, 0.852869, 0.500000),
        vec3(0.000000, 0.939693, 0.342020),
        vec3(-0.163176, 0.925417, 0.342020),
        vec3(0.000000, 0.984808, 0.173648),
        vec3(-0.171010, 0.969846, 0.173648),
        vec3(0.000000, 1.000000, 0.000000),
        vec3(-0.173648, 0.984808, 0.000000),
        vec3(0.000000, 0.984808, -0.173648),
        vec3(-0.171010, 0.969846, -0.173648),
        vec3(0.000000, 0.939693, -0.342020),
        vec3(-0.163176, 0.925417, -0.342020),
        vec3(0.000000, 0.866025, -0.500000),
        vec3(-0.150384, 0.852869, -0.500000),
        vec3(0.000000, 0.766044, -0.642788),
        vec3(-0.133022, 0.754407, -0.642788),
        vec3(0.000000, 0.642788, -0.766044),
        vec3(-0.111619, 0.633022, -0.766044),
        vec3(0.000000, 0.500000, -0.866025),
        vec3(-0.086824, 0.492404, -0.866025),
        vec3(0.000000, 0.342020, -0.939693),
        vec3(-0.059391, 0.336824, -0.939693),
        vec3(0.000000, 0.173648, -0.984808),
        vec3(-0.030154, 0.171010, -0.984808),
        vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.030154, 0.171010, -0.984808),
        vec3(-0.059391, 0.163176, -0.984808),
        vec3(-0.059391, 0.336824, -0.939693),
        vec3(-0.116978, 0.321394, -0.939693),
        vec3(-0.086824, 0.492404, -0.866025),
        vec3(-0.171010, 0.469846, -0.866025),
        vec3(-0.111619, 0.633022, -0.766044),
        vec3(-0.219846, 0.604023, -0.766044),
        vec3(-0.133022, 0.754407, -0.642788),
        vec3(-0.262003, 0.719846, -0.642788),
        vec3(-0.150384, 0.852869, -0.500000),
        vec3(-0.296198, 0.813798, -0.500000),
        vec3(-0.163176, 0.925417, -0.342020),
        vec3(-0.321394, 0.883022, -0.342020),
        vec3(-0.171010, 0.969846, -0.173648),
        vec3(-0.336824, 0.925417, -0.173648),
        vec3(-0.173648, 0.984808, 0.000000),
        vec3(-0.342020, 0.939693, 0.000000),
        vec3(-0.171010, 0.969846, 0.173648),
        vec3(-0.336824, 0.925417, 0.173648),
        vec3(-0.163176, 0.925417, 0.342020),
        vec3(-0.321394, 0.883022, 0.342020),
        vec3(-0.150384, 0.852869, 0.500000),
        vec3(-0.296198, 0.813798, 0.500000),
        vec3(-0.133022, 0.754407, 0.642788),
        vec3(-0.262003, 0.719846, 0.642788),
        vec3(-0.111619, 0.633022, 0.766044),
        vec3(-0.219846, 0.604023, 0.766044),
        vec3(-0.086824, 0.492404, 0.866025),
        vec3(-0.171010, 0.469846, 0.866025),
        vec3(-0.059391, 0.336824, 0.939693),
        vec3(-0.116978, 0.321394, 0.939693),
        vec3(-0.030154, 0.171010, 0.984808),
        vec3(-0.059391, 0.163176, 0.984808),
        vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.059391, 0.163176, 0.984808),
        vec3(-0.086824, 0.150384, 0.984808),
        vec3(-0.116978, 0.321394, 0.939693),
        vec3(-0.171010, 0.296198, 0.939693),
        vec3(-0.171010, 0.469846, 0.866025),
        vec3(-0.250000, 0.433013, 0.866025),
        vec3(-0.219846, 0.604023, 0.766044),
        vec3(-0.321394, 0.556670, 0.766044),
        vec3(-0.262003, 0.719846, 0.642788),
        vec3(-0.383022, 0.663414, 0.642788),
        vec3(-0.296198, 0.813798, 0.500000),
        vec3(-0.433013, 0.750000, 0.500000),
        vec3(-0.321394, 0.883022, 0.342020),
        vec3(-0.469846, 0.813798, 0.342020),
        vec3(-0.336824, 0.925417, 0.173648),
        vec3(-0.492404, 0.852869, 0.173648),
        vec3(-0.342020, 0.939693, 0.000000),
        vec3(-0.500000, 0.866025, 0.000000),
        vec3(-0.336824, 0.925417, -0.173648),
        vec3(-0.492404, 0.852869, -0.173648),
        vec3(-0.321394, 0.883022, -0.342020),
        vec3(-0.469846, 0.813798, -0.342020),
        vec3(-0.296198, 0.813798, -0.500000),
        vec3(-0.433013, 0.750000, -0.500000),
        vec3(-0.262003, 0.719846, -0.642788),
        vec3(-0.383022, 0.663414, -0.642788),
        vec3(-0.219846, 0.604023, -0.766044),
        vec3(-0.321394, 0.556670, -0.766044),
        vec3(-0.171010, 0.469846, -0.866025),
        vec3(-0.250000, 0.433013, -0.866025),
        vec3(-0.116978, 0.321394, -0.939693),
        vec3(-0.171010, 0.296198, -0.939693),
        vec3(-0.059391, 0.163176, -0.984808),
        vec3(-0.086824, 0.150384, -0.984808),
        vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.086824, 0.150384, -0.984808),
        vec3(-0.111619, 0.133022, -0.984808),
        vec3(-0.171010, 0.296198, -0.939693),
        vec3(-0.219846, 0.262003, -0.939693),
        vec3(-0.250000, 0.433013, -0.866025),
        vec3(-0.321394, 0.383022, -0.866025),
        vec3(-0.321394, 0.556670, -0.766044),
        vec3(-0.413176, 0.492404, -0.766044),
        vec3(-0.383022, 0.663414, -0.642788),
        vec3(-0.492404, 0.586824, -0.642788),
        vec3(-0.433013, 0.750000, -0.500000),
        vec3(-0.556670, 0.663414, -0.500000),
        vec3(-0.469846, 0.813798, -0.342020),
        vec3(-0.604023, 0.719846, -0.342020),
        vec3(-0.492404, 0.852869, -0.173648),
        vec3(-0.633022, 0.754407, -0.173648),
        vec3(-0.500000, 0.866025, 0.000000),
        vec3(-0.642788, 0.766044, 0.000000),
        vec3(-0.492404, 0.852869, 0.173648),
        vec3(-0.633022, 0.754407, 0.173648),
        vec3(-0.469846, 0.813798, 0.342020),
        vec3(-0.604023, 0.719846, 0.342020),
        vec3(-0.433013, 0.750000, 0.500000),
        vec3(-0.556670, 0.663414, 0.500000),
        vec3(-0.383022, 0.663414, 0.642788),
        vec3(-0.492404, 0.586824, 0.642788),
        vec3(-0.321394, 0.556670, 0.766044),
        vec3(-0.413176, 0.492404, 0.766044),
        vec3(-0.250000, 0.433013, 0.866025),
        vec3(-0.321394, 0.383022, 0.866025),
        vec3(-0.171010, 0.296198, 0.939693),
        vec3(-0.219846, 0.262003, 0.939693),
        vec3(-0.086824, 0.150384, 0.984808),
        vec3(-0.111619, 0.133022, 0.984808),
        vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.111619, 0.133022, 0.984808),
        vec3(-0.133022, 0.111619, 0.984808),
        vec3(-0.219846, 0.262003, 0.939693),
        vec3(-0.262003, 0.219846, 0.939693),
        vec3(-0.321394, 0.383022, 0.866025),
        vec3(-0.383022, 0.321394, 0.866025),
        vec3(-0.413176, 0.492404, 0.766044),
        vec3(-0.492404, 0.413176, 0.766044),
        vec3(-0.492404, 0.586824, 0.642788),
        vec3(-0.586824, 0.492404, 0.642788),
        vec3(-0.556670, 0.663414, 0.500000),
        vec3(-0.663414, 0.556670, 0.500000),
        vec3(-0.604023, 0.719846, 0.342020),
        vec3(-0.719846, 0.604023, 0.342020),
        vec3(-0.633022, 0.754407, 0.173648),
        vec3(-0.754407, 0.633022, 0.173648),
        vec3(-0.642788, 0.766044, 0.000000),
        vec3(-0.766044, 0.642788, 0.000000),
        vec3(-0.633022, 0.754407, -0.173648),
        vec3(-0.754407, 0.633022, -0.173648),
        vec3(-0.604023, 0.719846, -0.342020),
        vec3(-0.719846, 0.604023, -0.342020),
        vec3(-0.556670, 0.663414, -0.500000),
        vec3(-0.663414, 0.556670, -0.500000),
        vec3(-0.492404, 0.586824, -0.642788),
        vec3(-0.586824, 0.492404, -0.642788),
        vec3(-0.413176, 0.492404, -0.766044),
        vec3(-0.492404, 0.413176, -0.766044),
        vec3(-0.321394, 0.383022, -0.866025),
        vec3(-0.383022, 0.321394, -0.866025),
        vec3(-0.219846, 0.262003, -0.939693),
        vec3(-0.262003, 0.219846, -0.939693),
        vec3(-0.111619, 0.133022, -0.984808),
        vec3(-0.133022, 0.111619, -0.984808),
        vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.133022, 0.111619, -0.984808),
        vec3(-0.150384, 0.086824, -0.984808),
        vec3(-0.262003, 0.219846, -0.939693),
        vec3(-0.296198, 0.171010, -0.939693),
        vec3(-0.383022, 0.321394, -0.866025),
        vec3(-0.433013, 0.250000, -0.866025),
        vec3(-0.492404, 0.413176, -0.766044),
        vec3(-0.556670, 0.321394, -0.766044),
        vec3(-0.586824, 0.492404, -0.642788),
        vec3(-0.663414, 0.383022, -0.642788),
        vec3(-0.663414, 0.556670, -0.500000),
        vec3(-0.750000, 0.433013, -0.500000),
        vec3(-0.719846, 0.604023, -0.342020),
        vec3(-0.813798, 0.469846, -0.342020),
        vec3(-0.754407, 0.633022, -0.173648),
        vec3(-0.852869, 0.492404, -0.173648),
        vec3(-0.766044, 0.642788, 0.000000),
        vec3(-0.866025, 0.500000, 0.000000),
        vec3(-0.754407, 0.633022, 0.173648),
        vec3(-0.852869, 0.492404, 0.173648),
        vec3(-0.719846, 0.604023, 0.342020),
        vec3(-0.813798, 0.469846, 0.342020),
        vec3(-0.663414, 0.556670, 0.500000),
        vec3(-0.750000, 0.433013, 0.500000),
        vec3(-0.586824, 0.492404, 0.642788),
        vec3(-0.663414, 0.383022, 0.642788),
        vec3(-0.492404, 0.413176, 0.766044),
        vec3(-0.556670, 0.321394, 0.766044),
        vec3(-0.383022, 0.321394, 0.866025),
        vec3(-0.433013, 0.250000, 0.866025),
        vec3(-0.262003, 0.219846, 0.939693),
        vec3(-0.296198, 0.171010, 0.939693),
        vec3(-0.133022, 0.111619, 0.984808),
        vec3(-0.150384, 0.086824, 0.984808),
        vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.150384, 0.086824, 0.984808),
        vec3(-0.163176, 0.059391, 0.984808),
        vec3(-0.296198, 0.171010, 0.939693),
        vec3(-0.321394, 0.116978, 0.939693),
        vec3(-0.433013, 0.250000, 0.866025),
        vec3(-0.469846, 0.171010, 0.866025),
        vec3(-0.556670, 0.321394, 0.766044),
        vec3(-0.604023, 0.219846, 0.766044),
        vec3(-0.663414, 0.383022, 0.642788),
        vec3(-0.719846, 0.262003, 0.642788),
        vec3(-0.750000, 0.433013, 0.500000),
        vec3(-0.813798, 0.296198, 0.500000),
        vec3(-0.813798, 0.469846, 0.342020),
        vec3(-0.883022, 0.321394, 0.342020),
        vec3(-0.852869, 0.492404, 0.173648),
        vec3(-0.925417, 0.336824, 0.173648),
        vec3(-0.866025, 0.500000, 0.000000),
        vec3(-0.939693, 0.342020, 0.000000),
        vec3(-0.852869, 0.492404, -0.173648),
        vec3(-0.925417, 0.336824, -0.173648),
        vec3(-0.813798, 0.469846, -0.342020),
        vec3(-0.883022, 0.321394, -0.342020),
        vec3(-0.750000, 0.433013, -0.500000),
        vec3(-0.813798, 0.296198, -0.500000),
        vec3(-0.663414, 0.383022, -0.642788),
        vec3(-0.719846, 0.262003, -0.642788),
        vec3(-0.556670, 0.321394, -0.766044),
        vec3(-0.604023, 0.219846, -0.766044),
        vec3(-0.433013, 0.250000, -0.866025),
        vec3(-0.469846, 0.171010, -0.866025),
        vec3(-0.296198, 0.171010, -0.939693),
        vec3(-0.321394, 0.116978, -0.939693),
        vec3(-0.150384, 0.086824, -0.984808),
        vec3(-0.163176, 0.059391, -0.984808),
        vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.163176, 0.059391, -0.984808),
        vec3(-0.171010, 0.030154, -0.984808),
        vec3(-0.321394, 0.116978, -0.939693),
        vec3(-0.336824, 0.059391, -0.939693),
        vec3(-0.469846, 0.171010, -0.866025),
        vec3(-0.492404, 0.086824, -0.866025),
        vec3(-0.604023, 0.219846, -0.766044),
        vec3(-0.633022, 0.111619, -0.766044),
        vec3(-0.719846, 0.262003, -0.642788),
        vec3(-0.754407, 0.133022, -0.642788),
        vec3(-0.813798, 0.296198, -0.500000),
        vec3(-0.852869, 0.150384, -0.500000),
        vec3(-0.883022, 0.321394, -0.342020),
        vec3(-0.925417, 0.163176, -0.342020),
        vec3(-0.925417, 0.336824, -0.173648),
        vec3(-0.969846, 0.171010, -0.173648),
        vec3(-0.939693, 0.342020, 0.000000),
        vec3(-0.984808, 0.173648, 0.000000),
        vec3(-0.925417, 0.336824, 0.173648),
        vec3(-0.969846, 0.171010, 0.173648),
        vec3(-0.883022, 0.321394, 0.342020),
        vec3(-0.925417, 0.163176, 0.342020),
        vec3(-0.813798, 0.296198, 0.500000),
        vec3(-0.852869, 0.150384, 0.500000),
        vec3(-0.719846, 0.262003, 0.642788),
        vec3(-0.754407, 0.133022, 0.642788),
        vec3(-0.604023, 0.219846, 0.766044),
        vec3(-0.633022, 0.111619, 0.766044),
        vec3(-0.469846, 0.171010, 0.866025),
        vec3(-0.492404, 0.086824, 0.866025),
        vec3(-0.321394, 0.116978, 0.939693),
        vec3(-0.336824, 0.059391, 0.939693),
        vec3(-0.163176, 0.059391, 0.984808),
        vec3(-0.171010, 0.030154, 0.984808),
        vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.171010, 0.030154, 0.984808),
        vec3(-0.173648, 0.000000, 0.984808),
        vec3(-0.336824, 0.059391, 0.939693),
        vec3(-0.342020, 0.000000, 0.939693),
        vec3(-0.492404, 0.086824, 0.866025),
        vec3(-0.500000, 0.000000, 0.866025),
        vec3(-0.633022, 0.111619, 0.766044),
        vec3(-0.642788, 0.000000, 0.766044),
        vec3(-0.754407, 0.133022, 0.642788),
        vec3(-0.766044, 0.000000, 0.642788),
        vec3(-0.852869, 0.150384, 0.500000),
        vec3(-0.866025, 0.000000, 0.500000),
        vec3(-0.925417, 0.163176, 0.342020),
        vec3(-0.939693, 0.000000, 0.342020),
        vec3(-0.969846, 0.171010, 0.173648),
        vec3(-0.984808, 0.000000, 0.173648),
        vec3(-0.984808, 0.173648, 0.000000),
        vec3(-1.000000, 0.000000, 0.000000),
        vec3(-0.969846, 0.171010, -0.173648),
        vec3(-0.984808, 0.000000, -0.173648),
        vec3(-0.925417, 0.163176, -0.342020),
        vec3(-0.939693, 0.000000, -0.342020),
        vec3(-0.852869, 0.150384, -0.500000),
        vec3(-0.866025, 0.000000, -0.500000),
        vec3(-0.754407, 0.133022, -0.642788),
        vec3(-0.766044, 0.000000, -0.642788),
        vec3(-0.633022, 0.111619, -0.766044),
        vec3(-0.642788, 0.000000, -0.766044),
        vec3(-0.492404, 0.086824, -0.866025),
        vec3(-0.500000, 0.000000, -0.866025),
        vec3(-0.336824, 0.059391, -0.939693),
        vec3(-0.342020, 0.000000, -0.939693),
        vec3(-0.171010, 0.030154, -0.984808),
        vec3(-0.173648, 0.000000, -0.984808),
        vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.173648, 0.000000, -0.984808),
        vec3(-0.171010, -0.030154, -0.984808),
        vec3(-0.342020, 0.000000, -0.939693),
        vec3(-0.336824, -0.059391, -0.939693),
        vec3(-0.500000, 0.000000, -0.866025),
        vec3(-0.492404, -0.086824, -0.866025),
        vec3(-0.642788, 0.000000, -0.766044),
        vec3(-0.633022, -0.111619, -0.766044),
        vec3(-0.766044, 0.000000, -0.642788),
        vec3(-0.754407, -0.133022, -0.642788),
        vec3(-0.866025, 0.000000, -0.500000),
        vec3(-0.852869, -0.150384, -0.500000),
        vec3(-0.939693, 0.000000, -0.342020),
        vec3(-0.925417, -0.163176, -0.342020),
        vec3(-0.984808, 0.000000, -0.173648),
        vec3(-0.969846, -0.171010, -0.173648),
        vec3(-1.000000, 0.000000, 0.000000),
        vec3(-0.984808, -0.173648, 0.000000),
        vec3(-0.984808, 0.000000, 0.173648),
        vec3(-0.969846, -0.171010, 0.173648),
        vec3(-0.939693, 0.000000, 0.342020),
        vec3(-0.925417, -0.163176, 0.342020),
        vec3(-0.866025, 0.000000, 0.500000),
        vec3(-0.852869, -0.150384, 0.500000),
        vec3(-0.766044, 0.000000, 0.642788),
        vec3(-0.754407, -0.133022, 0.642788),
        vec3(-0.642788, 0.000000, 0.766044),
        vec3(-0.633022, -0.111619, 0.766044),
        vec3(-0.500000, 0.000000, 0.866025),
        vec3(-0.492404, -0.086824, 0.866025),
        vec3(-0.342020, 0.000000, 0.939693),
        vec3(-0.336824, -0.059391, 0.939693),
        vec3(-0.173648, 0.000000, 0.984808),
        vec3(-0.171010, -0.030154, 0.984808),
        vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.171010, -0.030154, 0.984808),
        vec3(-0.163176, -0.059391, 0.984808),
        vec3(-0.336824, -0.059391, 0.939693),
        vec3(-0.321394, -0.116978, 0.939693),
        vec3(-0.492404, -0.086824, 0.866025),
        vec3(-0.469846, -0.171010, 0.866025),
        vec3(-0.633022, -0.111619, 0.766044),
        vec3(-0.604023, -0.219846, 0.766044),
        vec3(-0.754407, -0.133022, 0.642788),
        vec3(-0.719846, -0.262003, 0.642788),
        vec3(-0.852869, -0.150384, 0.500000),
        vec3(-0.813798, -0.296198, 0.500000),
        vec3(-0.925417, -0.163176, 0.342020),
        vec3(-0.883022, -0.321394, 0.342020),
        vec3(-0.969846, -0.171010, 0.173648),
        vec3(-0.925417, -0.336824, 0.173648),
        vec3(-0.984808, -0.173648, 0.000000),
        vec3(-0.939693, -0.342020, 0.000000),
        vec3(-0.969846, -0.171010, -0.173648),
        vec3(-0.925417, -0.336824, -0.173648),
        vec3(-0.925417, -0.163176, -0.342020),
        vec3(-0.883022, -0.321394, -0.342020),
        vec3(-0.852869, -0.150384, -0.500000),
        vec3(-0.813798, -0.296198, -0.500000),
        vec3(-0.754407, -0.133022, -0.642788),
        vec3(-0.719846, -0.262003, -0.642788),
        vec3(-0.633022, -0.111619, -0.766044),
        vec3(-0.604023, -0.219846, -0.766044),
        vec3(-0.492404, -0.086824, -0.866025),
        vec3(-0.469846, -0.171010, -0.866025),
        vec3(-0.336824, -0.059391, -0.939693),
        vec3(-0.321394, -0.116978, -0.939693),
        vec3(-0.171010, -0.030154, -0.984808),
        vec3(-0.163176, -0.059391, -0.984808),
        vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.163176, -0.059391, -0.984808),
        vec3(-0.150384, -0.086824, -0.984808),
        vec3(-0.321394, -0.116978, -0.939693),
        vec3(-0.296198, -0.171010, -0.939693),
        vec3(-0.469846, -0.171010, -0.866025),
        vec3(-0.433013, -0.250000, -0.866025),
        vec3(-0.604023, -0.219846, -0.766044),
        vec3(-0.556670, -0.321394, -0.766044),
        vec3(-0.719846, -0.262003, -0.642788),
        vec3(-0.663414, -0.383022, -0.642788),
        vec3(-0.813798, -0.296198, -0.500000),
        vec3(-0.750000, -0.433013, -0.500000),
        vec3(-0.883022, -0.321394, -0.342020),
        vec3(-0.813798, -0.469846, -0.342020),
        vec3(-0.925417, -0.336824, -0.173648),
        vec3(-0.852869, -0.492404, -0.173648),
        vec3(-0.939693, -0.342020, 0.000000),
        vec3(-0.866025, -0.500000, 0.000000),
        vec3(-0.925417, -0.336824, 0.173648),
        vec3(-0.852869, -0.492404, 0.173648),
        vec3(-0.883022, -0.321394, 0.342020),
        vec3(-0.813798, -0.469846, 0.342020),
        vec3(-0.813798, -0.296198, 0.500000),
        vec3(-0.750000, -0.433013, 0.500000),
        vec3(-0.719846, -0.262003, 0.642788),
        vec3(-0.663414, -0.383022, 0.642788),
        vec3(-0.604023, -0.219846, 0.766044),
        vec3(-0.556670, -0.321394, 0.766044),
        vec3(-0.469846, -0.171010, 0.866025),
        vec3(-0.433013, -0.250000, 0.866025),
        vec3(-0.321394, -0.116978, 0.939693),
        vec3(-0.296198, -0.171010, 0.939693),
        vec3(-0.163176, -0.059391, 0.984808),
        vec3(-0.150384, -0.086824, 0.984808),
        vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.150384, -0.086824, 0.984808),
        vec3(-0.133022, -0.111619, 0.984808),
        vec3(-0.296198, -0.171010, 0.939693),
        vec3(-0.262003, -0.219846, 0.939693),
        vec3(-0.433013, -0.250000, 0.866025),
        vec3(-0.383022, -0.321394, 0.866025),
        vec3(-0.556670, -0.321394, 0.766044),
        vec3(-0.492404, -0.413176, 0.766044),
        vec3(-0.663414, -0.383022, 0.642788),
        vec3(-0.586824, -0.492404, 0.642788),
        vec3(-0.750000, -0.433013, 0.500000),
        vec3(-0.663414, -0.556670, 0.500000),
        vec3(-0.813798, -0.469846, 0.342020),
        vec3(-0.719846, -0.604023, 0.342020),
        vec3(-0.852869, -0.492404, 0.173648),
        vec3(-0.754407, -0.633022, 0.173648),
        vec3(-0.866025, -0.500000, 0.000000),
        vec3(-0.766044, -0.642788, 0.000000),
        vec3(-0.852869, -0.492404, -0.173648),
        vec3(-0.754407, -0.633022, -0.173648),
        vec3(-0.813798, -0.469846, -0.342020),
        vec3(-0.719846, -0.604023, -0.342020),
        vec3(-0.750000, -0.433013, -0.500000),
        vec3(-0.663414, -0.556670, -0.500000),
        vec3(-0.663414, -0.383022, -0.642788),
        vec3(-0.586824, -0.492404, -0.642788),
        vec3(-0.556670, -0.321394, -0.766044),
        vec3(-0.492404, -0.413176, -0.766044),
        vec3(-0.433013, -0.250000, -0.866025),
        vec3(-0.383022, -0.321394, -0.866025),
        vec3(-0.296198, -0.171010, -0.939693),
        vec3(-0.262003, -0.219846, -0.939693),
        vec3(-0.150384, -0.086824, -0.984808),
        vec3(-0.133022, -0.111619, -0.984808),
        vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.133022, -0.111619, -0.984808),
        vec3(-0.111619, -0.133022, -0.984808),
        vec3(-0.262003, -0.219846, -0.939693),
        vec3(-0.219846, -0.262003, -0.939693),
        vec3(-0.383022, -0.321394, -0.866025),
        vec3(-0.321394, -0.383022, -0.866025),
        vec3(-0.492404, -0.413176, -0.766044),
        vec3(-0.413176, -0.492404, -0.766044),
        vec3(-0.586824, -0.492404, -0.642788),
        vec3(-0.492404, -0.586824, -0.642788),
        vec3(-0.663414, -0.556670, -0.500000),
        vec3(-0.556670, -0.663414, -0.500000),
        vec3(-0.719846, -0.604023, -0.342020),
        vec3(-0.604023, -0.719846, -0.342020),
        vec3(-0.754407, -0.633022, -0.173648),
        vec3(-0.633022, -0.754407, -0.173648),
        vec3(-0.766044, -0.642788, 0.000000),
        vec3(-0.642788, -0.766044, 0.000000),
        vec3(-0.754407, -0.633022, 0.173648),
        vec3(-0.633022, -0.754407, 0.173648),
        vec3(-0.719846, -0.604023, 0.342020),
        vec3(-0.604023, -0.719846, 0.342020),
        vec3(-0.663414, -0.556670, 0.500000),
        vec3(-0.556670, -0.663414, 0.500000),
        vec3(-0.586824, -0.492404, 0.642788),
        vec3(-0.492404, -0.586824, 0.642788),
        vec3(-0.492404, -0.413176, 0.766044),
        vec3(-0.413176, -0.492404, 0.766044),
        vec3(-0.383022, -0.321394, 0.866025),
        vec3(-0.321394, -0.383022, 0.866025),
        vec3(-0.262003, -0.219846, 0.939693),
        vec3(-0.219846, -0.262003, 0.939693),
        vec3(-0.133022, -0.111619, 0.984808),
        vec3(-0.111619, -0.133022, 0.984808),
        vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.111619, -0.133022, 0.984808),
        vec3(-0.086824, -0.150384, 0.984808),
        vec3(-0.219846, -0.262003, 0.939693),
        vec3(-0.171010, -0.296198, 0.939693),
        vec3(-0.321394, -0.383022, 0.866025),
        vec3(-0.250000, -0.433013, 0.866025),
        vec3(-0.413176, -0.492404, 0.766044),
        vec3(-0.321394, -0.556670, 0.766044),
        vec3(-0.492404, -0.586824, 0.642788),
        vec3(-0.383022, -0.663414, 0.642788),
        vec3(-0.556670, -0.663414, 0.500000),
        vec3(-0.433013, -0.750000, 0.500000),
        vec3(-0.604023, -0.719846, 0.342020),
        vec3(-0.469846, -0.813798, 0.342020),
        vec3(-0.633022, -0.754407, 0.173648),
        vec3(-0.492404, -0.852869, 0.173648),
        vec3(-0.642788, -0.766044, 0.000000),
        vec3(-0.500000, -0.866025, 0.000000),
        vec3(-0.633022, -0.754407, -0.173648),
        vec3(-0.492404, -0.852869, -0.173648),
        vec3(-0.604023, -0.719846, -0.342020),
        vec3(-0.469846, -0.813798, -0.342020),
        vec3(-0.556670, -0.663414, -0.500000),
        vec3(-0.433013, -0.750000, -0.500000),
        vec3(-0.492404, -0.586824, -0.642788),
        vec3(-0.383022, -0.663414, -0.642788),
        vec3(-0.413176, -0.492404, -0.766044),
        vec3(-0.321394, -0.556670, -0.766044),
        vec3(-0.321394, -0.383022, -0.866025),
        vec3(-0.250000, -0.433013, -0.866025),
        vec3(-0.219846, -0.262003, -0.939693),
        vec3(-0.171010, -0.296198, -0.939693),
        vec3(-0.111619, -0.133022, -0.984808),
        vec3(-0.086824, -0.150384, -0.984808),
        vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.086824, -0.150384, -0.984808),
        vec3(-0.059391, -0.163176, -0.984808),
        vec3(-0.171010, -0.296198, -0.939693),
        vec3(-0.116978, -0.321394, -0.939693),
        vec3(-0.250000, -0.433013, -0.866025),
        vec3(-0.171010, -0.469846, -0.866025),
        vec3(-0.321394, -0.556670, -0.766044),
        vec3(-0.219846, -0.604023, -0.766044),
        vec3(-0.383022, -0.663414, -0.642788),
        vec3(-0.262003, -0.719846, -0.642788),
        vec3(-0.433013, -0.750000, -0.500000),
        vec3(-0.296198, -0.813798, -0.500000),
        vec3(-0.469846, -0.813798, -0.342020),
        vec3(-0.321394, -0.883022, -0.342020),
        vec3(-0.492404, -0.852869, -0.173648),
        vec3(-0.336824, -0.925417, -0.173648),
        vec3(-0.500000, -0.866025, 0.000000),
        vec3(-0.342020, -0.939693, 0.000000),
        vec3(-0.492404, -0.852869, 0.173648),
        vec3(-0.336824, -0.925417, 0.173648),
        vec3(-0.469846, -0.813798, 0.342020),
        vec3(-0.321394, -0.883022, 0.342020),
        vec3(-0.433013, -0.750000, 0.500000),
        vec3(-0.296198, -0.813798, 0.500000),
        vec3(-0.383022, -0.663414, 0.642788),
        vec3(-0.262003, -0.719846, 0.642788),
        vec3(-0.321394, -0.556670, 0.766044),
        vec3(-0.219846, -0.604023, 0.766044),
        vec3(-0.250000, -0.433013, 0.866025),
        vec3(-0.171010, -0.469846, 0.866025),
        vec3(-0.171010, -0.296198, 0.939693),
        vec3(-0.116978, -0.321394, 0.939693),
        vec3(-0.086824, -0.150384, 0.984808),
        vec3(-0.059391, -0.163176, 0.984808),
        vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.059391, -0.163176, 0.984808),
        vec3(-0.030154, -0.171010, 0.984808),
        vec3(-0.116978, -0.321394, 0.939693),
        vec3(-0.059391, -0.336824, 0.939693),
        vec3(-0.171010, -0.469846, 0.866025),
        vec3(-0.086824, -0.492404, 0.866025),
        vec3(-0.219846, -0.604023, 0.766044),
        vec3(-0.111619, -0.633022, 0.766044),
        vec3(-0.262003, -0.719846, 0.642788),
        vec3(-0.133022, -0.754407, 0.642788),
        vec3(-0.296198, -0.813798, 0.500000),
        vec3(-0.150384, -0.852869, 0.500000),
        vec3(-0.321394, -0.883022, 0.342020),
        vec3(-0.163176, -0.925417, 0.342020),
        vec3(-0.336824, -0.925417, 0.173648),
        vec3(-0.171010, -0.969846, 0.173648),
        vec3(-0.342020, -0.939693, 0.000000),
        vec3(-0.173648, -0.984808, 0.000000),
        vec3(-0.336824, -0.925417, -0.173648),
        vec3(-0.171010, -0.969846, -0.173648),
        vec3(-0.321394, -0.883022, -0.342020),
        vec3(-0.163176, -0.925417, -0.342020),
        vec3(-0.296198, -0.813798, -0.500000),
        vec3(-0.150384, -0.852869, -0.500000),
        vec3(-0.262003, -0.719846, -0.642788),
        vec3(-0.133022, -0.754407, -0.642788),
        vec3(-0.219846, -0.604023, -0.766044),
        vec3(-0.111619, -0.633022, -0.766044),
        vec3(-0.171010, -0.469846, -0.866025),
        vec3(-0.086824, -0.492404, -0.866025),
        vec3(-0.116978, -0.321394, -0.939693),
        vec3(-0.059391, -0.336824, -0.939693),
        vec3(-0.059391, -0.163176, -0.984808),
        vec3(-0.030154, -0.171010, -0.984808),
        vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.030154, -0.171010, -0.984808),
        vec3(-0.000000, -0.173648, -0.984808),
        vec3(-0.059391, -0.336824, -0.939693),
        vec3(-0.000000, -0.342020, -0.939693),
        vec3(-0.086824, -0.492404, -0.866025),
        vec3(-0.000000, -0.500000, -0.866025),
        vec3(-0.111619, -0.633022, -0.766044),
        vec3(-0.000000, -0.642788, -0.766044),
        vec3(-0.133022, -0.754407, -0.642788),
        vec3(-0.000000, -0.766044, -0.642788),
        vec3(-0.150384, -0.852869, -0.500000),
        vec3(-0.000000, -0.866025, -0.500000),
        vec3(-0.163176, -0.925417, -0.342020),
        vec3(-0.000000, -0.939693, -0.342020),
        vec3(-0.171010, -0.969846, -0.173648),
        vec3(-0.000000, -0.984808, -0.173648),
        vec3(-0.173648, -0.984808, 0.000000),
        vec3(-0.000000, -1.000000, 0.000000),
        vec3(-0.171010, -0.969846, 0.173648),
        vec3(-0.000000, -0.984808, 0.173648),
        vec3(-0.163176, -0.925417, 0.342020),
        vec3(-0.000000, -0.939693, 0.342020),
        vec3(-0.150384, -0.852869, 0.500000),
        vec3(-0.000000, -0.866025, 0.500000),
        vec3(-0.133022, -0.754407, 0.642788),
        vec3(-0.000000, -0.766044, 0.642788),
        vec3(-0.111619, -0.633022, 0.766044),
        vec3(-0.000000, -0.642788, 0.766044),
        vec3(-0.086824, -0.492404, 0.866025),
        vec3(-0.000000, -0.500000, 0.866025),
        vec3(-0.059391, -0.336824, 0.939693),
        vec3(-0.000000, -0.342020, 0.939693),
        vec3(-0.030154, -0.171010, 0.984808),
        vec3(-0.000000, -0.173648, 0.984808),
        vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.000000, -0.173648, 0.984808),
        vec3(0.030154, -0.171010, 0.984808),
        vec3(-0.000000, -0.342020, 0.939693),
        vec3(0.059391, -0.336824, 0.939693),
        vec3(-0.000000, -0.500000, 0.866025),
        vec3(0.086824, -0.492404, 0.866025),
        vec3(-0.000000, -0.642788, 0.766044),
        vec3(0.111619, -0.633022, 0.766044),
        vec3(-0.000000, -0.766044, 0.642788),
        vec3(0.133022, -0.754407, 0.642788),
        vec3(-0.000000, -0.866025, 0.500000),
        vec3(0.150384, -0.852869, 0.500000),
        vec3(-0.000000, -0.939693, 0.342020),
        vec3(0.163176, -0.925417, 0.342020),
        vec3(-0.000000, -0.984808, 0.173648),
        vec3(0.171010, -0.969846, 0.173648),
        vec3(-0.000000, -1.000000, 0.000000),
        vec3(0.173648, -0.984808, 0.000000),
        vec3(-0.000000, -0.984808, -0.173648),
        vec3(0.171010, -0.969846, -0.173648),
        vec3(-0.000000, -0.939693, -0.342020),
        vec3(0.163176, -0.925417, -0.342020),
        vec3(-0.000000, -0.866025, -0.500000),
        vec3(0.150384, -0.852869, -0.500000),
        vec3(-0.000000, -0.766044, -0.642788),
        vec3(0.133022, -0.754407, -0.642788),
        vec3(-0.000000, -0.642788, -0.766044),
        vec3(0.111619, -0.633022, -0.766044),
        vec3(-0.000000, -0.500000, -0.866025),
        vec3(0.086824, -0.492404, -0.866025),
        vec3(-0.000000, -0.342020, -0.939693),
        vec3(0.059391, -0.336824, -0.939693),
        vec3(-0.000000, -0.173648, -0.984808),
        vec3(0.030154, -0.171010, -0.984808),
        vec3(0.000000, 0.000000, -1.000000),
        vec3(0.030154, -0.171010, -0.984808),
        vec3(0.059391, -0.163176, -0.984808),
        vec3(0.059391, -0.336824, -0.939693),
        vec3(0.116978, -0.321394, -0.939693),
        vec3(0.086824, -0.492404, -0.866025),
        vec3(0.171010, -0.469846, -0.866025),
        vec3(0.111619, -0.633022, -0.766044),
        vec3(0.219846, -0.604023, -0.766044),
        vec3(0.133022, -0.754407, -0.642788),
        vec3(0.262003, -0.719846, -0.642788),
        vec3(0.150384, -0.852869, -0.500000),
        vec3(0.296198, -0.813798, -0.500000),
        vec3(0.163176, -0.925417, -0.342020),
        vec3(0.321394, -0.883022, -0.342020),
        vec3(0.171010, -0.969846, -0.173648),
        vec3(0.336824, -0.925417, -0.173648),
        vec3(0.173648, -0.984808, 0.000000),
        vec3(0.342020, -0.939693, 0.000000),
        vec3(0.171010, -0.969846, 0.173648),
        vec3(0.336824, -0.925417, 0.173648),
        vec3(0.163176, -0.925417, 0.342020),
        vec3(0.321394, -0.883022, 0.342020),
        vec3(0.150384, -0.852869, 0.500000),
        vec3(0.296198, -0.813798, 0.500000),
        vec3(0.133022, -0.754407, 0.642788),
        vec3(0.262003, -0.719846, 0.642788),
        vec3(0.111619, -0.633022, 0.766044),
        vec3(0.219846, -0.604023, 0.766044),
        vec3(0.086824, -0.492404, 0.866025),
        vec3(0.171010, -0.469846, 0.866025),
        vec3(0.059391, -0.336824, 0.939693),
        vec3(0.116978, -0.321394, 0.939693),
        vec3(0.030154, -0.171010, 0.984808),
        vec3(0.059391, -0.163176, 0.984808),
        vec3(0.000000, 0.000000, 1.000000),
        vec3(0.059391, -0.163176, 0.984808),
        vec3(0.086824, -0.150384, 0.984808),
        vec3(0.116978, -0.321394, 0.939693),
        vec3(0.171010, -0.296198, 0.939693),
        vec3(0.171010, -0.469846, 0.866025),
        vec3(0.250000, -0.433013, 0.866025),
        vec3(0.219846, -0.604023, 0.766044),
        vec3(0.321394, -0.556670, 0.766044),
        vec3(0.262003, -0.719846, 0.642788),
        vec3(0.383022, -0.663414, 0.642788),
        vec3(0.296198, -0.813798, 0.500000),
        vec3(0.433013, -0.750000, 0.500000),
        vec3(0.321394, -0.883022, 0.342020),
        vec3(0.469846, -0.813798, 0.342020),
        vec3(0.336824, -0.925417, 0.173648),
        vec3(0.492404, -0.852869, 0.173648),
        vec3(0.342020, -0.939693, 0.000000),
        vec3(0.500000, -0.866025, 0.000000),
        vec3(0.336824, -0.925417, -0.173648),
        vec3(0.492404, -0.852869, -0.173648),
        vec3(0.321394, -0.883022, -0.342020),
        vec3(0.469846, -0.813798, -0.342020),
        vec3(0.296198, -0.813798, -0.500000),
        vec3(0.433013, -0.750000, -0.500000),
        vec3(0.262003, -0.719846, -0.642788),
        vec3(0.383022, -0.663414, -0.642788),
        vec3(0.219846, -0.604023, -0.766044),
        vec3(0.321394, -0.556670, -0.766044),
        vec3(0.171010, -0.469846, -0.866025),
        vec3(0.250000, -0.433013, -0.866025),
        vec3(0.116978, -0.321394, -0.939693),
        vec3(0.171010, -0.296198, -0.939693),
        vec3(0.059391, -0.163176, -0.984808),
        vec3(0.086824, -0.150384, -0.984808),
        vec3(0.000000, 0.000000, -1.000000),
        vec3(0.086824, -0.150384, -0.984808),
        vec3(0.111619, -0.133022, -0.984808),
        vec3(0.171010, -0.296198, -0.939693),
        vec3(0.219846, -0.262003, -0.939693),
        vec3(0.250000, -0.433013, -0.866025),
        vec3(0.321394, -0.383022, -0.866025),
        vec3(0.321394, -0.556670, -0.766044),
        vec3(0.413176, -0.492404, -0.766044),
        vec3(0.383022, -0.663414, -0.642788),
        vec3(0.492404, -0.586824, -0.642788),
        vec3(0.433013, -0.750000, -0.500000),
        vec3(0.556670, -0.663414, -0.500000),
        vec3(0.469846, -0.813798, -0.342020),
        vec3(0.604023, -0.719846, -0.342020),
        vec3(0.492404, -0.852869, -0.173648),
        vec3(0.633022, -0.754407, -0.173648),
        vec3(0.500000, -0.866025, 0.000000),
        vec3(0.642788, -0.766044, 0.000000),
        vec3(0.492404, -0.852869, 0.173648),
        vec3(0.633022, -0.754407, 0.173648),
        vec3(0.469846, -0.813798, 0.342020),
        vec3(0.604023, -0.719846, 0.342020),
        vec3(0.433013, -0.750000, 0.500000),
        vec3(0.556670, -0.663414, 0.500000),
        vec3(0.383022, -0.663414, 0.642788),
        vec3(0.492404, -0.586824, 0.642788),
        vec3(0.321394, -0.556670, 0.766044),
        vec3(0.413176, -0.492404, 0.766044),
        vec3(0.250000, -0.433013, 0.866025),
        vec3(0.321394, -0.383022, 0.866025),
        vec3(0.171010, -0.296198, 0.939693),
        vec3(0.219846, -0.262003, 0.939693),
        vec3(0.086824, -0.150384, 0.984808),
        vec3(0.111619, -0.133022, 0.984808),
        vec3(0.000000, 0.000000, 1.000000),
        vec3(0.111619, -0.133022, 0.984808),
        vec3(0.133022, -0.111619, 0.984808),
        vec3(0.219846, -0.262003, 0.939693),
        vec3(0.262003, -0.219846, 0.939693),
        vec3(0.321394, -0.383022, 0.866025),
        vec3(0.383022, -0.321394, 0.866025),
        vec3(0.413176, -0.492404, 0.766044),
        vec3(0.492404, -0.413176, 0.766044),
        vec3(0.492404, -0.586824, 0.642788),
        vec3(0.586824, -0.492404, 0.642788),
        vec3(0.556670, -0.663414, 0.500000),
        vec3(0.663414, -0.556670, 0.500000),
        vec3(0.604023, -0.719846, 0.342020),
        vec3(0.719846, -0.604023, 0.342020),
        vec3(0.633022, -0.754407, 0.173648),
        vec3(0.754407, -0.633022, 0.173648),
        vec3(0.642788, -0.766044, 0.000000),
        vec3(0.766044, -0.642788, 0.000000),
        vec3(0.633022, -0.754407, -0.173648),
        vec3(0.754407, -0.633022, -0.173648),
        vec3(0.604023, -0.719846, -0.342020),
        vec3(0.719846, -0.604023, -0.342020),
        vec3(0.556670, -0.663414, -0.500000),
        vec3(0.663414, -0.556670, -0.500000),
        vec3(0.492404, -0.586824, -0.642788),
        vec3(0.586824, -0.492404, -0.642788),
        vec3(0.413176, -0.492404, -0.766044),
        vec3(0.492404, -0.413176, -0.766044),
        vec3(0.321394, -0.383022, -0.866025),
        vec3(0.383022, -0.321394, -0.866025),
        vec3(0.219846, -0.262003, -0.939693),
        vec3(0.262003, -0.219846, -0.939693),
        vec3(0.111619, -0.133022, -0.984808),
        vec3(0.133022, -0.111619, -0.984808),
        vec3(0.000000, 0.000000, -1.000000),
        vec3(0.133022, -0.111619, -0.984808),
        vec3(0.150384, -0.086824, -0.984808),
        vec3(0.262003, -0.219846, -0.939693),
        vec3(0.296198, -0.171010, -0.939693),
        vec3(0.383022, -0.321394, -0.866025),
        vec3(0.433013, -0.250000, -0.866025),
        vec3(0.492404, -0.413176, -0.766044),
        vec3(0.556670, -0.321394, -0.766044),
        vec3(0.586824, -0.492404, -0.642788),
        vec3(0.663414, -0.383022, -0.642788),
        vec3(0.663414, -0.556670, -0.500000),
        vec3(0.750000, -0.433013, -0.500000),
        vec3(0.719846, -0.604023, -0.342020),
        vec3(0.813798, -0.469846, -0.342020),
        vec3(0.754407, -0.633022, -0.173648),
        vec3(0.852869, -0.492404, -0.173648),
        vec3(0.766044, -0.642788, 0.000000),
        vec3(0.866025, -0.500000, 0.000000),
        vec3(0.754407, -0.633022, 0.173648),
        vec3(0.852869, -0.492404, 0.173648),
        vec3(0.719846, -0.604023, 0.342020),
        vec3(0.813798, -0.469846, 0.342020),
        vec3(0.663414, -0.556670, 0.500000),
        vec3(0.750000, -0.433013, 0.500000),
        vec3(0.586824, -0.492404, 0.642788),
        vec3(0.663414, -0.383022, 0.642788),
        vec3(0.492404, -0.413176, 0.766044),
        vec3(0.556670, -0.321394, 0.766044),
        vec3(0.383022, -0.321394, 0.866025),
        vec3(0.433013, -0.250000, 0.866025),
        vec3(0.262003, -0.219846, 0.939693),
        vec3(0.296198, -0.171010, 0.939693),
        vec3(0.133022, -0.111619, 0.984808),
        vec3(0.150384, -0.086824, 0.984808),
        vec3(0.000000, 0.000000, 1.000000),
        vec3(0.150384, -0.086824, 0.984808),
        vec3(0.163176, -0.059391, 0.984808),
        vec3(0.296198, -0.171010, 0.939693),
        vec3(0.321394, -0.116978, 0.939693),
        vec3(0.433013, -0.250000, 0.866025),
        vec3(0.469846, -0.171010, 0.866025),
        vec3(0.556670, -0.321394, 0.766044),
        vec3(0.604023, -0.219846, 0.766044),
        vec3(0.663414, -0.383022, 0.642788),
        vec3(0.719846, -0.262003, 0.642788),
        vec3(0.750000, -0.433013, 0.500000),
        vec3(0.813798, -0.296198, 0.500000),
        vec3(0.813798, -0.469846, 0.342020),
        vec3(0.883022, -0.321394, 0.342020),
        vec3(0.852869, -0.492404, 0.173648),
        vec3(0.925417, -0.336824, 0.173648),
        vec3(0.866025, -0.500000, 0.000000),
        vec3(0.939693, -0.342020, 0.000000),
        vec3(0.852869, -0.492404, -0.173648),
        vec3(0.925417, -0.336824, -0.173648),
        vec3(0.813798, -0.469846, -0.342020),
        vec3(0.883022, -0.321394, -0.342020),
        vec3(0.750000, -0.433013, -0.500000),
        vec3(0.813798, -0.296198, -0.500000),
        vec3(0.663414, -0.383022, -0.642788),
        vec3(0.719846, -0.262003, -0.642788),
        vec3(0.556670, -0.321394, -0.766044),
        vec3(0.604023, -0.219846, -0.766044),
        vec3(0.433013, -0.250000, -0.866025),
        vec3(0.469846, -0.171010, -0.866025),
        vec3(0.296198, -0.171010, -0.939693),
        vec3(0.321394, -0.116978, -0.939693),
        vec3(0.150384, -0.086824, -0.984808),
        vec3(0.163176, -0.059391, -0.984808),
        vec3(0.000000, 0.000000, -1.000000),
        vec3(0.163176, -0.059391, -0.984808),
        vec3(0.171010, -0.030154, -0.984808),
        vec3(0.321394, -0.116978, -0.939693),
        vec3(0.336824, -0.059391, -0.939693),
        vec3(0.469846, -0.171010, -0.866025),
        vec3(0.492404, -0.086824, -0.866025),
        vec3(0.604023, -0.219846, -0.766044),
        vec3(0.633022, -0.111619, -0.766044),
        vec3(0.719846, -0.262003, -0.642788),
        vec3(0.754407, -0.133022, -0.642788),
        vec3(0.813798, -0.296198, -0.500000),
        vec3(0.852869, -0.150384, -0.500000),
        vec3(0.883022, -0.321394, -0.342020),
        vec3(0.925417, -0.163176, -0.342020),
        vec3(0.925417, -0.336824, -0.173648),
        vec3(0.969846, -0.171010, -0.173648),
        vec3(0.939693, -0.342020, 0.000000),
        vec3(0.984808, -0.173648, 0.000000),
        vec3(0.925417, -0.336824, 0.173648),
        vec3(0.969846, -0.171010, 0.173648),
        vec3(0.883022, -0.321394, 0.342020),
        vec3(0.925417, -0.163176, 0.342020),
        vec3(0.813798, -0.296198, 0.500000),
        vec3(0.852869, -0.150384, 0.500000),
        vec3(0.719846, -0.262003, 0.642788),
        vec3(0.754407, -0.133022, 0.642788),
        vec3(0.604023, -0.219846, 0.766044),
        vec3(0.633022, -0.111619, 0.766044),
        vec3(0.469846, -0.171010, 0.866025),
        vec3(0.492404, -0.086824, 0.866025),
        vec3(0.321394, -0.116978, 0.939693),
        vec3(0.336824, -0.059391, 0.939693),
        vec3(0.163176, -0.059391, 0.984808),
        vec3(0.171010, -0.030154, 0.984808),
        vec3(0.000000, 0.000000, 1.000000),
        vec3(0.171010, -0.030154, 0.984808),
        vec3(0.173648, -0.000000, 0.984808),
        vec3(0.336824, -0.059391, 0.939693),
        vec3(0.342020, -0.000000, 0.939693),
        vec3(0.492404, -0.086824, 0.866025),
        vec3(0.500000, -0.000000, 0.866025),
        vec3(0.633022, -0.111619, 0.766044),
        vec3(0.642788, -0.000000, 0.766044),
        vec3(0.754407, -0.133022, 0.642788),
        vec3(0.766044, -0.000000, 0.642788),
        vec3(0.852869, -0.150384, 0.500000),
        vec3(0.866025, -0.000000, 0.500000),
        vec3(0.925417, -0.163176, 0.342020),
        vec3(0.939693, -0.000000, 0.342020),
        vec3(0.969846, -0.171010, 0.173648),
        vec3(0.984808, -0.000000, 0.173648),
        vec3(0.984808, -0.173648, 0.000000),
        vec3(1.000000, -0.000000, 0.000000),
        vec3(0.969846, -0.171010, -0.173648),
        vec3(0.984808, -0.000000, -0.173648),
        vec3(0.925417, -0.163176, -0.342020),
        vec3(0.939693, -0.000000, -0.342020),
        vec3(0.852869, -0.150384, -0.500000),
        vec3(0.866025, -0.000000, -0.500000),
        vec3(0.754407, -0.133022, -0.642788),
        vec3(0.766044, -0.000000, -0.642788),
        vec3(0.633022, -0.111619, -0.766044),
        vec3(0.642788, -0.000000, -0.766044),
        vec3(0.492404, -0.086824, -0.866025),
        vec3(0.500000, -0.000000, -0.866025),
        vec3(0.336824, -0.059391, -0.939693),
        vec3(0.342020, -0.000000, -0.939693),
        vec3(0.171010, -0.030154, -0.984808),
        vec3(0.173648, -0.000000, -0.984808),
        vec3(0.000000, 0.000000, -1.000000)
    };*/

    // Sphere data positon + normal
    vec3 vertexArray[] = {
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(0.173648, 0.000000, -0.984808), vec3(0.173648, 0.000000, -0.984808),
        vec3(0.171010, 0.030154, -0.984808), vec3(0.171010, 0.030154, -0.984808),
        vec3(0.342020, 0.000000, -0.939693), vec3(0.342020, 0.000000, -0.939693),
        vec3(0.336824, 0.059391, -0.939693), vec3(0.336824, 0.059391, -0.939693),
        vec3(0.500000, 0.000000, -0.866025), vec3(0.500000, 0.000000, -0.866025),
        vec3(0.492404, 0.086824, -0.866025), vec3(0.492404, 0.086824, -0.866025),
        vec3(0.642788, 0.000000, -0.766044), vec3(0.642788, 0.000000, -0.766044),
        vec3(0.633022, 0.111619, -0.766044), vec3(0.633022, 0.111619, -0.766044),
        vec3(0.766044, 0.000000, -0.642788), vec3(0.766044, 0.000000, -0.642788),
        vec3(0.754407, 0.133022, -0.642788), vec3(0.754407, 0.133022, -0.642788),
        vec3(0.866025, 0.000000, -0.500000), vec3(0.866025, 0.000000, -0.500000),
        vec3(0.852869, 0.150384, -0.500000), vec3(0.852869, 0.150384, -0.500000),
        vec3(0.939693, 0.000000, -0.342020), vec3(0.939693, 0.000000, -0.342020),
        vec3(0.925417, 0.163176, -0.342020), vec3(0.925417, 0.163176, -0.342020),
        vec3(0.984808, 0.000000, -0.173648), vec3(0.984808, 0.000000, -0.173648),
        vec3(0.969846, 0.171010, -0.173648), vec3(0.969846, 0.171010, -0.173648),
        vec3(1.000000, 0.000000, 0.000000), vec3(1.000000, 0.000000, 0.000000),
        vec3(0.984808, 0.173648, 0.000000), vec3(0.984808, 0.173648, 0.000000),
        vec3(0.984808, 0.000000, 0.173648), vec3(0.984808, 0.000000, 0.173648),
        vec3(0.969846, 0.171010, 0.173648), vec3(0.969846, 0.171010, 0.173648),
        vec3(0.939693, 0.000000, 0.342020), vec3(0.939693, 0.000000, 0.342020),
        vec3(0.925417, 0.163176, 0.342020), vec3(0.925417, 0.163176, 0.342020),
        vec3(0.866025, 0.000000, 0.500000), vec3(0.866025, 0.000000, 0.500000),
        vec3(0.852869, 0.150384, 0.500000), vec3(0.852869, 0.150384, 0.500000),
        vec3(0.766044, 0.000000, 0.642788), vec3(0.766044, 0.000000, 0.642788),
        vec3(0.754407, 0.133022, 0.642788), vec3(0.754407, 0.133022, 0.642788),
        vec3(0.642788, 0.000000, 0.766044), vec3(0.642788, 0.000000, 0.766044),
        vec3(0.633022, 0.111619, 0.766044), vec3(0.633022, 0.111619, 0.766044),
        vec3(0.500000, 0.000000, 0.866025), vec3(0.500000, 0.000000, 0.866025),
        vec3(0.492404, 0.086824, 0.866025), vec3(0.492404, 0.086824, 0.866025),
        vec3(0.342020, 0.000000, 0.939693), vec3(0.342020, 0.000000, 0.939693),
        vec3(0.336824, 0.059391, 0.939693), vec3(0.336824, 0.059391, 0.939693),
        vec3(0.173648, 0.000000, 0.984808), vec3(0.173648, 0.000000, 0.984808),
        vec3(0.171010, 0.030154, 0.984808), vec3(0.171010, 0.030154, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(0.171010, 0.030154, 0.984808), vec3(0.171010, 0.030154, 0.984808),
        vec3(0.163176, 0.059391, 0.984808), vec3(0.163176, 0.059391, 0.984808),
        vec3(0.336824, 0.059391, 0.939693), vec3(0.336824, 0.059391, 0.939693),
        vec3(0.321394, 0.116978, 0.939693), vec3(0.321394, 0.116978, 0.939693),
        vec3(0.492404, 0.086824, 0.866025), vec3(0.492404, 0.086824, 0.866025),
        vec3(0.469846, 0.171010, 0.866025), vec3(0.469846, 0.171010, 0.866025),
        vec3(0.633022, 0.111619, 0.766044), vec3(0.633022, 0.111619, 0.766044),
        vec3(0.604023, 0.219846, 0.766044), vec3(0.604023, 0.219846, 0.766044),
        vec3(0.754407, 0.133022, 0.642788), vec3(0.754407, 0.133022, 0.642788),
        vec3(0.719846, 0.262003, 0.642788), vec3(0.719846, 0.262003, 0.642788),
        vec3(0.852869, 0.150384, 0.500000), vec3(0.852869, 0.150384, 0.500000),
        vec3(0.813798, 0.296198, 0.500000), vec3(0.813798, 0.296198, 0.500000),
        vec3(0.925417, 0.163176, 0.342020), vec3(0.925417, 0.163176, 0.342020),
        vec3(0.883022, 0.321394, 0.342020), vec3(0.883022, 0.321394, 0.342020),
        vec3(0.969846, 0.171010, 0.173648), vec3(0.969846, 0.171010, 0.173648),
        vec3(0.925417, 0.336824, 0.173648), vec3(0.925417, 0.336824, 0.173648),
        vec3(0.984808, 0.173648, 0.000000), vec3(0.984808, 0.173648, 0.000000),
        vec3(0.939693, 0.342020, 0.000000), vec3(0.939693, 0.342020, 0.000000),
        vec3(0.969846, 0.171010, -0.173648), vec3(0.969846, 0.171010, -0.173648),
        vec3(0.925417, 0.336824, -0.173648), vec3(0.925417, 0.336824, -0.173648),
        vec3(0.925417, 0.163176, -0.342020), vec3(0.925417, 0.163176, -0.342020),
        vec3(0.883022, 0.321394, -0.342020), vec3(0.883022, 0.321394, -0.342020),
        vec3(0.852869, 0.150384, -0.500000), vec3(0.852869, 0.150384, -0.500000),
        vec3(0.813798, 0.296198, -0.500000), vec3(0.813798, 0.296198, -0.500000),
        vec3(0.754407, 0.133022, -0.642788), vec3(0.754407, 0.133022, -0.642788),
        vec3(0.719846, 0.262003, -0.642788), vec3(0.719846, 0.262003, -0.642788),
        vec3(0.633022, 0.111619, -0.766044), vec3(0.633022, 0.111619, -0.766044),
        vec3(0.604023, 0.219846, -0.766044), vec3(0.604023, 0.219846, -0.766044),
        vec3(0.492404, 0.086824, -0.866025), vec3(0.492404, 0.086824, -0.866025),
        vec3(0.469846, 0.171010, -0.866025), vec3(0.469846, 0.171010, -0.866025),
        vec3(0.336824, 0.059391, -0.939693), vec3(0.336824, 0.059391, -0.939693),
        vec3(0.321394, 0.116978, -0.939693), vec3(0.321394, 0.116978, -0.939693),
        vec3(0.171010, 0.030154, -0.984808), vec3(0.171010, 0.030154, -0.984808),
        vec3(0.163176, 0.059391, -0.984808), vec3(0.163176, 0.059391, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(0.163176, 0.059391, -0.984808), vec3(0.163176, 0.059391, -0.984808),
        vec3(0.150384, 0.086824, -0.984808), vec3(0.150384, 0.086824, -0.984808),
        vec3(0.321394, 0.116978, -0.939693), vec3(0.321394, 0.116978, -0.939693),
        vec3(0.296198, 0.171010, -0.939693), vec3(0.296198, 0.171010, -0.939693),
        vec3(0.469846, 0.171010, -0.866025), vec3(0.469846, 0.171010, -0.866025),
        vec3(0.433013, 0.250000, -0.866025), vec3(0.433013, 0.250000, -0.866025),
        vec3(0.604023, 0.219846, -0.766044), vec3(0.604023, 0.219846, -0.766044),
        vec3(0.556670, 0.321394, -0.766044), vec3(0.556670, 0.321394, -0.766044),
        vec3(0.719846, 0.262003, -0.642788), vec3(0.719846, 0.262003, -0.642788),
        vec3(0.663414, 0.383022, -0.642788), vec3(0.663414, 0.383022, -0.642788),
        vec3(0.813798, 0.296198, -0.500000), vec3(0.813798, 0.296198, -0.500000),
        vec3(0.750000, 0.433013, -0.500000), vec3(0.750000, 0.433013, -0.500000),
        vec3(0.883022, 0.321394, -0.342020), vec3(0.883022, 0.321394, -0.342020),
        vec3(0.813798, 0.469846, -0.342020), vec3(0.813798, 0.469846, -0.342020),
        vec3(0.925417, 0.336824, -0.173648), vec3(0.925417, 0.336824, -0.173648),
        vec3(0.852869, 0.492404, -0.173648), vec3(0.852869, 0.492404, -0.173648),
        vec3(0.939693, 0.342020, 0.000000), vec3(0.939693, 0.342020, 0.000000),
        vec3(0.866025, 0.500000, 0.000000), vec3(0.866025, 0.500000, 0.000000),
        vec3(0.925417, 0.336824, 0.173648), vec3(0.925417, 0.336824, 0.173648),
        vec3(0.852869, 0.492404, 0.173648), vec3(0.852869, 0.492404, 0.173648),
        vec3(0.883022, 0.321394, 0.342020), vec3(0.883022, 0.321394, 0.342020),
        vec3(0.813798, 0.469846, 0.342020), vec3(0.813798, 0.469846, 0.342020),
        vec3(0.813798, 0.296198, 0.500000), vec3(0.813798, 0.296198, 0.500000),
        vec3(0.750000, 0.433013, 0.500000), vec3(0.750000, 0.433013, 0.500000),
        vec3(0.719846, 0.262003, 0.642788), vec3(0.719846, 0.262003, 0.642788),
        vec3(0.663414, 0.383022, 0.642788), vec3(0.663414, 0.383022, 0.642788),
        vec3(0.604023, 0.219846, 0.766044), vec3(0.604023, 0.219846, 0.766044),
        vec3(0.556670, 0.321394, 0.766044), vec3(0.556670, 0.321394, 0.766044),
        vec3(0.469846, 0.171010, 0.866025), vec3(0.469846, 0.171010, 0.866025),
        vec3(0.433013, 0.250000, 0.866025), vec3(0.433013, 0.250000, 0.866025),
        vec3(0.321394, 0.116978, 0.939693), vec3(0.321394, 0.116978, 0.939693),
        vec3(0.296198, 0.171010, 0.939693), vec3(0.296198, 0.171010, 0.939693),
        vec3(0.163176, 0.059391, 0.984808), vec3(0.163176, 0.059391, 0.984808),
        vec3(0.150384, 0.086824, 0.984808), vec3(0.150384, 0.086824, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(0.150384, 0.086824, 0.984808), vec3(0.150384, 0.086824, 0.984808),
        vec3(0.133022, 0.111619, 0.984808), vec3(0.133022, 0.111619, 0.984808),
        vec3(0.296198, 0.171010, 0.939693), vec3(0.296198, 0.171010, 0.939693),
        vec3(0.262003, 0.219846, 0.939693), vec3(0.262003, 0.219846, 0.939693),
        vec3(0.433013, 0.250000, 0.866025), vec3(0.433013, 0.250000, 0.866025),
        vec3(0.383022, 0.321394, 0.866025), vec3(0.383022, 0.321394, 0.866025),
        vec3(0.556670, 0.321394, 0.766044), vec3(0.556670, 0.321394, 0.766044),
        vec3(0.492404, 0.413176, 0.766044), vec3(0.492404, 0.413176, 0.766045),
        vec3(0.663414, 0.383022, 0.642788), vec3(0.663414, 0.383022, 0.642788),
        vec3(0.586824, 0.492404, 0.642788), vec3(0.586824, 0.492404, 0.642788),
        vec3(0.750000, 0.433013, 0.500000), vec3(0.750000, 0.433013, 0.500000),
        vec3(0.663414, 0.556670, 0.500000), vec3(0.663414, 0.556670, 0.500000),
        vec3(0.813798, 0.469846, 0.342020), vec3(0.813798, 0.469846, 0.342020),
        vec3(0.719846, 0.604023, 0.342020), vec3(0.719846, 0.604023, 0.342020),
        vec3(0.852869, 0.492404, 0.173648), vec3(0.852869, 0.492404, 0.173648),
        vec3(0.754407, 0.633022, 0.173648), vec3(0.754407, 0.633022, 0.173648),
        vec3(0.866025, 0.500000, 0.000000), vec3(0.866025, 0.500000, 0.000000),
        vec3(0.766044, 0.642788, 0.000000), vec3(0.766044, 0.642788, 0.000000),
        vec3(0.852869, 0.492404, -0.173648), vec3(0.852869, 0.492404, -0.173648),
        vec3(0.754407, 0.633022, -0.173648), vec3(0.754407, 0.633022, -0.173648),
        vec3(0.813798, 0.469846, -0.342020), vec3(0.813798, 0.469846, -0.342020),
        vec3(0.719846, 0.604023, -0.342020), vec3(0.719846, 0.604023, -0.342020),
        vec3(0.750000, 0.433013, -0.500000), vec3(0.750000, 0.433013, -0.500000),
        vec3(0.663414, 0.556670, -0.500000), vec3(0.663414, 0.556670, -0.500000),
        vec3(0.663414, 0.383022, -0.642788), vec3(0.663414, 0.383022, -0.642788),
        vec3(0.586824, 0.492404, -0.642788), vec3(0.586824, 0.492404, -0.642788),
        vec3(0.556670, 0.321394, -0.766044), vec3(0.556670, 0.321394, -0.766044),
        vec3(0.492404, 0.413176, -0.766044), vec3(0.492404, 0.413176, -0.766045),
        vec3(0.433013, 0.250000, -0.866025), vec3(0.433013, 0.250000, -0.866025),
        vec3(0.383022, 0.321394, -0.866025), vec3(0.383022, 0.321394, -0.866025),
        vec3(0.296198, 0.171010, -0.939693), vec3(0.296198, 0.171010, -0.939693),
        vec3(0.262003, 0.219846, -0.939693), vec3(0.262003, 0.219846, -0.939693),
        vec3(0.150384, 0.086824, -0.984808), vec3(0.150384, 0.086824, -0.984808),
        vec3(0.133022, 0.111619, -0.984808), vec3(0.133022, 0.111619, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(0.133022, 0.111619, -0.984808), vec3(0.133022, 0.111619, -0.984808),
        vec3(0.111619, 0.133022, -0.984808), vec3(0.111619, 0.133022, -0.984808),
        vec3(0.262003, 0.219846, -0.939693), vec3(0.262003, 0.219846, -0.939693),
        vec3(0.219846, 0.262003, -0.939693), vec3(0.219846, 0.262003, -0.939693),
        vec3(0.383022, 0.321394, -0.866025), vec3(0.383022, 0.321394, -0.866025),
        vec3(0.321394, 0.383022, -0.866025), vec3(0.321394, 0.383022, -0.866025),
        vec3(0.492404, 0.413176, -0.766044), vec3(0.492404, 0.413176, -0.766045),
        vec3(0.413176, 0.492404, -0.766044), vec3(0.413176, 0.492404, -0.766045),
        vec3(0.586824, 0.492404, -0.642788), vec3(0.586824, 0.492404, -0.642788),
        vec3(0.492404, 0.586824, -0.642788), vec3(0.492404, 0.586824, -0.642788),
        vec3(0.663414, 0.556670, -0.500000), vec3(0.663414, 0.556670, -0.500000),
        vec3(0.556670, 0.663414, -0.500000), vec3(0.556670, 0.663414, -0.500000),
        vec3(0.719846, 0.604023, -0.342020), vec3(0.719846, 0.604023, -0.342020),
        vec3(0.604023, 0.719846, -0.342020), vec3(0.604023, 0.719846, -0.342020),
        vec3(0.754407, 0.633022, -0.173648), vec3(0.754407, 0.633022, -0.173648),
        vec3(0.633022, 0.754407, -0.173648), vec3(0.633022, 0.754407, -0.173648),
        vec3(0.766044, 0.642788, 0.000000), vec3(0.766044, 0.642788, 0.000000),
        vec3(0.642788, 0.766044, 0.000000), vec3(0.642788, 0.766044, 0.000000),
        vec3(0.754407, 0.633022, 0.173648), vec3(0.754407, 0.633022, 0.173648),
        vec3(0.633022, 0.754407, 0.173648), vec3(0.633022, 0.754407, 0.173648),
        vec3(0.719846, 0.604023, 0.342020), vec3(0.719846, 0.604023, 0.342020),
        vec3(0.604023, 0.719846, 0.342020), vec3(0.604023, 0.719846, 0.342020),
        vec3(0.663414, 0.556670, 0.500000), vec3(0.663414, 0.556670, 0.500000),
        vec3(0.556670, 0.663414, 0.500000), vec3(0.556670, 0.663414, 0.500000),
        vec3(0.586824, 0.492404, 0.642788), vec3(0.586824, 0.492404, 0.642788),
        vec3(0.492404, 0.586824, 0.642788), vec3(0.492404, 0.586824, 0.642788),
        vec3(0.492404, 0.413176, 0.766044), vec3(0.492404, 0.413176, 0.766045),
        vec3(0.413176, 0.492404, 0.766044), vec3(0.413176, 0.492404, 0.766045),
        vec3(0.383022, 0.321394, 0.866025), vec3(0.383022, 0.321394, 0.866025),
        vec3(0.321394, 0.383022, 0.866025), vec3(0.321394, 0.383022, 0.866025),
        vec3(0.262003, 0.219846, 0.939693), vec3(0.262003, 0.219846, 0.939693),
        vec3(0.219846, 0.262003, 0.939693), vec3(0.219846, 0.262003, 0.939693),
        vec3(0.133022, 0.111619, 0.984808), vec3(0.133022, 0.111619, 0.984808),
        vec3(0.111619, 0.133022, 0.984808), vec3(0.111619, 0.133022, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(0.111619, 0.133022, 0.984808), vec3(0.111619, 0.133022, 0.984808),
        vec3(0.086824, 0.150384, 0.984808), vec3(0.086824, 0.150384, 0.984808),
        vec3(0.219846, 0.262003, 0.939693), vec3(0.219846, 0.262003, 0.939693),
        vec3(0.171010, 0.296198, 0.939693), vec3(0.171010, 0.296198, 0.939693),
        vec3(0.321394, 0.383022, 0.866025), vec3(0.321394, 0.383022, 0.866025),
        vec3(0.250000, 0.433013, 0.866025), vec3(0.250000, 0.433013, 0.866025),
        vec3(0.413176, 0.492404, 0.766044), vec3(0.413176, 0.492404, 0.766045),
        vec3(0.321394, 0.556670, 0.766044), vec3(0.321394, 0.556670, 0.766044),
        vec3(0.492404, 0.586824, 0.642788), vec3(0.492404, 0.586824, 0.642788),
        vec3(0.383022, 0.663414, 0.642788), vec3(0.383022, 0.663414, 0.642788),
        vec3(0.556670, 0.663414, 0.500000), vec3(0.556670, 0.663414, 0.500000),
        vec3(0.433013, 0.750000, 0.500000), vec3(0.433013, 0.750000, 0.500000),
        vec3(0.604023, 0.719846, 0.342020), vec3(0.604023, 0.719846, 0.342020),
        vec3(0.469846, 0.813798, 0.342020), vec3(0.469846, 0.813798, 0.342020),
        vec3(0.633022, 0.754407, 0.173648), vec3(0.633022, 0.754407, 0.173648),
        vec3(0.492404, 0.852869, 0.173648), vec3(0.492404, 0.852869, 0.173648),
        vec3(0.642788, 0.766044, 0.000000), vec3(0.642788, 0.766044, 0.000000),
        vec3(0.500000, 0.866025, 0.000000), vec3(0.500000, 0.866025, 0.000000),
        vec3(0.633022, 0.754407, -0.173648), vec3(0.633022, 0.754407, -0.173648),
        vec3(0.492404, 0.852869, -0.173648), vec3(0.492404, 0.852869, -0.173648),
        vec3(0.604023, 0.719846, -0.342020), vec3(0.604023, 0.719846, -0.342020),
        vec3(0.469846, 0.813798, -0.342020), vec3(0.469846, 0.813798, -0.342020),
        vec3(0.556670, 0.663414, -0.500000), vec3(0.556670, 0.663414, -0.500000),
        vec3(0.433013, 0.750000, -0.500000), vec3(0.433013, 0.750000, -0.500000),
        vec3(0.492404, 0.586824, -0.642788), vec3(0.492404, 0.586824, -0.642788),
        vec3(0.383022, 0.663414, -0.642788), vec3(0.383022, 0.663414, -0.642788),
        vec3(0.413176, 0.492404, -0.766044), vec3(0.413176, 0.492404, -0.766045),
        vec3(0.321394, 0.556670, -0.766044), vec3(0.321394, 0.556670, -0.766044),
        vec3(0.321394, 0.383022, -0.866025), vec3(0.321394, 0.383022, -0.866025),
        vec3(0.250000, 0.433013, -0.866025), vec3(0.250000, 0.433013, -0.866025),
        vec3(0.219846, 0.262003, -0.939693), vec3(0.219846, 0.262003, -0.939693),
        vec3(0.171010, 0.296198, -0.939693), vec3(0.171010, 0.296198, -0.939693),
        vec3(0.111619, 0.133022, -0.984808), vec3(0.111619, 0.133022, -0.984808),
        vec3(0.086824, 0.150384, -0.984808), vec3(0.086824, 0.150384, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(0.086824, 0.150384, -0.984808), vec3(0.086824, 0.150384, -0.984808),
        vec3(0.059391, 0.163176, -0.984808), vec3(0.059391, 0.163176, -0.984808),
        vec3(0.171010, 0.296198, -0.939693), vec3(0.171010, 0.296198, -0.939693),
        vec3(0.116978, 0.321394, -0.939693), vec3(0.116978, 0.321394, -0.939693),
        vec3(0.250000, 0.433013, -0.866025), vec3(0.250000, 0.433013, -0.866025),
        vec3(0.171010, 0.469846, -0.866025), vec3(0.171010, 0.469846, -0.866025),
        vec3(0.321394, 0.556670, -0.766044), vec3(0.321394, 0.556670, -0.766044),
        vec3(0.219846, 0.604023, -0.766044), vec3(0.219846, 0.604023, -0.766044),
        vec3(0.383022, 0.663414, -0.642788), vec3(0.383022, 0.663414, -0.642788),
        vec3(0.262003, 0.719846, -0.642788), vec3(0.262003, 0.719846, -0.642788),
        vec3(0.433013, 0.750000, -0.500000), vec3(0.433013, 0.750000, -0.500000),
        vec3(0.296198, 0.813798, -0.500000), vec3(0.296198, 0.813798, -0.500000),
        vec3(0.469846, 0.813798, -0.342020), vec3(0.469846, 0.813798, -0.342020),
        vec3(0.321394, 0.883022, -0.342020), vec3(0.321394, 0.883022, -0.342020),
        vec3(0.492404, 0.852869, -0.173648), vec3(0.492404, 0.852869, -0.173648),
        vec3(0.336824, 0.925417, -0.173648), vec3(0.336824, 0.925417, -0.173648),
        vec3(0.500000, 0.866025, 0.000000), vec3(0.500000, 0.866025, 0.000000),
        vec3(0.342020, 0.939693, 0.000000), vec3(0.342020, 0.939693, 0.000000),
        vec3(0.492404, 0.852869, 0.173648), vec3(0.492404, 0.852869, 0.173648),
        vec3(0.336824, 0.925417, 0.173648), vec3(0.336824, 0.925417, 0.173648),
        vec3(0.469846, 0.813798, 0.342020), vec3(0.469846, 0.813798, 0.342020),
        vec3(0.321394, 0.883022, 0.342020), vec3(0.321394, 0.883022, 0.342020),
        vec3(0.433013, 0.750000, 0.500000), vec3(0.433013, 0.750000, 0.500000),
        vec3(0.296198, 0.813798, 0.500000), vec3(0.296198, 0.813798, 0.500000),
        vec3(0.383022, 0.663414, 0.642788), vec3(0.383022, 0.663414, 0.642788),
        vec3(0.262003, 0.719846, 0.642788), vec3(0.262003, 0.719846, 0.642788),
        vec3(0.321394, 0.556670, 0.766044), vec3(0.321394, 0.556670, 0.766044),
        vec3(0.219846, 0.604023, 0.766044), vec3(0.219846, 0.604023, 0.766044),
        vec3(0.250000, 0.433013, 0.866025), vec3(0.250000, 0.433013, 0.866025),
        vec3(0.171010, 0.469846, 0.866025), vec3(0.171010, 0.469846, 0.866025),
        vec3(0.171010, 0.296198, 0.939693), vec3(0.171010, 0.296198, 0.939693),
        vec3(0.116978, 0.321394, 0.939693), vec3(0.116978, 0.321394, 0.939693),
        vec3(0.086824, 0.150384, 0.984808), vec3(0.086824, 0.150384, 0.984808),
        vec3(0.059391, 0.163176, 0.984808), vec3(0.059391, 0.163176, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(0.059391, 0.163176, 0.984808), vec3(0.059391, 0.163176, 0.984808),
        vec3(0.030154, 0.171010, 0.984808), vec3(0.030154, 0.171010, 0.984808),
        vec3(0.116978, 0.321394, 0.939693), vec3(0.116978, 0.321394, 0.939693),
        vec3(0.059391, 0.336824, 0.939693), vec3(0.059391, 0.336824, 0.939693),
        vec3(0.171010, 0.469846, 0.866025), vec3(0.171010, 0.469846, 0.866025),
        vec3(0.086824, 0.492404, 0.866025), vec3(0.086824, 0.492404, 0.866025),
        vec3(0.219846, 0.604023, 0.766044), vec3(0.219846, 0.604023, 0.766044),
        vec3(0.111619, 0.633022, 0.766044), vec3(0.111619, 0.633022, 0.766044),
        vec3(0.262003, 0.719846, 0.642788), vec3(0.262003, 0.719846, 0.642788),
        vec3(0.133022, 0.754407, 0.642788), vec3(0.133022, 0.754407, 0.642788),
        vec3(0.296198, 0.813798, 0.500000), vec3(0.296198, 0.813798, 0.500000),
        vec3(0.150384, 0.852869, 0.500000), vec3(0.150384, 0.852869, 0.500000),
        vec3(0.321394, 0.883022, 0.342020), vec3(0.321394, 0.883022, 0.342020),
        vec3(0.163176, 0.925417, 0.342020), vec3(0.163176, 0.925417, 0.342020),
        vec3(0.336824, 0.925417, 0.173648), vec3(0.336824, 0.925417, 0.173648),
        vec3(0.171010, 0.969846, 0.173648), vec3(0.171010, 0.969846, 0.173648),
        vec3(0.342020, 0.939693, 0.000000), vec3(0.342020, 0.939693, 0.000000),
        vec3(0.173648, 0.984808, 0.000000), vec3(0.173648, 0.984808, 0.000000),
        vec3(0.336824, 0.925417, -0.173648), vec3(0.336824, 0.925417, -0.173648),
        vec3(0.171010, 0.969846, -0.173648), vec3(0.171010, 0.969846, -0.173648),
        vec3(0.321394, 0.883022, -0.342020), vec3(0.321394, 0.883022, -0.342020),
        vec3(0.163176, 0.925417, -0.342020), vec3(0.163176, 0.925417, -0.342020),
        vec3(0.296198, 0.813798, -0.500000), vec3(0.296198, 0.813798, -0.500000),
        vec3(0.150384, 0.852869, -0.500000), vec3(0.150384, 0.852869, -0.500000),
        vec3(0.262003, 0.719846, -0.642788), vec3(0.262003, 0.719846, -0.642788),
        vec3(0.133022, 0.754407, -0.642788), vec3(0.133022, 0.754407, -0.642788),
        vec3(0.219846, 0.604023, -0.766044), vec3(0.219846, 0.604023, -0.766044),
        vec3(0.111619, 0.633022, -0.766044), vec3(0.111619, 0.633022, -0.766044),
        vec3(0.171010, 0.469846, -0.866025), vec3(0.171010, 0.469846, -0.866025),
        vec3(0.086824, 0.492404, -0.866025), vec3(0.086824, 0.492404, -0.866025),
        vec3(0.116978, 0.321394, -0.939693), vec3(0.116978, 0.321394, -0.939693),
        vec3(0.059391, 0.336824, -0.939693), vec3(0.059391, 0.336824, -0.939693),
        vec3(0.059391, 0.163176, -0.984808), vec3(0.059391, 0.163176, -0.984808),
        vec3(0.030154, 0.171010, -0.984808), vec3(0.030154, 0.171010, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(0.030154, 0.171010, -0.984808), vec3(0.030154, 0.171010, -0.984808),
        vec3(0.000000, 0.173648, -0.984808), vec3(0.000000, 0.173648, -0.984808),
        vec3(0.059391, 0.336824, -0.939693), vec3(0.059391, 0.336824, -0.939693),
        vec3(0.000000, 0.342020, -0.939693), vec3(0.000000, 0.342020, -0.939693),
        vec3(0.086824, 0.492404, -0.866025), vec3(0.086824, 0.492404, -0.866025),
        vec3(0.000000, 0.500000, -0.866025), vec3(0.000000, 0.500000, -0.866025),
        vec3(0.111619, 0.633022, -0.766044), vec3(0.111619, 0.633022, -0.766044),
        vec3(0.000000, 0.642788, -0.766044), vec3(0.000000, 0.642788, -0.766044),
        vec3(0.133022, 0.754407, -0.642788), vec3(0.133022, 0.754407, -0.642788),
        vec3(0.000000, 0.766044, -0.642788), vec3(0.000000, 0.766044, -0.642788),
        vec3(0.150384, 0.852869, -0.500000), vec3(0.150384, 0.852869, -0.500000),
        vec3(0.000000, 0.866025, -0.500000), vec3(0.000000, 0.866025, -0.500000),
        vec3(0.163176, 0.925417, -0.342020), vec3(0.163176, 0.925417, -0.342020),
        vec3(0.000000, 0.939693, -0.342020), vec3(0.000000, 0.939693, -0.342020),
        vec3(0.171010, 0.969846, -0.173648), vec3(0.171010, 0.969846, -0.173648),
        vec3(0.000000, 0.984808, -0.173648), vec3(0.000000, 0.984808, -0.173648),
        vec3(0.173648, 0.984808, 0.000000), vec3(0.173648, 0.984808, 0.000000),
        vec3(0.000000, 1.000000, 0.000000), vec3(0.000000, 1.000000, 0.000000),
        vec3(0.171010, 0.969846, 0.173648), vec3(0.171010, 0.969846, 0.173648),
        vec3(0.000000, 0.984808, 0.173648), vec3(0.000000, 0.984808, 0.173648),
        vec3(0.163176, 0.925417, 0.342020), vec3(0.163176, 0.925417, 0.342020),
        vec3(0.000000, 0.939693, 0.342020), vec3(0.000000, 0.939693, 0.342020),
        vec3(0.150384, 0.852869, 0.500000), vec3(0.150384, 0.852869, 0.500000),
        vec3(0.000000, 0.866025, 0.500000), vec3(0.000000, 0.866025, 0.500000),
        vec3(0.133022, 0.754407, 0.642788), vec3(0.133022, 0.754407, 0.642788),
        vec3(0.000000, 0.766044, 0.642788), vec3(0.000000, 0.766044, 0.642788),
        vec3(0.111619, 0.633022, 0.766044), vec3(0.111619, 0.633022, 0.766044),
        vec3(0.000000, 0.642788, 0.766044), vec3(0.000000, 0.642788, 0.766044),
        vec3(0.086824, 0.492404, 0.866025), vec3(0.086824, 0.492404, 0.866025),
        vec3(0.000000, 0.500000, 0.866025), vec3(0.000000, 0.500000, 0.866025),
        vec3(0.059391, 0.336824, 0.939693), vec3(0.059391, 0.336824, 0.939693),
        vec3(0.000000, 0.342020, 0.939693), vec3(0.000000, 0.342020, 0.939693),
        vec3(0.030154, 0.171010, 0.984808), vec3(0.030154, 0.171010, 0.984808),
        vec3(0.000000, 0.173648, 0.984808), vec3(0.000000, 0.173648, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(0.000000, 0.173648, 0.984808), vec3(0.000000, 0.173648, 0.984808),
        vec3(-0.030154, 0.171010, 0.984808), vec3(-0.030154, 0.171010, 0.984808),
        vec3(0.000000, 0.342020, 0.939693), vec3(0.000000, 0.342020, 0.939693),
        vec3(-0.059391, 0.336824, 0.939693), vec3(-0.059391, 0.336824, 0.939693),
        vec3(0.000000, 0.500000, 0.866025), vec3(0.000000, 0.500000, 0.866025),
        vec3(-0.086824, 0.492404, 0.866025), vec3(-0.086824, 0.492404, 0.866025),
        vec3(0.000000, 0.642788, 0.766044), vec3(0.000000, 0.642788, 0.766044),
        vec3(-0.111619, 0.633022, 0.766044), vec3(-0.111619, 0.633022, 0.766044),
        vec3(0.000000, 0.766044, 0.642788), vec3(0.000000, 0.766044, 0.642788),
        vec3(-0.133022, 0.754407, 0.642788), vec3(-0.133022, 0.754407, 0.642788),
        vec3(0.000000, 0.866025, 0.500000), vec3(0.000000, 0.866025, 0.500000),
        vec3(-0.150384, 0.852869, 0.500000), vec3(-0.150384, 0.852869, 0.500000),
        vec3(0.000000, 0.939693, 0.342020), vec3(0.000000, 0.939693, 0.342020),
        vec3(-0.163176, 0.925417, 0.342020), vec3(-0.163176, 0.925417, 0.342020),
        vec3(0.000000, 0.984808, 0.173648), vec3(0.000000, 0.984808, 0.173648),
        vec3(-0.171010, 0.969846, 0.173648), vec3(-0.171010, 0.969846, 0.173648),
        vec3(0.000000, 1.000000, 0.000000), vec3(0.000000, 1.000000, 0.000000),
        vec3(-0.173648, 0.984808, 0.000000), vec3(-0.173648, 0.984808, 0.000000),
        vec3(0.000000, 0.984808, -0.173648), vec3(0.000000, 0.984808, -0.173648),
        vec3(-0.171010, 0.969846, -0.173648), vec3(-0.171010, 0.969846, -0.173648),
        vec3(0.000000, 0.939693, -0.342020), vec3(0.000000, 0.939693, -0.342020),
        vec3(-0.163176, 0.925417, -0.342020), vec3(-0.163176, 0.925417, -0.342020),
        vec3(0.000000, 0.866025, -0.500000), vec3(0.000000, 0.866025, -0.500000),
        vec3(-0.150384, 0.852869, -0.500000), vec3(-0.150384, 0.852869, -0.500000),
        vec3(0.000000, 0.766044, -0.642788), vec3(0.000000, 0.766044, -0.642788),
        vec3(-0.133022, 0.754407, -0.642788), vec3(-0.133022, 0.754407, -0.642788),
        vec3(0.000000, 0.642788, -0.766044), vec3(0.000000, 0.642788, -0.766044),
        vec3(-0.111619, 0.633022, -0.766044), vec3(-0.111619, 0.633022, -0.766044),
        vec3(0.000000, 0.500000, -0.866025), vec3(0.000000, 0.500000, -0.866025),
        vec3(-0.086824, 0.492404, -0.866025), vec3(-0.086824, 0.492404, -0.866025),
        vec3(0.000000, 0.342020, -0.939693), vec3(0.000000, 0.342020, -0.939693),
        vec3(-0.059391, 0.336824, -0.939693), vec3(-0.059391, 0.336824, -0.939693),
        vec3(0.000000, 0.173648, -0.984808), vec3(0.000000, 0.173648, -0.984808),
        vec3(-0.030154, 0.171010, -0.984808), vec3(-0.030154, 0.171010, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.030154, 0.171010, -0.984808), vec3(-0.030154, 0.171010, -0.984808),
        vec3(-0.059391, 0.163176, -0.984808), vec3(-0.059391, 0.163176, -0.984808),
        vec3(-0.059391, 0.336824, -0.939693), vec3(-0.059391, 0.336824, -0.939693),
        vec3(-0.116978, 0.321394, -0.939693), vec3(-0.116978, 0.321394, -0.939693),
        vec3(-0.086824, 0.492404, -0.866025), vec3(-0.086824, 0.492404, -0.866025),
        vec3(-0.171010, 0.469846, -0.866025), vec3(-0.171010, 0.469846, -0.866025),
        vec3(-0.111619, 0.633022, -0.766044), vec3(-0.111619, 0.633022, -0.766044),
        vec3(-0.219846, 0.604023, -0.766044), vec3(-0.219846, 0.604023, -0.766044),
        vec3(-0.133022, 0.754407, -0.642788), vec3(-0.133022, 0.754407, -0.642788),
        vec3(-0.262003, 0.719846, -0.642788), vec3(-0.262003, 0.719846, -0.642788),
        vec3(-0.150384, 0.852869, -0.500000), vec3(-0.150384, 0.852869, -0.500000),
        vec3(-0.296198, 0.813798, -0.500000), vec3(-0.296198, 0.813798, -0.500000),
        vec3(-0.163176, 0.925417, -0.342020), vec3(-0.163176, 0.925417, -0.342020),
        vec3(-0.321394, 0.883022, -0.342020), vec3(-0.321394, 0.883022, -0.342020),
        vec3(-0.171010, 0.969846, -0.173648), vec3(-0.171010, 0.969846, -0.173648),
        vec3(-0.336824, 0.925417, -0.173648), vec3(-0.336824, 0.925417, -0.173648),
        vec3(-0.173648, 0.984808, 0.000000), vec3(-0.173648, 0.984808, 0.000000),
        vec3(-0.342020, 0.939693, 0.000000), vec3(-0.342020, 0.939693, 0.000000),
        vec3(-0.171010, 0.969846, 0.173648), vec3(-0.171010, 0.969846, 0.173648),
        vec3(-0.336824, 0.925417, 0.173648), vec3(-0.336824, 0.925417, 0.173648),
        vec3(-0.163176, 0.925417, 0.342020), vec3(-0.163176, 0.925417, 0.342020),
        vec3(-0.321394, 0.883022, 0.342020), vec3(-0.321394, 0.883022, 0.342020),
        vec3(-0.150384, 0.852869, 0.500000), vec3(-0.150384, 0.852869, 0.500000),
        vec3(-0.296198, 0.813798, 0.500000), vec3(-0.296198, 0.813798, 0.500000),
        vec3(-0.133022, 0.754407, 0.642788), vec3(-0.133022, 0.754407, 0.642788),
        vec3(-0.262003, 0.719846, 0.642788), vec3(-0.262003, 0.719846, 0.642788),
        vec3(-0.111619, 0.633022, 0.766044), vec3(-0.111619, 0.633022, 0.766044),
        vec3(-0.219846, 0.604023, 0.766044), vec3(-0.219846, 0.604023, 0.766044),
        vec3(-0.086824, 0.492404, 0.866025), vec3(-0.086824, 0.492404, 0.866025),
        vec3(-0.171010, 0.469846, 0.866025), vec3(-0.171010, 0.469846, 0.866025),
        vec3(-0.059391, 0.336824, 0.939693), vec3(-0.059391, 0.336824, 0.939693),
        vec3(-0.116978, 0.321394, 0.939693), vec3(-0.116978, 0.321394, 0.939693),
        vec3(-0.030154, 0.171010, 0.984808), vec3(-0.030154, 0.171010, 0.984808),
        vec3(-0.059391, 0.163176, 0.984808), vec3(-0.059391, 0.163176, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.059391, 0.163176, 0.984808), vec3(-0.059391, 0.163176, 0.984808),
        vec3(-0.086824, 0.150384, 0.984808), vec3(-0.086824, 0.150384, 0.984808),
        vec3(-0.116978, 0.321394, 0.939693), vec3(-0.116978, 0.321394, 0.939693),
        vec3(-0.171010, 0.296198, 0.939693), vec3(-0.171010, 0.296198, 0.939693),
        vec3(-0.171010, 0.469846, 0.866025), vec3(-0.171010, 0.469846, 0.866025),
        vec3(-0.250000, 0.433013, 0.866025), vec3(-0.250000, 0.433013, 0.866025),
        vec3(-0.219846, 0.604023, 0.766044), vec3(-0.219846, 0.604023, 0.766044),
        vec3(-0.321394, 0.556670, 0.766044), vec3(-0.321394, 0.556670, 0.766044),
        vec3(-0.262003, 0.719846, 0.642788), vec3(-0.262003, 0.719846, 0.642788),
        vec3(-0.383022, 0.663414, 0.642788), vec3(-0.383022, 0.663414, 0.642788),
        vec3(-0.296198, 0.813798, 0.500000), vec3(-0.296198, 0.813798, 0.500000),
        vec3(-0.433013, 0.750000, 0.500000), vec3(-0.433013, 0.750000, 0.500000),
        vec3(-0.321394, 0.883022, 0.342020), vec3(-0.321394, 0.883022, 0.342020),
        vec3(-0.469846, 0.813798, 0.342020), vec3(-0.469846, 0.813798, 0.342020),
        vec3(-0.336824, 0.925417, 0.173648), vec3(-0.336824, 0.925417, 0.173648),
        vec3(-0.492404, 0.852869, 0.173648), vec3(-0.492404, 0.852869, 0.173648),
        vec3(-0.342020, 0.939693, 0.000000), vec3(-0.342020, 0.939693, 0.000000),
        vec3(-0.500000, 0.866025, 0.000000), vec3(-0.500000, 0.866025, 0.000000),
        vec3(-0.336824, 0.925417, -0.173648), vec3(-0.336824, 0.925417, -0.173648),
        vec3(-0.492404, 0.852869, -0.173648), vec3(-0.492404, 0.852869, -0.173648),
        vec3(-0.321394, 0.883022, -0.342020), vec3(-0.321394, 0.883022, -0.342020),
        vec3(-0.469846, 0.813798, -0.342020), vec3(-0.469846, 0.813798, -0.342020),
        vec3(-0.296198, 0.813798, -0.500000), vec3(-0.296198, 0.813798, -0.500000),
        vec3(-0.433013, 0.750000, -0.500000), vec3(-0.433013, 0.750000, -0.500000),
        vec3(-0.262003, 0.719846, -0.642788), vec3(-0.262003, 0.719846, -0.642788),
        vec3(-0.383022, 0.663414, -0.642788), vec3(-0.383022, 0.663414, -0.642788),
        vec3(-0.219846, 0.604023, -0.766044), vec3(-0.219846, 0.604023, -0.766044),
        vec3(-0.321394, 0.556670, -0.766044), vec3(-0.321394, 0.556670, -0.766044),
        vec3(-0.171010, 0.469846, -0.866025), vec3(-0.171010, 0.469846, -0.866025),
        vec3(-0.250000, 0.433013, -0.866025), vec3(-0.250000, 0.433013, -0.866025),
        vec3(-0.116978, 0.321394, -0.939693), vec3(-0.116978, 0.321394, -0.939693),
        vec3(-0.171010, 0.296198, -0.939693), vec3(-0.171010, 0.296198, -0.939693),
        vec3(-0.059391, 0.163176, -0.984808), vec3(-0.059391, 0.163176, -0.984808),
        vec3(-0.086824, 0.150384, -0.984808), vec3(-0.086824, 0.150384, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.086824, 0.150384, -0.984808), vec3(-0.086824, 0.150384, -0.984808),
        vec3(-0.111619, 0.133022, -0.984808), vec3(-0.111619, 0.133022, -0.984808),
        vec3(-0.171010, 0.296198, -0.939693), vec3(-0.171010, 0.296198, -0.939693),
        vec3(-0.219846, 0.262003, -0.939693), vec3(-0.219846, 0.262003, -0.939693),
        vec3(-0.250000, 0.433013, -0.866025), vec3(-0.250000, 0.433013, -0.866025),
        vec3(-0.321394, 0.383022, -0.866025), vec3(-0.321394, 0.383022, -0.866025),
        vec3(-0.321394, 0.556670, -0.766044), vec3(-0.321394, 0.556670, -0.766044),
        vec3(-0.413176, 0.492404, -0.766044), vec3(-0.413176, 0.492404, -0.766045),
        vec3(-0.383022, 0.663414, -0.642788), vec3(-0.383022, 0.663414, -0.642788),
        vec3(-0.492404, 0.586824, -0.642788), vec3(-0.492404, 0.586824, -0.642788),
        vec3(-0.433013, 0.750000, -0.500000), vec3(-0.433013, 0.750000, -0.500000),
        vec3(-0.556670, 0.663414, -0.500000), vec3(-0.556670, 0.663414, -0.500000),
        vec3(-0.469846, 0.813798, -0.342020), vec3(-0.469846, 0.813798, -0.342020),
        vec3(-0.604023, 0.719846, -0.342020), vec3(-0.604023, 0.719846, -0.342020),
        vec3(-0.492404, 0.852869, -0.173648), vec3(-0.492404, 0.852869, -0.173648),
        vec3(-0.633022, 0.754407, -0.173648), vec3(-0.633022, 0.754407, -0.173648),
        vec3(-0.500000, 0.866025, 0.000000), vec3(-0.500000, 0.866025, 0.000000),
        vec3(-0.642788, 0.766044, 0.000000), vec3(-0.642788, 0.766044, 0.000000),
        vec3(-0.492404, 0.852869, 0.173648), vec3(-0.492404, 0.852869, 0.173648),
        vec3(-0.633022, 0.754407, 0.173648), vec3(-0.633022, 0.754407, 0.173648),
        vec3(-0.469846, 0.813798, 0.342020), vec3(-0.469846, 0.813798, 0.342020),
        vec3(-0.604023, 0.719846, 0.342020), vec3(-0.604023, 0.719846, 0.342020),
        vec3(-0.433013, 0.750000, 0.500000), vec3(-0.433013, 0.750000, 0.500000),
        vec3(-0.556670, 0.663414, 0.500000), vec3(-0.556670, 0.663414, 0.500000),
        vec3(-0.383022, 0.663414, 0.642788), vec3(-0.383022, 0.663414, 0.642788),
        vec3(-0.492404, 0.586824, 0.642788), vec3(-0.492404, 0.586824, 0.642788),
        vec3(-0.321394, 0.556670, 0.766044), vec3(-0.321394, 0.556670, 0.766044),
        vec3(-0.413176, 0.492404, 0.766044), vec3(-0.413176, 0.492404, 0.766045),
        vec3(-0.250000, 0.433013, 0.866025), vec3(-0.250000, 0.433013, 0.866025),
        vec3(-0.321394, 0.383022, 0.866025), vec3(-0.321394, 0.383022, 0.866025),
        vec3(-0.171010, 0.296198, 0.939693), vec3(-0.171010, 0.296198, 0.939693),
        vec3(-0.219846, 0.262003, 0.939693), vec3(-0.219846, 0.262003, 0.939693),
        vec3(-0.086824, 0.150384, 0.984808), vec3(-0.086824, 0.150384, 0.984808),
        vec3(-0.111619, 0.133022, 0.984808), vec3(-0.111619, 0.133022, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.111619, 0.133022, 0.984808), vec3(-0.111619, 0.133022, 0.984808),
        vec3(-0.133022, 0.111619, 0.984808), vec3(-0.133022, 0.111619, 0.984808),
        vec3(-0.219846, 0.262003, 0.939693), vec3(-0.219846, 0.262003, 0.939693),
        vec3(-0.262003, 0.219846, 0.939693), vec3(-0.262003, 0.219846, 0.939693),
        vec3(-0.321394, 0.383022, 0.866025), vec3(-0.321394, 0.383022, 0.866025),
        vec3(-0.383022, 0.321394, 0.866025), vec3(-0.383022, 0.321394, 0.866025),
        vec3(-0.413176, 0.492404, 0.766044), vec3(-0.413176, 0.492404, 0.766045),
        vec3(-0.492404, 0.413176, 0.766044), vec3(-0.492404, 0.413176, 0.766045),
        vec3(-0.492404, 0.586824, 0.642788), vec3(-0.492404, 0.586824, 0.642788),
        vec3(-0.586824, 0.492404, 0.642788), vec3(-0.586824, 0.492404, 0.642788),
        vec3(-0.556670, 0.663414, 0.500000), vec3(-0.556670, 0.663414, 0.500000),
        vec3(-0.663414, 0.556670, 0.500000), vec3(-0.663414, 0.556670, 0.500000),
        vec3(-0.604023, 0.719846, 0.342020), vec3(-0.604023, 0.719846, 0.342020),
        vec3(-0.719846, 0.604023, 0.342020), vec3(-0.719846, 0.604023, 0.342020),
        vec3(-0.633022, 0.754407, 0.173648), vec3(-0.633022, 0.754407, 0.173648),
        vec3(-0.754407, 0.633022, 0.173648), vec3(-0.754407, 0.633022, 0.173648),
        vec3(-0.642788, 0.766044, 0.000000), vec3(-0.642788, 0.766044, 0.000000),
        vec3(-0.766044, 0.642788, 0.000000), vec3(-0.766044, 0.642788, 0.000000),
        vec3(-0.633022, 0.754407, -0.173648), vec3(-0.633022, 0.754407, -0.173648),
        vec3(-0.754407, 0.633022, -0.173648), vec3(-0.754407, 0.633022, -0.173648),
        vec3(-0.604023, 0.719846, -0.342020), vec3(-0.604023, 0.719846, -0.342020),
        vec3(-0.719846, 0.604023, -0.342020), vec3(-0.719846, 0.604023, -0.342020),
        vec3(-0.556670, 0.663414, -0.500000), vec3(-0.556670, 0.663414, -0.500000),
        vec3(-0.663414, 0.556670, -0.500000), vec3(-0.663414, 0.556670, -0.500000),
        vec3(-0.492404, 0.586824, -0.642788), vec3(-0.492404, 0.586824, -0.642788),
        vec3(-0.586824, 0.492404, -0.642788), vec3(-0.586824, 0.492404, -0.642788),
        vec3(-0.413176, 0.492404, -0.766044), vec3(-0.413176, 0.492404, -0.766045),
        vec3(-0.492404, 0.413176, -0.766044), vec3(-0.492404, 0.413176, -0.766045),
        vec3(-0.321394, 0.383022, -0.866025), vec3(-0.321394, 0.383022, -0.866025),
        vec3(-0.383022, 0.321394, -0.866025), vec3(-0.383022, 0.321394, -0.866025),
        vec3(-0.219846, 0.262003, -0.939693), vec3(-0.219846, 0.262003, -0.939693),
        vec3(-0.262003, 0.219846, -0.939693), vec3(-0.262003, 0.219846, -0.939693),
        vec3(-0.111619, 0.133022, -0.984808), vec3(-0.111619, 0.133022, -0.984808),
        vec3(-0.133022, 0.111619, -0.984808), vec3(-0.133022, 0.111619, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.133022, 0.111619, -0.984808), vec3(-0.133022, 0.111619, -0.984808),
        vec3(-0.150384, 0.086824, -0.984808), vec3(-0.150384, 0.086824, -0.984808),
        vec3(-0.262003, 0.219846, -0.939693), vec3(-0.262003, 0.219846, -0.939693),
        vec3(-0.296198, 0.171010, -0.939693), vec3(-0.296198, 0.171010, -0.939693),
        vec3(-0.383022, 0.321394, -0.866025), vec3(-0.383022, 0.321394, -0.866025),
        vec3(-0.433013, 0.250000, -0.866025), vec3(-0.433013, 0.250000, -0.866025),
        vec3(-0.492404, 0.413176, -0.766044), vec3(-0.492404, 0.413176, -0.766045),
        vec3(-0.556670, 0.321394, -0.766044), vec3(-0.556670, 0.321394, -0.766044),
        vec3(-0.586824, 0.492404, -0.642788), vec3(-0.586824, 0.492404, -0.642788),
        vec3(-0.663414, 0.383022, -0.642788), vec3(-0.663414, 0.383022, -0.642788),
        vec3(-0.663414, 0.556670, -0.500000), vec3(-0.663414, 0.556670, -0.500000),
        vec3(-0.750000, 0.433013, -0.500000), vec3(-0.750000, 0.433013, -0.500000),
        vec3(-0.719846, 0.604023, -0.342020), vec3(-0.719846, 0.604023, -0.342020),
        vec3(-0.813798, 0.469846, -0.342020), vec3(-0.813798, 0.469846, -0.342020),
        vec3(-0.754407, 0.633022, -0.173648), vec3(-0.754407, 0.633022, -0.173648),
        vec3(-0.852869, 0.492404, -0.173648), vec3(-0.852869, 0.492404, -0.173648),
        vec3(-0.766044, 0.642788, 0.000000), vec3(-0.766044, 0.642788, 0.000000),
        vec3(-0.866025, 0.500000, 0.000000), vec3(-0.866025, 0.500000, 0.000000),
        vec3(-0.754407, 0.633022, 0.173648), vec3(-0.754407, 0.633022, 0.173648),
        vec3(-0.852869, 0.492404, 0.173648), vec3(-0.852869, 0.492404, 0.173648),
        vec3(-0.719846, 0.604023, 0.342020), vec3(-0.719846, 0.604023, 0.342020),
        vec3(-0.813798, 0.469846, 0.342020), vec3(-0.813798, 0.469846, 0.342020),
        vec3(-0.663414, 0.556670, 0.500000), vec3(-0.663414, 0.556670, 0.500000),
        vec3(-0.750000, 0.433013, 0.500000), vec3(-0.750000, 0.433013, 0.500000),
        vec3(-0.586824, 0.492404, 0.642788), vec3(-0.586824, 0.492404, 0.642788),
        vec3(-0.663414, 0.383022, 0.642788), vec3(-0.663414, 0.383022, 0.642788),
        vec3(-0.492404, 0.413176, 0.766044), vec3(-0.492404, 0.413176, 0.766045),
        vec3(-0.556670, 0.321394, 0.766044), vec3(-0.556670, 0.321394, 0.766044),
        vec3(-0.383022, 0.321394, 0.866025), vec3(-0.383022, 0.321394, 0.866025),
        vec3(-0.433013, 0.250000, 0.866025), vec3(-0.433013, 0.250000, 0.866025),
        vec3(-0.262003, 0.219846, 0.939693), vec3(-0.262003, 0.219846, 0.939693),
        vec3(-0.296198, 0.171010, 0.939693), vec3(-0.296198, 0.171010, 0.939693),
        vec3(-0.133022, 0.111619, 0.984808), vec3(-0.133022, 0.111619, 0.984808),
        vec3(-0.150384, 0.086824, 0.984808), vec3(-0.150384, 0.086824, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.150384, 0.086824, 0.984808), vec3(-0.150384, 0.086824, 0.984808),
        vec3(-0.163176, 0.059391, 0.984808), vec3(-0.163176, 0.059391, 0.984808),
        vec3(-0.296198, 0.171010, 0.939693), vec3(-0.296198, 0.171010, 0.939693),
        vec3(-0.321394, 0.116978, 0.939693), vec3(-0.321394, 0.116978, 0.939693),
        vec3(-0.433013, 0.250000, 0.866025), vec3(-0.433013, 0.250000, 0.866025),
        vec3(-0.469846, 0.171010, 0.866025), vec3(-0.469846, 0.171010, 0.866025),
        vec3(-0.556670, 0.321394, 0.766044), vec3(-0.556670, 0.321394, 0.766044),
        vec3(-0.604023, 0.219846, 0.766044), vec3(-0.604023, 0.219846, 0.766044),
        vec3(-0.663414, 0.383022, 0.642788), vec3(-0.663414, 0.383022, 0.642788),
        vec3(-0.719846, 0.262003, 0.642788), vec3(-0.719846, 0.262003, 0.642788),
        vec3(-0.750000, 0.433013, 0.500000), vec3(-0.750000, 0.433013, 0.500000),
        vec3(-0.813798, 0.296198, 0.500000), vec3(-0.813798, 0.296198, 0.500000),
        vec3(-0.813798, 0.469846, 0.342020), vec3(-0.813798, 0.469846, 0.342020),
        vec3(-0.883022, 0.321394, 0.342020), vec3(-0.883022, 0.321394, 0.342020),
        vec3(-0.852869, 0.492404, 0.173648), vec3(-0.852869, 0.492404, 0.173648),
        vec3(-0.925417, 0.336824, 0.173648), vec3(-0.925417, 0.336824, 0.173648),
        vec3(-0.866025, 0.500000, 0.000000), vec3(-0.866025, 0.500000, 0.000000),
        vec3(-0.939693, 0.342020, 0.000000), vec3(-0.939693, 0.342020, 0.000000),
        vec3(-0.852869, 0.492404, -0.173648), vec3(-0.852869, 0.492404, -0.173648),
        vec3(-0.925417, 0.336824, -0.173648), vec3(-0.925417, 0.336824, -0.173648),
        vec3(-0.813798, 0.469846, -0.342020), vec3(-0.813798, 0.469846, -0.342020),
        vec3(-0.883022, 0.321394, -0.342020), vec3(-0.883022, 0.321394, -0.342020),
        vec3(-0.750000, 0.433013, -0.500000), vec3(-0.750000, 0.433013, -0.500000),
        vec3(-0.813798, 0.296198, -0.500000), vec3(-0.813798, 0.296198, -0.500000),
        vec3(-0.663414, 0.383022, -0.642788), vec3(-0.663414, 0.383022, -0.642788),
        vec3(-0.719846, 0.262003, -0.642788), vec3(-0.719846, 0.262003, -0.642788),
        vec3(-0.556670, 0.321394, -0.766044), vec3(-0.556670, 0.321394, -0.766044),
        vec3(-0.604023, 0.219846, -0.766044), vec3(-0.604023, 0.219846, -0.766044),
        vec3(-0.433013, 0.250000, -0.866025), vec3(-0.433013, 0.250000, -0.866025),
        vec3(-0.469846, 0.171010, -0.866025), vec3(-0.469846, 0.171010, -0.866025),
        vec3(-0.296198, 0.171010, -0.939693), vec3(-0.296198, 0.171010, -0.939693),
        vec3(-0.321394, 0.116978, -0.939693), vec3(-0.321394, 0.116978, -0.939693),
        vec3(-0.150384, 0.086824, -0.984808), vec3(-0.150384, 0.086824, -0.984808),
        vec3(-0.163176, 0.059391, -0.984808), vec3(-0.163176, 0.059391, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.163176, 0.059391, -0.984808), vec3(-0.163176, 0.059391, -0.984808),
        vec3(-0.171010, 0.030154, -0.984808), vec3(-0.171010, 0.030154, -0.984808),
        vec3(-0.321394, 0.116978, -0.939693), vec3(-0.321394, 0.116978, -0.939693),
        vec3(-0.336824, 0.059391, -0.939693), vec3(-0.336824, 0.059391, -0.939693),
        vec3(-0.469846, 0.171010, -0.866025), vec3(-0.469846, 0.171010, -0.866025),
        vec3(-0.492404, 0.086824, -0.866025), vec3(-0.492404, 0.086824, -0.866025),
        vec3(-0.604023, 0.219846, -0.766044), vec3(-0.604023, 0.219846, -0.766044),
        vec3(-0.633022, 0.111619, -0.766044), vec3(-0.633022, 0.111619, -0.766044),
        vec3(-0.719846, 0.262003, -0.642788), vec3(-0.719846, 0.262003, -0.642788),
        vec3(-0.754407, 0.133022, -0.642788), vec3(-0.754407, 0.133022, -0.642788),
        vec3(-0.813798, 0.296198, -0.500000), vec3(-0.813798, 0.296198, -0.500000),
        vec3(-0.852869, 0.150384, -0.500000), vec3(-0.852869, 0.150384, -0.500000),
        vec3(-0.883022, 0.321394, -0.342020), vec3(-0.883022, 0.321394, -0.342020),
        vec3(-0.925417, 0.163176, -0.342020), vec3(-0.925417, 0.163176, -0.342020),
        vec3(-0.925417, 0.336824, -0.173648), vec3(-0.925417, 0.336824, -0.173648),
        vec3(-0.969846, 0.171010, -0.173648), vec3(-0.969846, 0.171010, -0.173648),
        vec3(-0.939693, 0.342020, 0.000000), vec3(-0.939693, 0.342020, 0.000000),
        vec3(-0.984808, 0.173648, 0.000000), vec3(-0.984808, 0.173648, 0.000000),
        vec3(-0.925417, 0.336824, 0.173648), vec3(-0.925417, 0.336824, 0.173648),
        vec3(-0.969846, 0.171010, 0.173648), vec3(-0.969846, 0.171010, 0.173648),
        vec3(-0.883022, 0.321394, 0.342020), vec3(-0.883022, 0.321394, 0.342020),
        vec3(-0.925417, 0.163176, 0.342020), vec3(-0.925417, 0.163176, 0.342020),
        vec3(-0.813798, 0.296198, 0.500000), vec3(-0.813798, 0.296198, 0.500000),
        vec3(-0.852869, 0.150384, 0.500000), vec3(-0.852869, 0.150384, 0.500000),
        vec3(-0.719846, 0.262003, 0.642788), vec3(-0.719846, 0.262003, 0.642788),
        vec3(-0.754407, 0.133022, 0.642788), vec3(-0.754407, 0.133022, 0.642788),
        vec3(-0.604023, 0.219846, 0.766044), vec3(-0.604023, 0.219846, 0.766044),
        vec3(-0.633022, 0.111619, 0.766044), vec3(-0.633022, 0.111619, 0.766044),
        vec3(-0.469846, 0.171010, 0.866025), vec3(-0.469846, 0.171010, 0.866025),
        vec3(-0.492404, 0.086824, 0.866025), vec3(-0.492404, 0.086824, 0.866025),
        vec3(-0.321394, 0.116978, 0.939693), vec3(-0.321394, 0.116978, 0.939693),
        vec3(-0.336824, 0.059391, 0.939693), vec3(-0.336824, 0.059391, 0.939693),
        vec3(-0.163176, 0.059391, 0.984808), vec3(-0.163176, 0.059391, 0.984808),
        vec3(-0.171010, 0.030154, 0.984808), vec3(-0.171010, 0.030154, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.171010, 0.030154, 0.984808), vec3(-0.171010, 0.030154, 0.984808),
        vec3(-0.173648, 0.000000, 0.984808), vec3(-0.173648, 0.000000, 0.984808),
        vec3(-0.336824, 0.059391, 0.939693), vec3(-0.336824, 0.059391, 0.939693),
        vec3(-0.342020, 0.000000, 0.939693), vec3(-0.342020, 0.000000, 0.939693),
        vec3(-0.492404, 0.086824, 0.866025), vec3(-0.492404, 0.086824, 0.866025),
        vec3(-0.500000, 0.000000, 0.866025), vec3(-0.500000, 0.000000, 0.866025),
        vec3(-0.633022, 0.111619, 0.766044), vec3(-0.633022, 0.111619, 0.766044),
        vec3(-0.642788, 0.000000, 0.766044), vec3(-0.642788, 0.000000, 0.766044),
        vec3(-0.754407, 0.133022, 0.642788), vec3(-0.754407, 0.133022, 0.642788),
        vec3(-0.766044, 0.000000, 0.642788), vec3(-0.766044, 0.000000, 0.642788),
        vec3(-0.852869, 0.150384, 0.500000), vec3(-0.852869, 0.150384, 0.500000),
        vec3(-0.866025, 0.000000, 0.500000), vec3(-0.866025, 0.000000, 0.500000),
        vec3(-0.925417, 0.163176, 0.342020), vec3(-0.925417, 0.163176, 0.342020),
        vec3(-0.939693, 0.000000, 0.342020), vec3(-0.939693, 0.000000, 0.342020),
        vec3(-0.969846, 0.171010, 0.173648), vec3(-0.969846, 0.171010, 0.173648),
        vec3(-0.984808, 0.000000, 0.173648), vec3(-0.984808, 0.000000, 0.173648),
        vec3(-0.984808, 0.173648, 0.000000), vec3(-0.984808, 0.173648, 0.000000),
        vec3(-1.000000, 0.000000, 0.000000), vec3(-1.000000, 0.000000, 0.000000),
        vec3(-0.969846, 0.171010, -0.173648), vec3(-0.969846, 0.171010, -0.173648),
        vec3(-0.984808, 0.000000, -0.173648), vec3(-0.984808, 0.000000, -0.173648),
        vec3(-0.925417, 0.163176, -0.342020), vec3(-0.925417, 0.163176, -0.342020),
        vec3(-0.939693, 0.000000, -0.342020), vec3(-0.939693, 0.000000, -0.342020),
        vec3(-0.852869, 0.150384, -0.500000), vec3(-0.852869, 0.150384, -0.500000),
        vec3(-0.866025, 0.000000, -0.500000), vec3(-0.866025, 0.000000, -0.500000),
        vec3(-0.754407, 0.133022, -0.642788), vec3(-0.754407, 0.133022, -0.642788),
        vec3(-0.766044, 0.000000, -0.642788), vec3(-0.766044, 0.000000, -0.642788),
        vec3(-0.633022, 0.111619, -0.766044), vec3(-0.633022, 0.111619, -0.766044),
        vec3(-0.642788, 0.000000, -0.766044), vec3(-0.642788, 0.000000, -0.766044),
        vec3(-0.492404, 0.086824, -0.866025), vec3(-0.492404, 0.086824, -0.866025),
        vec3(-0.500000, 0.000000, -0.866025), vec3(-0.500000, 0.000000, -0.866025),
        vec3(-0.336824, 0.059391, -0.939693), vec3(-0.336824, 0.059391, -0.939693),
        vec3(-0.342020, 0.000000, -0.939693), vec3(-0.342020, 0.000000, -0.939693),
        vec3(-0.171010, 0.030154, -0.984808), vec3(-0.171010, 0.030154, -0.984808),
        vec3(-0.173648, 0.000000, -0.984808), vec3(-0.173648, 0.000000, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.173648, 0.000000, -0.984808), vec3(-0.173648, 0.000000, -0.984808),
        vec3(-0.171010, -0.030154, -0.984808), vec3(-0.171010, -0.030154, -0.984808),
        vec3(-0.342020, 0.000000, -0.939693), vec3(-0.342020, 0.000000, -0.939693),
        vec3(-0.336824, -0.059391, -0.939693), vec3(-0.336824, -0.059391, -0.939693),
        vec3(-0.500000, 0.000000, -0.866025), vec3(-0.500000, 0.000000, -0.866025),
        vec3(-0.492404, -0.086824, -0.866025), vec3(-0.492404, -0.086824, -0.866025),
        vec3(-0.642788, 0.000000, -0.766044), vec3(-0.642788, 0.000000, -0.766044),
        vec3(-0.633022, -0.111619, -0.766044), vec3(-0.633022, -0.111619, -0.766044),
        vec3(-0.766044, 0.000000, -0.642788), vec3(-0.766044, 0.000000, -0.642788),
        vec3(-0.754407, -0.133022, -0.642788), vec3(-0.754407, -0.133022, -0.642788),
        vec3(-0.866025, 0.000000, -0.500000), vec3(-0.866025, 0.000000, -0.500000),
        vec3(-0.852869, -0.150384, -0.500000), vec3(-0.852869, -0.150384, -0.500000),
        vec3(-0.939693, 0.000000, -0.342020), vec3(-0.939693, 0.000000, -0.342020),
        vec3(-0.925417, -0.163176, -0.342020), vec3(-0.925417, -0.163176, -0.342020),
        vec3(-0.984808, 0.000000, -0.173648), vec3(-0.984808, 0.000000, -0.173648),
        vec3(-0.969846, -0.171010, -0.173648), vec3(-0.969846, -0.171010, -0.173648),
        vec3(-1.000000, 0.000000, 0.000000), vec3(-1.000000, 0.000000, 0.000000),
        vec3(-0.984808, -0.173648, 0.000000), vec3(-0.984808, -0.173648, 0.000000),
        vec3(-0.984808, 0.000000, 0.173648), vec3(-0.984808, 0.000000, 0.173648),
        vec3(-0.969846, -0.171010, 0.173648), vec3(-0.969846, -0.171010, 0.173648),
        vec3(-0.939693, 0.000000, 0.342020), vec3(-0.939693, 0.000000, 0.342020),
        vec3(-0.925417, -0.163176, 0.342020), vec3(-0.925417, -0.163176, 0.342020),
        vec3(-0.866025, 0.000000, 0.500000), vec3(-0.866025, 0.000000, 0.500000),
        vec3(-0.852869, -0.150384, 0.500000), vec3(-0.852869, -0.150384, 0.500000),
        vec3(-0.766044, 0.000000, 0.642788), vec3(-0.766044, 0.000000, 0.642788),
        vec3(-0.754407, -0.133022, 0.642788), vec3(-0.754407, -0.133022, 0.642788),
        vec3(-0.642788, 0.000000, 0.766044), vec3(-0.642788, 0.000000, 0.766044),
        vec3(-0.633022, -0.111619, 0.766044), vec3(-0.633022, -0.111619, 0.766044),
        vec3(-0.500000, 0.000000, 0.866025), vec3(-0.500000, 0.000000, 0.866025),
        vec3(-0.492404, -0.086824, 0.866025), vec3(-0.492404, -0.086824, 0.866025),
        vec3(-0.342020, 0.000000, 0.939693), vec3(-0.342020, 0.000000, 0.939693),
        vec3(-0.336824, -0.059391, 0.939693), vec3(-0.336824, -0.059391, 0.939693),
        vec3(-0.173648, 0.000000, 0.984808), vec3(-0.173648, 0.000000, 0.984808),
        vec3(-0.171010, -0.030154, 0.984808), vec3(-0.171010, -0.030154, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.171010, -0.030154, 0.984808), vec3(-0.171010, -0.030154, 0.984808),
        vec3(-0.163176, -0.059391, 0.984808), vec3(-0.163176, -0.059391, 0.984808),
        vec3(-0.336824, -0.059391, 0.939693), vec3(-0.336824, -0.059391, 0.939693),
        vec3(-0.321394, -0.116978, 0.939693), vec3(-0.321394, -0.116978, 0.939693),
        vec3(-0.492404, -0.086824, 0.866025), vec3(-0.492404, -0.086824, 0.866025),
        vec3(-0.469846, -0.171010, 0.866025), vec3(-0.469846, -0.171010, 0.866025),
        vec3(-0.633022, -0.111619, 0.766044), vec3(-0.633022, -0.111619, 0.766044),
        vec3(-0.604023, -0.219846, 0.766044), vec3(-0.604023, -0.219846, 0.766044),
        vec3(-0.754407, -0.133022, 0.642788), vec3(-0.754407, -0.133022, 0.642788),
        vec3(-0.719846, -0.262003, 0.642788), vec3(-0.719846, -0.262003, 0.642788),
        vec3(-0.852869, -0.150384, 0.500000), vec3(-0.852869, -0.150384, 0.500000),
        vec3(-0.813798, -0.296198, 0.500000), vec3(-0.813798, -0.296198, 0.500000),
        vec3(-0.925417, -0.163176, 0.342020), vec3(-0.925417, -0.163176, 0.342020),
        vec3(-0.883022, -0.321394, 0.342020), vec3(-0.883022, -0.321394, 0.342020),
        vec3(-0.969846, -0.171010, 0.173648), vec3(-0.969846, -0.171010, 0.173648),
        vec3(-0.925417, -0.336824, 0.173648), vec3(-0.925417, -0.336824, 0.173648),
        vec3(-0.984808, -0.173648, 0.000000), vec3(-0.984808, -0.173648, 0.000000),
        vec3(-0.939693, -0.342020, 0.000000), vec3(-0.939693, -0.342020, 0.000000),
        vec3(-0.969846, -0.171010, -0.173648), vec3(-0.969846, -0.171010, -0.173648),
        vec3(-0.925417, -0.336824, -0.173648), vec3(-0.925417, -0.336824, -0.173648),
        vec3(-0.925417, -0.163176, -0.342020), vec3(-0.925417, -0.163176, -0.342020),
        vec3(-0.883022, -0.321394, -0.342020), vec3(-0.883022, -0.321394, -0.342020),
        vec3(-0.852869, -0.150384, -0.500000), vec3(-0.852869, -0.150384, -0.500000),
        vec3(-0.813798, -0.296198, -0.500000), vec3(-0.813798, -0.296198, -0.500000),
        vec3(-0.754407, -0.133022, -0.642788), vec3(-0.754407, -0.133022, -0.642788),
        vec3(-0.719846, -0.262003, -0.642788), vec3(-0.719846, -0.262003, -0.642788),
        vec3(-0.633022, -0.111619, -0.766044), vec3(-0.633022, -0.111619, -0.766044),
        vec3(-0.604023, -0.219846, -0.766044), vec3(-0.604023, -0.219846, -0.766044),
        vec3(-0.492404, -0.086824, -0.866025), vec3(-0.492404, -0.086824, -0.866025),
        vec3(-0.469846, -0.171010, -0.866025), vec3(-0.469846, -0.171010, -0.866025),
        vec3(-0.336824, -0.059391, -0.939693), vec3(-0.336824, -0.059391, -0.939693),
        vec3(-0.321394, -0.116978, -0.939693), vec3(-0.321394, -0.116978, -0.939693),
        vec3(-0.171010, -0.030154, -0.984808), vec3(-0.171010, -0.030154, -0.984808),
        vec3(-0.163176, -0.059391, -0.984808), vec3(-0.163176, -0.059391, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.163176, -0.059391, -0.984808), vec3(-0.163176, -0.059391, -0.984808),
        vec3(-0.150384, -0.086824, -0.984808), vec3(-0.150384, -0.086824, -0.984808),
        vec3(-0.321394, -0.116978, -0.939693), vec3(-0.321394, -0.116978, -0.939693),
        vec3(-0.296198, -0.171010, -0.939693), vec3(-0.296198, -0.171010, -0.939693),
        vec3(-0.469846, -0.171010, -0.866025), vec3(-0.469846, -0.171010, -0.866025),
        vec3(-0.433013, -0.250000, -0.866025), vec3(-0.433013, -0.250000, -0.866025),
        vec3(-0.604023, -0.219846, -0.766044), vec3(-0.604023, -0.219846, -0.766044),
        vec3(-0.556670, -0.321394, -0.766044), vec3(-0.556670, -0.321394, -0.766044),
        vec3(-0.719846, -0.262003, -0.642788), vec3(-0.719846, -0.262003, -0.642788),
        vec3(-0.663414, -0.383022, -0.642788), vec3(-0.663414, -0.383022, -0.642788),
        vec3(-0.813798, -0.296198, -0.500000), vec3(-0.813798, -0.296198, -0.500000),
        vec3(-0.750000, -0.433013, -0.500000), vec3(-0.750000, -0.433013, -0.500000),
        vec3(-0.883022, -0.321394, -0.342020), vec3(-0.883022, -0.321394, -0.342020),
        vec3(-0.813798, -0.469846, -0.342020), vec3(-0.813798, -0.469846, -0.342020),
        vec3(-0.925417, -0.336824, -0.173648), vec3(-0.925417, -0.336824, -0.173648),
        vec3(-0.852869, -0.492404, -0.173648), vec3(-0.852869, -0.492404, -0.173648),
        vec3(-0.939693, -0.342020, 0.000000), vec3(-0.939693, -0.342020, 0.000000),
        vec3(-0.866025, -0.500000, 0.000000), vec3(-0.866025, -0.500000, 0.000000),
        vec3(-0.925417, -0.336824, 0.173648), vec3(-0.925417, -0.336824, 0.173648),
        vec3(-0.852869, -0.492404, 0.173648), vec3(-0.852869, -0.492404, 0.173648),
        vec3(-0.883022, -0.321394, 0.342020), vec3(-0.883022, -0.321394, 0.342020),
        vec3(-0.813798, -0.469846, 0.342020), vec3(-0.813798, -0.469846, 0.342020),
        vec3(-0.813798, -0.296198, 0.500000), vec3(-0.813798, -0.296198, 0.500000),
        vec3(-0.750000, -0.433013, 0.500000), vec3(-0.750000, -0.433013, 0.500000),
        vec3(-0.719846, -0.262003, 0.642788), vec3(-0.719846, -0.262003, 0.642788),
        vec3(-0.663414, -0.383022, 0.642788), vec3(-0.663414, -0.383022, 0.642788),
        vec3(-0.604023, -0.219846, 0.766044), vec3(-0.604023, -0.219846, 0.766044),
        vec3(-0.556670, -0.321394, 0.766044), vec3(-0.556670, -0.321394, 0.766044),
        vec3(-0.469846, -0.171010, 0.866025), vec3(-0.469846, -0.171010, 0.866025),
        vec3(-0.433013, -0.250000, 0.866025), vec3(-0.433013, -0.250000, 0.866025),
        vec3(-0.321394, -0.116978, 0.939693), vec3(-0.321394, -0.116978, 0.939693),
        vec3(-0.296198, -0.171010, 0.939693), vec3(-0.296198, -0.171010, 0.939693),
        vec3(-0.163176, -0.059391, 0.984808), vec3(-0.163176, -0.059391, 0.984808),
        vec3(-0.150384, -0.086824, 0.984808), vec3(-0.150384, -0.086824, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.150384, -0.086824, 0.984808), vec3(-0.150384, -0.086824, 0.984808),
        vec3(-0.133022, -0.111619, 0.984808), vec3(-0.133022, -0.111619, 0.984808),
        vec3(-0.296198, -0.171010, 0.939693), vec3(-0.296198, -0.171010, 0.939693),
        vec3(-0.262003, -0.219846, 0.939693), vec3(-0.262003, -0.219846, 0.939693),
        vec3(-0.433013, -0.250000, 0.866025), vec3(-0.433013, -0.250000, 0.866025),
        vec3(-0.383022, -0.321394, 0.866025), vec3(-0.383022, -0.321394, 0.866025),
        vec3(-0.556670, -0.321394, 0.766044), vec3(-0.556670, -0.321394, 0.766044),
        vec3(-0.492404, -0.413176, 0.766044), vec3(-0.492404, -0.413176, 0.766045),
        vec3(-0.663414, -0.383022, 0.642788), vec3(-0.663414, -0.383022, 0.642788),
        vec3(-0.586824, -0.492404, 0.642788), vec3(-0.586824, -0.492404, 0.642788),
        vec3(-0.750000, -0.433013, 0.500000), vec3(-0.750000, -0.433013, 0.500000),
        vec3(-0.663414, -0.556670, 0.500000), vec3(-0.663414, -0.556670, 0.500000),
        vec3(-0.813798, -0.469846, 0.342020), vec3(-0.813798, -0.469846, 0.342020),
        vec3(-0.719846, -0.604023, 0.342020), vec3(-0.719846, -0.604023, 0.342020),
        vec3(-0.852869, -0.492404, 0.173648), vec3(-0.852869, -0.492404, 0.173648),
        vec3(-0.754407, -0.633022, 0.173648), vec3(-0.754407, -0.633022, 0.173648),
        vec3(-0.866025, -0.500000, 0.000000), vec3(-0.866025, -0.500000, 0.000000),
        vec3(-0.766044, -0.642788, 0.000000), vec3(-0.766044, -0.642788, 0.000000),
        vec3(-0.852869, -0.492404, -0.173648), vec3(-0.852869, -0.492404, -0.173648),
        vec3(-0.754407, -0.633022, -0.173648), vec3(-0.754407, -0.633022, -0.173648),
        vec3(-0.813798, -0.469846, -0.342020), vec3(-0.813798, -0.469846, -0.342020),
        vec3(-0.719846, -0.604023, -0.342020), vec3(-0.719846, -0.604023, -0.342020),
        vec3(-0.750000, -0.433013, -0.500000), vec3(-0.750000, -0.433013, -0.500000),
        vec3(-0.663414, -0.556670, -0.500000), vec3(-0.663414, -0.556670, -0.500000),
        vec3(-0.663414, -0.383022, -0.642788), vec3(-0.663414, -0.383022, -0.642788),
        vec3(-0.586824, -0.492404, -0.642788), vec3(-0.586824, -0.492404, -0.642788),
        vec3(-0.556670, -0.321394, -0.766044), vec3(-0.556670, -0.321394, -0.766044),
        vec3(-0.492404, -0.413176, -0.766044), vec3(-0.492404, -0.413176, -0.766045),
        vec3(-0.433013, -0.250000, -0.866025), vec3(-0.433013, -0.250000, -0.866025),
        vec3(-0.383022, -0.321394, -0.866025), vec3(-0.383022, -0.321394, -0.866025),
        vec3(-0.296198, -0.171010, -0.939693), vec3(-0.296198, -0.171010, -0.939693),
        vec3(-0.262003, -0.219846, -0.939693), vec3(-0.262003, -0.219846, -0.939693),
        vec3(-0.150384, -0.086824, -0.984808), vec3(-0.150384, -0.086824, -0.984808),
        vec3(-0.133022, -0.111619, -0.984808), vec3(-0.133022, -0.111619, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.133022, -0.111619, -0.984808), vec3(-0.133022, -0.111619, -0.984808),
        vec3(-0.111619, -0.133022, -0.984808), vec3(-0.111619, -0.133022, -0.984808),
        vec3(-0.262003, -0.219846, -0.939693), vec3(-0.262003, -0.219846, -0.939693),
        vec3(-0.219846, -0.262003, -0.939693), vec3(-0.219846, -0.262003, -0.939693),
        vec3(-0.383022, -0.321394, -0.866025), vec3(-0.383022, -0.321394, -0.866025),
        vec3(-0.321394, -0.383022, -0.866025), vec3(-0.321394, -0.383022, -0.866025),
        vec3(-0.492404, -0.413176, -0.766044), vec3(-0.492404, -0.413176, -0.766045),
        vec3(-0.413176, -0.492404, -0.766044), vec3(-0.413176, -0.492404, -0.766045),
        vec3(-0.586824, -0.492404, -0.642788), vec3(-0.586824, -0.492404, -0.642788),
        vec3(-0.492404, -0.586824, -0.642788), vec3(-0.492404, -0.586824, -0.642788),
        vec3(-0.663414, -0.556670, -0.500000), vec3(-0.663414, -0.556670, -0.500000),
        vec3(-0.556670, -0.663414, -0.500000), vec3(-0.556670, -0.663414, -0.500000),
        vec3(-0.719846, -0.604023, -0.342020), vec3(-0.719846, -0.604023, -0.342020),
        vec3(-0.604023, -0.719846, -0.342020), vec3(-0.604023, -0.719846, -0.342020),
        vec3(-0.754407, -0.633022, -0.173648), vec3(-0.754407, -0.633022, -0.173648),
        vec3(-0.633022, -0.754407, -0.173648), vec3(-0.633022, -0.754407, -0.173648),
        vec3(-0.766044, -0.642788, 0.000000), vec3(-0.766044, -0.642788, 0.000000),
        vec3(-0.642788, -0.766044, 0.000000), vec3(-0.642788, -0.766044, 0.000000),
        vec3(-0.754407, -0.633022, 0.173648), vec3(-0.754407, -0.633022, 0.173648),
        vec3(-0.633022, -0.754407, 0.173648), vec3(-0.633022, -0.754407, 0.173648),
        vec3(-0.719846, -0.604023, 0.342020), vec3(-0.719846, -0.604023, 0.342020),
        vec3(-0.604023, -0.719846, 0.342020), vec3(-0.604023, -0.719846, 0.342020),
        vec3(-0.663414, -0.556670, 0.500000), vec3(-0.663414, -0.556670, 0.500000),
        vec3(-0.556670, -0.663414, 0.500000), vec3(-0.556670, -0.663414, 0.500000),
        vec3(-0.586824, -0.492404, 0.642788), vec3(-0.586824, -0.492404, 0.642788),
        vec3(-0.492404, -0.586824, 0.642788), vec3(-0.492404, -0.586824, 0.642788),
        vec3(-0.492404, -0.413176, 0.766044), vec3(-0.492404, -0.413176, 0.766045),
        vec3(-0.413176, -0.492404, 0.766044), vec3(-0.413176, -0.492404, 0.766045),
        vec3(-0.383022, -0.321394, 0.866025), vec3(-0.383022, -0.321394, 0.866025),
        vec3(-0.321394, -0.383022, 0.866025), vec3(-0.321394, -0.383022, 0.866025),
        vec3(-0.262003, -0.219846, 0.939693), vec3(-0.262003, -0.219846, 0.939693),
        vec3(-0.219846, -0.262003, 0.939693), vec3(-0.219846, -0.262003, 0.939693),
        vec3(-0.133022, -0.111619, 0.984808), vec3(-0.133022, -0.111619, 0.984808),
        vec3(-0.111619, -0.133022, 0.984808), vec3(-0.111619, -0.133022, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.111619, -0.133022, 0.984808), vec3(-0.111619, -0.133022, 0.984808),
        vec3(-0.086824, -0.150384, 0.984808), vec3(-0.086824, -0.150384, 0.984808),
        vec3(-0.219846, -0.262003, 0.939693), vec3(-0.219846, -0.262003, 0.939693),
        vec3(-0.171010, -0.296198, 0.939693), vec3(-0.171010, -0.296198, 0.939693),
        vec3(-0.321394, -0.383022, 0.866025), vec3(-0.321394, -0.383022, 0.866025),
        vec3(-0.250000, -0.433013, 0.866025), vec3(-0.250000, -0.433013, 0.866025),
        vec3(-0.413176, -0.492404, 0.766044), vec3(-0.413176, -0.492404, 0.766045),
        vec3(-0.321394, -0.556670, 0.766044), vec3(-0.321394, -0.556670, 0.766044),
        vec3(-0.492404, -0.586824, 0.642788), vec3(-0.492404, -0.586824, 0.642788),
        vec3(-0.383022, -0.663414, 0.642788), vec3(-0.383022, -0.663414, 0.642788),
        vec3(-0.556670, -0.663414, 0.500000), vec3(-0.556670, -0.663414, 0.500000),
        vec3(-0.433013, -0.750000, 0.500000), vec3(-0.433013, -0.750000, 0.500000),
        vec3(-0.604023, -0.719846, 0.342020), vec3(-0.604023, -0.719846, 0.342020),
        vec3(-0.469846, -0.813798, 0.342020), vec3(-0.469846, -0.813798, 0.342020),
        vec3(-0.633022, -0.754407, 0.173648), vec3(-0.633022, -0.754407, 0.173648),
        vec3(-0.492404, -0.852869, 0.173648), vec3(-0.492404, -0.852869, 0.173648),
        vec3(-0.642788, -0.766044, 0.000000), vec3(-0.642788, -0.766044, 0.000000),
        vec3(-0.500000, -0.866025, 0.000000), vec3(-0.500000, -0.866025, 0.000000),
        vec3(-0.633022, -0.754407, -0.173648), vec3(-0.633022, -0.754407, -0.173648),
        vec3(-0.492404, -0.852869, -0.173648), vec3(-0.492404, -0.852869, -0.173648),
        vec3(-0.604023, -0.719846, -0.342020), vec3(-0.604023, -0.719846, -0.342020),
        vec3(-0.469846, -0.813798, -0.342020), vec3(-0.469846, -0.813798, -0.342020),
        vec3(-0.556670, -0.663414, -0.500000), vec3(-0.556670, -0.663414, -0.500000),
        vec3(-0.433013, -0.750000, -0.500000), vec3(-0.433013, -0.750000, -0.500000),
        vec3(-0.492404, -0.586824, -0.642788), vec3(-0.492404, -0.586824, -0.642788),
        vec3(-0.383022, -0.663414, -0.642788), vec3(-0.383022, -0.663414, -0.642788),
        vec3(-0.413176, -0.492404, -0.766044), vec3(-0.413176, -0.492404, -0.766045),
        vec3(-0.321394, -0.556670, -0.766044), vec3(-0.321394, -0.556670, -0.766044),
        vec3(-0.321394, -0.383022, -0.866025), vec3(-0.321394, -0.383022, -0.866025),
        vec3(-0.250000, -0.433013, -0.866025), vec3(-0.250000, -0.433013, -0.866025),
        vec3(-0.219846, -0.262003, -0.939693), vec3(-0.219846, -0.262003, -0.939693),
        vec3(-0.171010, -0.296198, -0.939693), vec3(-0.171010, -0.296198, -0.939693),
        vec3(-0.111619, -0.133022, -0.984808), vec3(-0.111619, -0.133022, -0.984808),
        vec3(-0.086824, -0.150384, -0.984808), vec3(-0.086824, -0.150384, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.086824, -0.150384, -0.984808), vec3(-0.086824, -0.150384, -0.984808),
        vec3(-0.059391, -0.163176, -0.984808), vec3(-0.059391, -0.163176, -0.984808),
        vec3(-0.171010, -0.296198, -0.939693), vec3(-0.171010, -0.296198, -0.939693),
        vec3(-0.116978, -0.321394, -0.939693), vec3(-0.116978, -0.321394, -0.939693),
        vec3(-0.250000, -0.433013, -0.866025), vec3(-0.250000, -0.433013, -0.866025),
        vec3(-0.171010, -0.469846, -0.866025), vec3(-0.171010, -0.469846, -0.866025),
        vec3(-0.321394, -0.556670, -0.766044), vec3(-0.321394, -0.556670, -0.766044),
        vec3(-0.219846, -0.604023, -0.766044), vec3(-0.219846, -0.604023, -0.766044),
        vec3(-0.383022, -0.663414, -0.642788), vec3(-0.383022, -0.663414, -0.642788),
        vec3(-0.262003, -0.719846, -0.642788), vec3(-0.262003, -0.719846, -0.642788),
        vec3(-0.433013, -0.750000, -0.500000), vec3(-0.433013, -0.750000, -0.500000),
        vec3(-0.296198, -0.813798, -0.500000), vec3(-0.296198, -0.813798, -0.500000),
        vec3(-0.469846, -0.813798, -0.342020), vec3(-0.469846, -0.813798, -0.342020),
        vec3(-0.321394, -0.883022, -0.342020), vec3(-0.321394, -0.883022, -0.342020),
        vec3(-0.492404, -0.852869, -0.173648), vec3(-0.492404, -0.852869, -0.173648),
        vec3(-0.336824, -0.925417, -0.173648), vec3(-0.336824, -0.925417, -0.173648),
        vec3(-0.500000, -0.866025, 0.000000), vec3(-0.500000, -0.866025, 0.000000),
        vec3(-0.342020, -0.939693, 0.000000), vec3(-0.342020, -0.939693, 0.000000),
        vec3(-0.492404, -0.852869, 0.173648), vec3(-0.492404, -0.852869, 0.173648),
        vec3(-0.336824, -0.925417, 0.173648), vec3(-0.336824, -0.925417, 0.173648),
        vec3(-0.469846, -0.813798, 0.342020), vec3(-0.469846, -0.813798, 0.342020),
        vec3(-0.321394, -0.883022, 0.342020), vec3(-0.321394, -0.883022, 0.342020),
        vec3(-0.433013, -0.750000, 0.500000), vec3(-0.433013, -0.750000, 0.500000),
        vec3(-0.296198, -0.813798, 0.500000), vec3(-0.296198, -0.813798, 0.500000),
        vec3(-0.383022, -0.663414, 0.642788), vec3(-0.383022, -0.663414, 0.642788),
        vec3(-0.262003, -0.719846, 0.642788), vec3(-0.262003, -0.719846, 0.642788),
        vec3(-0.321394, -0.556670, 0.766044), vec3(-0.321394, -0.556670, 0.766044),
        vec3(-0.219846, -0.604023, 0.766044), vec3(-0.219846, -0.604023, 0.766044),
        vec3(-0.250000, -0.433013, 0.866025), vec3(-0.250000, -0.433013, 0.866025),
        vec3(-0.171010, -0.469846, 0.866025), vec3(-0.171010, -0.469846, 0.866025),
        vec3(-0.171010, -0.296198, 0.939693), vec3(-0.171010, -0.296198, 0.939693),
        vec3(-0.116978, -0.321394, 0.939693), vec3(-0.116978, -0.321394, 0.939693),
        vec3(-0.086824, -0.150384, 0.984808), vec3(-0.086824, -0.150384, 0.984808),
        vec3(-0.059391, -0.163176, 0.984808), vec3(-0.059391, -0.163176, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.059391, -0.163176, 0.984808), vec3(-0.059391, -0.163176, 0.984808),
        vec3(-0.030154, -0.171010, 0.984808), vec3(-0.030154, -0.171010, 0.984808),
        vec3(-0.116978, -0.321394, 0.939693), vec3(-0.116978, -0.321394, 0.939693),
        vec3(-0.059391, -0.336824, 0.939693), vec3(-0.059391, -0.336824, 0.939693),
        vec3(-0.171010, -0.469846, 0.866025), vec3(-0.171010, -0.469846, 0.866025),
        vec3(-0.086824, -0.492404, 0.866025), vec3(-0.086824, -0.492404, 0.866025),
        vec3(-0.219846, -0.604023, 0.766044), vec3(-0.219846, -0.604023, 0.766044),
        vec3(-0.111619, -0.633022, 0.766044), vec3(-0.111619, -0.633022, 0.766044),
        vec3(-0.262003, -0.719846, 0.642788), vec3(-0.262003, -0.719846, 0.642788),
        vec3(-0.133022, -0.754407, 0.642788), vec3(-0.133022, -0.754407, 0.642788),
        vec3(-0.296198, -0.813798, 0.500000), vec3(-0.296198, -0.813798, 0.500000),
        vec3(-0.150384, -0.852869, 0.500000), vec3(-0.150384, -0.852869, 0.500000),
        vec3(-0.321394, -0.883022, 0.342020), vec3(-0.321394, -0.883022, 0.342020),
        vec3(-0.163176, -0.925417, 0.342020), vec3(-0.163176, -0.925417, 0.342020),
        vec3(-0.336824, -0.925417, 0.173648), vec3(-0.336824, -0.925417, 0.173648),
        vec3(-0.171010, -0.969846, 0.173648), vec3(-0.171010, -0.969846, 0.173648),
        vec3(-0.342020, -0.939693, 0.000000), vec3(-0.342020, -0.939693, 0.000000),
        vec3(-0.173648, -0.984808, 0.000000), vec3(-0.173648, -0.984808, 0.000000),
        vec3(-0.336824, -0.925417, -0.173648), vec3(-0.336824, -0.925417, -0.173648),
        vec3(-0.171010, -0.969846, -0.173648), vec3(-0.171010, -0.969846, -0.173648),
        vec3(-0.321394, -0.883022, -0.342020), vec3(-0.321394, -0.883022, -0.342020),
        vec3(-0.163176, -0.925417, -0.342020), vec3(-0.163176, -0.925417, -0.342020),
        vec3(-0.296198, -0.813798, -0.500000), vec3(-0.296198, -0.813798, -0.500000),
        vec3(-0.150384, -0.852869, -0.500000), vec3(-0.150384, -0.852869, -0.500000),
        vec3(-0.262003, -0.719846, -0.642788), vec3(-0.262003, -0.719846, -0.642788),
        vec3(-0.133022, -0.754407, -0.642788), vec3(-0.133022, -0.754407, -0.642788),
        vec3(-0.219846, -0.604023, -0.766044), vec3(-0.219846, -0.604023, -0.766044),
        vec3(-0.111619, -0.633022, -0.766044), vec3(-0.111619, -0.633022, -0.766044),
        vec3(-0.171010, -0.469846, -0.866025), vec3(-0.171010, -0.469846, -0.866025),
        vec3(-0.086824, -0.492404, -0.866025), vec3(-0.086824, -0.492404, -0.866025),
        vec3(-0.116978, -0.321394, -0.939693), vec3(-0.116978, -0.321394, -0.939693),
        vec3(-0.059391, -0.336824, -0.939693), vec3(-0.059391, -0.336824, -0.939693),
        vec3(-0.059391, -0.163176, -0.984808), vec3(-0.059391, -0.163176, -0.984808),
        vec3(-0.030154, -0.171010, -0.984808), vec3(-0.030154, -0.171010, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(-0.030154, -0.171010, -0.984808), vec3(-0.030154, -0.171010, -0.984808),
        vec3(-0.000000, -0.173648, -0.984808), vec3(-0.000000, -0.173648, -0.984808),
        vec3(-0.059391, -0.336824, -0.939693), vec3(-0.059391, -0.336824, -0.939693),
        vec3(-0.000000, -0.342020, -0.939693), vec3(-0.000000, -0.342020, -0.939693),
        vec3(-0.086824, -0.492404, -0.866025), vec3(-0.086824, -0.492404, -0.866025),
        vec3(-0.000000, -0.500000, -0.866025), vec3(-0.000000, -0.500000, -0.866025),
        vec3(-0.111619, -0.633022, -0.766044), vec3(-0.111619, -0.633022, -0.766044),
        vec3(-0.000000, -0.642788, -0.766044), vec3(-0.000000, -0.642788, -0.766044),
        vec3(-0.133022, -0.754407, -0.642788), vec3(-0.133022, -0.754407, -0.642788),
        vec3(-0.000000, -0.766044, -0.642788), vec3(-0.000000, -0.766044, -0.642788),
        vec3(-0.150384, -0.852869, -0.500000), vec3(-0.150384, -0.852869, -0.500000),
        vec3(-0.000000, -0.866025, -0.500000), vec3(-0.000000, -0.866025, -0.500000),
        vec3(-0.163176, -0.925417, -0.342020), vec3(-0.163176, -0.925417, -0.342020),
        vec3(-0.000000, -0.939693, -0.342020), vec3(-0.000000, -0.939693, -0.342020),
        vec3(-0.171010, -0.969846, -0.173648), vec3(-0.171010, -0.969846, -0.173648),
        vec3(-0.000000, -0.984808, -0.173648), vec3(-0.000000, -0.984808, -0.173648),
        vec3(-0.173648, -0.984808, 0.000000), vec3(-0.173648, -0.984808, 0.000000),
        vec3(-0.000000, -1.000000, 0.000000), vec3(-0.000000, -1.000000, 0.000000),
        vec3(-0.171010, -0.969846, 0.173648), vec3(-0.171010, -0.969846, 0.173648),
        vec3(-0.000000, -0.984808, 0.173648), vec3(-0.000000, -0.984808, 0.173648),
        vec3(-0.163176, -0.925417, 0.342020), vec3(-0.163176, -0.925417, 0.342020),
        vec3(-0.000000, -0.939693, 0.342020), vec3(-0.000000, -0.939693, 0.342020),
        vec3(-0.150384, -0.852869, 0.500000), vec3(-0.150384, -0.852869, 0.500000),
        vec3(-0.000000, -0.866025, 0.500000), vec3(-0.000000, -0.866025, 0.500000),
        vec3(-0.133022, -0.754407, 0.642788), vec3(-0.133022, -0.754407, 0.642788),
        vec3(-0.000000, -0.766044, 0.642788), vec3(-0.000000, -0.766044, 0.642788),
        vec3(-0.111619, -0.633022, 0.766044), vec3(-0.111619, -0.633022, 0.766044),
        vec3(-0.000000, -0.642788, 0.766044), vec3(-0.000000, -0.642788, 0.766044),
        vec3(-0.086824, -0.492404, 0.866025), vec3(-0.086824, -0.492404, 0.866025),
        vec3(-0.000000, -0.500000, 0.866025), vec3(-0.000000, -0.500000, 0.866025),
        vec3(-0.059391, -0.336824, 0.939693), vec3(-0.059391, -0.336824, 0.939693),
        vec3(-0.000000, -0.342020, 0.939693), vec3(-0.000000, -0.342020, 0.939693),
        vec3(-0.030154, -0.171010, 0.984808), vec3(-0.030154, -0.171010, 0.984808),
        vec3(-0.000000, -0.173648, 0.984808), vec3(-0.000000, -0.173648, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(-0.000000, -0.173648, 0.984808), vec3(-0.000000, -0.173648, 0.984808),
        vec3(0.030154, -0.171010, 0.984808), vec3(0.030154, -0.171010, 0.984808),
        vec3(-0.000000, -0.342020, 0.939693), vec3(-0.000000, -0.342020, 0.939693),
        vec3(0.059391, -0.336824, 0.939693), vec3(0.059391, -0.336824, 0.939693),
        vec3(-0.000000, -0.500000, 0.866025), vec3(-0.000000, -0.500000, 0.866025),
        vec3(0.086824, -0.492404, 0.866025), vec3(0.086824, -0.492404, 0.866025),
        vec3(-0.000000, -0.642788, 0.766044), vec3(-0.000000, -0.642788, 0.766044),
        vec3(0.111619, -0.633022, 0.766044), vec3(0.111619, -0.633022, 0.766044),
        vec3(-0.000000, -0.766044, 0.642788), vec3(-0.000000, -0.766044, 0.642788),
        vec3(0.133022, -0.754407, 0.642788), vec3(0.133022, -0.754407, 0.642788),
        vec3(-0.000000, -0.866025, 0.500000), vec3(-0.000000, -0.866025, 0.500000),
        vec3(0.150384, -0.852869, 0.500000), vec3(0.150384, -0.852869, 0.500000),
        vec3(-0.000000, -0.939693, 0.342020), vec3(-0.000000, -0.939693, 0.342020),
        vec3(0.163176, -0.925417, 0.342020), vec3(0.163176, -0.925417, 0.342020),
        vec3(-0.000000, -0.984808, 0.173648), vec3(-0.000000, -0.984808, 0.173648),
        vec3(0.171010, -0.969846, 0.173648), vec3(0.171010, -0.969846, 0.173648),
        vec3(-0.000000, -1.000000, 0.000000), vec3(-0.000000, -1.000000, 0.000000),
        vec3(0.173648, -0.984808, 0.000000), vec3(0.173648, -0.984808, 0.000000),
        vec3(-0.000000, -0.984808, -0.173648), vec3(-0.000000, -0.984808, -0.173648),
        vec3(0.171010, -0.969846, -0.173648), vec3(0.171010, -0.969846, -0.173648),
        vec3(-0.000000, -0.939693, -0.342020), vec3(-0.000000, -0.939693, -0.342020),
        vec3(0.163176, -0.925417, -0.342020), vec3(0.163176, -0.925417, -0.342020),
        vec3(-0.000000, -0.866025, -0.500000), vec3(-0.000000, -0.866025, -0.500000),
        vec3(0.150384, -0.852869, -0.500000), vec3(0.150384, -0.852869, -0.500000),
        vec3(-0.000000, -0.766044, -0.642788), vec3(-0.000000, -0.766044, -0.642788),
        vec3(0.133022, -0.754407, -0.642788), vec3(0.133022, -0.754407, -0.642788),
        vec3(-0.000000, -0.642788, -0.766044), vec3(-0.000000, -0.642788, -0.766044),
        vec3(0.111619, -0.633022, -0.766044), vec3(0.111619, -0.633022, -0.766044),
        vec3(-0.000000, -0.500000, -0.866025), vec3(-0.000000, -0.500000, -0.866025),
        vec3(0.086824, -0.492404, -0.866025), vec3(0.086824, -0.492404, -0.866025),
        vec3(-0.000000, -0.342020, -0.939693), vec3(-0.000000, -0.342020, -0.939693),
        vec3(0.059391, -0.336824, -0.939693), vec3(0.059391, -0.336824, -0.939693),
        vec3(-0.000000, -0.173648, -0.984808), vec3(-0.000000, -0.173648, -0.984808),
        vec3(0.030154, -0.171010, -0.984808), vec3(0.030154, -0.171010, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(0.030154, -0.171010, -0.984808), vec3(0.030154, -0.171010, -0.984808),
        vec3(0.059391, -0.163176, -0.984808), vec3(0.059391, -0.163176, -0.984808),
        vec3(0.059391, -0.336824, -0.939693), vec3(0.059391, -0.336824, -0.939693),
        vec3(0.116978, -0.321394, -0.939693), vec3(0.116978, -0.321394, -0.939693),
        vec3(0.086824, -0.492404, -0.866025), vec3(0.086824, -0.492404, -0.866025),
        vec3(0.171010, -0.469846, -0.866025), vec3(0.171010, -0.469846, -0.866025),
        vec3(0.111619, -0.633022, -0.766044), vec3(0.111619, -0.633022, -0.766044),
        vec3(0.219846, -0.604023, -0.766044), vec3(0.219846, -0.604023, -0.766044),
        vec3(0.133022, -0.754407, -0.642788), vec3(0.133022, -0.754407, -0.642788),
        vec3(0.262003, -0.719846, -0.642788), vec3(0.262003, -0.719846, -0.642788),
        vec3(0.150384, -0.852869, -0.500000), vec3(0.150384, -0.852869, -0.500000),
        vec3(0.296198, -0.813798, -0.500000), vec3(0.296198, -0.813798, -0.500000),
        vec3(0.163176, -0.925417, -0.342020), vec3(0.163176, -0.925417, -0.342020),
        vec3(0.321394, -0.883022, -0.342020), vec3(0.321394, -0.883022, -0.342020),
        vec3(0.171010, -0.969846, -0.173648), vec3(0.171010, -0.969846, -0.173648),
        vec3(0.336824, -0.925417, -0.173648), vec3(0.336824, -0.925417, -0.173648),
        vec3(0.173648, -0.984808, 0.000000), vec3(0.173648, -0.984808, 0.000000),
        vec3(0.342020, -0.939693, 0.000000), vec3(0.342020, -0.939693, 0.000000),
        vec3(0.171010, -0.969846, 0.173648), vec3(0.171010, -0.969846, 0.173648),
        vec3(0.336824, -0.925417, 0.173648), vec3(0.336824, -0.925417, 0.173648),
        vec3(0.163176, -0.925417, 0.342020), vec3(0.163176, -0.925417, 0.342020),
        vec3(0.321394, -0.883022, 0.342020), vec3(0.321394, -0.883022, 0.342020),
        vec3(0.150384, -0.852869, 0.500000), vec3(0.150384, -0.852869, 0.500000),
        vec3(0.296198, -0.813798, 0.500000), vec3(0.296198, -0.813798, 0.500000),
        vec3(0.133022, -0.754407, 0.642788), vec3(0.133022, -0.754407, 0.642788),
        vec3(0.262003, -0.719846, 0.642788), vec3(0.262003, -0.719846, 0.642788),
        vec3(0.111619, -0.633022, 0.766044), vec3(0.111619, -0.633022, 0.766044),
        vec3(0.219846, -0.604023, 0.766044), vec3(0.219846, -0.604023, 0.766044),
        vec3(0.086824, -0.492404, 0.866025), vec3(0.086824, -0.492404, 0.866025),
        vec3(0.171010, -0.469846, 0.866025), vec3(0.171010, -0.469846, 0.866025),
        vec3(0.059391, -0.336824, 0.939693), vec3(0.059391, -0.336824, 0.939693),
        vec3(0.116978, -0.321394, 0.939693), vec3(0.116978, -0.321394, 0.939693),
        vec3(0.030154, -0.171010, 0.984808), vec3(0.030154, -0.171010, 0.984808),
        vec3(0.059391, -0.163176, 0.984808), vec3(0.059391, -0.163176, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(0.059391, -0.163176, 0.984808), vec3(0.059391, -0.163176, 0.984808),
        vec3(0.086824, -0.150384, 0.984808), vec3(0.086824, -0.150384, 0.984808),
        vec3(0.116978, -0.321394, 0.939693), vec3(0.116978, -0.321394, 0.939693),
        vec3(0.171010, -0.296198, 0.939693), vec3(0.171010, -0.296198, 0.939693),
        vec3(0.171010, -0.469846, 0.866025), vec3(0.171010, -0.469846, 0.866025),
        vec3(0.250000, -0.433013, 0.866025), vec3(0.250000, -0.433013, 0.866025),
        vec3(0.219846, -0.604023, 0.766044), vec3(0.219846, -0.604023, 0.766044),
        vec3(0.321394, -0.556670, 0.766044), vec3(0.321394, -0.556670, 0.766044),
        vec3(0.262003, -0.719846, 0.642788), vec3(0.262003, -0.719846, 0.642788),
        vec3(0.383022, -0.663414, 0.642788), vec3(0.383022, -0.663414, 0.642788),
        vec3(0.296198, -0.813798, 0.500000), vec3(0.296198, -0.813798, 0.500000),
        vec3(0.433013, -0.750000, 0.500000), vec3(0.433013, -0.750000, 0.500000),
        vec3(0.321394, -0.883022, 0.342020), vec3(0.321394, -0.883022, 0.342020),
        vec3(0.469846, -0.813798, 0.342020), vec3(0.469846, -0.813798, 0.342020),
        vec3(0.336824, -0.925417, 0.173648), vec3(0.336824, -0.925417, 0.173648),
        vec3(0.492404, -0.852869, 0.173648), vec3(0.492404, -0.852869, 0.173648),
        vec3(0.342020, -0.939693, 0.000000), vec3(0.342020, -0.939693, 0.000000),
        vec3(0.500000, -0.866025, 0.000000), vec3(0.500000, -0.866025, 0.000000),
        vec3(0.336824, -0.925417, -0.173648), vec3(0.336824, -0.925417, -0.173648),
        vec3(0.492404, -0.852869, -0.173648), vec3(0.492404, -0.852869, -0.173648),
        vec3(0.321394, -0.883022, -0.342020), vec3(0.321394, -0.883022, -0.342020),
        vec3(0.469846, -0.813798, -0.342020), vec3(0.469846, -0.813798, -0.342020),
        vec3(0.296198, -0.813798, -0.500000), vec3(0.296198, -0.813798, -0.500000),
        vec3(0.433013, -0.750000, -0.500000), vec3(0.433013, -0.750000, -0.500000),
        vec3(0.262003, -0.719846, -0.642788), vec3(0.262003, -0.719846, -0.642788),
        vec3(0.383022, -0.663414, -0.642788), vec3(0.383022, -0.663414, -0.642788),
        vec3(0.219846, -0.604023, -0.766044), vec3(0.219846, -0.604023, -0.766044),
        vec3(0.321394, -0.556670, -0.766044), vec3(0.321394, -0.556670, -0.766044),
        vec3(0.171010, -0.469846, -0.866025), vec3(0.171010, -0.469846, -0.866025),
        vec3(0.250000, -0.433013, -0.866025), vec3(0.250000, -0.433013, -0.866025),
        vec3(0.116978, -0.321394, -0.939693), vec3(0.116978, -0.321394, -0.939693),
        vec3(0.171010, -0.296198, -0.939693), vec3(0.171010, -0.296198, -0.939693),
        vec3(0.059391, -0.163176, -0.984808), vec3(0.059391, -0.163176, -0.984808),
        vec3(0.086824, -0.150384, -0.984808), vec3(0.086824, -0.150384, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(0.086824, -0.150384, -0.984808), vec3(0.086824, -0.150384, -0.984808),
        vec3(0.111619, -0.133022, -0.984808), vec3(0.111619, -0.133022, -0.984808),
        vec3(0.171010, -0.296198, -0.939693), vec3(0.171010, -0.296198, -0.939693),
        vec3(0.219846, -0.262003, -0.939693), vec3(0.219846, -0.262003, -0.939693),
        vec3(0.250000, -0.433013, -0.866025), vec3(0.250000, -0.433013, -0.866025),
        vec3(0.321394, -0.383022, -0.866025), vec3(0.321394, -0.383022, -0.866025),
        vec3(0.321394, -0.556670, -0.766044), vec3(0.321394, -0.556670, -0.766044),
        vec3(0.413176, -0.492404, -0.766044), vec3(0.413176, -0.492404, -0.766045),
        vec3(0.383022, -0.663414, -0.642788), vec3(0.383022, -0.663414, -0.642788),
        vec3(0.492404, -0.586824, -0.642788), vec3(0.492404, -0.586824, -0.642788),
        vec3(0.433013, -0.750000, -0.500000), vec3(0.433013, -0.750000, -0.500000),
        vec3(0.556670, -0.663414, -0.500000), vec3(0.556670, -0.663414, -0.500000),
        vec3(0.469846, -0.813798, -0.342020), vec3(0.469846, -0.813798, -0.342020),
        vec3(0.604023, -0.719846, -0.342020), vec3(0.604023, -0.719846, -0.342020),
        vec3(0.492404, -0.852869, -0.173648), vec3(0.492404, -0.852869, -0.173648),
        vec3(0.633022, -0.754407, -0.173648), vec3(0.633022, -0.754407, -0.173648),
        vec3(0.500000, -0.866025, 0.000000), vec3(0.500000, -0.866025, 0.000000),
        vec3(0.642788, -0.766044, 0.000000), vec3(0.642788, -0.766044, 0.000000),
        vec3(0.492404, -0.852869, 0.173648), vec3(0.492404, -0.852869, 0.173648),
        vec3(0.633022, -0.754407, 0.173648), vec3(0.633022, -0.754407, 0.173648),
        vec3(0.469846, -0.813798, 0.342020), vec3(0.469846, -0.813798, 0.342020),
        vec3(0.604023, -0.719846, 0.342020), vec3(0.604023, -0.719846, 0.342020),
        vec3(0.433013, -0.750000, 0.500000), vec3(0.433013, -0.750000, 0.500000),
        vec3(0.556670, -0.663414, 0.500000), vec3(0.556670, -0.663414, 0.500000),
        vec3(0.383022, -0.663414, 0.642788), vec3(0.383022, -0.663414, 0.642788),
        vec3(0.492404, -0.586824, 0.642788), vec3(0.492404, -0.586824, 0.642788),
        vec3(0.321394, -0.556670, 0.766044), vec3(0.321394, -0.556670, 0.766044),
        vec3(0.413176, -0.492404, 0.766044), vec3(0.413176, -0.492404, 0.766045),
        vec3(0.250000, -0.433013, 0.866025), vec3(0.250000, -0.433013, 0.866025),
        vec3(0.321394, -0.383022, 0.866025), vec3(0.321394, -0.383022, 0.866025),
        vec3(0.171010, -0.296198, 0.939693), vec3(0.171010, -0.296198, 0.939693),
        vec3(0.219846, -0.262003, 0.939693), vec3(0.219846, -0.262003, 0.939693),
        vec3(0.086824, -0.150384, 0.984808), vec3(0.086824, -0.150384, 0.984808),
        vec3(0.111619, -0.133022, 0.984808), vec3(0.111619, -0.133022, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(0.111619, -0.133022, 0.984808), vec3(0.111619, -0.133022, 0.984808),
        vec3(0.133022, -0.111619, 0.984808), vec3(0.133022, -0.111619, 0.984808),
        vec3(0.219846, -0.262003, 0.939693), vec3(0.219846, -0.262003, 0.939693),
        vec3(0.262003, -0.219846, 0.939693), vec3(0.262003, -0.219846, 0.939693),
        vec3(0.321394, -0.383022, 0.866025), vec3(0.321394, -0.383022, 0.866025),
        vec3(0.383022, -0.321394, 0.866025), vec3(0.383022, -0.321394, 0.866025),
        vec3(0.413176, -0.492404, 0.766044), vec3(0.413176, -0.492404, 0.766045),
        vec3(0.492404, -0.413176, 0.766044), vec3(0.492404, -0.413176, 0.766045),
        vec3(0.492404, -0.586824, 0.642788), vec3(0.492404, -0.586824, 0.642788),
        vec3(0.586824, -0.492404, 0.642788), vec3(0.586824, -0.492404, 0.642788),
        vec3(0.556670, -0.663414, 0.500000), vec3(0.556670, -0.663414, 0.500000),
        vec3(0.663414, -0.556670, 0.500000), vec3(0.663414, -0.556670, 0.500000),
        vec3(0.604023, -0.719846, 0.342020), vec3(0.604023, -0.719846, 0.342020),
        vec3(0.719846, -0.604023, 0.342020), vec3(0.719846, -0.604023, 0.342020),
        vec3(0.633022, -0.754407, 0.173648), vec3(0.633022, -0.754407, 0.173648),
        vec3(0.754407, -0.633022, 0.173648), vec3(0.754407, -0.633022, 0.173648),
        vec3(0.642788, -0.766044, 0.000000), vec3(0.642788, -0.766044, 0.000000),
        vec3(0.766044, -0.642788, 0.000000), vec3(0.766044, -0.642788, 0.000000),
        vec3(0.633022, -0.754407, -0.173648), vec3(0.633022, -0.754407, -0.173648),
        vec3(0.754407, -0.633022, -0.173648), vec3(0.754407, -0.633022, -0.173648),
        vec3(0.604023, -0.719846, -0.342020), vec3(0.604023, -0.719846, -0.342020),
        vec3(0.719846, -0.604023, -0.342020), vec3(0.719846, -0.604023, -0.342020),
        vec3(0.556670, -0.663414, -0.500000), vec3(0.556670, -0.663414, -0.500000),
        vec3(0.663414, -0.556670, -0.500000), vec3(0.663414, -0.556670, -0.500000),
        vec3(0.492404, -0.586824, -0.642788), vec3(0.492404, -0.586824, -0.642788),
        vec3(0.586824, -0.492404, -0.642788), vec3(0.586824, -0.492404, -0.642788),
        vec3(0.413176, -0.492404, -0.766044), vec3(0.413176, -0.492404, -0.766045),
        vec3(0.492404, -0.413176, -0.766044), vec3(0.492404, -0.413176, -0.766045),
        vec3(0.321394, -0.383022, -0.866025), vec3(0.321394, -0.383022, -0.866025),
        vec3(0.383022, -0.321394, -0.866025), vec3(0.383022, -0.321394, -0.866025),
        vec3(0.219846, -0.262003, -0.939693), vec3(0.219846, -0.262003, -0.939693),
        vec3(0.262003, -0.219846, -0.939693), vec3(0.262003, -0.219846, -0.939693),
        vec3(0.111619, -0.133022, -0.984808), vec3(0.111619, -0.133022, -0.984808),
        vec3(0.133022, -0.111619, -0.984808), vec3(0.133022, -0.111619, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(0.133022, -0.111619, -0.984808), vec3(0.133022, -0.111619, -0.984808),
        vec3(0.150384, -0.086824, -0.984808), vec3(0.150384, -0.086824, -0.984808),
        vec3(0.262003, -0.219846, -0.939693), vec3(0.262003, -0.219846, -0.939693),
        vec3(0.296198, -0.171010, -0.939693), vec3(0.296198, -0.171010, -0.939693),
        vec3(0.383022, -0.321394, -0.866025), vec3(0.383022, -0.321394, -0.866025),
        vec3(0.433013, -0.250000, -0.866025), vec3(0.433013, -0.250000, -0.866025),
        vec3(0.492404, -0.413176, -0.766044), vec3(0.492404, -0.413176, -0.766045),
        vec3(0.556670, -0.321394, -0.766044), vec3(0.556670, -0.321394, -0.766044),
        vec3(0.586824, -0.492404, -0.642788), vec3(0.586824, -0.492404, -0.642788),
        vec3(0.663414, -0.383022, -0.642788), vec3(0.663414, -0.383022, -0.642788),
        vec3(0.663414, -0.556670, -0.500000), vec3(0.663414, -0.556670, -0.500000),
        vec3(0.750000, -0.433013, -0.500000), vec3(0.750000, -0.433013, -0.500000),
        vec3(0.719846, -0.604023, -0.342020), vec3(0.719846, -0.604023, -0.342020),
        vec3(0.813798, -0.469846, -0.342020), vec3(0.813798, -0.469846, -0.342020),
        vec3(0.754407, -0.633022, -0.173648), vec3(0.754407, -0.633022, -0.173648),
        vec3(0.852869, -0.492404, -0.173648), vec3(0.852869, -0.492404, -0.173648),
        vec3(0.766044, -0.642788, 0.000000), vec3(0.766044, -0.642788, 0.000000),
        vec3(0.866025, -0.500000, 0.000000), vec3(0.866025, -0.500000, 0.000000),
        vec3(0.754407, -0.633022, 0.173648), vec3(0.754407, -0.633022, 0.173648),
        vec3(0.852869, -0.492404, 0.173648), vec3(0.852869, -0.492404, 0.173648),
        vec3(0.719846, -0.604023, 0.342020), vec3(0.719846, -0.604023, 0.342020),
        vec3(0.813798, -0.469846, 0.342020), vec3(0.813798, -0.469846, 0.342020),
        vec3(0.663414, -0.556670, 0.500000), vec3(0.663414, -0.556670, 0.500000),
        vec3(0.750000, -0.433013, 0.500000), vec3(0.750000, -0.433013, 0.500000),
        vec3(0.586824, -0.492404, 0.642788), vec3(0.586824, -0.492404, 0.642788),
        vec3(0.663414, -0.383022, 0.642788), vec3(0.663414, -0.383022, 0.642788),
        vec3(0.492404, -0.413176, 0.766044), vec3(0.492404, -0.413176, 0.766045),
        vec3(0.556670, -0.321394, 0.766044), vec3(0.556670, -0.321394, 0.766044),
        vec3(0.383022, -0.321394, 0.866025), vec3(0.383022, -0.321394, 0.866025),
        vec3(0.433013, -0.250000, 0.866025), vec3(0.433013, -0.250000, 0.866025),
        vec3(0.262003, -0.219846, 0.939693), vec3(0.262003, -0.219846, 0.939693),
        vec3(0.296198, -0.171010, 0.939693), vec3(0.296198, -0.171010, 0.939693),
        vec3(0.133022, -0.111619, 0.984808), vec3(0.133022, -0.111619, 0.984808),
        vec3(0.150384, -0.086824, 0.984808), vec3(0.150384, -0.086824, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(0.150384, -0.086824, 0.984808), vec3(0.150384, -0.086824, 0.984808),
        vec3(0.163176, -0.059391, 0.984808), vec3(0.163176, -0.059391, 0.984808),
        vec3(0.296198, -0.171010, 0.939693), vec3(0.296198, -0.171010, 0.939693),
        vec3(0.321394, -0.116978, 0.939693), vec3(0.321394, -0.116978, 0.939693),
        vec3(0.433013, -0.250000, 0.866025), vec3(0.433013, -0.250000, 0.866025),
        vec3(0.469846, -0.171010, 0.866025), vec3(0.469846, -0.171010, 0.866025),
        vec3(0.556670, -0.321394, 0.766044), vec3(0.556670, -0.321394, 0.766044),
        vec3(0.604023, -0.219846, 0.766044), vec3(0.604023, -0.219846, 0.766044),
        vec3(0.663414, -0.383022, 0.642788), vec3(0.663414, -0.383022, 0.642788),
        vec3(0.719846, -0.262003, 0.642788), vec3(0.719846, -0.262003, 0.642788),
        vec3(0.750000, -0.433013, 0.500000), vec3(0.750000, -0.433013, 0.500000),
        vec3(0.813798, -0.296198, 0.500000), vec3(0.813798, -0.296198, 0.500000),
        vec3(0.813798, -0.469846, 0.342020), vec3(0.813798, -0.469846, 0.342020),
        vec3(0.883022, -0.321394, 0.342020), vec3(0.883022, -0.321394, 0.342020),
        vec3(0.852869, -0.492404, 0.173648), vec3(0.852869, -0.492404, 0.173648),
        vec3(0.925417, -0.336824, 0.173648), vec3(0.925417, -0.336824, 0.173648),
        vec3(0.866025, -0.500000, 0.000000), vec3(0.866025, -0.500000, 0.000000),
        vec3(0.939693, -0.342020, 0.000000), vec3(0.939693, -0.342020, 0.000000),
        vec3(0.852869, -0.492404, -0.173648), vec3(0.852869, -0.492404, -0.173648),
        vec3(0.925417, -0.336824, -0.173648), vec3(0.925417, -0.336824, -0.173648),
        vec3(0.813798, -0.469846, -0.342020), vec3(0.813798, -0.469846, -0.342020),
        vec3(0.883022, -0.321394, -0.342020), vec3(0.883022, -0.321394, -0.342020),
        vec3(0.750000, -0.433013, -0.500000), vec3(0.750000, -0.433013, -0.500000),
        vec3(0.813798, -0.296198, -0.500000), vec3(0.813798, -0.296198, -0.500000),
        vec3(0.663414, -0.383022, -0.642788), vec3(0.663414, -0.383022, -0.642788),
        vec3(0.719846, -0.262003, -0.642788), vec3(0.719846, -0.262003, -0.642788),
        vec3(0.556670, -0.321394, -0.766044), vec3(0.556670, -0.321394, -0.766044),
        vec3(0.604023, -0.219846, -0.766044), vec3(0.604023, -0.219846, -0.766044),
        vec3(0.433013, -0.250000, -0.866025), vec3(0.433013, -0.250000, -0.866025),
        vec3(0.469846, -0.171010, -0.866025), vec3(0.469846, -0.171010, -0.866025),
        vec3(0.296198, -0.171010, -0.939693), vec3(0.296198, -0.171010, -0.939693),
        vec3(0.321394, -0.116978, -0.939693), vec3(0.321394, -0.116978, -0.939693),
        vec3(0.150384, -0.086824, -0.984808), vec3(0.150384, -0.086824, -0.984808),
        vec3(0.163176, -0.059391, -0.984808), vec3(0.163176, -0.059391, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000),
        vec3(0.163176, -0.059391, -0.984808), vec3(0.163176, -0.059391, -0.984808),
        vec3(0.171010, -0.030154, -0.984808), vec3(0.171010, -0.030154, -0.984808),
        vec3(0.321394, -0.116978, -0.939693), vec3(0.321394, -0.116978, -0.939693),
        vec3(0.336824, -0.059391, -0.939693), vec3(0.336824, -0.059391, -0.939693),
        vec3(0.469846, -0.171010, -0.866025), vec3(0.469846, -0.171010, -0.866025),
        vec3(0.492404, -0.086824, -0.866025), vec3(0.492404, -0.086824, -0.866025),
        vec3(0.604023, -0.219846, -0.766044), vec3(0.604023, -0.219846, -0.766044),
        vec3(0.633022, -0.111619, -0.766044), vec3(0.633022, -0.111619, -0.766044),
        vec3(0.719846, -0.262003, -0.642788), vec3(0.719846, -0.262003, -0.642788),
        vec3(0.754407, -0.133022, -0.642788), vec3(0.754407, -0.133022, -0.642788),
        vec3(0.813798, -0.296198, -0.500000), vec3(0.813798, -0.296198, -0.500000),
        vec3(0.852869, -0.150384, -0.500000), vec3(0.852869, -0.150384, -0.500000),
        vec3(0.883022, -0.321394, -0.342020), vec3(0.883022, -0.321394, -0.342020),
        vec3(0.925417, -0.163176, -0.342020), vec3(0.925417, -0.163176, -0.342020),
        vec3(0.925417, -0.336824, -0.173648), vec3(0.925417, -0.336824, -0.173648),
        vec3(0.969846, -0.171010, -0.173648), vec3(0.969846, -0.171010, -0.173648),
        vec3(0.939693, -0.342020, 0.000000), vec3(0.939693, -0.342020, 0.000000),
        vec3(0.984808, -0.173648, 0.000000), vec3(0.984808, -0.173648, 0.000000),
        vec3(0.925417, -0.336824, 0.173648), vec3(0.925417, -0.336824, 0.173648),
        vec3(0.969846, -0.171010, 0.173648), vec3(0.969846, -0.171010, 0.173648),
        vec3(0.883022, -0.321394, 0.342020), vec3(0.883022, -0.321394, 0.342020),
        vec3(0.925417, -0.163176, 0.342020), vec3(0.925417, -0.163176, 0.342020),
        vec3(0.813798, -0.296198, 0.500000), vec3(0.813798, -0.296198, 0.500000),
        vec3(0.852869, -0.150384, 0.500000), vec3(0.852869, -0.150384, 0.500000),
        vec3(0.719846, -0.262003, 0.642788), vec3(0.719846, -0.262003, 0.642788),
        vec3(0.754407, -0.133022, 0.642788), vec3(0.754407, -0.133022, 0.642788),
        vec3(0.604023, -0.219846, 0.766044), vec3(0.604023, -0.219846, 0.766044),
        vec3(0.633022, -0.111619, 0.766044), vec3(0.633022, -0.111619, 0.766044),
        vec3(0.469846, -0.171010, 0.866025), vec3(0.469846, -0.171010, 0.866025),
        vec3(0.492404, -0.086824, 0.866025), vec3(0.492404, -0.086824, 0.866025),
        vec3(0.321394, -0.116978, 0.939693), vec3(0.321394, -0.116978, 0.939693),
        vec3(0.336824, -0.059391, 0.939693), vec3(0.336824, -0.059391, 0.939693),
        vec3(0.163176, -0.059391, 0.984808), vec3(0.163176, -0.059391, 0.984808),
        vec3(0.171010, -0.030154, 0.984808), vec3(0.171010, -0.030154, 0.984808),
        vec3(0.000000, 0.000000, 1.000000), vec3(0.000000, 0.000000, 1.000000),
        vec3(0.171010, -0.030154, 0.984808), vec3(0.171010, -0.030154, 0.984808),
        vec3(0.173648, -0.000000, 0.984808), vec3(0.173648, -0.000000, 0.984808),
        vec3(0.336824, -0.059391, 0.939693), vec3(0.336824, -0.059391, 0.939693),
        vec3(0.342020, -0.000000, 0.939693), vec3(0.342020, -0.000000, 0.939693),
        vec3(0.492404, -0.086824, 0.866025), vec3(0.492404, -0.086824, 0.866025),
        vec3(0.500000, -0.000000, 0.866025), vec3(0.500000, -0.000000, 0.866025),
        vec3(0.633022, -0.111619, 0.766044), vec3(0.633022, -0.111619, 0.766044),
        vec3(0.642788, -0.000000, 0.766044), vec3(0.642788, -0.000000, 0.766044),
        vec3(0.754407, -0.133022, 0.642788), vec3(0.754407, -0.133022, 0.642788),
        vec3(0.766044, -0.000000, 0.642788), vec3(0.766044, -0.000000, 0.642788),
        vec3(0.852869, -0.150384, 0.500000), vec3(0.852869, -0.150384, 0.500000),
        vec3(0.866025, -0.000000, 0.500000), vec3(0.866025, -0.000000, 0.500000),
        vec3(0.925417, -0.163176, 0.342020), vec3(0.925417, -0.163176, 0.342020),
        vec3(0.939693, -0.000000, 0.342020), vec3(0.939693, -0.000000, 0.342020),
        vec3(0.969846, -0.171010, 0.173648), vec3(0.969846, -0.171010, 0.173648),
        vec3(0.984808, -0.000000, 0.173648), vec3(0.984808, -0.000000, 0.173648),
        vec3(0.984808, -0.173648, 0.000000), vec3(0.984808, -0.173648, 0.000000),
        vec3(1.000000, -0.000000, 0.000000), vec3(1.000000, -0.000000, 0.000000),
        vec3(0.969846, -0.171010, -0.173648), vec3(0.969846, -0.171010, -0.173648),
        vec3(0.984808, -0.000000, -0.173648), vec3(0.984808, -0.000000, -0.173648),
        vec3(0.925417, -0.163176, -0.342020), vec3(0.925417, -0.163176, -0.342020),
        vec3(0.939693, -0.000000, -0.342020), vec3(0.939693, -0.000000, -0.342020),
        vec3(0.852869, -0.150384, -0.500000), vec3(0.852869, -0.150384, -0.500000),
        vec3(0.866025, -0.000000, -0.500000), vec3(0.866025, -0.000000, -0.500000),
        vec3(0.754407, -0.133022, -0.642788), vec3(0.754407, -0.133022, -0.642788),
        vec3(0.766044, -0.000000, -0.642788), vec3(0.766044, -0.000000, -0.642788),
        vec3(0.633022, -0.111619, -0.766044), vec3(0.633022, -0.111619, -0.766044),
        vec3(0.642788, -0.000000, -0.766044), vec3(0.642788, -0.000000, -0.766044),
        vec3(0.492404, -0.086824, -0.866025), vec3(0.492404, -0.086824, -0.866025),
        vec3(0.500000, -0.000000, -0.866025), vec3(0.500000, -0.000000, -0.866025),
        vec3(0.336824, -0.059391, -0.939693), vec3(0.336824, -0.059391, -0.939693),
        vec3(0.342020, -0.000000, -0.939693), vec3(0.342020, -0.000000, -0.939693),
        vec3(0.171010, -0.030154, -0.984808), vec3(0.171010, -0.030154, -0.984808),
        vec3(0.173648, -0.000000, -0.984808), vec3(0.173648, -0.000000, -0.984808),
        vec3(0.000000, 0.000000, -1.000000), vec3(0.000000, 0.000000, -1.000000)
    };

    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);


    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    // Reading position data
    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        2 * sizeof(vec3),    // stride - each vertex contain 1 vec3 (position)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);

    // Reading normal data
    glVertexAttribPointer(1,                    // attribute 1 matches aNormal in Vertex Shader
        3,                    // size
        GL_FLOAT,             // type
        GL_FALSE,             // normalized?
        2 * sizeof(vec3),     // stride - each vertex contain 1 vec3 (position)
        (void*)(sizeof(vec3)) // array buffer offset
    );
    glEnableVertexAttribArray(1);

    return vertexArrayObject;
}

int loadTexture(char* imagepath)
{
    // Load image using the Free Image library
    FREE_IMAGE_FORMAT format = FreeImage_GetFileType(imagepath, 0);
    FIBITMAP* image = FreeImage_Load(format, imagepath);
    FIBITMAP* image32bits = FreeImage_ConvertTo32Bits(image);

    // Get an available texture index from OpenGL
    GLuint texture = 0;
    glGenTextures(1, &texture);
    assert(texture != 0);

    // Set OpenGL filtering properties (bi-linear interpolation)
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Retrieve width and hight
    int width = FreeImage_GetWidth(image32bits);
    int height = FreeImage_GetHeight(image32bits);

    // This will upload the texture to the GPU memory
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height,
        0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(image32bits));

    // Free images
    FreeImage_Unload(image);
    FreeImage_Unload(image32bits);

    return texture;
}

void setProjectionMatrix(int shaderProgram, mat4 projectionMatrix)
{
    glUseProgram(shaderProgram);
    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
}
void setViewMatrix(int shaderProgram, mat4 viewMatrix)
{
    glUseProgram(shaderProgram);
    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
}
void setWorldMatrix(int shaderProgram, mat4 worldMatrix)
{
    glUseProgram(shaderProgram);
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
}
void setLightSpaceMatrix(int shaderProgram, mat4 lightSpaceMatrix)
{
    glUseProgram(shaderProgram);
    GLuint lightSpaceMatrixLocation = glGetUniformLocation(shaderProgram, "lightSpaceMatrix");
    glUniformMatrix4fv(lightSpaceMatrixLocation, 1, GL_FALSE, &lightSpaceMatrix[0][0]);
}

int main(int argc, char* argv[])
{
    // Initialize GLFW and OpenGL version
    glfwInit();

#if defined(PLATFORM_OSX)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    // On windows, we set OpenGL version to 2.1, to support more hardware
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif

    // Create Window and rendering context using GLFW, resolution is 800x600
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "COMP-371 Winter 2020 Assignment 2", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Key press callback
    glfwSetKeyCallback(window, keyCallback);

    // Disable mouse cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set background color
    glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);

    // Load textures
    snowTextureID = loadTexture("../Assets/Textures/snow.jpg");
    carrotTextureID = loadTexture("../Assets/Textures/carrot.jpg");
    brickTextureID = loadTexture("../Assets/Textures/brick.jpg");

    // Compile and link shaders
    string vertexCode = loadShaderFile("../Assets/Shaders/shader.vs");
    string fragmentCode = loadShaderFile("../Assets/Shaders/shader.fs");
    int shaderProgram = compileAndLinkShaders(vertexCode.c_str(), fragmentCode.c_str());

    string lightingVertexCode = loadShaderFile("../Assets/Shaders/lightingShader.vs");
    string lightingFragmentCode = loadShaderFile("../Assets/Shaders/lightingShader.fs");
    int lightingShaderProgram = compileAndLinkShaders(lightingVertexCode.c_str(), lightingFragmentCode.c_str());

    string textureVertexCode = loadShaderFile("../Assets/Shaders/textureShader.vs");
    string textureFragmentCode = loadShaderFile("../Assets/Shaders/textureShader.fs");
    int textureShaderProgram = compileAndLinkShaders(textureVertexCode.c_str(), textureFragmentCode.c_str());

    string depthDebugVertexCode = loadShaderFile("../Assets/Shaders/debugDepthShader.vs");
    string depthDebugFragmentCode = loadShaderFile("../Assets/Shaders/debugDepthShader.fs");
    int debugDepthShaderProgram = compileAndLinkShaders(depthDebugVertexCode.c_str(), depthDebugFragmentCode.c_str());

    string depthVertexCode = loadShaderFile("../Assets/Shaders/depthShader.vs");
    string depthFragmentCode = loadShaderFile("../Assets/Shaders/depthShader.fs");
    int depthShaderProgram = compileAndLinkShaders(depthVertexCode.c_str(), depthFragmentCode.c_str());

    string testVertexCode = loadShaderFile("../Assets/Shaders/testShader.vs");
    string testFragmentCode = loadShaderFile("../Assets/Shaders/testShader.fs");
    int testShaderProgram = compileAndLinkShaders(testVertexCode.c_str(), testFragmentCode.c_str());

    // Configure depth map FBO
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture
    unsigned int depthMap;
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    glUseProgram(testShaderProgram);
    GLuint textureSamplerLocation = glGetUniformLocation(testShaderProgram, "diffuseTexture");
    glUniform1i(textureSamplerLocation, 0);
    GLuint shadowMapLocation = glGetUniformLocation(testShaderProgram, "shadowMap");
    glUniform1i(shadowMapLocation, 1);

    glUseProgram(debugDepthShaderProgram);
    GLuint depthMapLocation = glGetUniformLocation(debugDepthShaderProgram, "depthMap");
    glUniform1i(debugDepthShaderProgram, 0);

    // Set projection matrix
    mat4 projectionMatrix = perspective(camera_fov,                           // field of view in degrees
                                             (float)SCREEN_WIDTH / SCREEN_HEIGHT,  // aspect ratio
                                             0.01f, 100.0f);                       // near and far (near > 0)

    setProjectionMatrix(shaderProgram, projectionMatrix);
    setProjectionMatrix(lightingShaderProgram, projectionMatrix);
    setProjectionMatrix(textureShaderProgram, projectionMatrix);

    // Set initial view matrix
    mat4 viewMatrix = lookAt(cameraPosition,                 // eye
                             cameraPosition + cameraLookAt,  // center
                             cameraUp);                      // up

    setViewMatrix(shaderProgram, viewMatrix);
    setViewMatrix(lightingShaderProgram, viewMatrix);
    setViewMatrix(textureShaderProgram, viewMatrix);

    // Changing back to default shader program
    glUseProgram(shaderProgram);

    // Set light position in fragment shader
    GLuint lightPositionLocation = glGetUniformLocation(shaderProgram, "lightPos");
    glUniform3fv(lightPositionLocation, 1, &lightPos[0]);

    // Set camera position in fragment shader
    GLuint viewPositionLocation = glGetUniformLocation(shaderProgram, "viewPos");
    glUniform3fv(viewPositionLocation, 1, &cameraPosition[0]);

    // Get color uniform locaiton
    GLuint colorLocation = glGetUniformLocation(shaderProgram, "color");

    // Get world uniform locaiton
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

    // Get world uniform locaiton
    GLuint textureWorldMatrixLocation = glGetUniformLocation(textureShaderProgram, "worldMatrix");

    // Define and upload geometry to the GPU here
    lineVAO = createLineSegmentVertexArrayObject();
    cubeVAO = createCubeVertexArrayObject();
    sphereVAO = createSphereVertexArrayObject();
    testVAO = createTestVAO();
    lampVAO = createCubeVertexArrayObject();
    texturedCubeVAO = createTexturedCubeVertexArrayObject();

    float planeVertices[] = {
        // positions            // normals         // texcoords
         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

         25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
        -25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
         25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 10.0f
    };
    // plane VAO
    unsigned int planeVBO;
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);
    glBindVertexArray(planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glBindVertexArray(0);

    // For frame time
    float lastFrameTime = glfwGetTime();
    int lastMouseLeftState = GLFW_RELEASE;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);

    // Enable Backface culling
    glEnable(GL_CULL_FACE);

    // Enable Depth Test
    glEnable(GL_DEPTH_TEST);


    // Entering Main Loop
    while (!glfwWindowShouldClose(window))
    {
        // Frame time calculation
        dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 1. render depth of scene to texture (from light's perspective)
        // --------------------------------------------------------------
        mat4 lightProjection, lightView;
        mat4 lightSpaceMatrix;
        float near_plane = 1.0f, far_plane = 7.5f;
        lightProjection = ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        lightView = lookAt(lightPos, vec3(0.0f), vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;

        // render scene from light's point of view
        glUseProgram(depthShaderProgram);
        setLightSpaceMatrix(depthShaderProgram, lightSpaceMatrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);

        GLuint modelMatrixLocation = glGetUniformLocation(depthShaderProgram, "model");
        renderScene(depthShaderProgram, modelMatrixLocation);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        //glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //// render Depth map to quad for visual debugging
        //// ---------------------------------------------
        //glUseProgram(debugDepthShaderProgram);

        //GLuint near_planeLocation = glGetUniformLocation(debugDepthShaderProgram, "near_plane");
        //GLuint far_planeLocation = glGetUniformLocation(debugDepthShaderProgram, "far_plane");
        //glUniform1f(near_planeLocation, near_plane);
        //glUniform1f(far_planeLocation, far_plane);

        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, depthMap);
        //renderQuad();


        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(testShaderProgram);

        GLuint testProjectionMatrixLocation = glGetUniformLocation(testShaderProgram, "projection");
        glUniformMatrix4fv(testProjectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

        GLuint testViewMatrixLocation = glGetUniformLocation(testShaderProgram, "view");
        glUniformMatrix4fv(testViewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

        GLuint testLightSpacerMatrixLocation = glGetUniformLocation(testShaderProgram, "lightSpaceMatrix");
        glUniformMatrix4fv(testLightSpacerMatrixLocation, 1, GL_FALSE, &lightSpaceMatrix[0][0]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, snowTextureID);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);

        GLuint testWorldModelMatrixLocation = glGetUniformLocation(testShaderProgram, "model");

        renderScene(testShaderProgram, testWorldModelMatrixLocation);

        // Testing moving light 
        //lightPos = vec3(0.0f, 0.0f, 15 * sin(glfwGetTime()));


        /*
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        float near_plane = 1.0f, far_plane = 7.5f;
        mat4 lightProjection = ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
        mat4 lightView = lookAt(lightPos,
                                vec3(0.0f, 0.0f, 0.0f),
                                vec3(0.0f, 1.0f, 0.0f));
        mat4 lightSpaceMatrix = lightProjection * lightView;
        
        glUseProgram(depthShaderProgram);
        setLightSpaceMatrix(depthShaderProgram, lightSpaceMatrix);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);

        isCalculatingShadows = true;
        GLuint modelMatrixLocation = glGetUniformLocation(depthShaderProgram, "model");


        // Load texture cube vao
        glBindVertexArray(texturedCubeVAO);

        // Draw snow ground
        drawGround(textureShaderProgram, modelMatrixLocation);

        // Load line vao
        glBindVertexArray(lineVAO);

        // Draw Grid
        drawGrid(shaderProgram, modelMatrixLocation, colorLocation);

        // Draw coordinate axis lines
        drawCoordinateAxis(shaderProgram, modelMatrixLocation, colorLocation);

        // Load sphere vao
        glBindVertexArray(sphereVAO);

        // Get color uniform locaiton
        //GLuint colorLocation = glGetUniformLocation(shaderProgram, "color");

        // Get world uniform locaiton
        //GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

        // Draw Olaf
        drawOlaf(shaderProgram, modelMatrixLocation, colorLocation, lastFrameTime);

        // Load texture cube vao
        glBindVertexArray(texturedCubeVAO);

        // Draw carrot nose
        drawCarrotNose(textureShaderProgram, modelMatrixLocation);

        //// Update view and projection matrices
        //glUseProgram(shaderProgram);
        //updateViewAndProjection(shaderProgram);

        //// Update texture shader program
        //glUseProgram(textureShaderProgram);
        //updateViewAndProjection(textureShaderProgram);

        //// Use lighting shader program
        //glUseProgram(lightingShaderProgram);
        //updateViewAndProjection(lightingShaderProgram);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);


        // ========================== SECOND PASS =====================================

        isCalculatingShadows = false;

        // Clear Color Buffer Bit and Depth Buffer Bit 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

        // Use texture shader program
        glUseProgram(textureShaderProgram);

        // Load texture cube vao
        glBindVertexArray(texturedCubeVAO);

        // Draw snow ground
        drawGround(textureShaderProgram, textureWorldMatrixLocation);

        // Use Assignment 1 shader program
        glUseProgram(shaderProgram);

        // Load line vao
        glBindVertexArray(lineVAO);

        // Draw Grid
        drawGrid(shaderProgram, worldMatrixLocation, colorLocation);

        // Draw coordinate axis lines
        drawCoordinateAxis(shaderProgram, worldMatrixLocation, colorLocation);

        // Load sphere vao
        glBindVertexArray(sphereVAO);

        // Get color uniform locaiton
        //GLuint colorLocation = glGetUniformLocation(shaderProgram, "color");

        // Get world uniform locaiton
        //GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

        // Draw Olaf
        drawOlaf(shaderProgram, worldMatrixLocation, colorLocation, lastFrameTime);

        // Change to texture shader program
        glUseProgram(textureShaderProgram);

        // Load texture cube vao
        glBindVertexArray(texturedCubeVAO);

        // Draw carrot nose
        drawCarrotNose(textureShaderProgram, textureWorldMatrixLocation);

        // Update view and projection matrices
        glUseProgram(shaderProgram);
        updateViewAndProjection(shaderProgram);

        // Update texture shader program
        glUseProgram(textureShaderProgram);
        updateViewAndProjection(textureShaderProgram);

        // Use lighting shader program
        glUseProgram(lightingShaderProgram);
        updateViewAndProjection(lightingShaderProgram);

        //// Testing moving light 
        //lightPos = vec3(5.0f, 8.0f, -2.5f) + vec3(0.0f, 0.0f, 15 * sin(glfwGetTime()));
        //GLuint lightPositionLocation = glGetUniformLocation(shaderProgram, "lightPos");
        //glUniform3fv(shaderProgram, 1, &lightPos[0]);

        mat4 modelMatrix = mat4(1.0f);
        modelMatrix = translate(modelMatrix, lightPos);
        modelMatrix = scale(modelMatrix, vec3(1.5f));

        GLuint lightingWorldMatrixLocation = glGetUniformLocation(lightingShaderProgram, "worldMatrix");
        glUniformMatrix4fv(lightingWorldMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

        glBindVertexArray(lampVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        */

        /*
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        // Load line vao
        glBindVertexArray(lineVAO);

        // Draw Grid
        //drawGrid(shaderProgram, worldMatrixLocation, colorLocation);

        // Draw coordinate axis lines
        //drawCoordinateAxis(shaderProgram, worldMatrixLocation, colorLocation);

        renderScene(shaderProgram, worldMatrixLocation);
        updateViewAndProjection(shaderProgram);
        */


        // End Frame
        glfwSwapBuffers(window);

        // Check for user inputs
        glfwPollEvents();

        // Handle inputs
        processUserInput(window);

    }

    // Shutdown GLFW
    glfwTerminate();

    return 0;
}

void renderScene(int shaderProgram, GLuint modelMatrixLocation)
{
    glCullFace(GL_FRONT);

    // floor
    mat4 model = mat4(1.0f);
    model = translate(model, vec3(0.0f, -1.0f, 0.0));
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
    glBindVertexArray(planeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glCullFace(GL_BACK);

    // cubes
    model = mat4(1.0f);
    model = translate(model, vec3(0.0f, 2.5f, 0.0));
    model = scale(model, vec3(1.0f));
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
    //glBindVertexArray(texturedCubeVAO);
    //glDrawArrays(GL_TRIANGLES, 0, 36);
    //glBindVertexArray(0);
    renderCube();


    model = mat4(1.0f);
    model = translate(model, vec3(4.0f, 0.0f, 1.0));
    model = scale(model, vec3(1.0f));
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
    //glBindVertexArray(texturedCubeVAO);
    //glDrawArrays(GL_TRIANGLES, 0, 36);
    //glBindVertexArray(0);
    renderCube();


    model = mat4(1.0f);
    model = translate(model, vec3(-2.0f, 0.0f, 2.0));
    model = rotate(model, radians(60.0f), normalize(vec3(1.0, 0.0, 1.0)));
    model = scale(model, vec3(1.0));
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
 /*   glBindVertexArray(texturedCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);*/
    renderCube();

}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{

    if (quadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

float getRandomFloat(float start, float end) {
    // The following code to generate a random float within a range was obtained from: https://stackoverflow.com/a/5289624
    float random = ((float)rand()) / (float)RAND_MAX;
    float difference = end - start;
    float offset = random * difference;
    return start + offset;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Reset world orientation
    if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        cameraPosition = vec3(15.0f, 5.0f, 15.0f);
        cameraLookAt = normalize(vec3(-15.0f, -5.0f, -15.0f));
        cameraUp = vec3(0.0f, 1.0f, 0.0f);

        cameraSideVector = cross(cameraLookAt, cameraUp);
        camera_fov = 70.0f;

        cameraRotateX = 0.0f;
        cameraRotateY = 0.0f;

        cameraHorizontalAngle = 180.0f - degrees(acos(dot(normalize(vec3(1, 0, 0)), normalize(cameraPosition))));
        cameraVerticalAngle = -degrees(acos(dot(normalize(vec3(1, 0, 1)), normalize(cameraPosition))));
    }

    // Reset Olaf position
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        olafAngle = 90.0f;
        olafScale = 0.6f;
        olafPosition = vec3(0.0f, -1.2f, 0.0f);
        olafLookAt = vec3(1.0f, 0.0f, 0.0f);
        olafUp = vec3(0.0f, 1.0f, 0.0f);
        olafSideVector = cross(olafLookAt, olafUp);
    }

    // Randomly reposition olaf
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        olafPosition = vec3(getRandomFloat(-(float)sizeOfGrid / 2, (float)sizeOfGrid / 2),
            0.0f,
            getRandomFloat(-(float)sizeOfGrid / 2, (float)sizeOfGrid / 2));
    }

    // Scale up olaf
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        olafScale += olafScaleAmount;
    }

    // Scale down olaf
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        olafScale -= olafScaleAmount;

        // Limit the scale of Olaf
        if (olafScale <= 0.05f) {
            olafScale = 0.05f;
        }
    }

    // Set render mode to points
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        showGrid = false;
        renderMode = GL_POINTS;
        glDisable(GL_CULL_FACE);
    }

    // Set render mode to lines
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        showGrid = true;
        renderMode = GL_TRIANGLE_STRIP;
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
    }

    // Set render mode to triangles
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        showGrid = false;
        renderMode = GL_TRIANGLE_STRIP;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_CULL_FACE);
    }

}

void processUserInput(GLFWwindow* window)
{
    // Close window on escape
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // Used to translate Olaf
    isHoldingShift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
    moveForwardAndBack = false;
    moveLeftRight = false;

    // Move olaf forward
    if (isHoldingShift && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        olafPosition += olafLookAt * olafSpeed * dt;
        moveForwardAndBack = true;
    }

    // Move olaf backward
    if (isHoldingShift && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        olafPosition -= olafLookAt * olafSpeed * dt;
        moveForwardAndBack = true;
    }

    // Move olaf left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        if (isHoldingShift)
        {
            olafPosition -= olafSideVector * olafSpeed * dt;
            moveLeftRight = true;
        }
        else
        {
            olafAngle += olafRotationAmount;
            olafLookAt = vec3(sin(radians(olafAngle)), 0.0f, cos(radians(olafAngle)));
            olafSideVector = cross(olafLookAt, vec3(0.0f, 1.0f, 0.0f));
            moveForwardAndBack = true;
        }
    }

    // Move olaf right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        if (isHoldingShift)
        {
            olafPosition += olafSideVector * olafSpeed * dt;
            moveLeftRight = true;
        }
        else
        {
            olafAngle -= olafRotationAmount;
            olafLookAt = vec3(sin(radians(olafAngle)), 0.0f, cos(radians(olafAngle)));
            olafSideVector = cross(olafLookAt, vec3(0.0f, 1.0f, 0.0f));
            moveForwardAndBack = true;
        }
    }

    // Caculate cursor position
    double mousePosX, mousePosY;
    glfwGetCursorPos(window, &mousePosX, &mousePosY);

    // Determine the change in cursor position
    double dx = mousePosX - lastMousePosX;
    double dy = mousePosY - lastMousePosY;

    // Update previous cursor position values
    lastMousePosX = mousePosX;
    lastMousePosY = mousePosY;

    // Check if left mouse btn is held
    isLeftMouseBtnHeld = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;

    // Check if right mouse btn is held
    isRightMouseBtnHeld = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

    // Check if middle mouse btn is held
    isMiddleMouseBtnHeld = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS;

    // Allow panning if right mouse btn is held
    if (isRightMouseBtnHeld)
    {
        cameraHorizontalAngle -= dx * cameraPanningSpeed * dt;
    }

    // Allow tilting if middle mouse btn is held
    if (isMiddleMouseBtnHeld)
    {
        cameraVerticalAngle -= dy * cameraTiltingSpeed * dt;
    }

    // Clamp vertical angle to [-85, 85] degrees
    cameraVerticalAngle = max(-85.0f, min(85.0f, cameraVerticalAngle));
    if (cameraHorizontalAngle > 360)
    {
        cameraHorizontalAngle -= 360;
    }
    else if (cameraHorizontalAngle < -360)
    {
        cameraHorizontalAngle += 360;
    }

    float theta = radians(cameraHorizontalAngle);
    float phi = radians(cameraVerticalAngle);

    cameraLookAt = vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));
    cameraSideVector = normalize(cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f)));

    // Allow zomming if left mouse btn is held
    if (isLeftMouseBtnHeld)
    {
        camera_fov += cameraFovChangeSpeed * (float)dy;
    }

    // Rotate camera up
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        cameraRotateY += cameraRotationAmount;
    }

    // Rotate camera down
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        cameraRotateY -= cameraRotationAmount;
    }

    // Rotate camera left
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        cameraRotateX += cameraRotationAmount;
    }

    // Rotate camera right
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        cameraRotateX -= cameraRotationAmount;
    }
}

void outlineShape(GLuint colorLocation)
{
    //GLint polygonMode[2];
    //glGetIntegerv(GL_POLYGON_MODE, polygonMode);

    //if (polygonMode[0] != GL_LINES)
    //{
    //    glLineWidth(6);
    //    glUniform3fv(colorLocation, 1, &black[0]);
    //    glCullFace(GL_FRONT);
    //    //glPolygonMode(GL_BACK, GL_LINE);
    //    glDrawArrays(renderMode, 0, numTriangles);
    //    glCullFace(GL_BACK);
    //    glLineWidth(1);
    //}

}

void drawGrid(int shaderProgram, GLuint worldMatrixLocation, GLuint colorLocation)
{
    // Draw grid
    glLineWidth(1);

    // Set color of grid
    glUniform3fv(colorLocation, 1, &yellow[0]);

    // Translate grid origin to world origin
    mat4 worldMatrix = translate(mat4(1.0f), vec3((-sizeOfGrid / 2) - 1, 0, -sizeOfGrid / 2));

    // Draw horizontal lines
    for (int i = 0; i < sizeOfGrid + 1; ++i)
    {
        for (int j = 0; j < sizeOfGrid; ++j)
        {
            worldMatrix = translate(worldMatrix, vec3(1, 0, 0));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
            if (showGrid) 
                glDrawArrays(GL_LINES, 0, 2);
        }
        worldMatrix = translate(worldMatrix, vec3(-sizeOfGrid, 0, 1));
    }

    // Rotate world to draw vertical lines
    worldMatrix = translate(mat4(1.0f), vec3((-sizeOfGrid / 2) - 1, 0, -sizeOfGrid / 2));
    worldMatrix = rotate(worldMatrix, radians(-90.0f), vec3(0.0f, 1.0f, 0.0f));

    // Draw vertical lines
    for (int i = 0; i < sizeOfGrid; ++i)
    {
        for (int j = 0; j < sizeOfGrid + 1; ++j)
        {
            worldMatrix = translate(worldMatrix, vec3(0, 0, -1));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
            if (showGrid)
                glDrawArrays(GL_LINES, 0, 2);
        }
        worldMatrix = translate(worldMatrix, vec3(1, 0, sizeOfGrid + 1));
    }
}

void drawGround(int shaderProgram, GLuint worldMatrixLocation)
{
    if (!isCalculatingShadows)
    {
        // Draw snow ground
        glActiveTexture(GL_TEXTURE0);
        GLuint textureLocation = glGetUniformLocation(shaderProgram, "textureSampler");
        glBindTexture(GL_TEXTURE_2D, snowTextureID);
        glUniform1i(textureLocation, 0);                // Set our Texture sampler to user Texture Unit 0

    }
    mat4 groundWorldMatrix = translate(mat4(1.0f), vec3(0.0f, -0.01f, 0.0f)) * scale(mat4(1.0f), vec3(sizeOfGrid, 0.02f, sizeOfGrid));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &groundWorldMatrix[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0
}

void drawCoordinateAxis(int shaderProgram, GLuint worldMatrixLocation, GLuint colorLocation)
{
    glLineWidth(8);

    // Draw red X axis
    glUniform3fv(colorLocation, 1, &red[0]);

    mat4 worldMatrix = translate(mat4(1.0f), vec3(0.0f, 0.01f, 0.0f));
    worldMatrix = scale(worldMatrix, vec3(5.0f, 0.0f, 0.0f));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(GL_LINES, 0, 2);

    // Draw blue z axis
    glUniform3fv(colorLocation, 1, &blue[0]);

    worldMatrix = mat4(1.0f);
    worldMatrix = translate(worldMatrix, vec3(0.0f, 0.01f, 0.0f));
    worldMatrix = rotate(worldMatrix, radians(-90.0f), vec3(0.0f, 1.0f, 0.0f));
    worldMatrix = scale(worldMatrix, vec3(5.0f, 0.0f, 0.0f));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(GL_LINES, 0, 2);

    // Draw green y axis
    glUniform3fv(colorLocation, 1, &green[0]);

    worldMatrix = mat4(1.0f);
    worldMatrix = translate(worldMatrix, vec3(0.0f, 0.01f, 0.0f));
    worldMatrix = rotate(worldMatrix, radians(90.0f), vec3(0.0f, 0.0f, 1.0f));
    worldMatrix = scale(worldMatrix, vec3(5.0f, 0.0f, 0.0f));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    glDrawArrays(GL_LINES, 0, 2);
}

void drawObject()
{
    if (drawWithReversedFaceCulling)
    {
        glCullFace(GL_FRONT);
        glDrawArrays(renderMode, 0, numTriangles);
        glCullFace(GL_BACK);
    }
    else
    {
        glDrawArrays(renderMode, 0, numTriangles);
    }
}

void drawOlaf(int shaderProgram, GLuint worldMatrixLocation, GLuint colorLocation, float lastFrameTime)
{    
    // Set point size and line width for rendering Olaf as points or as wireframe
    glPointSize(2);
    glLineWidth(1);

    // Olaf base
    glUniform3fv(colorLocation, 1, &white[0]);

    mat4 olafBaseWorldMatrix = mat4(1.0f);
    olafBaseWorldMatrix = translate(olafBaseWorldMatrix, vec3(0.0f, 3.5f, 0.0f));
    olafBaseWorldMatrix = translate(olafBaseWorldMatrix, olafPosition);
    olafBaseWorldMatrix = rotate(olafBaseWorldMatrix, radians(olafAngle), vec3(0.0f, 1.0f, 0.0f));
    olafBaseWorldMatrix = scale(olafBaseWorldMatrix, vec3(3.5f, 3.0f, 3.0f));
    olafBaseWorldMatrix = scale(olafBaseWorldMatrix, vec3(olafScale, olafScale, olafScale));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafBaseWorldMatrix[0][0]);
    drawObject();

    outlineShape(colorLocation);

    // Olaf base dots
    glUniform3fv(colorLocation, 1, &darkBlue[0]);
    mat4 olafBaseDotOne = olafBaseWorldMatrix;
    olafBaseDotOne = translate(olafBaseDotOne, vec3(0.0f, 0.07f, 1.0f));
    olafBaseDotOne = scale(olafBaseDotOne, vec3(0.07f, 0.07f, 0.07f));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafBaseDotOne[0][0]);
    drawObject();

    mat4 olafBaseDotTwo = translate(olafBaseDotOne, vec3(0.0f, -3.5f, 0.0f));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafBaseDotTwo[0][0]);
    drawObject();

    // Olaf belt
    glUniform3fv(colorLocation, 1, &brown[0]);

    mat4 olafBeltWorldMatrix = olafBaseWorldMatrix;
    olafBeltWorldMatrix = translate(olafBeltWorldMatrix, vec3(0.0f, 0.65f, 0.0f));
    olafBeltWorldMatrix = scale(olafBeltWorldMatrix, vec3(0.85f, 0.2f, 0.85f));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafBeltWorldMatrix[0][0]);
    drawObject();

    outlineShape(colorLocation);

    // Olaf belt bulk
    glUniform3fv(colorLocation, 1, &gray[0]);
    olafBeltWorldMatrix = translate(olafBeltWorldMatrix, vec3(0.0f, 0.0f, 0.8f));
    olafBeltWorldMatrix = scale(olafBeltWorldMatrix, vec3(0.3f, 0.7f, 0.3f));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafBeltWorldMatrix[0][0]);
    drawObject();

    outlineShape(colorLocation);

    glPointSize(0.01);
    glLineWidth(0.01);


    float footAngle = 0.0f;
    float footDistanceFromBase = 1.0f;
    if (moveForwardAndBack)
    {
        footAngle = (int)(40 * sin(lastFrameTime * 5)) % 75;
    }
    else if (moveLeftRight)
    {
        footAngle = (int)(10 * sin(lastFrameTime * 3)) % 75 + 20;
    }

    // Olaf right foot
    glUniform3fv(colorLocation, 1, &white[0]);

    mat4 olafRightFootWorldMatrix = olafBaseWorldMatrix;
    if (moveForwardAndBack)
    {
        olafRightFootWorldMatrix = rotate(olafRightFootWorldMatrix, radians(-footAngle), vec3(1.0f, 0.0f, 0.0f));
    }
    else if (moveLeftRight)
    {
        olafRightFootWorldMatrix = rotate(olafRightFootWorldMatrix, radians(-footAngle), vec3(0.0f, 0.0f, 1.0f));
    }
    olafRightFootWorldMatrix = translate(olafRightFootWorldMatrix, vec3(-0.3f, -footDistanceFromBase, 0.0f));
    olafRightFootWorldMatrix = scale(olafRightFootWorldMatrix, vec3(0.2f, 0.2f, 0.5f));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafRightFootWorldMatrix[0][0]);
    drawObject();

    outlineShape(colorLocation);

    // Olaf left foot
    glUniform3fv(colorLocation, 1, &white[0]);

    mat4 olafLeftFootWorldMatrix = olafBaseWorldMatrix;
    if (moveForwardAndBack)
    {
        olafLeftFootWorldMatrix = rotate(olafLeftFootWorldMatrix, radians(footAngle), vec3(1.0f, 0.0f, 0.0f));
    }
    else if (moveLeftRight)
    {
        olafLeftFootWorldMatrix = rotate(olafLeftFootWorldMatrix, radians(footAngle), vec3(0.0f, 0.0f, 1.0f));
    }
    olafLeftFootWorldMatrix = translate(olafLeftFootWorldMatrix, vec3(0.3f, -footDistanceFromBase, 0.0f));
    olafLeftFootWorldMatrix = scale(olafLeftFootWorldMatrix, vec3(0.2f, 0.2f, 0.5f));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafLeftFootWorldMatrix[0][0]);
    drawObject();

    outlineShape(colorLocation);

    glPointSize(2);
    glLineWidth(1);

    // Olaf torso
    glUniform3fv(colorLocation, 1, &white[0]);

    mat4 olafTorsoWorldMatrix = olafBaseWorldMatrix;
    olafTorsoWorldMatrix = translate(olafTorsoWorldMatrix, vec3(0.0f, 1.0f, 0.0f));
    olafTorsoWorldMatrix = scale(olafTorsoWorldMatrix, vec3(0.75f, 0.75f, 0.75f));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafTorsoWorldMatrix[0][0]);
    drawObject();

    outlineShape(colorLocation);

    // Olaf torso dot
    glUniform3fv(colorLocation, 1, &darkBlue[0]);
    mat4 olafTorsoDot = olafBaseWorldMatrix;
    olafTorsoDot = translate(olafTorsoDot, vec3(0.0f, 1.0f, 0.75f));
    olafTorsoDot = scale(olafTorsoDot, vec3(0.07f, 0.07f, 0.07f));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafTorsoDot[0][0]);
    drawObject();

    // Olaf scarf base
    glUniform3fv(colorLocation, 1, &red[0]);
    mat4 olafScarfWorldMatrix = olafTorsoWorldMatrix;
    olafScarfWorldMatrix = translate(olafScarfWorldMatrix, vec3(0.0f, 0.5f, 0.0f));
    olafScarfWorldMatrix = scale(olafScarfWorldMatrix, vec3(1.0f, 0.4f, 1.0f));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafScarfWorldMatrix[0][0]);
    drawObject();

    outlineShape(colorLocation);

    // Olaf scarf end
    glUniform3fv(colorLocation, 1, &red[0]);
    olafScarfWorldMatrix = olafTorsoWorldMatrix;
    olafScarfWorldMatrix = translate(olafScarfWorldMatrix, vec3(0.3f, 0.2f, 0.9f));
    olafScarfWorldMatrix = scale(olafScarfWorldMatrix, vec3(0.2f, 0.4f, 0.1f));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafScarfWorldMatrix[0][0]);
    drawObject();

    outlineShape(colorLocation);

    // Olaf head
    glUniform3fv(colorLocation, 1, &white[0]);
    mat4 olafHeadWorldMatrix = olafTorsoWorldMatrix;
    olafHeadWorldMatrix = translate(olafHeadWorldMatrix, vec3(0.0f, 1.2f, 0.0f));
    olafHeadWorldMatrix = scale(olafHeadWorldMatrix, vec3(0.8f, 0.9f, 0.8f));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafHeadWorldMatrix[0][0]);
    drawObject();

    outlineShape(colorLocation);

    // Olaf nose
    glUniform3fv(colorLocation, 1, &orange[0]);

    olafNoseWorldMatrix = olafTorsoWorldMatrix;
    olafNoseWorldMatrix = translate(olafNoseWorldMatrix, vec3(0.0f, 1.0f, 0.6f));
    olafNoseWorldMatrix = scale(olafNoseWorldMatrix, vec3(0.25f, 0.25f, 0.5f));                         // 4. Add nose scale
    olafNoseWorldMatrix = scale(olafNoseWorldMatrix, vec3(1.0f / 3.5f, 1.0f / 3.0f, 1.0f));             // 3. revert base scale
    olafNoseWorldMatrix = scale(olafNoseWorldMatrix, vec3(1.0f / 0.75f, 1.0f / 0.5f, 1.0f / 0.75f));    // 2. revert torso scale
    olafNoseWorldMatrix = scale(olafNoseWorldMatrix, vec3(1.0f / 0.8f, 1.0f, 1.0f / 0.8f));             // 1. revert head scale

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafNoseWorldMatrix[0][0]);
    //drawObject();

    // Olaf right eye
    glUniform3fv(colorLocation, 1, &black[0]);

    mat4 olafRightEyeWorldMatrix = olafNoseWorldMatrix;
    olafRightEyeWorldMatrix = translate(olafRightEyeWorldMatrix, vec3(-1.6f, 1.0f, -0.25f));
    olafRightEyeWorldMatrix = scale(olafRightEyeWorldMatrix, vec3(1.0f, 1.0f, 0.5f));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafRightEyeWorldMatrix[0][0]);
    drawObject();

    // Olaf left eye
    mat4 olafLeftEyeWorldMatrix = olafNoseWorldMatrix;
    olafLeftEyeWorldMatrix = translate(olafLeftEyeWorldMatrix, vec3(1.6f, 1.0f, -0.25f));
    olafLeftEyeWorldMatrix = scale(olafLeftEyeWorldMatrix, vec3(1.0f, 1.0f, 0.5f));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafLeftEyeWorldMatrix[0][0]);
    drawObject();

    // Olaf top hat base
    glUniform3fv(colorLocation, 1, &black[0]);

    mat4 olafHatWorldMatrix = olafHeadWorldMatrix;
    olafHatWorldMatrix = translate(olafHatWorldMatrix, vec3(0.0f, 0.6f, 0.0f));
    olafHatWorldMatrix = scale(olafHatWorldMatrix, vec3(1.5f, 0.25f, 1.5f));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafHatWorldMatrix[0][0]);
    drawObject();

    // Olaf top hat mid section
    glUniform3fv(colorLocation, 1, &gray[0]);

    mat4 olafHatMidMatrix = olafHatWorldMatrix;
    olafHatMidMatrix = translate(olafHatMidMatrix, vec3(0.0f, 0.88f, 0.0f));
    olafHatMidMatrix = scale(olafHatMidMatrix, vec3(0.75f, 0.75f, 0.75f));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafHatMidMatrix[0][0]);
    drawObject();

    // Olaf top hat top section
    glUniform3fv(colorLocation, 1, &black[0]);

    mat4 olafHatTopMatrix = olafHatMidMatrix;
    olafHatTopMatrix = translate(olafHatTopMatrix, vec3(0.0f, 3.05f, 0.0f));
    olafHatTopMatrix = scale(olafHatTopMatrix, vec3(1.0f, 6.0f, 1.0f));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafHatTopMatrix[0][0]);
    drawObject();

    float initialArmAngle = 70.0f;
    float armPivotLength = 1.25f;
    float armAngle = (int)(40 * sin(lastFrameTime * 5)) % 75;
    float armDistanceFromTorso = 1.0f;
    float gloveDistance = 0.75f;

    // Olaf right arm
    glUniform3fv(colorLocation, 1, &white[0]);

    mat4 olafRightArmWorldMatrix = olafTorsoWorldMatrix;

    if (moveForwardAndBack)
    {
        olafRightArmWorldMatrix = translate(olafRightArmWorldMatrix, vec3(-armDistanceFromTorso, 1.4f, 0.0f));
        olafRightArmWorldMatrix = translate(olafRightArmWorldMatrix, armPivotLength * vec3(0.0f, -cos(radians(armAngle)), -sin(radians(armAngle))));
        olafRightArmWorldMatrix = translate(olafRightArmWorldMatrix, armPivotLength * vec3(-cos(radians(initialArmAngle)), -sin(radians(initialArmAngle)), 0.0f));
        olafRightArmWorldMatrix = rotate(olafRightArmWorldMatrix, radians(armAngle), vec3(1.0f, 0.0f, 0.0f));
        olafRightArmWorldMatrix = rotate(olafRightArmWorldMatrix, radians(initialArmAngle), vec3(0.0f, 0.0f, 1.0f));
        olafRightArmWorldMatrix = rotate(olafRightArmWorldMatrix, radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
        olafRightArmWorldMatrix = scale(olafRightArmWorldMatrix, vec3(1.5f, 0.25f, 0.3f));
    }
    else
    {
        olafRightArmWorldMatrix = translate(olafRightArmWorldMatrix, vec3(-armDistanceFromTorso, 0.2f, 0.0f));
        olafRightArmWorldMatrix = translate(olafRightArmWorldMatrix, armPivotLength * vec3(-cos(radians(-armAngle)), -sin(radians(-armAngle)), 0.0f));
        olafRightArmWorldMatrix = rotate(olafRightArmWorldMatrix, radians(-armAngle), vec3(0.0f, 0.0f, 1.0f));
        olafRightArmWorldMatrix = scale(olafRightArmWorldMatrix, vec3(1.5f, 0.25f, 0.3f));
    }

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafRightArmWorldMatrix[0][0]);
    drawObject();

    outlineShape(colorLocation);

    // Olaf right glove
    glUniform3fv(colorLocation, 1, &red[0]);

    olafRightArmWorldMatrix = translate(olafRightArmWorldMatrix, vec3(-gloveDistance, 0.0f, 0.0f));
    olafRightArmWorldMatrix = scale(olafRightArmWorldMatrix, vec3(0.25f, 1.2f, 1.15f));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafRightArmWorldMatrix[0][0]);
    drawObject();

    outlineShape(colorLocation);

    // Olaf right thumb
    glUniform3fv(colorLocation, 1, &red[0]);
    olafRightArmWorldMatrix = translate(olafRightArmWorldMatrix, vec3(0.25f, 0.55f, 0.2f));
    olafRightArmWorldMatrix = scale(olafRightArmWorldMatrix, vec3(0.3f, 1.0f, 0.75f));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafRightArmWorldMatrix[0][0]);
    drawObject();

    outlineShape(colorLocation);

    // Olaf left arm
    glUniform3fv(colorLocation, 1, &white[0]);

    mat4 olafLeftArmWorldMatrix = olafTorsoWorldMatrix;

    if (moveForwardAndBack)
    {
        olafLeftArmWorldMatrix = translate(olafLeftArmWorldMatrix, vec3(armDistanceFromTorso, 1.4f, 0.0f));
        olafLeftArmWorldMatrix = translate(olafLeftArmWorldMatrix, armPivotLength * vec3(0.0f, -cos(radians(-armAngle)), -sin(radians(-armAngle))));
        olafLeftArmWorldMatrix = translate(olafLeftArmWorldMatrix, armPivotLength * vec3(cos(radians(-initialArmAngle)), sin(radians(-initialArmAngle)), 0.0f));
        olafLeftArmWorldMatrix = rotate(olafLeftArmWorldMatrix, radians(-armAngle), vec3(1.0f, 0.0f, 0.0f));
        olafLeftArmWorldMatrix = rotate(olafLeftArmWorldMatrix, radians(-initialArmAngle), vec3(0.0f, 0.0f, 1.0f));
        olafLeftArmWorldMatrix = rotate(olafLeftArmWorldMatrix, radians(90.0f), vec3(1.0f, 0.0f, 0.0f));
        olafLeftArmWorldMatrix = scale(olafLeftArmWorldMatrix, vec3(1.5f, 0.25f, 0.3f));
    }
    else
    {
        olafLeftArmWorldMatrix = translate(olafLeftArmWorldMatrix, vec3(armDistanceFromTorso, 0.2f, 0.0f));
        olafLeftArmWorldMatrix = translate(olafLeftArmWorldMatrix, armPivotLength * vec3(cos(radians(armAngle)), sin(radians(armAngle)), 0.0f));
        olafLeftArmWorldMatrix = rotate(olafLeftArmWorldMatrix, radians(armAngle), vec3(0.0f, 0.0f, 1.0f));
        olafLeftArmWorldMatrix = scale(olafLeftArmWorldMatrix, vec3(1.5f, 0.25f, 0.3f));
    }

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafLeftArmWorldMatrix[0][0]);
    drawObject();

    outlineShape(colorLocation);

    // Olaf left glove
    glUniform3fv(colorLocation, 1, &red[0]);
    olafLeftArmWorldMatrix = translate(olafLeftArmWorldMatrix, vec3(gloveDistance, 0.0f, 0.0f));
    olafLeftArmWorldMatrix = scale(olafLeftArmWorldMatrix, vec3(0.25f, 1.2f, 1.15f));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafLeftArmWorldMatrix[0][0]);
    drawObject();

    outlineShape(colorLocation);

    // Olaf left thumb
    glUniform3fv(colorLocation, 1, &red[0]);
    olafLeftArmWorldMatrix = translate(olafLeftArmWorldMatrix, vec3(-0.25f, 0.55f, 0.2f));
    olafLeftArmWorldMatrix = scale(olafLeftArmWorldMatrix, vec3(0.3f, 1.0f, .75f));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafLeftArmWorldMatrix[0][0]);
    drawObject();

    outlineShape(colorLocation);
    
}

void drawCarrotNose(int textureShaderProgram, GLuint texturedWorldMatrixLocation)
{
    // Draw carrot
    if (!isCalculatingShadows)
    {
        glActiveTexture(GL_TEXTURE1);
        GLuint textureLocation = glGetUniformLocation(textureShaderProgram, "textureSampler");
        glBindTexture(GL_TEXTURE_2D, carrotTextureID);
        glUniform1i(textureLocation, 1);
    }

    mat4 carrotNoseWorldMatrix = olafNoseWorldMatrix;
    carrotNoseWorldMatrix = translate(carrotNoseWorldMatrix, vec3(0.0f, 0.0f, 0.3f));
    glUniformMatrix4fv(texturedWorldMatrixLocation, 1, GL_FALSE, &carrotNoseWorldMatrix[0][0]);

    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void updateViewAndProjection(int shaderProgram)
{
    // Update projection matrix
    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    mat4 projectionMatrix = perspective(camera_fov,                                     // field of view in degrees
        (float)SCREEN_WIDTH / SCREEN_HEIGHT,            // aspect ratio
        0.01f, 100.0f);                                 // near and far (near > 0)
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Update view matrix
    mat4 viewMatrix = mat4(1.0);
    viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
    viewMatrix = rotate(viewMatrix, radians(cameraRotationSpeed * cameraRotateX), cameraUp);

    float cameraRotationAngle = cameraRotationSpeed * (cameraRotateX - 45.0f);
    vec3 cameraRotationPos = vec3(-cos(radians(cameraRotationAngle)), 0.0f, -sin(radians(cameraRotationAngle)));
    viewMatrix = rotate(viewMatrix, radians(cameraRotationSpeed * cameraRotateY), cameraRotationPos);

    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
}
