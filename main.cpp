#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "basic_camera.h"
#include "camera.h"
#include "pointLight.h"
#include "stb_image.h"
#include "cube.h"
#include "SpotLight.h"
#include <cmath>

#include <iostream>

using namespace std;


void drawChair(unsigned int VAO, Shader& shader, float tx, float ty, float tz, float scale, glm::vec4 backrestColor, glm::vec4 seatColor, glm::vec3 legColor);
void drawSofa(glm::vec3 position, float rotationY, float uniformScale, Shader& shader, Cube& sofa);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawTableChair(unsigned int VAO, Shader ourShader, glm::mat4 sm);
void drawFan(unsigned int VAO, Shader ourShader, glm::mat4 translateMatrix, glm::mat4 sm);
void lightEffect(unsigned int VAO, Shader lightShader, glm::mat4 model, glm::vec3 color);
void drawCube(unsigned int VAO, Shader shader, glm::mat4 model, glm::vec4 color);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L);
void read_file(string file_name, vector<float>& vec);
unsigned int hollowBezier(GLfloat ctrlpoints[], int L, vector<float>& coordinates, vector<float>& normals, vector<int>& indices, vector<float>& vertices, float div);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);
long long nCr(int n, int r);
void load_texture(unsigned int& texture, string image_name, GLenum format);
void textureEffect(unsigned int VAO, Shader shader, glm::mat4 model, unsigned int indicesSize, unsigned int texture0);
void createFence(float baseX, float baseY, float baseZ, int stickCount, float spacing,
    GLuint graveVAO, GLuint cubeVAO, Shader& lightingShader,
    glm::vec3 stickColor, glm::vec3 horizontalStickColor, float sp,
    float rotationAngle, glm::vec3 rotationAxis);
void createFencebuilding(float baseX, float baseY, float baseZ, int stickCount, float spacing,
    GLuint graveVAO, GLuint cubeVAO, Shader& lightingShader,
    glm::vec3 stickColor, glm::vec3 horizontalStickColor, float sp,
    float rotationAngle, glm::vec3 rotationAxis);
void lightEffectB(unsigned int VAO, Shader lightShader, glm::mat4 model, glm::vec3 color, unsigned int indicesb);
void createSteelGate(float baseX, float baseY, float baseZ,
    float gateWidth, float gateHeight,
    int verticalBarCount, int horizontalBarCount,
    GLuint rotorVAO, Shader& lightingShader, glm::vec3 barColor, float gateRotationAngle);
void drawHalfTorus(Shader& lightingShader, float tx, float ty, float tz, float majorRadius, float minorRadius,
    unsigned int segmentsU, unsigned int segmentsV, glm::vec3 color,
    float rotateX, float rotateY, float rotateZ);
void drawStairs(
    unsigned int cubeVAO,
    Shader& shader,
    glm::vec3 startPosition,
    glm::vec3 stepSize,
    int numSteps,
    glm::vec3 color
);
void placeCylindersInHexagon(float centerX, float centerY, float centerZ, float radius,
    Shader& lightingShaderWithTexture, GLuint rotorVAO,
    int indicesCount, GLuint texture);
void drawCylinder(Shader& shader, float tx, float ty, float tz, float radius, float height, unsigned int segments, glm::vec3 color);
void createTwoPartWindow(
    Shader& shader,
    Cube& rusticWindow,
    float baseX,
    float baseY,
    float baseZ,
    float angle,
    float scaleX ,
    float scaleY ,
    float scaleZ 
);
void createTwoPartDoor(
    Shader& shader,
    Cube& rusticWindow,
    float baseX,
    float baseY,
    float baseZ,
    float angle,
    float scaleX,
    float scaleY,
    float scaleZ
);

void drawStairsWithFences(
    unsigned int cubeVAO,
    Shader& shader,
    glm::vec3 startPosition,
    glm::vec3 stepSize,
    int numSteps,
    glm::vec3 color,
    glm::vec3 fenceSize, // New parameter for the fence size
    glm::vec3 fenceOffset // New parameter for the horizontal offset from the stairs (left/right)
);








// draw object functions
//void drawCube(Shader shaderProgram, unsigned int VAO, glm::mat4 parentTrans, float posX = 0.0, float posY = 0.0, float posz = 0.0, float rotX = 0.0, float rotY = 0.0, float rotZ = 0.0, float scX = 1.0, float scY = 1.0, float scZ = 1.0);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 1.0;
float scale_Y = 1.0;
float scale_Z = 1.0;
float windowangle = 0.0f;
bool windoopen = false;

//// camera
//float lastX = SCR_WIDTH / 2.0f;
//float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// camera
Camera camera(glm::vec3(5.0f, 5.0f, 20.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

float eyeX = 1.35, eyeY = 4.8, eyeZ = 10.0;
float lookAtX = 4.0, lookAtY = 4.0, lookAtZ = 6.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;

bool Fon = false;

//birds eye
bool birdEye = false;
glm::vec3 cameraPos(-2.0f, 5.0f, 13.0f);
glm::vec3 target(-2.0f, 0.0f, 5.5f);
float birdEyeSpeed = 1.0f;


glm::vec3 pointLightPositions[] = {
    glm::vec3(16.75, 1.8f,  -2.8f),  // Left side
    glm::vec3(9.5, 5.0, 4.5),  // Right side
    glm::vec3(1.25f,   1.9f,  1.7f),  // Front side

    glm::vec3(0.0f, 40.0f,  -5.0f)  // Left side


};


// Point Light 1
PointLight pointlight1(
    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient - very dim, eerie glow
    0.2f, 0.2f, 0.2f,       // diffuse - still dim but enough to cast faint light
    0.1f, 0.1f, 0.1f,       // specular - slightly higher for subtle reflection
    1.0f,   //k_c
    0.022f, //k_l
    0.0019f,//k_q
    1       // light number
);

// Point Light 2
PointLight pointlight2(
    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient - dim, subtle glow
    0.2f, 0.2f, 0.2f,       // diffuse - slightly stronger but still subtle
    0.1f, 0.1f, 0.1f,       // specular - for the eerie reflections
    1.0f,   //k_c
    0.022f, //k_l
    0.0019f,//k_q
    2       // light number
);

// Point Light 3
PointLight pointlight3(
    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient - dark, subtle effect
    0.2f, 0.2f, 0.2f,       // diffuse - provides faint visibility
    0.1f, 0.1f, 0.1f,       // specular - muted reflections
    1.0f,   //k_c
    0.022f, //k_l
    0.0019f,//k_q
    3       // light number
);

float amx = 1.0f, amy = 1.0f, amz = 1.0f;
float dmx = 1.0f, dmy =1.0f, dmz = 1.0f;
float smx = 1.0f, smy = 1.0f, smz=1.0f;

PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    amx, amy, amz,     // ambient
    dmx, dmy, dmz,     // diffuse
    smx, smy, smz,        // specular
    1.0f,   //k_c
    0.022f,  //k_l
    0.0019f, //k_q
    4      // light number
);

SpotLight spotlight1(
    9.5f, 2.8f, 7.5f,            // position
    0.3f, 0.0f, 0.0f,            // ambient (slightly brighter red)
    1.0f, 0.0f, 0.0f,            // diffuse (bright red)
    1.0f, 0.0f, 0.0f,            // specular (very shiny red)
    0.5f,                        // k_c (constant)
    0.09f,                       // k_l (linear)
    0.032f,                      // k_q (quadratic)
    3,                            // light number
    glm::cos(glm::radians(5.0f)),  // inner_circle
    glm::cos(glm::radians(10.0f)), // outer_circle
    0, -1, 0                      // direction
);



// light settings
bool onOffToggle = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;
bool dl = true;
bool spt = true;
bool point1 = true;
bool point2 = true;
bool point3 = true;
bool point4 = true;
bool maingate = false;
float gateRotationAngle = 0.0f;

glm::mat4 transformation(float tx, float ty, float tz, float rx, float ry, float rz, float sx, float sy, float sz) {
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(tx, ty, tz));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rx), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(ry), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rz), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(sx, sy, sz));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    return model;
}

//float d_amb_on = 1.0f;
//float d_def_on = 1.0f;
//float d_spec_on = 1.0f;

glm::vec3 amb(0.2f, 0.2f, 0.2f);
glm::vec3 def(0.8f, 0.8f, 0.8f);
glm::vec3 spec(1.0f, 1.0f, 1.0f);

float fov = glm::radians(camera.Zoom);
float aspect = (float)SCR_WIDTH / (float)SCR_HEIGHT;
float near = 0.1f;
float far = 100.0f;

float tanHalfFOV = tan(fov / 2.0f);

const double pi = 3.14159265389;
const int nt = 40;
const int ntheta = 30;
unsigned int ropeVAO, rotorVAO, hillVAO, pillarVAO, gatepillerVAO,wallpillerVAO,lampVAO;

vector <float> cntrlPointslamp, cntrlPoints, cntrlPointsRope, cntrlPointsRotor, cntrlPointsPilar,walcntrlPointsPilar;
vector <float> coordinates, coordinateslamp, coordinatesRope, coordinatesRotor, coordinatesPillar, wallcoordinatesPillar;
vector <float> normals, normalslamp, normalsRope, normalsRotor, normalsPillar,wallnormalsPillar;
vector <int> indices, indiceslamp, indicesRope, indicesRotor, indicesPillar,wallindicesPillar;
vector <float> vertices, verticeslamp, verticesRope, verticesRotor, verticesPillar,wallverticesPillar;
//vector <float> textureCoords, textureCoordsRope, textureCoordsRope2;

// texture
float extra = 4.0f;
float TXmin = 0.0f;
float TXmax = 1.0f;
float TYmin = 0.0f;
float TYmax = 1.0f;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "A Haunted Palace", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    //Shader constantShader("vertexShader.vs", "fragmentShaderV2.fs");

    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    Shader textureShader("texture_vertex.vs", "texture_fragment.fs");
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------


    float cube_vertices[] = {
        // positions      // normals         // texture coords
            0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, TXmax, TYmin,
            1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, TXmin, TYmin,
            1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, TXmin, TYmax,
            0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, TXmax, TYmax,

            1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, TXmax, TYmin,
            1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, TXmax, TYmax,
            1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, TXmin, TYmin,
            1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, TXmin, TYmax,

            0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, TXmin, TYmin,
            1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, TXmax, TYmin,
            1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, TXmax, TYmax,
            0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, TXmin, TYmax,

            0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, TXmax, TYmin,
            0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, TXmax, TYmax,
            0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, TXmin, TYmax,
            0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, TXmin, TYmin,

            1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, TXmax, TYmin,
            1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, TXmax, TYmax,
            0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, TXmin, TYmax,
            0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, TXmin, TYmin,

            0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, TXmin, TYmin,
            1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, TXmax, TYmin,
            1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, TXmax, TYmax,
            0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, TXmin, TYmax
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };
    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)24);
    glEnableVertexAttribArray(2);

    //light's VAO
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

