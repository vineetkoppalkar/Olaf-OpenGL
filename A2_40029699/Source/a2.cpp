#include <iostream>
#include <string>
#include <fstream>
#include <list>

#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler

#include <GLFW/glfw3.h> // cross-platform interface for creating a graphical context,
                        // initializing OpenGL and binding inputs

#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>

#include "SphereModel.h"

using namespace glm;
using namespace std;

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void processUserInput(GLFWwindow* window);
void drawGrid(int shaderProgram, GLuint worldMatrixLocation, GLuint colorLocation);
void drawCoordinateAxis(int shaderProgram, GLuint worldMatrixLocation, GLuint colorLocation);
void drawOlaf(GLuint worldMatrixLocation, GLuint colorLocation, float lastFrameTime);
void updateViewAndProjection(int shaderProgram);

// Screen sizes
int SCREEN_WIDTH = 1024;
int SCREEN_HEIGHT = 768;

// Grid parameters
int sizeOfGrid = 100;

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

// Olaf parameters
vec3 olafPosition(0.0f, -1.2f, 0.0f);
vec3 olafLookAt(1.0f, 0.0f, 0.0f);
vec3 olafUp(0.0f, 1.0f, 0.0f);
vec3 olafSideVector = cross(olafLookAt, olafUp);

// Olaf other parameters
float olafAngle = 90.0f;
float olafSpeed = 5.0f;
float olafScale = 0.6f;
float olafRotationAmount = 5.0f;
float olafScaleAmount = 0.05f;

// Rendering parameters
bool drawWithReversedFaceCulling = true;
GLenum renderMode = GL_TRIANGLE_STRIP;
int numTriangles = 1267;

// Lighting position
vec3 lightPos(5.0f, 8.0f, -2.5f);

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

const char* getVertexShaderSource()
{
    return
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;"
        ""
        "uniform mat4 worldMatrix;"
        "uniform mat4 viewMatrix = mat4(1.0);"
        "uniform mat4 projectionMatrix = mat4(1.0);"
        ""
        "void main()"
        "{"
        "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
        "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
        "}";
}
const char* getFragmentShaderSource()
{
    return
        "#version 330 core\n"
        ""
        "uniform vec3 color = vec3(1.0f, 1.0f, 1.0f);"
        "uniform vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);"
        "out vec4 FragColor;"
        ""
        "void main()"
        "{"
        "   FragColor = vec4(lightColor * color, 1.0f);"
        "}";
}

const char* getLightingVertexShaderSource()
{
    return
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;"
        ""
        "uniform mat4 worldMatrix;"
        "uniform mat4 viewMatrix = mat4(1.0);"
        "uniform mat4 projectionMatrix = mat4(1.0);"
        ""
        "void main()"
        "{"
        "   mat4 modelViewProjection = projectionMatrix * viewMatrix * worldMatrix;"
        "   gl_Position = modelViewProjection * vec4(aPos.x, aPos.y, aPos.z, 1.0);"
        "}";
}
const char* getLightingFragmentShaderSource()
{
    return
        "#version 330 core\n"
        "out vec4 FragColor;"
        ""
        "void main()"
        "{"
        "    FragColor = vec4(1.0f);"
        "}";
}

