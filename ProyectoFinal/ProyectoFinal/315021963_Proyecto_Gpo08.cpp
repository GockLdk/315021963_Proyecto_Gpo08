#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void animacion();
void animPuertas();
void animKeyFrame();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 1.0f, 4.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 PosIni(-6.13f, -0.5f, 4.55f);
bool active = true;
bool luces = true;

// Variables animaciones
float rotPuerta = 0;
bool abiertaPuerta = false;
float movCristal = 0;
bool abiertaCristal = false;

// Positions of the point lights
// Indicar en el lighting.frag el num de point ligths y agregar sus posiciones aqui

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,5.0f,0.0f)
	/*glm::vec3(-3.15f,6.3f,10.79f),
	glm::vec3(9.33f,6.3f,10.79f),
	glm::vec3(0.0f,0.0f,0.0f),*/
};

glm::vec3 Light1 = glm::vec3(0);
//glm::vec3 Light2 = glm::vec3(0);
//glm::vec3 Light3 = glm::vec3(0);

// Cuadrado para referencia de las luces
//float vertices[] = {
//	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
//
//	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
//
//	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
//	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
//
//		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
//		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
//
//	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
//	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
//
//	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
//		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
//		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
//	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
//};



// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Keyframes
float posX = PosIni.x, posY = PosIni.y, posZ = PosIni.z, rotRodIzq = 0;

#define MAX_FRAMES 9
int i_max_steps = 50;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float incX;		//Variable para IncrementoX
	float incY;		//Variable para IncrementoY
	float incZ;		//Variable para IncrementoZ
	float rotRodIzq;
	float rotInc;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 2;			//introducir datos
bool play = false;
int playIndex = 0;



//glm::vec3 LightP1;


void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;

}