#define NUM_SLICES 20
#define PI 3.14159265358979f
#define ANGLE_STEP (PI / NUM_SLICES)  // Half circle, so 180 degrees

    float grave_vertices[] = {
        // Cube vertices
        0.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // Bottom-left
        1.0f, 0.0f, 0.0f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-right
        1.0f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f, 0.0f, 0.75f, // Top-right
        0.0f, 1.0f, 0.0f,  0.0f, 0.0f, -1.0f, 1.0f, 0.75f, // Top-left

        1.0f, 0.0f, 0.0f,  1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,  1.0f, 0.0f,  0.0f, 1.0f, 0.75f,
        1.0f, 0.0f, 1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.75f,

        0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f,  0.0f, 0.0f,  1.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,  0.0f, 0.0f,  1.0f, 1.0f, 0.75f,
        0.0f, 1.0f, 1.0f,  0.0f, 0.0f,  1.0f, 0.0f, 0.75f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.75f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f,  0.0f, 0.0f, 0.75f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f,  0.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,  0.0f, 1.0f,  0.0f, 1.0f, 0.75f,
        0.0f, 1.0f, 0.0f,  0.0f, 1.0f,  0.0f, 0.0f, 0.75f,
        0.0f, 1.0f, 1.0f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.75f,
        0.0f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.75f,


        0.5f, 1.5f, 0.0f,  0.0f, 1.0f,  0.0f, 0.5f, 1.0f,
        0.5f, 1.5f, 1.0f,  0.0f, 1.0f,  0.0f, 0.5f, 1.0f,
    };

    // Indices for the model
    unsigned int grave_indices[] = {
        0, 3, 2,  
        2, 1, 0,
        
        4, 5, 7,  
        7, 6, 4,
        
        8, 9, 10, 
        10, 11, 8,
        
        12, 13, 14,
        14, 15, 12,
        
        20, 21, 22,
        22, 23, 20,
        
        18,24,17,
        19,25,16,

        16,17,24,
        24,25,16,

        19,25,18,
		18,25,24

    };


    // OpenGL buffer setup remains the same
    unsigned int graveVAO, graveVBO, graveEBO;
    glGenVertexArrays(1, &graveVAO);
    glGenBuffers(1, &graveVBO);
    glGenBuffers(1, &graveEBO);

    glBindVertexArray(graveVAO);
    glBindBuffer(GL_ARRAY_BUFFER, graveVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(grave_vertices), grave_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, graveEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(grave_indices), grave_indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //ourShader.use();
    //constantShader.use();

	read_file("lamp.txt", cntrlPointslamp);
	lampVAO = hollowBezier(cntrlPointslamp.data(), ((unsigned int)cntrlPointslamp.size() / 3) - 1, coordinateslamp, normalslamp, indiceslamp, verticeslamp, 1.0);

    read_file("sky.txt", cntrlPoints);
    hillVAO = hollowBezier(cntrlPoints.data(), ((unsigned int)cntrlPoints.size() / 3) - 1, coordinates, normals, indices, vertices, 1.0);
	read_file("gate.txt", cntrlPointsPilar);
	gatepillerVAO = hollowBezier(cntrlPointsPilar.data(), ((unsigned int)cntrlPointsPilar.size() / 3) - 1, coordinatesPillar, normalsPillar, indicesPillar, verticesPillar, 1.0);
	read_file("wallpiller.txt", walcntrlPointsPilar);
	wallpillerVAO = hollowBezier(walcntrlPointsPilar.data(), ((unsigned int)walcntrlPointsPilar.size() / 3) - 1, wallcoordinatesPillar, wallnormalsPillar, wallindicesPillar, wallverticesPillar, 1.0);
	read_file("cylinder.txt", cntrlPointsRotor);
	rotorVAO = hollowBezier(cntrlPointsRotor.data(), ((unsigned int)cntrlPointsRotor.size() / 3) - 1, coordinatesRotor, normalsRotor, indicesRotor, verticesRotor, 1.0);

	read_file("halftours.txt", cntrlPointsRope);
	ropeVAO = hollowBezier(cntrlPointsRope.data(), ((unsigned int)cntrlPointsRope.size() / 3) - 1, coordinatesRope, normalsRope, indicesRope, verticesRope, 1.0);

    // Texture loading

    //load_texture(texture1, "grass.jpg", GL_RGBA);

    unsigned int texture, texturegrass, texturekobor, texturemud,grassdead, bricksl, bricks, bricksd,pilertex;

    string ImgPath = "deadgrass.png";
    texturegrass = loadTexture(ImgPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    ImgPath = "muddirt.jpg";
    grassdead = loadTexture(ImgPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    ImgPath = "piller.jpg";
    pilertex = loadTexture(ImgPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);


    ImgPath = "SKY.jpg";
    texture = loadTexture(ImgPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    ImgPath = "kobor.jpg";
    texturekobor = loadTexture(ImgPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    ImgPath = "mudgrave.jpg";
    texturemud = loadTexture(ImgPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	bricks = loadTexture("bricks.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	bricksl = loadTexture("brickl.jpg", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
	bricksd = loadTexture("bricksd.png", GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);

    //grass texture
    string diffuseMapPath3 = "deadgrass.png";
    string specularMapPath3 = "deadgrass.png";
    unsigned int diffMap3 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap3 = loadTexture(specularMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    float textureScaleX = 4.0f;
    float textureScaleY = 4.0f;// Repeat texture 4 times
    Cube cube3 = Cube(diffMap3, specMap3, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f, textureScaleX, textureScaleY);

    //grass texture
    string diffuseMapPathwall = "brickl.jpg";
    string specularMapPathwall = "brickl.jpg";
    unsigned int diffMapwall = loadTexture(diffuseMapPathwall.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapwall = loadTexture(specularMapPathwall.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    textureScaleX = 10.0f; // Repeat texture 4 times
    textureScaleY = 5.0f;
    Cube wall = Cube(diffMapwall, specMapwall, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f, textureScaleX, textureScaleY);

    textureScaleX = 2.0f; // Repeat texture 4 times
    textureScaleY = 5.0f;
    Cube wall3 = Cube(diffMapwall, specMapwall, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f, textureScaleX, textureScaleY);

    //window texture
    string diffuseMapPathw = "window.jpg";
    string specularMapPathw = "window.jpg";
    unsigned int diffMapw = loadTexture(diffuseMapPathw.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapw = loadTexture(specularMapPathw.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    textureScaleX = 1.0f;
    textureScaleY = 1.0f;// Repeat texture 4 times
    Cube windowside = Cube(diffMapw, specMapw, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f, textureScaleX, textureScaleY);
    //window texture
    string diffuseMapPathwf = "bricks.jpg";
    string specularMapPathwf = "bricks.jpg";
    unsigned int diffMapwf = loadTexture(diffuseMapPathwf.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapwf = loadTexture(specularMapPathwf.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    textureScaleX = 2.0f;
    textureScaleY = 1.0f;// Repeat texture 4 times
    Cube wallfront = Cube(diffMapwf, specMapwf, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f, textureScaleX, textureScaleY);
    Cube wallfrontwindow = Cube(diffMapwf, specMapwf, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f, 2, 0.5);

    string diffuseMapPathd = "door.jpg";
    string specularMapPathd = "door.jpg";
    unsigned int diffMapd = loadTexture(diffuseMapPathd.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapd = loadTexture(specularMapPathd.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    textureScaleX = 1.0f;
    textureScaleY = 1.0f;// Repeat texture 4 times
    Cube door = Cube(diffMapd, specMapd, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f, textureScaleX, textureScaleY);

    string diffuseMapPathrw = "rusticwindow.png";
    string specularMapPatrw = "rusticwindow.png";
    unsigned int diffMaprw = loadTexture(diffuseMapPathrw.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMaprw = loadTexture(specularMapPatrw.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    textureScaleX = 1.0f;
    textureScaleY = 1.0f;
    Cube rusticwindow = Cube(diffMaprw, specMaprw, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f, textureScaleX, textureScaleY);

    string diffuseMapPathmd = "maindoor.jpg";
    string specularMapPatmd = "maindoor.jpg";
    unsigned int diffMapmd = loadTexture(diffuseMapPathmd.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapmd = loadTexture(specularMapPatmd.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    textureScaleX = 1.0f;
    textureScaleY = 1.0f;
    Cube rusticdoor = Cube(diffMapmd, specMapmd, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f, textureScaleX, textureScaleY);

    string diffuseMapPaths = "sofa.jpg";
    string specularMapPats = "sofa.jpg";
    unsigned int diffMaps = loadTexture(diffuseMapPaths.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMaps = loadTexture(specularMapPats.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    textureScaleX = 2.0f;
    textureScaleY = 1.0f;
    Cube sofa = Cube(diffMaps, specMaps, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f, textureScaleX, textureScaleY);
    
    string diffuseMapPathc = "carpetd.jpg";
    string specularMapPatc = "carpetd.jpg";
    unsigned int diffMapc = loadTexture(diffuseMapPathc.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapc = loadTexture(specularMapPatc.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    textureScaleX = 1.0f;
    textureScaleY = 1.0f;
    Cube carpet = Cube(diffMapc, specMapc, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f, textureScaleX, textureScaleY);

    string diffuseMapPathb = "bedsheet.jpg";
    string specularMapPatb = "bedsheet.jpg";
    unsigned int diffMapb = loadTexture(diffuseMapPathb.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapb = loadTexture(specularMapPatb.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    textureScaleX = 1.0f;
    textureScaleY = 2.0f;
    Cube bedsheet = Cube(diffMapb, specMapb, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f, textureScaleX, textureScaleY);

    string diffuseMapPathwo = "wood.jpg";
    string specularMapPatwo = "wood.jpg";
    unsigned int diffMapwo = loadTexture(diffuseMapPathwo.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMapwo = loadTexture(specularMapPatwo.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    textureScaleX = 1.0f;
    textureScaleY = 1.0f;
    Cube bedsheetw = Cube(diffMapwo, specMapwo, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f, textureScaleX, textureScaleY);


    float timeOfDay = 12.0f;
    float r = 0.0f;
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // Slow down the timeOfDay increment to simulate a slower day-night cycle
        timeOfDay += 0.01f;  // Slower progression, change the value to control the cycle speed

        if (timeOfDay > 24.0f) {  // Reset after 24 hours
            timeOfDay = 0.0f;
        }

        // Normalize the timeOfDay value to be between 0 and 1
        float timeFactor = glm::fract(timeOfDay / 24.0f); // Normalize to [0, 1]

        // Sine function to simulate sunrise and sunset (smooth transition)
        float intensity = 0.5f * (1.0f + glm::sin(timeFactor * glm::two_pi<float>())); // Oscillates between 0 and 1

        // Set the light's color and intensity for the day-night cycle
        glm::vec3 ambientColor = glm::mix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(amx, amy, amz), intensity); // Morning to evening
        glm::vec3 diffuseColor = glm::mix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(dmx, dmy, dmz), intensity); // Morning to evening
        glm::vec3 specularColor = glm::mix(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(smx, smy, smz), intensity); // Morning to evening

        // Adjust the sun's position over the day (scale the movement for a longer day)
        float sunPositionY = glm::sin(timeFactor * glm::two_pi<float>());  // Smooth transition for sun's path

        // Update the point light (pointLight4) properties based on timeOfDay
        pointlight4.position.y = sunPositionY * 10.0f;  // Adjust Y position based on sun movement

        // Apply the light color changes to the point light
        pointlight4.ambient = ambientColor;  // Set ambient color based on intensity
        pointlight4.diffuse = diffuseColor;  // Set diffuse color based on intensity
        pointlight4.specular = specularColor;  // Set specular color based on intensity

        // Set the updated point light in the shader
        pointlight4.setUpPointLight(lightingShader);



        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


        // camera/view transformation

        glm::mat4 view;

        if (birdEye) {
            glm::vec3 up(0.0f, 1.0f, 0.0f);
            view = glm::lookAt(cameraPos, target, up);
        }
        else {
            //view = basic_camera.createViewMatrix();
            view = camera.GetViewMatrix();

        }

        glm::mat4 identityMatrix = glm::mat4(1.0f);
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model, modelCentered,
            translateMatrixprev;








        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);
        lightingShaderWithTexture.setMat4("projection", projection);
        lightingShaderWithTexture.setMat4("view", view);

        // point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        pointlight4.setUpPointLight(lightingShaderWithTexture);

        spotlight1.setUpspotLight(lightingShaderWithTexture);

        lightingShaderWithTexture.setVec3("directionalLight.directiaon", 0.0f, -3.0f, 0.0f);
        lightingShaderWithTexture.setVec3("directionalLight.ambient", .5f, .5f, .5f);
        lightingShaderWithTexture.setVec3("directionalLight.diffuse", .8f, .8f, .8f);
        lightingShaderWithTexture.setVec3("directionalLight.specular", 1.0f, 1.0f, 1.0f);
        lightingShaderWithTexture.setBool("directionLightOn", true);

        model = transformation(10.0, -1.0, 0.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 50.0, 50.0, 50.0);
        textureEffect(hillVAO, lightingShaderWithTexture, model, indices.size(), texture);

        model = transformation(-10.0, 0.0, -5.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 35.0, 0.2, 23.0);
		//textureEffect(cubeVAO, lightingShaderWithTexture, model, sizeof(cube_indices) / sizeof(unsigned int), texturegrass);
        cube3.drawCubeWithTexture(lightingShaderWithTexture, model);
        model = transformation(-8, 0.3, -3, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, -1.0f, 0.8f, -0.1f);
        textureEffect(graveVAO, lightingShaderWithTexture, model, sizeof(grave_indices) / sizeof(unsigned int), texturekobor);
        //graveyards
        model = transformation(-8, 0.5, -3, rotateAngle_X, rotateAngle_Y, rotateAngle_Z + 90, -0.2f, 1.0f, 2.0f);
		textureEffect(cubeVAO, lightingShaderWithTexture, model, sizeof(cube_indices) / sizeof(unsigned int), texturemud);
       
        // Grave
        model = transformation(-8, 0.3, 1, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, -1.0f, 0.8f, -0.1f);
        textureEffect(graveVAO, lightingShaderWithTexture, model, sizeof(grave_indices) / sizeof(unsigned int), texturekobor);

        // Graveyard
        model = transformation(-8, 0.5, 1, rotateAngle_X, rotateAngle_Y, rotateAngle_Z + 90, -0.2f, 1.0f, 2.0f);
        textureEffect(cubeVAO, lightingShaderWithTexture, model, sizeof(cube_indices) / sizeof(unsigned int), texturemud);

        model = transformation(-5, 0.3, -3, rotateAngle_X, rotateAngle_Y , rotateAngle_Z, -1.0f, 0.8f, -0.1f);
        textureEffect(graveVAO, lightingShaderWithTexture, model, sizeof(grave_indices) / sizeof(unsigned int), texturekobor);
        //graveyards
        model = transformation(-5, 0.5, -3, rotateAngle_X, rotateAngle_Y, rotateAngle_Z + 90, -0.2f, 1.0f, 2.0f);
        textureEffect(cubeVAO, lightingShaderWithTexture, model, sizeof(cube_indices) / sizeof(unsigned int), texturemud);

        // Grave
        model = transformation(-5, 0.3, 1, rotateAngle_X, rotateAngle_Y , rotateAngle_Z, -1.0f, 0.8f, -0.1f);
        textureEffect(graveVAO, lightingShaderWithTexture, model, sizeof(grave_indices) / sizeof(unsigned int), texturekobor);

        // Graveyard
        model = transformation(-5, 0.5, 1, rotateAngle_X, rotateAngle_Y, rotateAngle_Z + 90, -0.2f, 1.0f, 2.0f);
        textureEffect(cubeVAO, lightingShaderWithTexture, model, sizeof(cube_indices) / sizeof(unsigned int), texturemud);

        model = transformation(-4, 0.22, 4, rotateAngle_X, rotateAngle_Y, rotateAngle_Z + 90, -0.1f, 6.0f, -9.0f);
        textureEffect(cubeVAO, lightingShaderWithTexture, model, sizeof(cube_indices) / sizeof(unsigned int), grassdead);
        // slider_wall1

         // Hexagon parameters
        float hexagonRadius = 2.0f;
        float hexagonCenterX =-0.8f;
        float hexagonCenterY = 2.3f;
        float hexagonCenterZ = 11.0f;

        // Place cylinders in a hexagon
        placeCylindersInHexagon(hexagonCenterX, hexagonCenterY, hexagonCenterZ, hexagonRadius,
            lightingShaderWithTexture, rotorVAO, indicesRotor.size(), bricks);

        hexagonRadius = 2.0f;
        hexagonCenterX = 18.8f;
        hexagonCenterY = 2.3f;
        hexagonCenterZ = 11.0f;

        // Place cylinders in a hexagon
        placeCylindersInHexagon(hexagonCenterX, hexagonCenterY, hexagonCenterZ, hexagonRadius,
            lightingShaderWithTexture, rotorVAO, indicesRotor.size(), bricks);
        //side walls

        model = transformation(-0.75, 0.6, -3.75, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 19.5f, 4.4f, 0.1f);
        wall.drawCubeWithTexture(lightingShaderWithTexture, model);

        model = transformation(-0.68, 0.6, -3.75, rotateAngle_X, rotateAngle_Y-90, rotateAngle_Z, 12.6f, 4.4f, 0.1f);
        wall.drawCubeWithTexture(lightingShaderWithTexture, model);
        model = transformation(18.8f, 0.6, -3.75, rotateAngle_X, rotateAngle_Y -90, rotateAngle_Z, 12.6f, 4.4f, 0.1f);
        wall.drawCubeWithTexture(lightingShaderWithTexture, model);
        // Wall 1
        model = transformation(-0.68, 0.6, 9, rotateAngle_X, rotateAngle_Y - 135, rotateAngle_Z, 3.0f, 4.4f, 0.1f);
        wall3.drawCubeWithTexture(lightingShaderWithTexture, model);
        // Centered Window for Wall 1
        model = transformation(
            -1.35,                             // Same x-position as the wall
            0.6 + (3.4f - 1.3f) / 2.0f,        // Center vertically: adjust y based on wall and window height
            9.72,                                 // Same z-position as the wall
            rotateAngle_X,
            rotateAngle_Y - 135,
            rotateAngle_Z,
            1.3f, 1.3f, 0.15f);
        windowside.drawCubeWithTexture(lightingShaderWithTexture, model);

        // Wall 2
        model = transformation(-2.9, 0.6, 10.8, rotateAngle_X, rotateAngle_Y - 45, rotateAngle_Z, 3.0f, 4.4f, 0.1f);
        wall3.drawCubeWithTexture(lightingShaderWithTexture, model);
        // Centered Window for Wall 2
        model = transformation(
            -2.2,                              // Same x-position as the wall
            0.6 + (3.4f - 1.3f) / 2.0f,        // Center vertically
            11.5,                              // Same z-position as the wall
            rotateAngle_X,
            rotateAngle_Y - 45,
            rotateAngle_Z,
            1.3f, 1.3f, 0.15f);
        windowside.drawCubeWithTexture(lightingShaderWithTexture, model);

        // Wall 3
        model = transformation(1.2, 0.6, 11.0, rotateAngle_X, rotateAngle_Y - 135, rotateAngle_Z, 3.0f, 4.4f, 0.1f);
        wall3.drawCubeWithTexture(lightingShaderWithTexture, model);
        // Centered Window for Wall 3
        model = transformation(
            0.7,                               // Same x-position as the wall
            0.6 + (3.4f - 1.3f) / 2.0f,        // Center vertically
            11.55,                              // Same z-position as the wall
            rotateAngle_X,
            rotateAngle_Y - 135,
            rotateAngle_Z,
            1.3f, 1.3f, 0.15f);
        windowside.drawCubeWithTexture(lightingShaderWithTexture, model);

		// Wall 4
        // Wall 1
        model = transformation(-0.56 + 19.2f, 0.6, 9 , rotateAngle_X, rotateAngle_Y - 45, rotateAngle_Z, 2.8f, 4.4f, 0.1f);
        wall3.drawCubeWithTexture(lightingShaderWithTexture, model);
        // Centered Window for Wall 1
        model = transformation(
            -1.35 + 19.7+1.2,                             // Same x-position as the wall
            0.6 + (3.4f - 1.3f) / 2.0f,        // Center vertically: adjust y based on wall and window height
            9.8,                                 // Same z-position as the wall
            rotateAngle_X,
            rotateAngle_Y - 45,
            rotateAngle_Z,
            1.3f, 1.3f, 0.15f);
        windowside.drawCubeWithTexture(lightingShaderWithTexture, model);

        // Wall 2
        model = transformation(-2.9 + 19.5+0.2, 0.6, 10.8 + 0.2, rotateAngle_X, rotateAngle_Y - 45, rotateAngle_Z, 3.0f, 4.4f, 0.1f);
        wall3.drawCubeWithTexture(lightingShaderWithTexture, model);
        // Centered Window for Wall 2
        model = transformation(
            -2.2 + 19.5f+0.2,                              // Same x-position as the wall
            0.6 + (3.4f - 1.3f) / 2.0f,        // Center vertically
            11.5 + 0.2,                              // Same z-position as the wall
            rotateAngle_X,
            rotateAngle_Y - 45,
            rotateAngle_Z,
            1.3f, 1.3f, 0.15f);
        windowside.drawCubeWithTexture(lightingShaderWithTexture, model);

        // Wall 3
        model = transformation(1.2 + 19.5f+0.3, 0.6, 11.0 + 0.1, rotateAngle_X, rotateAngle_Y - 135, rotateAngle_Z, 3.0f, 4.4f, 0.1f);
        wall3.drawCubeWithTexture(lightingShaderWithTexture, model);
        // Centered Window for Wall 3
        model = transformation(
            0.7 + 19.5f+0.3,                               // Same x-position as the wall
            0.6 + (3.4f - 1.3f) / 2.0f,        // Center vertically
            11.55+0.1,                              // Same z-position as the wall
            rotateAngle_X,
            rotateAngle_Y - 135,
            rotateAngle_Z,
            1.3f, 1.3f, 0.15f);
        windowside.drawCubeWithTexture(lightingShaderWithTexture, model);



        //front walls
        //1
        model = transformation(1.0, 1.0, 9.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 3.0f, 2.0f, 0.1f);
        wallfront.drawCubeWithTexture(lightingShaderWithTexture, model);

        model = transformation(5.0, 1.0, 9.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 3.0f, 2.0f, 0.1f);
        wallfront.drawCubeWithTexture(lightingShaderWithTexture, model);

        model = transformation(4.0, 2.5, 9.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 3.0f, 0.5f, 0.1f);
        wallfrontwindow.drawCubeWithTexture(lightingShaderWithTexture, model);

		model = transformation(4.0, 1.0, 9.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 3.0f, 0.5f, 0.1f);
        wallfrontwindow.drawCubeWithTexture(lightingShaderWithTexture, model);
        model = transformation(1.0, 1.0, 9.0, rotateAngle_X, rotateAngle_Y-90, rotateAngle_Z, 2.0f, 2.0f, 0.1f);
        wallfront.drawCubeWithTexture(lightingShaderWithTexture, model);

        //2
        model = transformation(10.0, 1.0, 9.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 3.0f, 2.0f, 0.1f);
        wallfront.drawCubeWithTexture(lightingShaderWithTexture, model);

        model = transformation(14.0, 1.0, 9.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 3.0f, 2.0f, 0.1f);
        wallfront.drawCubeWithTexture(lightingShaderWithTexture, model);

        model = transformation(13.0, 2.5, 9.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 3.0f, 0.5f, 0.1f);
        wallfrontwindow.drawCubeWithTexture(lightingShaderWithTexture, model);

        model = transformation(13.0, 1.0, 9.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 3.0f, 0.5f, 0.1f);
        wallfrontwindow.drawCubeWithTexture(lightingShaderWithTexture, model);

        model = transformation(17.0, 1.0, 9.0, rotateAngle_X, rotateAngle_Y-90, rotateAngle_Z, 2.0f, 2.0f, 0.1f);
        wallfront.drawCubeWithTexture(lightingShaderWithTexture, model);
        //front wall 2nd floor
        //front walls
        //1
        model = transformation(1.0, 3.0, 9.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 3.0f, 2.0f, 0.1f);
        wallfront.drawCubeWithTexture(lightingShaderWithTexture, model);

        model = transformation(5, 3.0, 9.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 3.25f, 2.0f, 0.1f);
        wallfront.drawCubeWithTexture(lightingShaderWithTexture, model);

        model = transformation(4.0, 3, 9.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 3.0f, 0.5f, 0.1f);
        wallfrontwindow.drawCubeWithTexture(lightingShaderWithTexture, model);

        model = transformation(4.0, 4.5, 9.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 3.0f, 0.5f, 0.1f);
        wallfrontwindow.drawCubeWithTexture(lightingShaderWithTexture, model);
        model = transformation(1.0, 3.0, 9.0, rotateAngle_X, rotateAngle_Y - 90, rotateAngle_Z, 2.0f, 2.0f, 0.1f);
        wallfront.drawCubeWithTexture(lightingShaderWithTexture, model);

        //2
        model = transformation(9.0, 3.0, 9.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 4.0f, 2.0f, 0.1f);
        wallfront.drawCubeWithTexture(lightingShaderWithTexture, model);

        model = transformation(14, 3.0, 9.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 4.0f, 2.0f, 0.1f);
        wallfront.drawCubeWithTexture(lightingShaderWithTexture, model);

        model = transformation(13.0, 3.0, 9.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 3.0f, 0.5f, 0.1f);
        wallfrontwindow.drawCubeWithTexture(lightingShaderWithTexture, model);

        model = transformation(13.0, 4.5, 9.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 3.0f, 0.5f, 0.1f);
        wallfrontwindow.drawCubeWithTexture(lightingShaderWithTexture, model);

        model = transformation(17.0, 3.0, 9.0, rotateAngle_X, rotateAngle_Y - 90, rotateAngle_Z, 2.0f, 2.0f, 0.1f);
        wallfront.drawCubeWithTexture(lightingShaderWithTexture, model);
        
        model = transformation(16.0, 4.5, 9.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 3.0f, 0.5f, 0.1f);
        wallfrontwindow.drawCubeWithTexture(lightingShaderWithTexture, model);
        model = transformation(-1.0, 4.5, 9.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 3.0f, 0.5f, 0.1f);
        wallfrontwindow.drawCubeWithTexture(lightingShaderWithTexture, model);
        model = transformation(8.0, 4.5, 9.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 3.0f, 0.5f, 0.1f);
        wallfrontwindow.drawCubeWithTexture(lightingShaderWithTexture, model);

        model = transformation(16.85, 3.0, 9.4, rotateAngle_X, rotateAngle_Y-90, rotateAngle_Z, 0.7f, 1.7f, 0.1f);
        door.drawCubeWithTexture(lightingShaderWithTexture, model);

        model = transformation(1.1, 3.0, 9.4, rotateAngle_X, rotateAngle_Y - 90, rotateAngle_Z, 0.7f, 1.7f, 0.1f);
        door.drawCubeWithTexture(lightingShaderWithTexture, model);

        model = transformation(8.2, 3.0, 9, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.8f, 1.5f, 0.1f);
        door.drawCubeWithTexture(lightingShaderWithTexture, model);

        if (windoopen) {
            // Open the gate by increasing the angle up to 90 degrees
            if (windowangle < 90) {
                windowangle += 0.5;
            }
            else {
                windowangle = 90; // Clamp to 90 degrees
            }
        }
        else {
            // Close the gate by decreasing the angle down to 0 degrees
            if (windowangle > 0) {
                windowangle -= 0.5;
            }
            else {
                windowangle = 0; // Clamp to 0 degrees
            }
        }
        
        // Create a two-part window at position (4.0, 3.5, 9) with a 30-degree opening angle
        createTwoPartWindow(lightingShaderWithTexture, rusticwindow, 4.0f, 3.5f, 9.0f, windowangle,0.5f,1.0f,0.1f);
        createTwoPartWindow(lightingShaderWithTexture, rusticwindow, 4.0f, 1.5f, 9.0f, windowangle, 0.5f, 1.0f, 0.1f);
        createTwoPartWindow(lightingShaderWithTexture, rusticwindow, 13.0f, 3.5f, 9.0f, windowangle, 0.5f, 1.0f, 0.1f);
        createTwoPartWindow(lightingShaderWithTexture, rusticwindow, 13.0f, 1.5f, 9.0f, windowangle, 0.5f, 1.0f, 0.1f);

        createTwoPartDoor(lightingShaderWithTexture, rusticdoor, 8.0f, 1.0f, 9.0f, windowangle, 1.0f, 2.0f, 0.1f);

        //front piller
        model = transformation(2 + 0.5, 2.8, 10.38, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.25f, 0.28f, 0.25f);
        textureEffect(wallpillerVAO, lightingShaderWithTexture, model, wallindicesPillar.size(), pilertex);
        model = transformation(6.33 + 0.5, 2.8, 10.38, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.25f, 0.28f, 0.25f);
        textureEffect(wallpillerVAO, lightingShaderWithTexture, model, wallindicesPillar.size(), pilertex);

        model = transformation(10.66 + 0.5, 2.8, 10.38, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.25f, 0.28f, 0.25f);
        textureEffect(wallpillerVAO, lightingShaderWithTexture, model, wallindicesPillar.size(), pilertex);

        model = transformation(15+0.5, 2.8, 10.38, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.25f, 0.28f, 0.25f);
        textureEffect(wallpillerVAO, lightingShaderWithTexture, model, wallindicesPillar.size(), pilertex);

        
        float uniformScale = 0.5f;

        // Sofa
        drawSofa(glm::vec3(0.5f * uniformScale, 2.0f * uniformScale, 5.0f * uniformScale),
            180.0f, uniformScale, lightingShaderWithTexture, sofa);


        //BED
        // Mattress (the base of the bed)
        glm::mat4 mattressModel = transformation(
            17.56f, 1.3f, -2.9f,       // Position (center of the mattress)
            0.0f, 0.0f, 0.0f,       // Rotation (no rotation)
            2.0f * uniformScale, 0.2f * uniformScale, 3.0f * uniformScale);  // Mattress size (width, height, depth)
        bedsheet.drawCubeWithTexture(lightingShaderWithTexture, mattressModel);
        glm::mat4 tableModel = transformation(
            16.5f, 1.0f, -3.0f,       // Position (move it to the left of the bed)
            0.0f, 0.0f, 0.0f,         // Rotation (no rotation)
            1.0f * uniformScale, 1.0f * uniformScale, 1.0f * uniformScale);  // Size of the table (smaller than the bed)
        bedsheetw.drawCubeWithTexture(lightingShaderWithTexture, tableModel);
        tableModel = transformation(
            16.45f, 1.5f, -3.05f,       // Position (move it to the left of the bed)
            0.0f, 0.0f, 0.0f,         // Rotation (no rotation)
            1.2f * uniformScale, 0.1f * uniformScale, 1.2f * uniformScale);  // Size of the table (smaller than the bed)
        bedsheetw.drawCubeWithTexture(lightingShaderWithTexture, tableModel);
        
        // Headboard (optional)
        glm::mat4 headboardModel = transformation(
            17.56f, 1.4f, -3.0f,       // Position (place at the head of the bed, along the Z-axis)
            0.0f, 0.0f, 0.0f,       // Rotation (no rotation)
            2.0f * uniformScale, 1.0f * uniformScale, 0.2f * uniformScale);  // Headboard size
        bedsheetw.drawCubeWithTexture(lightingShaderWithTexture, headboardModel);

        // Bed Legs (draw four legs at the corners of the bed)
        float legHeight = 0.8f * uniformScale; // Leg height adjusted for better proportion

        // Front-left corner leg
        glm::mat4 leg1Model = transformation(
            17.6f, 1.0f, -3.0f,       // Position for the first leg (front-left corner)
            0.0f, 0.0f, 0.0f,       // Rotation (no rotation)
            0.2f * uniformScale, legHeight, 0.2f * uniformScale);  // Leg size
        bedsheetw.drawCubeWithTexture(lightingShaderWithTexture, leg1Model);

        // Front-right corner leg
        glm::mat4 leg2Model = transformation(
            18.4f, 1.0f, -3.0f,       // Position for the second leg (front-right corner)
            0.0f, 0.0f, 0.0f,       // Rotation (no rotation)
            0.2f * uniformScale, legHeight, 0.2f * uniformScale);  // Leg size
        bedsheetw.drawCubeWithTexture(lightingShaderWithTexture, leg2Model);

        // Back-left corner leg
        glm::mat4 leg3Model = transformation(
            17.6f, 1.0f, -1.5f,       // Position for the third leg (back-left corner)
            0.0f, 0.0f, 0.0f,       // Rotation (no rotation)
            0.2f * uniformScale, legHeight, 0.2f * uniformScale);  // Leg size
        bedsheetw.drawCubeWithTexture(lightingShaderWithTexture, leg3Model);

        // Back-right corner leg
        glm::mat4 leg4Model = transformation(
            18.4f, 1.0f, -1.5f,       // Position for the fourth leg (back-right corner)
            0.0f, 0.0f, 0.0f,       // Rotation (no rotation)
            0.2f * uniformScale, legHeight, 0.2f * uniformScale);  // Leg size
        bedsheetw.drawCubeWithTexture(lightingShaderWithTexture, leg4Model);

        model = transformation(17.25, 0.6, 0.7, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 1.5f, 2.4f, 0.1f);
        wall3.drawCubeWithTexture(lightingShaderWithTexture, model);
        model = transformation(15.5, 0.6, 0.1, rotateAngle_X, rotateAngle_Y+90, rotateAngle_Z, 4.0f, 2.4f, 0.1f);
        wall3.drawCubeWithTexture(lightingShaderWithTexture, model);
        model = transformation(15.5, 0.6, 0.1, rotateAngle_X, rotateAngle_Y -30, rotateAngle_Z, 1.0f, 2.4f, 0.1f);
        wall3.drawCubeWithTexture(lightingShaderWithTexture, model);
		model = transformation(7.87f, 1.0f, 4.5f, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 3.0f, 0.05f, 2.0f);
        carpet.drawCubeWithTexture(lightingShaderWithTexture, model);

        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);


        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        pointlight4.setUpPointLight(lightingShader);

        spotlight1.setUpspotLight(lightingShader);

        //constantShader.setMat4("view", view);
        //lightingShader.setMat4("view", view);

        lightingShader.setVec3("directionalLight.directiaon", 0.0f, -3.0f, 0.0f);
        lightingShader.setVec3("directionalLight.ambient", .5f, .5f, .5f);
        lightingShader.setVec3("directionalLight.diffuse", .8f, .8f, .8f);
        lightingShader.setVec3("directionalLight.specular", 1.0f, 1.0f, 1.0f);

        lightingShader.setBool("directionLightOn", true);

        model = transformation(16.75f + 0.05, 1.5f + 0.45, -2.8f + 0.05, rotateAngle_X, rotateAngle_Y, rotateAngle_Z + 180, 0.2, 0.04, 0.2);
        lightEffectB(lampVAO, lightingShader, model, glm::vec3(0.5f, 0.0f, 0.5f), indiceslamp.size());
        model = transformation(16.75f, 1.5f - 0.2, -2.8f, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1, 0.5, 0.1);
        lightEffect(cubeVAO, lightingShader, model, glm::vec3(0.6f, 0.4f, 0.2f)); // Neutral stone-like color

        model = transformation(9.5, 5.0, 4.5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z + 180, 0.5, 0.1, 0.5);
        lightEffectB(lampVAO, lightingShader, model, glm::vec3(0.5f, 0.0f, 0.5f), indiceslamp.size());


        // Table properties
        float tableRadius = 1.0f * uniformScale;    // Radius of the table top
        float tableHeight = 0.25f * uniformScale;    // Height of the table top
        float legRadius = 0.1f * uniformScale;      // Radius of the table legs
        legHeight = 1.0f * uniformScale;      // Height of the table legs
        unsigned int segments = 32;                 // Number of segments for smooth cylinder
        glm::vec3 tableColor = glm::vec3(0.6f, 0.3f, 0.1f); // Brownish table color

        // Position the table in front of the sofa
        glm::vec3 sofaPosition = glm::vec3(0.5f * uniformScale, 2.0f * uniformScale, 5.0f * uniformScale);
        float sofaOrientation = 0.0f; // In degrees
        float tableDistanceFromSofa = 2.0f * uniformScale; // Distance of table from the sofa

        // Calculate table position based on sofa orientation
        float tableX = sofaPosition.x + glm::cos(glm::radians(sofaOrientation)) * tableDistanceFromSofa;
        float tableZ = -0.8+sofaPosition.z + glm::sin(glm::radians(sofaOrientation)) * tableDistanceFromSofa;
        
       float tableY = 0.5+sofaPosition.y + tableHeight; // Adjust table height above ground
       
       model = transformation(tableX+0.05, tableY + 0.45, tableZ+0.05, rotateAngle_X, rotateAngle_Y, rotateAngle_Z + 180, 0.2, 0.04, 0.2);
        lightEffectB(lampVAO, lightingShader, model, glm::vec3(0.5f, 0.0f, 0.5f), indiceslamp.size());
        model = transformation(tableX, tableY-0.2, tableZ, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1, 0.5, 0.1);
        lightEffect(cubeVAO, lightingShader, model, glm::vec3(0.6f, 0.4f, 0.2f)); // Neutral stone-like color


        // Draw table top
        drawCylinder(lightingShader, tableX, tableY-0.2, tableZ, tableRadius, tableHeight-0.025, segments, tableColor);

        // Draw table legs
        float legOffset = 0.5f * tableRadius; // Distance of legs from center
        drawCylinder(lightingShader, tableX + legOffset, tableY - legHeight, tableZ + legOffset,
            legRadius, legHeight, segments, tableColor);
        drawCylinder(lightingShader, tableX - legOffset, tableY - legHeight, tableZ + legOffset,
            legRadius, legHeight, segments, tableColor);
        drawCylinder(lightingShader, tableX + legOffset, tableY - legHeight, tableZ - legOffset,
            legRadius, legHeight, segments, tableColor);
        drawCylinder(lightingShader, tableX - legOffset, tableY - legHeight, tableZ - legOffset,
            legRadius, legHeight, segments, tableColor);
        // Define the position, scale, and colors for the chair
        glm::vec4 backrestColor(0.5f, 0.2f, 0.0f, 1.0f);  // Brown color for the backrest
        glm::vec4 seatColor(0.2f, 0.2f, 0.2f, 1.0f);      // Dark gray color for the seat
        glm::vec4 legColor(0.3f, 0.3f, 0.3f, 1.0f);       // Dark gray color for the legs

        float scale = 0.8f * uniformScale;  // Scale factor for the chair
        float tx = 1.0f, ty = 1.3f, tz = 0.7f; // Position of the chair

        // Draw the chair
        drawChair(cubeVAO, lightingShader, tx, ty, tz, scale, backrestColor, seatColor, legColor);



        

        // slider_wall1
        model = transformation(-10, 0.0, -5.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 35.0f, 1.4f, 0.1f);
        lightEffect(cubeVAO, lightingShader, model, glm::vec3(0.65f, 0.55f, 0.45f)); // Neutral stone-like color

        // slider_wall41
        model = transformation(-10, 0.0, 18.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 12.0f, 1.4f, 0.1f);
        lightEffect(cubeVAO, lightingShader, model, glm::vec3(0.65f, 0.55f, 0.45f)); // Neutral stone-like color
        // slider_wall42
        model = transformation(6, 0.0, 18.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 19.1f, 1.4f, 0.1f);
        lightEffect(cubeVAO, lightingShader, model, glm::vec3(0.65f, 0.55f, 0.45f)); // Neutral stone-like color

        // slider_wall2
        model = transformation(25, 0.0, -5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1f, 1.4f, 23.0f);
        lightEffect(cubeVAO, lightingShader, model, glm::vec3(0.65f, 0.55f, 0.45f)); // Neutral stone-like color

        // slider_wall3
        model = transformation(-10, 0.0, -5, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.1f, 1.4f, 23.0f);
        lightEffect(cubeVAO, lightingShader, model, glm::vec3(0.65f, 0.55f, 0.45f)); // Neutral stone-like color


        model = transformation(6, 3, 18.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.2, 0.2, 0.2);
        lightEffectB(ropeVAO, lightingShader, model, glm::vec3(0.3882f, 0.4000f, 0.4157f), indicesRope.size());
        
        model = transformation(2, 3, 18.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.2, 0.2, 0.2);
        lightEffectB(ropeVAO, lightingShader, model, glm::vec3(0.3882f, 0.4000f, 0.4157f), indicesRope.size());

        

        drawHalfTorus(lightingShader, 4, 1.8, 18.0, 2.35f, 0.1f, 32, 16, glm::vec3(0.3882f, 0.4000f, 0.4157f), 0.0f, 0.0f, 30.0f);

        //gates 2 piller
        model = transformation(6, 1.5, 18.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.25, 0.2, 0.25);
        lightEffectB(gatepillerVAO, lightingShader, model, glm::vec3(0.3882f, 0.4000f, 0.4157f), wallindicesPillar.size());

        model = transformation(2, 1.5, 18.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 0.25, 0.2, 0.25);
        lightEffectB(gatepillerVAO, lightingShader, model, glm::vec3(0.3882f, 0.4000f, 0.4157f),wallindicesPillar.size());



        float gateWidth = 3.7f;
        float gateHeight = 0.25f;
        int verticalBarCount = 17;
        int horizontalBarCount = 3;
        if (maingate) {
            // Open the gate by increasing the angle up to 90 degrees
            if (gateRotationAngle < 90) {
                gateRotationAngle += 0.5;
            }
            else {
                gateRotationAngle = 90; // Clamp to 90 degrees
            }
        }
        else {
            // Close the gate by decreasing the angle down to 0 degrees
            if (gateRotationAngle > 0) {
                gateRotationAngle -= 0.5;
            }
            else {
                gateRotationAngle = 0; // Clamp to 0 degrees
            }
        }

        //fence of graveyard
        createFencebuilding(1.0f, 3.0f, 10.38f,//x,y,z
            80, 0.2f, cubeVAO, cubeVAO, lightingShader,
            glm::vec3(0.73f, 0.73f, 0.73f), // Vertical stick color
            glm::vec3(0.73f, 0.73f, 0.73f), // Horizontal stick color
            32.0f,                          // Horizontal stick spacing adjustment
            0.0f,                         // Rotation angle (e.g., 45 degrees)
            glm::vec3(0.0f, 1.0f, 0.0f));  // Rotation axis (Y-axis)

        // First Side (Provided)
        createFencebuilding(3.5f, 3.0f, 6.9f, // x, y, z
            43, 0.25f, cubeVAO, cubeVAO, lightingShader,
            glm::vec3(0.8f, 0.7f, 0.5f), // Vertical stick color
            glm::vec3(00.8f, 0.7f, 0.5f), // Horizontal stick color
            32.0f,                          // Horizontal stick spacing adjustment
            0.0f,                           // Rotation angle
            glm::vec3(0.0f, 1.0f, 0.0f));   // Rotation axis (Y-axis)

        // Second Side (Provided)
        createFencebuilding(-7.0f, 3.0f, 14.25f, // x, y, z
            28, 0.25f, cubeVAO, cubeVAO, lightingShader,
            glm::vec3(0.8f, 0.7f, 0.5f), // Vertical stick color
            glm::vec3(0.8f, 0.7f, 0.5f), // Horizontal stick color
            32.0f,                          // Horizontal stick spacing adjustment
            90.0f,                          // Rotation angle
            glm::vec3(0.0f, 1.0f, 0.0f));   // Rotation axis (Y-axis)
        // First Side (Provided)
        createFencebuilding(3.5f, 3.0f, 0.0f, // x, y, z
            20, 0.25f, cubeVAO, cubeVAO, lightingShader,
            glm::vec3(0.8f, 0.7f, 0.5f), // Vertical stick color
            glm::vec3(0.8f, 0.7f, 0.5f), // Horizontal stick color
            32.0f,                          // Horizontal stick spacing adjustment
            0.0f,                           // Rotation angle
            glm::vec3(0.0f, 1.0f, 0.0f));   // Rotation axis (Y-axis)
        // Second Side (Provided)
        createFencebuilding(-7.0f, 3.0f, 3.5f, // x, y, z
            28, 0.25f, cubeVAO, cubeVAO, lightingShader,
            glm::vec3(0.8f, 0.7f, 0.5f), // Vertical stick color
            glm::vec3(0.8f, 0.7f, 0.5f), // Horizontal stick color
            32.0f,                          // Horizontal stick spacing adjustment
            90.0f,                          // Rotation angle
            glm::vec3(0.0f, 1.0f, 0.0f));   // Rotation axis (Y-axis)
        createFencebuilding(10.5f, 3.0f, 0.0f, // x, y, z
            20, 0.25f, cubeVAO, cubeVAO, lightingShader,
            glm::vec3(0.8f, 0.7f, 0.5f), // Vertical stick color
            glm::vec3(0.8f, 0.7f, 0.5f), // Horizontal stick color
            32.0f,                          // Horizontal stick spacing adjustment
            0.0f,                           // Rotation angle
            glm::vec3(0.0f, 1.0f, 0.0f));   // Rotation axis (Y-axis)

        createSteelGate(2.2f, 1.5f, 18.0f,
            gateWidth, gateHeight,
            verticalBarCount, horizontalBarCount,
            rotorVAO, lightingShader,
            glm::vec3(0.2627f, 0.2745f, 0.2941f), // Silver color
            gateRotationAngle); // Pass rotation angle

		//fence of graveyard
        createFence(-10.0f, 0.2f, 4.0f,//x,y,z
            30, 0.2f, graveVAO, cubeVAO, lightingShader,
            glm::vec3(0.4f, 0.26f, 0.13f), // Vertical stick color
            glm::vec3(0.4f, 0.26f, 0.13f), // Horizontal stick color
            32.0f,                          // Horizontal stick spacing adjustment
            0.0f,                         // Rotation angle (e.g., 45 degrees)
            glm::vec3(0.0f, 1.0f, 0.0f));  // Rotation axis (Y-axis)

        createFence(-4.1f, 0.2f, -4.0f,//x,y,z
            45, 0.2f, graveVAO, cubeVAO, lightingShader,
            glm::vec3(0.4f, 0.26f, 0.13f), // Vertical stick color
            glm::vec3(0.4f, 0.26f, 0.13f), // Horizontal stick color
            32.0f,                          // Horizontal stick spacing adjustment
            90.0f,                         // Rotation angle (e.g., 45 degrees)
            glm::vec3(0.0f, 1.0f, 0.0f));  // Rotation axis (Y-axis)

        // fan, 6, 5, 6
    //on = true;
        if (Fon) {
            r += 5;
        }
      

       
        //fan stick
        translateMatrix = glm::translate(identityMatrix, glm::vec3(3.0, 4.0, 3.0));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.1f, 0.9f, 0.1));
        model = translateMatrix * scaleMatrix;
        ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(0.0, 0.0, 0.0, 1.0));
        glBindVertexArray(cubeVAO);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // fan rotation
        translateMatrixprev = translateMatrix;
        glm::mat4 translateMatrix2, translateMatrixBack, test;

        translateMatrix2 = glm::translate(identityMatrix, glm::vec3(3.025, 10.0, 3.025));
        translateMatrixBack = glm::translate(identityMatrix, glm::vec3(-3.025, -4.0, -3.02));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(r), glm::vec3(0.0, 1.0, 0.0));
        model = translateMatrixBack * rotateYMatrix * translateMatrix2;
        drawFan(graveVAO, lightingShader, translateMatrix, rotateYMatrix);


        //BUILDING MAKING
        // slider_wall1
        model = transformation(-1.0, 0.0, -4.0, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 20.0f, 0.3f, 15.0f);
        lightEffect(cubeVAO, lightingShader, model, glm::vec3(0.5f, 0.5f, 0.5f)); // Neutral stone-like color
        model = transformation(-0.75, 0.3, -3.75, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 19.5f, 0.7f, 14.5f);
        lightEffect(cubeVAO, lightingShader, model, glm::vec3(0.35f, 0.3f, 0.25f)); // Neutral stone-like color
        //toplevel
        model = transformation(-0.75, 2.9, -3.75, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 19.5f, 0.1f, 4.0f);
        lightEffect(cubeVAO, lightingShader, model, glm::vec3(0.35f, 0.3f, 0.25f)); // Neutral stone-like color
        model = transformation(-0.75, 2.9, -3.75, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 4.5f, 0.1f, 14.5f);
        lightEffect(cubeVAO, lightingShader, model, glm::vec3(0.35f, 0.3f, 0.25f)); // Neutral stone-like color
        model = transformation(-0.75, 2.9, 6.75, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 19.5f, 0.1f, 4.0f);
        lightEffect(cubeVAO, lightingShader, model, glm::vec3(0.35f, 0.3f, 0.25f)); // Neutral stone-like color
        model = transformation(14.2, 2.9, -3.75, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 4.5f, 0.1f, 14.5f);
        lightEffect(cubeVAO, lightingShader, model, glm::vec3(0.35f, 0.3f, 0.25f)); // Neutral stone-like color

        
        model = transformation(-0.75, 5.0, -3.75, rotateAngle_X, rotateAngle_Y, rotateAngle_Z, 19.5f, 0.5f, 14.5f);
        lightEffect(cubeVAO, lightingShader, model, glm::vec3(0.35f, 0.3f, 0.25f)); // Neutral stone-like color

        drawCylinder(lightingShader, -0.8, 0.4, 11.0, 2.5, 1.0, 36, glm::vec3(0.35f, 0.3f, 0.25f));
		drawCylinder(lightingShader, -0.8, 5.3, 11.0, 2.5, 0.6, 36, glm::vec3(0.35f, 0.3f, 0.25f));
        drawCylinder(lightingShader, 18.8, 0.4, 11.0, 2.5, 1.0, 36, glm::vec3(0.35f, 0.3f, 0.25f));
        drawCylinder(lightingShader, 18.8, 5.3, 11.0, 2.5, 0.6, 36, glm::vec3(0.35f, 0.3f, 0.25f));
        //create stairs
        glm::vec3 startPosition = glm::vec3(9.0f, 0.7f, 10.75f); // Starting position
        glm::vec3 stepSize = glm::vec3(1.5f, 0.3f, 0.3f);         // Step dimensions
        int numSteps = 5;                                         // Number of steps
        glm::vec3 stairColor = glm::vec3(0.35f, 0.3f, 0.25f);       // Neutral stone-like color

        // Draw reversed stairs
        drawStairs(cubeVAO, lightingShader, startPosition, stepSize, numSteps, stairColor);



        // Define parameters for the stairs and fences
        startPosition=glm::vec3(9.37f, 2.7f, 0.25f);  // Starting position for the stairs
        stepSize=glm::vec3(2.0f, 0.3f, 0.5f);       // Size of each step (width, height, depth)
        numSteps =7;                          // Number of steps
        glm::vec3 color(0.8f, 0.7f, 0.5f);          // Color of the stairs and fences (RGB)
        glm::vec3 fenceSize(0.1f, 0.8f, 0.1f);      // Size of each fence (width, height, depth)
        glm::vec3 fenceOffset(0.0f, 0.0f, 0.0f);    // Offset for the fences from the stairs (horizontal distance)

        // Now call the function to draw the stairs with fences
        drawStairsWithFences(cubeVAO, lightingShader, startPosition, stepSize, numSteps, color, fenceSize, fenceOffset);

        model = transformation(8.38f, 3.5f, 0.22f, rotateAngle_X+30, rotateAngle_Y-9.5, rotateAngle_Z, 0.1f, 0.1f, 4.2f);
        lightEffect(cubeVAO, lightingShader, model, glm::vec3(0.8f, 0.7f, 0.5f)); // Neutral stone-like color

        model = transformation(10.38f, 3.5f, 0.22f, rotateAngle_X + 30, rotateAngle_Y + 9.5, rotateAngle_Z, 0.1f, 0.1f, 4.2f);
        lightEffect(cubeVAO, lightingShader, model, glm::vec3(0.8f, 0.7f, 0.5f)); // Neutral stone-like color




        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        //glm::mat4 view = basic_camera.createViewMatrix();
      

        glBindVertexArray(lightCubeVAO);
        for (unsigned int i = 0; i < 3; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.1f)); // Make it a smaller cube
            ourShader.setMat4("model", model);
            ourShader.setVec4("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(9.5, 2.8, 7.5));
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        ourShader.setMat4("model", model);
        ourShader.setVec4("color", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
	glDeleteVertexArrays(1, &graveVAO);
    glDeleteBuffers(1, &cubeVAO);
    glDeleteBuffers(1, &lightCubeVAO);
	glDeleteBuffers(1, &graveVAO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void drawFan(unsigned int VAO, Shader ourShader, glm::mat4 translateMatrix, glm::mat4 sm)
{
    ourShader.use();
    glm::mat4 identityMatrix = glm::mat4(1.0f); // Initialize matrix to identity matrix first
    glm::mat4 rotateXMatrix, rotateZMatrix, scaleMatrix, model, modelCentered, translateMatrixprev;
    glm::mat4 middleTranslate, leftBladeTranslate, frontBladeTranslate, rightBladeTranslate, backBladeTranslate;

    // Rotation angles in radians (45 degrees)
    float angleX = glm::radians(-30.0f); // Rotation for right and left blades around X-axis
    float angleZ = glm::radians(-30.0f); // Rotation for front and back blades around Z-axis

    // Fan middle part (pivot point)
    middleTranslate = glm::translate(identityMatrix, glm::vec3(-0.2, 1.0, -0.2));  // Move the pivot up much more (increase Y value significantly)
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, -0.3f, 0.4f)); // Decrease size of the pivot cube
    model = translateMatrix * sm * middleTranslate * scaleMatrix;
    lightEffect(VAO, ourShader, model, glm::vec3(0.5, 0.6, 0.4));

    translateMatrixprev = translateMatrix;

    // Rotate right-left blades around X-axis
    rotateXMatrix = glm::rotate(identityMatrix, angleX, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X-axis

    // Left fan blade (rotate around X-axis)
    leftBladeTranslate = glm::translate(identityMatrix, glm::vec3(-0.2, 0.7, -0.2)); // Move the blade up much more (increase Y value significantly)
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-1.2f, -0.08f, 0.25f)); // Decrease size of left blade
    model = translateMatrix * sm * leftBladeTranslate * rotateXMatrix * scaleMatrix;
    lightEffect(VAO, ourShader, model, glm::vec3(0.5, 0.6, 0.5));

    // Right fan blade (rotate around X-axis)
    rightBladeTranslate = glm::translate(identityMatrix, glm::vec3(0.15, 0.8, 0.2)); // Fix gap, moved closer to the pivot
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(1.2f, -0.08f, -0.25f)); // Decrease size of right blade
    model = translateMatrix * sm * rightBladeTranslate * rotateXMatrix * scaleMatrix;
    lightEffect(VAO, ourShader, model, glm::vec3(0.5, 0.6, 0.5));

    // Rotate front-back blades around Z-axis
    rotateZMatrix = glm::rotate(identityMatrix, angleZ, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z-axis

    // Front fan blade (rotate around Z-axis)
    frontBladeTranslate = glm::translate(identityMatrix, glm::vec3(-0.2, 0.8, 0.2)); // Fix gap, moved closer to the pivot
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.25f, -0.08f, 1.2f)); // Decrease size of front blade
    model = translateMatrix * sm * frontBladeTranslate * rotateZMatrix * scaleMatrix;
    lightEffect(VAO, ourShader, model, glm::vec3(0.5, 0.6, 0.5));

    // Back fan blade (rotate around Z-axis)
    backBladeTranslate = glm::translate(identityMatrix, glm::vec3(0.3, 0.7, -0.2)); // Move the blade up much more (increase Y value significantly)
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-0.25f, -0.08f, -1.2f)); // Decrease size of back blade
    model = translateMatrix * sm * backBladeTranslate * rotateZMatrix * scaleMatrix;
    lightEffect(VAO, ourShader, model, glm::vec3(0.5, 0.6, 0.5));
}





void lightEffect(unsigned int VAO, Shader lightShader, glm::mat4 model, glm::vec3 color)
{
    lightShader.use();
    lightShader.setVec3("material.ambient", color);
    lightShader.setVec3("material.diffuse", color);
    lightShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightShader.setFloat("material.shininess", 32.0f);

    lightShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, (unsigned int)indices.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}

void lightEffectB(unsigned int VAO, Shader lightShader, glm::mat4 model, glm::vec3 color, unsigned int indicesb)
{
    lightShader.use();
    lightShader.setVec3("material.ambient", color);
    lightShader.setVec3("material.diffuse", color);
    lightShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightShader.setFloat("material.shininess", 32.0f);

    lightShader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indicesb, GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}
void drawChair(unsigned int VAO, Shader& shader, float tx, float ty, float tz, float scale, glm::vec4 backrestColor, glm::vec4 seatColor, glm::vec3 legColor)
{
    // Backrest (Cube shape)
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(tx-0.2, ty - 0.1f * scale, tz-0.18));
    model = glm::scale(model, glm::vec3(1.0f * scale, 1.0f * scale, 0.2f * scale));
    drawCube(VAO, shader, model, backrestColor);

    // Seat (Cube shape)
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(tx-0.2, ty - 0.2f * scale, tz - 0.18));
    model = glm::scale(model, glm::vec3(1.0f * scale, 0.2f * scale, 1.0f * scale));
    drawCube(VAO, shader, model, seatColor);

    // Front left leg (Cylinder shape)
    drawCylinder(shader, tx - 0.4f * scale, ty - 0.5f * scale, tz - 0.4f * scale, 0.1f * scale, 0.6f * scale, 12, legColor);

    // Front right leg (Cylinder shape)
    drawCylinder(shader, tx + 0.4f * scale, ty - 0.5f * scale, tz - 0.4f * scale, 0.1f * scale, 0.6f * scale, 12, legColor);

    // Back left leg (Cylinder shape)
    drawCylinder(shader, tx - 0.4f * scale, ty - 0.5f * scale, tz + 0.4f * scale, 0.1f * scale, 0.6f * scale, 12, legColor);

    // Back right leg (Cylinder shape)
    drawCylinder(shader, tx + 0.4f * scale, ty - 0.5f * scale, tz + 0.4f * scale, 0.1f * scale, 0.6f * scale, 12, legColor);
}
void drawCube(unsigned int VAO, Shader shader, glm::mat4 model, glm::vec4 color)
{
    shader.setMat4("model", model);
    shader.setVec4("color", color);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

long long nCr(int n, int r)
{
    if (r > n / 2)
        r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for (i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}

void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L)
{
    double y = 0;
    double x = 0;
    t = t > 1.0 ? 1.0 : t;
    for (int i = 0; i < L + 1; i++)
    {
        long long ncr = nCr(L, i);
        double oneMinusTpow = pow(1 - t, double(L - i));
        double tPow = pow(t, double(i));
        double coef = oneMinusTpow * tPow * ncr;
        x += coef * ctrlpoints[i * 3];
        y += coef * ctrlpoints[(i * 3) + 1];

    }
    xy[0] = float(x);
    xy[1] = float(y);
}

unsigned int hollowBezier(GLfloat ctrlpoints[], int L, vector<float>& coordinates, vector<float>& normals, vector<int>& indices, vector<float>& vertices, float div = 1.0)
{
    int i, j;
    float x, y, z, r;                //current coordinates
    float theta;
    float nx, ny, nz, lengthInv;    // vertex normal


    const float dtheta = (2 * pi / ntheta) / div;        //angular step size

    float t = 0;
    float dt = 1.0 / nt;
    float xy[2];
    vector <float> textureCoords;
    for (i = 0; i <= nt; ++i)              //step through y
    {
        BezierCurve(t, xy, ctrlpoints, L);
        r = xy[0];
        y = xy[1];
        theta = 0;
        t += dt;
        lengthInv = 1.0 / r;

        for (j = 0; j <= ntheta; ++j)
        {
            double cosa = cos(theta);
            double sina = sin(theta);
            z = r * cosa;
            x = r * sina;

            coordinates.push_back(x);
            coordinates.push_back(y);
            coordinates.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            // center point of the circle (0,y,0)
            nx = (x - 0) * lengthInv;
            ny = (y - y) * lengthInv;
            nz = (z - 0) * lengthInv;

            normals.push_back(-nx);
            normals.push_back(-ny);
            normals.push_back(-nz);

            // Texture coordinates (u, v)
            float u = float(j) / float(ntheta); // Around the circle
            float v = float(i) / float(nt);     // Along the curve
            textureCoords.push_back(u);
            textureCoords.push_back(v);

            theta += dtheta;
        }
    }

    // generate index list of triangles
    // k1--k1+1
    // |  / |
    // | /  |
    // k2--k2+1

    int k1, k2;
    for (int i = 0; i < nt; ++i)
    {
        k1 = i * (ntheta + 1);     // beginning of current stack
        k2 = k1 + ntheta + 1;      // beginning of next stack

        for (int j = 0; j < ntheta; ++j, ++k1, ++k2)
        {
            // k1 => k2 => k1+1
            indices.push_back(k1);
            indices.push_back(k2);
            indices.push_back(k1 + 1);

            // k1+1 => k2 => k2+1
            indices.push_back(k1 + 1);
            indices.push_back(k2);
            indices.push_back(k2 + 1);
        }
    }

    size_t count = coordinates.size();
    for (int i = 0; i < count; i += 3)
    {
        vertices.push_back(coordinates[i]);
        vertices.push_back(coordinates[i + 1]);
        vertices.push_back(coordinates[i + 2]);

        vertices.push_back(normals[i]);
        vertices.push_back(normals[i + 1]);
        vertices.push_back(normals[i + 2]);

        // Add texture coordinates
        vertices.push_back(textureCoords[i / 3 * 2]);
        vertices.push_back(textureCoords[i / 3 * 2 + 1]);
    }

    unsigned int bezierVAO;
    glGenVertexArrays(1, &bezierVAO);
    glBindVertexArray(bezierVAO);

    // create VBO to copy vertex data to VBO
    unsigned int bezierVBO;
    glGenBuffers(1, &bezierVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bezierVBO);           // for vertex data
    glBufferData(GL_ARRAY_BUFFER,                   // target
        (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
        vertices.data(),   // ptr to vertex data
        GL_STATIC_DRAW);                   // usage

    // create EBO to copy index data
    unsigned int bezierEBO;
    glGenBuffers(1, &bezierEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bezierEBO);   // for index data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
        (unsigned int)indices.size() * sizeof(unsigned int),             // data size, # of bytes
        indices.data(),               // ptr to index data
        GL_STATIC_DRAW);                   // usage

    // activate attrib arrays
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // set attrib arrays with stride and offset
    int stride = 32;     // should be 32 bytes
    glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float))); // Texture Coord


    // unbind VAO, VBO and EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return bezierVAO;
}

void read_file(string file_name, vector<float>& vec)
{
    ifstream file(file_name);
    float number;

    while (file >> number)
        vec.push_back(number);

    file.close();
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        if (!birdEye)
            camera.ProcessKeyboard(UP, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        if (!birdEye)
            camera.ProcessKeyboard(DOWN, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        if (!birdEye)
            camera.ProcessKeyboard(P_UP, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        if (!birdEye)
            camera.ProcessKeyboard(P_DOWN, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        if (!birdEye)
            camera.ProcessKeyboard(Y_LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        if (!birdEye)
            camera.ProcessKeyboard(Y_RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
        if (!birdEye)
            camera.ProcessKeyboard(R_LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        if (!birdEye)
            camera.ProcessKeyboard(R_RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
            maingate = !maingate;
            cout << "Main Gate:"<<maingate << endl;

        
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        Fon = !Fon;
        cout << "Fan ON?:" << Fon << endl;


    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		windoopen = !windoopen;
        cout << "Window:" << Fon << endl;


    }



    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) birdEye = true;
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) birdEye = false;




}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{


    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        if (dl)
        {       dl = false;
        spotlight1.turnOff();
    }
        
        else {
            spotlight1.turnOn();
            dl = true;
        }

    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        if (point1)
        {
            point1 = false;
            pointlight1.turnOff();
        }
        else
        {
            point1 = true;
            pointlight1.turnOn();
        }

    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
    {
        if (point2)
        {
            point2 = false;
            pointlight2.turnOff();
        }
        else
        {
            point2 = true;
            pointlight2.turnOn();
        }

    }
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)
    {
        if (point4)
        {
            point4 = false;
            pointlight4.turnOff();
        }
        else
        {
            point4 = true;
            pointlight4.turnOn();
        }

    }
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
    {
        if (point3)
        {
            point3 = false;
            pointlight3.turnOff();
        }
        else
        {
            point3 = true;
            pointlight3.turnOn();
        }

    }

    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
    {
        if (ambientToggle)
        {
            pointlight1.turnAmbientOff();
            pointlight2.turnAmbientOff();
            pointlight3.turnAmbientOff();
			spotlight1.turnAmbientOff();
            ambientToggle = false;
        }
        else
        {
            pointlight1.turnAmbientOn();
            pointlight2.turnAmbientOn();
            pointlight3.turnAmbientOn();
            spotlight1.turnAmbientOn();
            ambientToggle = true;
        }

    }

    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)
    {
        if (diffuseToggle)
        {
            pointlight1.turnDiffuseOff();
            pointlight2.turnDiffuseOff();
            pointlight3.turnDiffuseOff();
			spotlight1.turnDiffuseOff();
          
            //d_def_on = 0.0f;

            diffuseToggle = false;
        }
        else
        {
            pointlight1.turnDiffuseOn();
            pointlight2.turnDiffuseOn();
            pointlight3.turnDiffuseOn();
			spotlight1.turnDiffuseOn();
           

            //d_def_on = 1.0f;
            diffuseToggle = true;
        }

    }

    if (glfwGetKey(window, GLFW_KEY_7) == GLFW_PRESS)
    {
        if (specularToggle)
        {
            pointlight1.turnSpecularOff();
            pointlight2.turnSpecularOff();
            pointlight3.turnSpecularOff();
			spotlight1.turnSpecularOff();
            
            //d_def_on = 0.0f;

            specularToggle = false;
        }
        else
        {
            pointlight1.turnSpecularOn();
            pointlight2.turnSpecularOn();
            pointlight3.turnSpecularOn();
			spotlight1.turnSpecularOn();

            //d_def_on = 1.0f;
            specularToggle = true;
        }

    }


}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
void load_texture(unsigned int& texture, string image_name, GLenum format)
{
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(image_name.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        cout << "Failed to load texture " << image_name << endl;
    }
    stbi_image_free(data);
}

void textureEffect(unsigned int VAO, Shader shader, glm::mat4 model, unsigned int indicesSize, unsigned int texture0)
{
    shader.use();
    shader.setInt("material.diffuse", 0);
    shader.setInt("material.specular", 1);
    shader.setFloat("material.shininess", 32.0f);
    shader.setMat4("model", model);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture0);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indicesSize, GL_UNSIGNED_INT, (void*)0);
}
void createFence(float baseX, float baseY, float baseZ, int stickCount, float spacing,
    GLuint graveVAO, GLuint cubeVAO, Shader& lightingShader,
    glm::vec3 stickColor, glm::vec3 horizontalStickColor, float sp,
    float rotationAngle, glm::vec3 rotationAxis) {

    // Calculate the rotation matrix
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), rotationAxis);

    // Create vertical sticks
    for (int i = 0; i < stickCount; ++i) {
        float offsetX = baseX + i * spacing; // Calculate X position for each stick
        glm::mat4 model = transformation(offsetX, baseY, baseZ, 0.0f, 0.0f, 0.0f, 0.08f, 0.6f, 0.08f);
        model = rotationMatrix * model; // Apply rotation
        lightEffect(graveVAO, lightingShader, model, stickColor); // Vertical stick color
    }

    // Parameters for the horizontal sticks
    float horizontalY1 = baseY + 0.5f; // Height of the upper horizontal stick
    float horizontalY2 = baseY + 0.1f; // Height of the lower horizontal stick
    float horizontalLength = (stickCount - 1) * spacing + 0.08f; // Length of the horizontal sticks

    // Create the upper horizontal stick
    glm::mat4 modelUpper = transformation(baseX + (horizontalLength / (2.0f * sp)), horizontalY1, baseZ, 0.0f, 0.0f, 0.0f, horizontalLength, 0.08f, 0.08f);
    modelUpper = rotationMatrix * modelUpper; // Apply rotation
    lightEffect(cubeVAO, lightingShader, modelUpper, horizontalStickColor); // Upper horizontal stick color

    // Create the lower horizontal stick
    glm::mat4 modelLower = transformation(baseX + (horizontalLength / (2.0f * sp)), horizontalY2, baseZ, 0.0f, 0.0f, 0.0f, horizontalLength, 0.08f, 0.08f);
    modelLower = rotationMatrix * modelLower; // Apply rotation
    lightEffect(cubeVAO, lightingShader, modelLower, horizontalStickColor); // Lower horizontal stick color
}

void createFencebuilding(float baseX, float baseY, float baseZ, int stickCount, float spacing,
    GLuint graveVAO, GLuint cubeVAO, Shader& lightingShader,
    glm::vec3 stickColor, glm::vec3 horizontalStickColor, float sp,
    float rotationAngle, glm::vec3 rotationAxis) {

    // Calculate the rotation matrix
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), rotationAxis);

    // Create vertical sticks
    for (int i = 0; i < stickCount; ++i) {
        float offsetX = baseX + i * spacing; // Calculate X position for each stick
        glm::mat4 model = transformation(offsetX, baseY, baseZ, 0.0f, 0.0f, 0.0f, 0.08f, 0.6f, 0.08f);
        model = rotationMatrix * model; // Apply rotation
        lightEffect(graveVAO, lightingShader, model, stickColor); // Vertical stick color
    }

    // Parameters for the horizontal sticks
    float horizontalY1 = baseY+ 0.55f; // Height of the upper horizontal stick
    float horizontalY2 = baseY + 0.1f; // Height of the lower horizontal stick
    float horizontalLength = (stickCount - 1) * spacing + 0.08f; // Length of the horizontal sticks

    // Create the upper horizontal stick
    glm::mat4 modelUpper = transformation(baseX + (horizontalLength / (2.0f * sp)), horizontalY1, baseZ, 0.0f, 0.0f, 0.0f, horizontalLength, 0.08f, 0.08f);
    modelUpper = rotationMatrix * modelUpper; // Apply rotation
    lightEffect(cubeVAO, lightingShader, modelUpper, horizontalStickColor); // Upper horizontal stick color

    // Create the lower horizontal stick
    glm::mat4 modelLower = transformation(baseX + (horizontalLength / (2.0f * sp)), horizontalY2, baseZ, 0.0f, 0.0f, 0.0f, horizontalLength, 0.08f, 0.08f);
    modelLower = rotationMatrix * modelLower; // Apply rotation
    lightEffect(cubeVAO, lightingShader, modelLower, horizontalStickColor); // Lower horizontal stick color
}

void createSteelGate(float baseX, float baseY, float baseZ,
    float gateWidth, float gateHeight,
    int verticalBarCount, int horizontalBarCount,
    GLuint rotorVAO, Shader& lightingShader, glm::vec3 barColor, float gateRotationAngle) {
    float verticalSpacing = gateWidth / verticalBarCount; // Spacing between vertical bars
    float horizontalSpacing = gateHeight * 3; // Spacing between horizontal bars

    // Create a rotation transformation for the entire gate
    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    rotationMatrix = glm::translate(rotationMatrix, glm::vec3(baseX, baseY, baseZ)); // Move to base position
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(gateRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // Apply rotation
    rotationMatrix = glm::translate(rotationMatrix, glm::vec3(-baseX, -baseY, -baseZ)); // Return to origin

    // Create vertical bars
    for (int i = 0; i < verticalBarCount; ++i) {
        float offsetX = baseX + i * verticalSpacing;
        glm::mat4 model = transformation(offsetX, baseY, baseZ, 0.0f, 0.0f, 0.0f, 0.05f, gateHeight, 0.05f);
        model = rotationMatrix * model; // Apply the rotation transformation
        lightEffect(rotorVAO, lightingShader, model, barColor); // Steel bar color
    }

    // Create horizontal bars
    for (int i = 0; i < horizontalBarCount; ++i) {
        float offsetY = baseY / 2.5 + i * horizontalSpacing;
        glm::mat4 model = transformation(baseX + gateWidth / 2.0f, offsetY, baseZ, 0.0f, 0.0f, 0.0f, gateWidth / 1.5, 0.03f, 0.05f);
        model = rotationMatrix * model; // Apply the rotation transformation
        lightEffect(rotorVAO, lightingShader, model, barColor); // Steel bar color
    }
}
void drawHalfTorus(Shader& lightingShader, float tx, float ty, float tz, float majorRadius, float minorRadius,
    unsigned int segmentsU, unsigned int segmentsV, glm::vec3 color,
    float rotateX, float rotateY, float rotateZ) {
    // Create vertex and index arrays for the half torus
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // Generate vertices and normals for the half torus
    for (unsigned int i = 0; i <= segmentsU; ++i) {
        for (unsigned int j = 0; j <= segmentsV; ++j) {
            // Limit U (revolution angle) to 180° (π radians)
            float u = (float)i / (float)segmentsU * (2.0f * glm::pi<float>() / 3.0f);


            float v = (float)j / (float)segmentsV * 2.0f * glm::pi<float>();

            // Parametric equations for the torus
            float x = (majorRadius + minorRadius * cos(v)) * cos(u);
            float y = (majorRadius + minorRadius * cos(v)) * sin(u);
            float z = minorRadius * sin(v);

            // Center of the minor circle for normal calculation
            float centerX = majorRadius * cos(u);
            float centerY = majorRadius * sin(u);
            float centerZ = 0.0f;

            // Compute normal
            glm::vec3 normal = glm::normalize(glm::vec3(x - centerX, y - centerY, z - centerZ));

            // Add vertex position
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);

            // Add vertex normal
            vertices.push_back(normal.x);
            vertices.push_back(normal.y);
            vertices.push_back(normal.z);
        }
    }

    // Generate indices for the triangles
    for (unsigned int i = 0; i < segmentsU; ++i) {
        for (unsigned int j = 0; j < segmentsV; ++j) {
            unsigned int first = (i * (segmentsV + 1)) + j;
            unsigned int second = ((i + 1) * (segmentsV + 1)) + j;
            unsigned int third = ((i + 1) * (segmentsV + 1)) + (j + 1);
            unsigned int fourth = (i * (segmentsV + 1)) + (j + 1);

            // Two triangles per quad
            indices.push_back(first);
            indices.push_back(second);
            indices.push_back(third);

            indices.push_back(first);
            indices.push_back(third);
            indices.push_back(fourth);
        }
    }

    // Create VAO and VBO for the torus
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Apply transformations
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(tx, ty, tz)); // Translation
    model = glm::rotate(model, glm::radians(rotateX), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotate around X axis
    model = glm::rotate(model, glm::radians(rotateY), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotate around Y axis
    model = glm::rotate(model, glm::radians(rotateZ), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotate around Z axis

    lightEffect(VAO, lightingShader, model, color);
    // Clean up
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}


void placeCylindersInHexagon(float centerX, float centerY, float centerZ, float radius,
    Shader& lightingShaderWithTexture, GLuint rotorVAO,
    int indicesCount, GLuint texture) {
    const int edges = 4; // Hexagon has 6 edges
    float angleStep = 360.0f / edges; // Each edge is 60 degrees apart

    for (int i = 0; i < edges; ++i) {
        float angle = glm::radians(i * angleStep); // Convert to radians
        float x = centerX + radius * cos(angle);  // x-coordinate of vertex
        float z = centerZ + radius * sin(angle);  // z-coordinate of vertex
        float y = centerY;                        // y-coordinate remains constant

        // Create transformation for each cylinderdrawcy
        glm::mat4 model = transformation(x, y, z, 0.0f, 0.0f, 0.0f, 0.3f, 0.6f, 0.3f);

        // Render the cylinder at this position
        textureEffect(rotorVAO, lightingShaderWithTexture, model, indicesCount, texture);
    }
}

void drawCylinder(Shader& shader, float tx, float ty, float tz, float radius, float height, unsigned int segments, glm::vec3 color) {
    // Create vertex and index arrays for the cylinder
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    // Top center vertex
    vertices.push_back(0.0f);  // X
    vertices.push_back(height / 2.0f); // Y
    vertices.push_back(0.0f);  // Z
    vertices.push_back(0.0f);  // Normal X
    vertices.push_back(1.0f);  // Normal Y
    vertices.push_back(0.0f);  // Normal Z
    vertices.push_back(color.r);  // R
    vertices.push_back(color.g);  // G
    vertices.push_back(color.b);  // B

    // Bottom center vertex
    vertices.push_back(0.0f);  // X
    vertices.push_back(-height / 2.0f); // Y
    vertices.push_back(0.0f);  // Z
    vertices.push_back(0.0f);  // Normal X
    vertices.push_back(-1.0f); // Normal Y
    vertices.push_back(0.0f);  // Normal Z
    vertices.push_back(color.r);  // R
    vertices.push_back(color.g);  // G
    vertices.push_back(color.b);  // B

    // Generate the top and bottom vertices around the circle
    for (unsigned int i = 0; i <= segments; ++i) {
        float angle = (float)i / (float)segments * 2.0f * glm::pi<float>();
        float x = radius * cos(angle);
        float z = radius * sin(angle);

        // Top vertices
        vertices.push_back(x); // X
        vertices.push_back(height / 2.0f); // Y
        vertices.push_back(z); // Z
        glm::vec3 sideNormal = glm::normalize(glm::vec3(x, 0.0f, z)); // Correct normal for sides
        vertices.push_back(sideNormal.x);  // Normal X
        vertices.push_back(sideNormal.y);  // Normal Y
        vertices.push_back(sideNormal.z);  // Normal Z
        vertices.push_back(color.r);  // R
        vertices.push_back(color.g);  // G
        vertices.push_back(color.b);  // B

        // Bottom vertices
        vertices.push_back(x); // X
        vertices.push_back(-height / 2.0f); // Y
        vertices.push_back(z); // Z
        vertices.push_back(sideNormal.x);  // Normal X (same as top)
        vertices.push_back(sideNormal.y);  // Normal Y (same as top)
        vertices.push_back(sideNormal.z);  // Normal Z (same as top)
        vertices.push_back(color.r);  // R
        vertices.push_back(color.g);  // G
        vertices.push_back(color.b);  // B
    }

    // Generate indices for the top and bottom circles
    for (unsigned int i = 0; i < segments; ++i) {
        // Top circle
        indices.push_back(0); // Center
        indices.push_back(2 + 2 * i);
        indices.push_back(2 + 2 * (i + 1));

        // Bottom circle
        indices.push_back(1); // Center
        indices.push_back(3 + 2 * (i + 1));
        indices.push_back(3 + 2 * i);
    }

    // Generate indices for the side quads (two triangles per quad)
    for (unsigned int i = 0; i < segments; ++i) {
        unsigned int top1 = 2 + 2 * i;
        unsigned int top2 = 2 + 2 * (i + 1);
        unsigned int bottom1 = 3 + 2 * i;
        unsigned int bottom2 = 3 + 2 * (i + 1);

        // First triangle
        indices.push_back(top1);
        indices.push_back(bottom1);
        indices.push_back(top2);

        // Second triangle
        indices.push_back(top2);
        indices.push_back(bottom1);
        indices.push_back(bottom2);
    }

    // Create VAO and VBO for the cylinder
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Upload vertex data
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Upload index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0); // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float))); // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float))); // Color
    glEnableVertexAttribArray(2);

    // Apply the transformation and render the cylinder
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(tx, ty, tz)); // Translate
    shader.setMat4("model", model);

    lightEffect(VAO, shader, model, color);

    // Clean up
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
void createTwoPartWindow(
    Shader& shader,
    Cube& rusticWindow,
    float baseX,
    float baseY,
    float baseZ,
    float angle,
    float scaleX = 0.5f,
    float scaleY = 1.0f,
    float scaleZ = 0.1f
) {
    // First window panel
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(baseX, baseY, baseZ)); // Base position
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f)); // Open by angle
    model = glm::scale(model, glm::vec3(scaleX, scaleY, scaleZ)); // Scale
    rusticWindow.drawCubeWithTexture(shader, model);

    // Second window panel
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(baseX + 1.0f, baseY, baseZ + 0.1f)); // Adjusted position
    model = glm::rotate(model, glm::radians(-angle + 180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Open in the opposite direction
    model = glm::scale(model, glm::vec3(scaleX, scaleY, scaleZ)); // Scale
    rusticWindow.drawCubeWithTexture(shader, model);
}
void createTwoPartDoor(
    Shader& shader,
    Cube& rusticWindow,
    float baseX,
    float baseY,
    float baseZ,
    float angle,
    float scaleX = 0.5f,
    float scaleY = 1.0f,
    float scaleZ = 0.1f
) {
    // First window panel
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(baseX, baseY, baseZ)); // Base position
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f)); // Open by angle
    model = glm::scale(model, glm::vec3(scaleX, scaleY, scaleZ)); // Scale
    rusticWindow.drawCubeWithTexture(shader, model);

    // Second window panel
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(baseX + 2.0f, baseY, baseZ + 0.1f)); // Adjusted position
    model = glm::rotate(model, glm::radians(-angle + 180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Open in the opposite direction
    model = glm::scale(model, glm::vec3(scaleX, scaleY, scaleZ)); // Scale
    rusticWindow.drawCubeWithTexture(shader, model);
}

void drawStairs(
    unsigned int cubeVAO,
    Shader& shader,
    glm::vec3 startPosition,
    glm::vec3 stepSize,
    int numSteps,
    glm::vec3 color
) {
    for (int i = 0; i < numSteps; ++i) {
        glm::mat4 model = glm::mat4(1.0f);

        // Reverse the position for the current step
        glm::vec3 stepPosition = startPosition;
        stepPosition -= glm::vec3(0.0f, i * stepSize.y, -i * stepSize.z); // Move down and back

        // Scale the step width on the X-axis (increasing width)
        glm::vec3 currentStepSize = stepSize;
        currentStepSize.x = stepSize.x + i * 0.5f; // Increase width gradually

        // Apply transformation for the current step
        model = glm::translate(model, stepPosition);

        // After scaling, move the step to the left by half of its width to keep it centered
        model = glm::translate(model, glm::vec3(-currentStepSize.x * 0.5f, 0.0f, 0.0f));

        // Now scale to the size of the step
        model = glm::scale(model, currentStepSize); // Scale to the size of the step

        // Render the step
        lightEffect(cubeVAO, shader, model, color);
    }
}

void drawStairsWithFences(
    unsigned int cubeVAO,
    Shader& shader,
    glm::vec3 startPosition,
    glm::vec3 stepSize,
    int numSteps,
    glm::vec3 color,
    glm::vec3 fenceSize, // New parameter for the fence size
    glm::vec3 fenceOffset // New parameter for the horizontal offset from the stairs (left/right)
) {
    for (int i = 0; i < numSteps; ++i) {
        glm::mat4 model = glm::mat4(1.0f);

        // Reverse the position for the current step
        glm::vec3 stepPosition = startPosition;
        stepPosition -= glm::vec3(0.0f, i * stepSize.y, -i * stepSize.z); // Move down and back

        // Scale the step width on the X-axis (increasing width)
        glm::vec3 currentStepSize = stepSize;
        currentStepSize.x = stepSize.x + i * 0.2f; // Increase width gradually

        // Apply transformation for the current step
        model = glm::translate(model, stepPosition);

        // After scaling, move the step to the left by half of its width to keep it centered
        model = glm::translate(model, glm::vec3(-currentStepSize.x * 0.5f, 0.0f, 0.0f));

        // Now scale to the size of the step
        model = glm::scale(model, currentStepSize); // Scale to the size of the step

        // Render the step
        lightEffect(cubeVAO, shader, model, color);

        // Now add the fences on both sides of the step
        glm::mat4 fenceModel = glm::mat4(1.0f);

        // Left side fence (shifted to the left of the step)
        fenceModel = glm::translate(fenceModel, stepPosition);
        fenceModel = glm::translate(fenceModel, glm::vec3(-currentStepSize.x * 0.5f - fenceOffset.x, 0.0f, 0.0f)); // Offset to the left
        fenceModel = glm::scale(fenceModel, fenceSize); // Scale the fence

        // Render the left fence
        lightEffect(cubeVAO, shader, fenceModel, color);

        // Right side fence (shifted to the right of the step)
        fenceModel = glm::mat4(1.0f); // Reset the model matrix
        fenceModel = glm::translate(fenceModel, stepPosition);
        fenceModel = glm::translate(fenceModel, glm::vec3(currentStepSize.x * 0.5f + fenceOffset.x, 0.0f, 0.0f)); // Offset to the right
        fenceModel = glm::scale(fenceModel, fenceSize); // Scale the fence

        // Render the right fence
        lightEffect(cubeVAO, shader, fenceModel, color);
    }
}

void drawSofa(glm::vec3 position, float rotationYDegrees, float uniformScale, Shader& shader, Cube& sofa) {
    // Convert rotation from degrees to radians
    float rotationY = glm::radians(rotationYDegrees);

    // Precompute trigonometric values for rotation
    float cosY = glm::cos(rotationY);
    float sinY = glm::sin(rotationY);

    // Sofa backrest
    glm::vec3 backrestPos = position;
    glm::mat4 model = transformation(
        backrestPos.x, backrestPos.y, backrestPos.z,
        0.0f, rotationYDegrees, 0.0f,
        1.2f * uniformScale, 0.5f * uniformScale, 3.0f * uniformScale);
    sofa.drawCubeWithTexture(shader, model);

    // Sofa left armrest
    glm::vec3 leftArmPos = position + glm::vec3(0.8f * cosY, 0.0f, -0.8f * sinY) * uniformScale;
    model = transformation(
        leftArmPos.x, leftArmPos.y, leftArmPos.z,
        0.0f, rotationYDegrees, 0.0f,
        0.4f * uniformScale, 1.2f * uniformScale, 3.0f * uniformScale);
    sofa.drawCubeWithTexture(shader, model);

    // Sofa seat
    glm::vec3 seatPos = position + glm::vec3(-0.2f * sinY, 0.0f, -0.2f * cosY) * uniformScale;
    model = transformation(
        seatPos.x, seatPos.y, seatPos.z,
        0.0f, rotationYDegrees, 0.0f,
        1.2f * uniformScale, 0.8f * uniformScale, 0.2f * uniformScale);
    sofa.drawCubeWithTexture(shader, model);

    // Sofa right armrest
    glm::vec3 rightArmPos = position + glm::vec3(-3.0f * sinY, 0.0f, 3.0f * cosY) * uniformScale;
    model = transformation(
        rightArmPos.x, rightArmPos.y, rightArmPos.z,
        0.0f, rotationYDegrees, 0.0f,
        1.2f * uniformScale, 0.8f * uniformScale, 0.2f * uniformScale);
    sofa.drawCubeWithTexture(shader, model);
}