const char* getSphereVertexShaderSource()
{
    return
        "#version 330 core\n"
        "\n"
        "// Input vertex data, different for all executions of this shader.\n"
        "layout (location = 0) in vec3 vertexPosition_modelspace;\n"
        "layout (location = 1) in vec3 vertexNormal_modelspace;  // You will need this when you do lighting\n"
        "layout (location = 2) in vec3 vertexColor;\n"
        ""
        "// Values that stay constant for the whole mesh.\n"
        "uniform mat4 projectionMatrix = mat4(1.0);\n"
        "uniform mat4 ViewProjectionTransform;\n"
        "uniform mat4 WorldTransform;\n"
        "\n"
        "// Outputs to fragment shader"
        "out vec3 normal;  // You will need this when you do per-fragment lighting\n"
        "out vec4 v_color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "	// Output position of the vertex, in clip space : MVP * position\n"
        "	gl_Position =  projectionMatrix * ViewProjectionTransform * WorldTransform * vec4(vertexPosition_modelspace,1);\n"
        "\n"
        "	normal = vertexNormal_modelspace; // Does this need to be transformed when we pass it to the fragment shader?\n"
        "	v_color = vec4(vertexColor, 1.0f);\n"
        "}";
}
const char* getSphereFragmentShaderSource()
{
    return
        "#version 330 core\n"
        "\n"
        "// Output to fragment shader\n"
        "out vec3 color;\n"
        "\n"
        "in vec4 v_color;\n"
        "in vec3 normal; // You will need this when you do per-fragment lighting\n"
        "\n"
        "void main()\n"
        "{\n"
        "	// set the fragment color to the interpolated vertex color\n"
        "	color = v_color.rgb;\n"
        "}";
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
int complieAndLinkSphereShaders(const char* vertexShaderSource, const char* fragmentShaderSource)
{
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    cout << "Compiling sphere vertex shader" << endl;
    glShaderSource(VertexShaderID, 1, &vertexShaderSource, nullptr);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    cout << "Compiling sphere fragment shader" << endl;
    glShaderSource(FragmentShaderID, 1, &fragmentShaderSource, nullptr);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

GLuint LoadShaders(std::string vertex_shader_path, std::string fragment_shader_path)
{
    // Create the shaders
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Read the Vertex Shader code from the file
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_shader_path, std::ios::in);
    if (VertexShaderStream.is_open()) {
        std::string Line = "";
        while (getline(VertexShaderStream, Line))
            VertexShaderCode += "\n" + Line;
        VertexShaderStream.close();
    }
    else {
        printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_shader_path.c_str());
        getchar();
        exit(-1);
    }

    // Read the Fragment Shader code from the file
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_shader_path, std::ios::in);
    if (FragmentShaderStream.is_open()) {
        std::string Line = "";
        while (getline(FragmentShaderStream, Line))
            FragmentShaderCode += "\n" + Line;
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Compile Vertex Shader
    printf("Compiling shader : %s\n", vertex_shader_path.c_str());
    char const* VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, nullptr);
    glCompileShader(VertexShaderID);

    // Check Vertex Shader
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, nullptr, &VertexShaderErrorMessage[0]);
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    printf("Compiling shader : %s\n", fragment_shader_path.c_str());
    char const* FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, nullptr);
    glCompileShader(FragmentShaderID);

    // Check Fragment Shader
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, nullptr, &FragmentShaderErrorMessage[0]);
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    printf("Linking program\n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Check the program
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
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
int createCubeVertexArrayObject()
{
    // Cube model
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
    // Sphere data
    vec3 vertexArray[] = {
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

int main(int argc, char*argv[])
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
    
    // Compile and link shaders
    int shaderProgram = compileAndLinkShaders(getVertexShaderSource(), getFragmentShaderSource());
    //int sphereShaderProgram = compileAndLinkShaders(getSphereVertexShaderSource(), getSphereFragmentShaderSource());
    //int sphereShaderProgram = LoadShaders("../Assets/Shaders/SolidColor.vertexshader", "../Assets/Shaders/BlueColor.fragmentshader");
    int lightingShaderProgram = compileAndLinkShaders(getLightingVertexShaderSource(), getLightingFragmentShaderSource());

    // We can set the shader once, since we have only one
    glUseProgram(shaderProgram);
    
    // Set projection matrix for shader
    mat4 projectionMatrix = glm::perspective(camera_fov,                           // field of view in degrees
                                             (float)SCREEN_WIDTH / SCREEN_HEIGHT,  // aspect ratio
                                             0.01f, 100.0f);                       // near and far (near > 0)
    
    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Set initial view matrix
    mat4 viewMatrix = lookAt(cameraPosition,                 // eye
                             cameraPosition + cameraLookAt,  // center
                             cameraUp);                      // up
    
    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

    // Set the same perspective and view matrices in the lighting shader program
    glUseProgram(lightingShaderProgram);

    GLuint lightingProjectionMatrixLocation = glGetUniformLocation(lightingShaderProgram, "projectionMatrix");
    glUniformMatrix4fv(lightingProjectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    GLuint lightingViewMatrixLocation = glGetUniformLocation(lightingShaderProgram, "viewMatrix");
    glUniformMatrix4fv(lightingViewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
    glUseProgram(shaderProgram);


    // Get color uniform locaiton
    GLuint colorLocation = glGetUniformLocation(shaderProgram, "color");
     
    // Get world uniform locaiton
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");

    // Define and upload geometry to the GPU here
    int lineVAO = createLineSegmentVertexArrayObject();
    int cubeVAO = createCubeVertexArrayObject();
    int sphereVAO = createSphereVertexArrayObject();
    int lampVAO = createCubeVertexArrayObject();
    
    // For frame time
    float lastFrameTime = glfwGetTime();
    int lastMouseLeftState = GLFW_RELEASE;
    glfwGetCursorPos(window, &lastMousePosX, &lastMousePosY);
    
    // Enable Backface culling
    glEnable(GL_CULL_FACE);

    // Enable Depth Test
    glEnable(GL_DEPTH_TEST);
    
    SphereModel* sphere = new SphereModel(vec3(10.0f, 10.0f, 10.0f));

    // Entering Main Loop
    while(!glfwWindowShouldClose(window))
    {
        // Frame time calculation
        dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;

        // Clear Color Buffer Bit and Depth Buffer Bit 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         
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

        // Draw Olaf
        drawOlaf(worldMatrixLocation, colorLocation, lastFrameTime);

        // Update view and projection matrices
        updateViewAndProjection(shaderProgram);

        // Use lighting shader program
        glUseProgram(lightingShaderProgram);
        updateViewAndProjection(lightingShaderProgram);

        mat4 modelMatrix = mat4(1.0f);
        modelMatrix = translate(modelMatrix, lightPos);
        modelMatrix = scale(modelMatrix, vec3(1.5f));

        GLuint lightingWorldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");
        glUniformMatrix4fv(lightingWorldMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

        glBindVertexArray(lampVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


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
        renderMode = GL_POINTS;
        glDisable(GL_CULL_FACE);
    }

    // Set render mode to lines
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        renderMode = GL_TRIANGLE_STRIP;
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDisable(GL_CULL_FACE);
    }

    // Set render mode to triangles
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        renderMode = GL_TRIANGLE_STRIP;
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glEnable(GL_CULL_FACE);
    }

}

void processUserInput(GLFWwindow* window)
{
    // Close window on esecape
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // Used to translate Olaf
    bool isHoldingShift = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;

    // Move olaf forward
    if (isHoldingShift && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        olafPosition += olafLookAt * olafSpeed * dt;
    }

    // Move olaf backward
    if (isHoldingShift && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        olafPosition -= olafLookAt * olafSpeed * dt;
    }

    // Move olaf left
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        if (isHoldingShift)
        {
            olafPosition -= olafSideVector * olafSpeed * dt;
        }
        else
        {
            olafAngle += olafRotationAmount;
            olafLookAt = vec3(sin(radians(olafAngle)), 0.0f, cos(radians(olafAngle)));
            olafSideVector = cross(olafLookAt, vec3(0.0f, 1.0f, 0.0f));
        }
    }

    // Move olaf right
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        if (isHoldingShift)
        {
            olafPosition += olafSideVector * olafSpeed * dt;
        }
        else
        {
            olafAngle -= olafRotationAmount;
            olafLookAt = vec3(sin(radians(olafAngle)), 0.0f, cos(radians(olafAngle)));
            olafSideVector = cross(olafLookAt, vec3(0.0f, 1.0f, 0.0f));
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
            glDrawArrays(GL_LINES, 0, 2);
        }
        worldMatrix = translate(worldMatrix, vec3(1, 0, sizeOfGrid + 1));
    }
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

void drawOlaf(GLuint worldMatrixLocation, GLuint colorLocation, float lastFrameTime)
{
    // Set point size and line width for rendering Olaf as points or as wireframe
    glPointSize(2.5);
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

    // Olaf right foot
    glUniform3fv(colorLocation, 1, &white[0]);

    mat4 olafRightFootWorldMatrix = olafBaseWorldMatrix;
    olafRightFootWorldMatrix = translate(olafRightFootWorldMatrix, vec3(-0.3f, -1.0f, 0.0f));
    //olafRightFootWorldMatrix = scale(olafRightFootWorldMatrix, vec3(0.8f, 0.75f, 0.8f));
    olafRightFootWorldMatrix = scale(olafRightFootWorldMatrix, vec3(0.2f, 0.2f, 0.2f));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafRightFootWorldMatrix[0][0]);
    drawObject();

    outlineShape(colorLocation);

    // Olaf left foot
    glUniform3fv(colorLocation, 1, &white[0]);

    mat4 olafLeftFootWorldMatrix = olafBaseWorldMatrix;
    olafLeftFootWorldMatrix = translate(olafLeftFootWorldMatrix, vec3(0.3f, -1.0f, 0.0f));
    //olafLeftFootWorldMatrix = scale(olafLeftFootWorldMatrix, vec3(0.8f, 0.75f, 0.8f));
    olafLeftFootWorldMatrix = scale(olafLeftFootWorldMatrix, vec3(0.2f, 0.2f, 0.2f));

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafLeftFootWorldMatrix[0][0]);
    drawObject();

    outlineShape(colorLocation);

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

    mat4 olafNoseWorldMatrix = olafTorsoWorldMatrix;
    olafNoseWorldMatrix = translate(olafNoseWorldMatrix, vec3(0.0f, 1.0f, 0.6f));
    olafNoseWorldMatrix = scale(olafNoseWorldMatrix, vec3(0.25f, 0.25f, 0.5f));                         // 4. Add nose scale
    olafNoseWorldMatrix = scale(olafNoseWorldMatrix, vec3(1.0f / 3.5f, 1.0f / 3.0f, 1.0f));             // 3. revert base scale
    olafNoseWorldMatrix = scale(olafNoseWorldMatrix, vec3(1.0f / 0.75f, 1.0f / 0.5f, 1.0f / 0.75f));    // 2. revert torso scale
    olafNoseWorldMatrix = scale(olafNoseWorldMatrix, vec3(1.0f / 0.8f, 1.0f, 1.0f / 0.8f));             // 1. revert head scale

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &olafNoseWorldMatrix[0][0]);
    drawObject();

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

    float armPivotLength = 1.25f;
    float armAngle = (int)(40 * sin(lastFrameTime)) % 75;
    float armDistanceFromTorso = 0.85f;
    float gloveDistance = 0.75f;

    // Olaf right arm
    glUniform3fv(colorLocation, 1, &white[0]);

    mat4 olafRightArmWorldMatrix = olafTorsoWorldMatrix;
    olafRightArmWorldMatrix = translate(olafRightArmWorldMatrix, vec3(-armDistanceFromTorso, 0.2f, 0.0f));
    olafRightArmWorldMatrix = translate(olafRightArmWorldMatrix, armPivotLength * vec3(-cos(radians(-armAngle)), -sin(radians(-armAngle)), 0.0f));
    olafRightArmWorldMatrix = rotate(olafRightArmWorldMatrix, radians(-armAngle), vec3(0.0f, 0.0f, 1.0f));
    olafRightArmWorldMatrix = scale(olafRightArmWorldMatrix, vec3(1.5f, 0.25f, 0.3f));

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
    olafLeftArmWorldMatrix = translate(olafLeftArmWorldMatrix, vec3(armDistanceFromTorso, 0.2f, 0.0f));
    olafLeftArmWorldMatrix = translate(olafLeftArmWorldMatrix, armPivotLength * vec3(cos(radians(armAngle)), sin(radians(armAngle)), 0.0f));
    olafLeftArmWorldMatrix = rotate(olafLeftArmWorldMatrix, radians(armAngle), vec3(0.0f, 0.0f, 1.0f));
    olafLeftArmWorldMatrix = scale(olafLeftArmWorldMatrix, vec3(1.5f, 0.25f, 0.3f));

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