void interpolation(void)
{

	KeyFrame[playIndex].incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	KeyFrame[playIndex].incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	KeyFrame[playIndex].incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	KeyFrame[playIndex].rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;

}

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Saitama's House", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	// Setup and compile our shaders
	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");

	Model Casa ((char*)"Models/Casa/Casa.obj");
	Model puerta ((char*)"Models/Casa/Puerta2.obj");
	Model marcoVen ((char*)"Models/Casa/MarcoVen.obj");
	Model cristalVen ((char*)"Models/Casa/CristalVen.obj");
	Model puertaCrisA ((char*)"Models/Casa/PuertaCrisA.obj");
	Model puertaMarA ((char*)"Models/Casa/PuertaMarcA.obj");
	Model puertaCrisC ((char*)"Models/Casa/PuertaCrisC.obj");
	Model puertaMarC ((char*)"Models/Casa/PuertaMarcC.obj");
	Model cerca((char*)"Models/Casa/fenceFinal.obj");

	Model mesa ((char*)"Models/Mesa/Mesa.obj");
	Model tapete ((char*)"Models/Tapete/Tapete.obj");
	Model escritorio ((char*)"Models/Escritorio/Escritorio.obj");
	Model silla ((char*)"Models/Silla/Silla.obj");
	Model laptop ((char*)"Models/Laptop/Laptop.obj");
	Model cama ((char*)"Models/Cama/Cama.obj");
	Model muebleTV ((char*)"Models/MuebleTV/Cabinet.obj");
	Model tv ((char*)"Models/TV/TV.obj");
	Model controlTV ((char*)"Models/TV/Control.obj");

	/*Model Esfera((char*)"Models/Esfera/Esfera.obj");*/


	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].incX = 0;
		KeyFrame[i].incY = 0;
		KeyFrame[i].incZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].rotInc = 0;
	}
	// Fuargando animacion KEYFRAMES
	KeyFrame[0].posX = posX;
	KeyFrame[0].posY = posY;
	KeyFrame[0].posZ = posZ;
	KeyFrame[0].rotRodIzq = rotRodIzq;

	// Fuargando animacion KEYFRAMES
	KeyFrame[1].posX = -4.13f;
	KeyFrame[1].posY = -0.5f;
	KeyFrame[1].posZ = 3.0f;
	KeyFrame[1].rotRodIzq = 135.0f;

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		animacion();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);



		//Load Model
	    // Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.4f, 0.4f, 0.4f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 1.0f, 1.0f, 1.0f);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = Light1.x;
		lightColor.y = Light1.y;
		lightColor.z =  Light1.z;

		/*glm::vec3 lightColor2;
		lightColor2.x = Light2.x;
		lightColor2.y = Light2.y;
		lightColor2.z = Light2.z;

		glm::vec3 lightColor3;
		lightColor3.x = Light3.x;
		lightColor3.y = Light3.y;
		lightColor3.z = Light3.z;*/

		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);

		//// Point Light 2
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor2.x, lightColor2.y, lightColor2.z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor2.x, lightColor2.y, lightColor2.z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 0.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);
		//
		//// Point Light 3
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor3.x, lightColor3.y, lightColor3.z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor3.x, lightColor3.y, lightColor3.z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 0.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);


		
		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"),1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.32f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.44f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.5f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(15.0f)));

		

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);

		//Carga de modelo 
		view = camera.GetViewMatrix();

		// ********************** CASA **********************
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		Casa.Draw(lightingShader);

		// Esfera con Transparencia
		//glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		//glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 0.0, 0.75);
		//Esfera.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		//// Para que no cambie el color el canal alpha a los demas objetos
		//glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);

		// Puerta Entrada
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-7.16f, 3.05f, 9.6f));
		model = glm::rotate(model, glm::radians(rotPuerta),glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		puerta.Draw(lightingShader);

		// Ventana
		// Marco
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		marcoVen.Draw(lightingShader);
		// Cristal
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.5);
		cristalVen.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		// Para que no cambie el color el canal alpha a los demas objetos
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);
		
		// Puerta de Cristal
		// Marco que se queda cerrado
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.025f, -0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		puertaMarC.Draw(lightingShader);
		// Cristal que se queda cerrado
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.025f, -0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.5);
		puertaCrisC.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		// Para que no cambie el color el canal alpha a los demas objetos
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);
		// Marco que se abre
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.025f, -0.5f, 0.0f));
		model = glm::translate(model, glm::vec3(movCristal, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		puertaMarA.Draw(lightingShader);
		// Cristal que se abre
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.025f, -0.5f, 0.0f));
		model = glm::translate(model, glm::vec3(movCristal, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1);
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.5);
		puertaCrisA.Draw(lightingShader);
		glDisable(GL_BLEND);  //Desactiva el canal alfa 
		// Para que no cambie el color el canal alpha a los demas objetos
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);
		// ********************** CASA FIN **********************

		// Cerca
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.025f, -0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		cerca.Draw(lightingShader);


		// Mesa
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.025f, -0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		mesa.Draw(lightingShader);
		// Tapete
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.025f, -0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		tapete.Draw(lightingShader);
		// Escritorio
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.025f, -0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		escritorio.Draw(lightingShader);
		// Silla
		model = glm::mat4(1);
		//model = glm::translate(model, glm::vec3(0.025f, -0.5f, 0.0f));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		model = glm::rotate(model, glm::radians(rotRodIzq), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		silla.Draw(lightingShader);
		// Laptop
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.025f, -0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		laptop.Draw(lightingShader);
		// Cama
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.025f, -0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		cama.Draw(lightingShader);
		// Mueble TV
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.025f, -0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		muebleTV.Draw(lightingShader);
		// TV
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.025f, -0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		tv.Draw(lightingShader);
		// Control TV
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.025f, -0.5f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
		controlTV.Draw(lightingShader);
		
		glBindVertexArray(0);


		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}


	

	

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		
		active = !active;
	}

	if (keys[GLFW_KEY_L])
	{
		luces = !luces;
		// Luz
		if (luces)
		{
			Light1 = glm::vec3(1.0f, 1.0f, 1.0f);
			/*Light2 = glm::vec3(1.0f, 1.0f, 0.0f);
			Light3 = glm::vec3(1.0f, 1.0f, 0.0f);*/
			
		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
			/*Light2 = glm::vec3(0);
			Light3 = glm::vec3(0);*/
		}
	}
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}

// Funcion que controla todas las animaciones
void animacion()
{
	if (active)
	{
		animPuertas();
		animKeyFrame();
	}

	
	
}

// Animacion de la puerta de entrada
void animPuertas()
{
	// Puerta Entrada
	if (rotPuerta > -90 && !abiertaPuerta)
		rotPuerta -= 1.0f;
	else
		abiertaPuerta = true;
	if (rotPuerta <= 0 && abiertaPuerta)
		rotPuerta += 1.0f;
	else
		abiertaPuerta = false;

	// Puerta Cristal
	if (movCristal > -5.0 && !abiertaCristal)
		movCristal -= 0.1f;
	else
		abiertaCristal = true;
	if (movCristal < -0.1 && abiertaCristal)
		movCristal += 0.1f;
	else
		abiertaCristal = false;
}

void animKeyFrame()
{
	if (play == false && (FrameIndex > 1))
	{

		resetElements();
		//First Interpolation				
		interpolation();
		
		play = true;
		playIndex = 0;
		i_curr_steps = 0;
	}
	if (play)
	{
		
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				printf("Anim KeyFrame Silla\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += KeyFrame[playIndex].incX;
			posY += KeyFrame[playIndex].incY;
			posZ += KeyFrame[playIndex].incZ;

			rotRodIzq += KeyFrame[playIndex].rotInc;

			i_curr_steps++;
		}

	}
}