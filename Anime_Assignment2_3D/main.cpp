#define GLEW_STATIC
#include <iostream>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <math.h>
#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

#include "Shader.h"
#include "Camera.h"
#include "Material.h"
#include "LightDirectional.h"
//#include "LightPoint.h"
//#include "LightSpot.h"
#include "Mesh.h"
#include "Model.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

int screenWidth = 1400;
int screenHeight = 1000;
int cursorPosX = screenWidth / 2;
int cursorPosY = screenHeight / 2;


glm::vec3 cube_position(0.0f, 0.0f, 0.0f);
float cube_x = 3.0;
float cube_y = 0.0;
float cube_z = 0.0;


#pragma region Model Data
float vertices_ground[] = {
	// positions          // normals           // texture coords
	-0.5f, -0.0f, -0.5f,   0.0f, 1.0f, 0.0f,   100.0f, 100.0f,   // top right
	 0.5f, -0.0f, -0.5f,   0.0f, 1.0f, 0.0f,   100.0f, 0.0f,   // bottom right
	 0.5f, -0.0f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 0.0f,   // bottom left
	-0.5f, -0.0f,  0.5f,   0.0f, 1.0f, 0.0f,   0.0f, 100.0f    // top left 
};

unsigned int indices_ground[] = {
	0, 1, 2,
	2, 3, 0
};

float cubeVertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

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
#pragma endregion

#pragma region Light Declaration
LightDirectional lightD(glm::vec3(10.0f, 30.0f, 20.0f), glm::vec3(glm::radians(110.0f), glm::radians(30.0f), 0), glm::vec3(1.0f, 0.95f, 0.8f));
#pragma endregion

#pragma region Camera Declaration
// Initialize camera class
//Camera camera(glm::vec3(0, 0, 3.0f), glm::vec3(0, -1.0f, 0), glm::vec3(0, 1.0f, 0));
Camera camera(glm::vec3(0.0f, 5.0f, 20.0f), glm::radians(0.0f), glm::radians(-90.0f), glm::vec3(0, 1.0f, 0));
#pragma endregion

#pragma region Texture Loading Function
unsigned int LoadTextureImageToGPU(const char* filename, GLint internalFormat, GLenum format, int textureSlot)
{
	unsigned int TexBuffer;
	glGenTextures(1, &TexBuffer);

	glActiveTexture(GL_TEXTURE0 + textureSlot);
	glBindTexture(GL_TEXTURE_2D, TexBuffer);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16.0f);

	int width, height, nrChannel;
	unsigned char *data = stbi_load(filename, &width, &height, &nrChannel, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		cout << "Texture image " << filename << " load failed." << endl;
	}
	stbi_image_free(data);

	//cout << filename << "Slot:  " << TexBuffer << endl;

	return TexBuffer;
}
#pragma endregion

#pragma region Camera Operation Funtions
double previous_xPos, previous_yPos;
bool first_initialise_mouse = true;

void processInput_camera(GLFWwindow* window)
{

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.SpeedZ = 1.0;
	}
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.SpeedZ = -1.0;
	}
	else
	{
		camera.SpeedZ = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.SpeedX = 1.0;
	}
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.SpeedX = -1.0;
	}
	else
	{
		camera.SpeedX = 0;
	}

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
	{
		camera.SpeedY = 1.0;
	}
	else if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		camera.SpeedY = -1.0;
	}
	else
	{
		camera.SpeedY = 0;
	}
}

void mouse_callback_camera(GLFWwindow* window, double xPos, double yPos)
{
	if (first_initialise_mouse == true)
	{
		previous_xPos = xPos;
		previous_yPos = yPos;
		first_initialise_mouse = false;
	}

	double Delta_x = xPos - previous_xPos;
	double Delta_y = yPos - previous_yPos;

	previous_xPos = xPos;
	previous_yPos = yPos;

	cursorPosX = xPos;
	cursorPosY = yPos;
	//cout << Delta_x << "   " << Delta_y << endl;
	//cout << xPos << "   " << yPos << endl;
	camera.ProcessMouseMovement(Delta_x, Delta_y);
}
#pragma endregion

#pragma region Move Cube
void processInput_cube(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
		//cube_position += 0.1f * camera.Forward;
		cube_position += 0.1f * glm::vec3(0.0f, 0.0f, -1.0f);
	}
	else if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		//cube_position += -0.1f * camera.Forward;
		cube_position += -0.1f * glm::vec3(0.0f, 0.0f, -1.0f);
	}


	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		//cube_position += 0.1f * camera.Right;
		cube_position += 0.1f * glm::vec3(1.0f, 0.0f, 0.0f);
	}
	else if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	{
		//cube_position += -0.1f * camera.Right;
		cube_position += -0.1f * glm::vec3(1.0f, 0.0f, 0.0f);
	}


	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
	{
		//cube_position += 0.1f * camera.Up;
		cube_position += 0.1f * glm::vec3(0.0f, 1.0f, 0.0f);
	}
	else if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	{
		//cube_position += -0.1f * camera.Up;
		cube_position += -0.1f * glm::vec3(0.0f, 1.0f, 0.0f);
	}

}
#pragma endregion

#pragma region Load Sky Box

unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
			std::cout << "Cubemap tex succeeded to load at path: " << faces[i] << std::endl;
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

#pragma endregion

#pragma region CCD Loop Over Condition
bool CCDoverCondition(glm::vec3 _target_position, glm::vec3 _limb1_position, float _humanchain_length, glm::vec3 _humanchain_endPosition)
{
	bool over = false;

	if (glm::length(_target_position - _limb1_position) <= 0.95 * _humanchain_length)
	{
		if (_humanchain_length && glm::length(_target_position - _humanchain_endPosition) < 0.1)
			over = true;
	}
	else if (glm::length(_target_position - _limb1_position) <= _humanchain_length)
	{
		if (_humanchain_length && glm::length(_target_position - _humanchain_endPosition) < 0.2)
			over = true;
	}
	else
	{
		glm::vec3 p2e = _humanchain_endPosition - _limb1_position;
		glm::vec3 p2t = _target_position - _limb1_position;
		glm::vec3 e2t = _target_position - _humanchain_endPosition;
		float angle = std::acos(glm::dot(glm::normalize(p2e), glm::normalize(p2t)));

		if (angle < 0.01 && glm::length(e2t) < 0.5)
			over = true;
	}

	return over;
}
#pragma endregion

int main(int argc, char* argv[])
{
	std::string exePath = argv[0];

#pragma region Open a Window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "My openGL game", NULL, NULL);

	if (window == NULL)
	{
		cout << "Open window failed." << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		cout << "Init GlEW failed." << endl;
		glfwTerminate();
		return -1;
	}

	glfwSetCursorPosCallback(window, mouse_callback_camera);
	//glfwSetCursorPosCallback(window, mouse_callback_camera_ThirdPersonView);
	//glfwSetScrollCallback(window, scroll_callback_camera_ThirdPersonView);

	//glViewport(0, 0, 800, 600);
	glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#pragma endregion

#pragma region Init Shader Program
	Shader* shader_ground = new Shader("VertexShader_ground.vert", "FragmentShader_ground.frag");
	Shader* shader_blinnPhong = new Shader("VertexShader_blinnPhong.vert", "FragmentShader_blinnPhong.frag");
	Shader* shader_cube = new Shader("VertexShader_cube.vert", "FragmentShader_cube.frag");
	Shader* shader_skybox = new Shader("VertexShader_skybox.vert", "FragmentShader_skybox.frag");
#pragma endregion 

#pragma region Pass light to shaders;
	shader_blinnPhong->use();
	shader_blinnPhong->SetUniform3f("lightD.pos", glm::vec3(lightD.position.x, lightD.position.y, lightD.position.z));
	shader_blinnPhong->SetUniform3f("lightD.color", glm::vec3(lightD.color.x, lightD.color.y, lightD.color.z));
	shader_blinnPhong->SetUniform3f("lightD.dirToLight", glm::vec3(lightD.direction.x, lightD.direction.y, lightD.direction.z));

	shader_ground->use();
	shader_ground->SetUniform3f("lightD.pos", glm::vec3(lightD.position.x, lightD.position.y, lightD.position.z));
	shader_ground->SetUniform3f("lightD.color", glm::vec3(lightD.color.x, lightD.color.y, lightD.color.z));
	shader_ground->SetUniform3f("lightD.dirToLight", glm::vec3(lightD.direction.x, lightD.direction.y, lightD.direction.z));

#pragma endregion

#pragma region Load Models

	Model bunny(exePath.substr(0, exePath.find_last_of('\\')) + "\\bunny\\bunny.obj");
	Model bone(exePath.substr(0, exePath.find_last_of('\\')) + "\\bone\\bone.obj");
	Model cube(exePath.substr(0, exePath.find_last_of('\\')) + "\\cube\\cube.obj");
	Model human(exePath.substr(0, exePath.find_last_of('\\')) + "\\human\\human.obj");
	Model human_limb(exePath.substr(0, exePath.find_last_of('\\')) + "\\human\\limb\\limb.obj");
	Model human_finger(exePath.substr(0, exePath.find_last_of('\\')) + "\\human\\finger\\finger.obj");

	unsigned int VBO_ground;
	glGenBuffers(1, &VBO_ground);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_ground);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_ground), vertices_ground, GL_STATIC_DRAW);

	unsigned int VAO_ground;
	glGenVertexArrays(1, &VAO_ground);
	glBindVertexArray(VAO_ground);

	unsigned int EBO_ground;
	glGenBuffers(1, &EBO_ground);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ground);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_ground), indices_ground, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

	// cube VAO VBO
	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
#pragma endregion

#pragma region Init and Load Texture;
	stbi_set_flip_vertically_on_load(true);
	unsigned int TexBuffer_ground;
	TexBuffer_ground = LoadTextureImageToGPU("ground.jpg", GL_RGB, GL_RGB, 0);
	//unsigned int TexBufferB;
	//TexBufferB = LoadTextureImageToGPU("awesomeface.png", GL_RGBA, GL_RGBA, 1);
#pragma endregion

#pragma region Init Skybox Texture;
	vector<std::string> faces
	{
		exePath.substr(0, exePath.find_last_of('\\')) + "\\skybox\\dusk\\posx.jpg",
		exePath.substr(0, exePath.find_last_of('\\')) + "\\skybox\\dusk\\negx.jpg",
		exePath.substr(0, exePath.find_last_of('\\')) + "\\skybox\\dusk\\posy.jpg",
		exePath.substr(0, exePath.find_last_of('\\')) + "\\skybox\\dusk\\negy.jpg",
		exePath.substr(0, exePath.find_last_of('\\')) + "\\skybox\\dusk\\posz.jpg",
		exePath.substr(0, exePath.find_last_of('\\')) + "\\skybox\\dusk\\negz.jpg"
	};
	stbi_set_flip_vertically_on_load(false);
	unsigned int cubemapTexture = loadCubemap(faces);
#pragma endregion

#pragma region Prepare MVP matrices
	//glm::mat4 scale;  glm::scale(transMatrix, glm::vec3(100.0f, 100.0f, 100.0f));
	glm::mat4 boneScale;
	glm::mat4 trans;
	glm::mat4 modelMat;
	//modelMat = glm::rotate(modelMat, glm::radians(0.0f), glm::vec3(1.0f, 0, 0));
	glm::mat4 viewMat;
	glm::mat4 projMat;
	projMat = glm::perspective(glm::radians(45.0f), 1400.0f / 1000.0f, 0.1f, 300.0f);
	//projMat = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -5.0f, 5.0f);
#pragma endregion

#pragma region Preparation for IK
	glm::vec3 rotAxis;
	glm::quat rot;

	glm::vec3 limb1_position = glm::vec3(0.5f, 3.8f, 0.0f);
	glm::vec3 limb2_position = glm::vec3(0.0f);
	glm::vec3 limb3_position = glm::vec3(0.0f);
	glm::vec3 finger1_position = glm::vec3(0.0f);
	glm::vec3 humanchain_endPosition = glm::vec3(0.0f);
	float limb1_length = 1.05;
	float limb2_length = 1.05;
	float limb3_length = 1.25;
	float finger1_length = 1.0;
	float humanchain_length = 4.35;

	glm::quat limb1_quat = glm::angleAxis(glm::radians(0.1f), glm::vec3(1., 0., 0.));
	glm::quat limb2_quat = glm::angleAxis(glm::radians(0.1f), glm::vec3(1., 0., 0.));
	glm::quat limb3_quat = glm::angleAxis(glm::radians(-0.1f), glm::vec3(1., 0., 0.));
	glm::quat finger1_quat = glm::angleAxis(glm::radians(-0.1f), glm::vec3(1., 0., 0.));
#pragma endregion

#pragma region Preparation for Cubic Spline
	glm::mat4 B(
		0.0f, 1.0f, 8.0f, 27.0f,
		0.0f, 1.0f, 4.0f, 9.0f,
		0.0f, 1.0f, 2.0f, 3.0f,
		1.0f, 1.0f, 1.0f, 1.0f
	);
	B = glm::inverse(glm::transpose(B)); // transpose is because the order in Opengl is already transposed

	glm::vec4 spline_p1(1.0f, 5.5f, -1.0f, 0.0f);
	glm::vec4 spline_p2(2.0f, 3.f, 1.0f, 0.0f);
	glm::vec4 spline_p3(3.0f, 5.5f, -1.0f, 0.0f);
	glm::vec4 spline_p4(4.0f, 3.0f, 0.5f, 0.0f);
	glm::mat4 G(spline_p1, spline_p2, spline_p3, spline_p4);

	std::vector<glm::vec3> splinePoints;

	for (int i = 0; i <= 30; i++)
	{
		//float time = 0.1 * i;
		float time = (std::atan(5 * (0.1 * i - 1.5)) + M_PI / 2) / M_PI * 3.0;
		glm::vec4 t(std::pow(time, 3), std::pow(time, 2), time, 1);
		glm::vec3 p = glm::vec3(G * B * t);
		splinePoints.push_back(p);

		std::cout << p.x << " " << p.y << " " << p.z << std::endl;
	}

	for (int i = 29; i >= 0; i--)
	{
		//float time = 0.1 * i;
		float time = (std::atan(5 * (0.1 * i - 1.5)) + M_PI / 2) / M_PI * 3.0;
		glm::vec4 t(std::pow(time, 3), std::pow(time, 2), time, 1);
		glm::vec3 p = glm::vec3(G * B * t);
		splinePoints.push_back(p);

		std::cout << p.x << " " << p.y << " " << p.z << std::endl;
	}
#pragma endregion


	// Start drawing
	while (!glfwWindowShouldClose(window))
	{
		// Mouse Action
		processInput_camera(window);
		// Move the cube
		processInput_cube(window);
		// Clear Screen
		glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, screenWidth, screenHeight);
		viewMat = camera.GetViewMatrix();

#pragma region Draw ground
		// Ground
		shader_ground->use();
		trans = glm::scale(glm::mat4(1.0f), glm::vec3(1000.0f, 1000.0f, 1000.0f));
		modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		shader_ground->SetMatrix("transform", trans);
		shader_ground->SetMatrix("modelMat", modelMat);
		shader_ground->SetMatrix("viewMat", viewMat);
		shader_ground->SetMatrix("projMat", projMat);
		shader_ground->SetUniform3f("viewPos", camera.Position);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TexBuffer_ground);
		shader_ground->SetUniform1i("groundTexture_diffuse", 0);

		glBindVertexArray(VAO_ground);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_ground);
		glDrawElements(GL_TRIANGLES, sizeof(indices_ground), GL_UNSIGNED_INT, 0);
#pragma endregion

#pragma region Human, Limb and Finger Position
		float time = glfwGetTime();
		cube_position = splinePoints[int(time * 10) % 61];

		int counter = 5000;
		float clip_angle = 0.05;

		trans = glm::toMat4(limb1_quat);
		limb2_position = glm::mat3(trans) * glm::vec3(limb1_length, 0.0f, 0.0f) + limb1_position;
		trans *= glm::toMat4(limb2_quat);
		limb3_position = glm::mat3(trans) * glm::vec3(limb2_length, 0.0f, 0.0f) + limb2_position;
		trans *= glm::toMat4(limb3_quat);
		finger1_position = glm::mat3(trans) * glm::vec3(limb3_length, 0.0f, 0.0f) + limb3_position;
		trans *= glm::toMat4(finger1_quat);
		humanchain_endPosition = glm::mat3(trans) * glm::vec3(finger1_length, 0.0f, 0.0f) + finger1_position;

		while (--counter)
		{
			if (CCDoverCondition(cube_position, limb1_position, humanchain_length, humanchain_endPosition)) 
				break;

			glm::vec3 p2e;
			glm::vec3 p2t;
			float angle;
			p2e = glm::normalize(humanchain_endPosition - finger1_position);
			p2t = glm::normalize(cube_position - finger1_position);
			angle = std::acos(std::max(std::min(glm::dot(p2e, p2t), 1.0f), -1.0f));

			if (angle >= 0.005)
			{
				if (angle > clip_angle) angle = clip_angle;

				rotAxis = glm::normalize(glm::cross(p2e, p2t));
				rot = glm::angleAxis(angle, rotAxis);
				finger1_quat = rot * finger1_quat;
	
				trans = glm::toMat4(limb1_quat);
				limb2_position = glm::mat3(trans) * glm::vec3(limb1_length, 0.0f, 0.0f) + limb1_position;
				trans *= glm::toMat4(limb2_quat);
				limb3_position = glm::mat3(trans) * glm::vec3(limb2_length, 0.0f, 0.0f) + limb2_position;
				trans *= glm::toMat4(limb3_quat);
				finger1_position = glm::mat3(trans) * glm::vec3(limb3_length, 0.0f, 0.0f) + limb3_position;
				trans *= glm::toMat4(finger1_quat);
				humanchain_endPosition = glm::mat3(trans) * glm::vec3(finger1_length, 0.0f, 0.0f) + finger1_position;
			}
			if (CCDoverCondition(cube_position, limb1_position, humanchain_length, humanchain_endPosition)) 
				break;

			p2e = glm::normalize(humanchain_endPosition - limb3_position);
			p2t = glm::normalize(cube_position - limb3_position);
			angle = std::acos(std::max(std::min(glm::dot(p2e, p2t), 1.0f), -1.0f));

			if (angle >= 0.005)
			{
				if (angle > clip_angle) angle = clip_angle;

				rotAxis = glm::normalize(glm::cross(p2e, p2t));
				rot = glm::angleAxis(angle, rotAxis);
				limb3_quat = rot * limb3_quat;

				trans = glm::toMat4(limb1_quat);
				limb2_position = glm::mat3(trans) * glm::vec3(limb1_length, 0.0f, 0.0f) + limb1_position;
				trans *= glm::toMat4(limb2_quat);
				limb3_position = glm::mat3(trans) * glm::vec3(limb2_length, 0.0f, 0.0f) + limb2_position;
				trans *= glm::toMat4(limb3_quat);
				finger1_position = glm::mat3(trans) * glm::vec3(limb3_length, 0.0f, 0.0f) + limb3_position;
				trans *= glm::toMat4(finger1_quat);
				humanchain_endPosition = glm::mat3(trans) * glm::vec3(finger1_length, 0.0f, 0.0f) + finger1_position;
			}
			if (CCDoverCondition(cube_position, limb1_position, humanchain_length, humanchain_endPosition)) 
				break;

			p2e = glm::normalize(humanchain_endPosition - limb2_position);
			p2t = glm::normalize(cube_position - limb2_position);
			angle = std::acos(std::max(std::min(glm::dot(p2e, p2t), 1.0f), -1.0f));

			if (angle >= 0.005)
			{
				if (angle > clip_angle) angle = clip_angle;

				rotAxis = glm::normalize(glm::cross(p2e, p2t));
				rot = glm::angleAxis(angle, rotAxis);
				limb2_quat = rot * limb2_quat;

				trans = glm::toMat4(limb1_quat);
				limb2_position = glm::mat3(trans) * glm::vec3(limb1_length, 0.0f, 0.0f) + limb1_position;
				trans *= glm::toMat4(limb2_quat);
				limb3_position = glm::mat3(trans) * glm::vec3(limb2_length, 0.0f, 0.0f) + limb2_position;
				trans *= glm::toMat4(limb3_quat);
				finger1_position = glm::mat3(trans) * glm::vec3(limb3_length, 0.0f, 0.0f) + limb3_position;
				trans *= glm::toMat4(finger1_quat);
				humanchain_endPosition = glm::mat3(trans) * glm::vec3(finger1_length, 0.0f, 0.0f) + finger1_position;
			}
			if (CCDoverCondition(cube_position, limb1_position, humanchain_length, humanchain_endPosition)) 
				break;

			p2e = glm::normalize(humanchain_endPosition - limb1_position);
			p2t = glm::normalize(cube_position - limb1_position);
			angle = std::acos(std::max(std::min(glm::dot(p2e, p2t), 1.0f), -1.0f));

			if (angle >= 0.005)
			{
				if (angle > clip_angle) angle = clip_angle;

				rotAxis = glm::normalize(glm::cross(p2e, p2t));
				rot = glm::angleAxis(angle, rotAxis);
				limb1_quat = rot * limb1_quat;

				trans = glm::toMat4(limb1_quat);
				limb2_position = glm::mat3(trans) * glm::vec3(limb1_length, 0.0f, 0.0f) + limb1_position;
				trans *= glm::toMat4(limb2_quat);
				limb3_position = glm::mat3(trans) * glm::vec3(limb2_length, 0.0f, 0.0f) + limb2_position;
				trans *= glm::toMat4(limb3_quat);
				finger1_position = glm::mat3(trans) * glm::vec3(limb3_length, 0.0f, 0.0f) + limb3_position;
				trans *= glm::toMat4(finger1_quat);
				humanchain_endPosition = glm::mat3(trans) * glm::vec3(finger1_length, 0.0f, 0.0f) + finger1_position;
			}
		}

		shader_blinnPhong->use();
		trans = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
		modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		shader_blinnPhong->SetMatrix("transform", trans);
		shader_blinnPhong->SetMatrix("modelMat", modelMat);
		shader_blinnPhong->SetMatrix("viewMat", viewMat);
		shader_blinnPhong->SetMatrix("projMat", projMat);
		shader_ground->SetUniform3f("viewPos", camera.Position);
		human.Draw(shader_blinnPhong);

		trans = glm::toMat4(limb1_quat);
		modelMat = glm::translate(glm::mat4(1.0f), limb1_position);
		shader_blinnPhong->SetMatrix("transform", trans);
		shader_blinnPhong->SetMatrix("modelMat", modelMat);
		shader_blinnPhong->SetMatrix("viewMat", viewMat);
		shader_blinnPhong->SetMatrix("projMat", projMat);
		shader_ground->SetUniform3f("viewPos", camera.Position);
		human_limb.Draw(shader_blinnPhong);

		trans *= glm::toMat4(limb2_quat);
		modelMat = glm::translate(glm::mat4(1.0f), limb2_position);
		shader_blinnPhong->SetMatrix("transform", trans);
		shader_blinnPhong->SetMatrix("modelMat", modelMat);
		human_limb.Draw(shader_blinnPhong);

		trans *= glm::toMat4(limb3_quat);
		modelMat = glm::translate(glm::mat4(1.0f), limb3_position);
		shader_blinnPhong->SetMatrix("transform", trans);
		shader_blinnPhong->SetMatrix("modelMat", modelMat);
		human_limb.Draw(shader_blinnPhong);

		glm::vec3 finger_z = glm::mat3(trans) * glm::vec3(0.0f, 0.0f, 1.0f);
		rot = glm::angleAxis(glm::radians(45.0f), finger_z);
		glm::quat subfinger_quat = rot * finger1_quat;
		glm::mat4 trans2 = trans * glm::toMat4(subfinger_quat);
		modelMat = glm::translate(glm::mat4(1.0f), finger1_position);
		shader_blinnPhong->SetMatrix("transform", trans2);
		shader_blinnPhong->SetMatrix("modelMat", modelMat);
		human_finger.Draw(shader_blinnPhong);

		rot = glm::angleAxis(glm::radians(-45.0f), finger_z);
		subfinger_quat = rot * finger1_quat;
		trans2 = trans * glm::toMat4(subfinger_quat);
		modelMat = glm::translate(glm::mat4(1.0f), finger1_position);
		shader_blinnPhong->SetMatrix("transform", trans2);
		shader_blinnPhong->SetMatrix("modelMat", modelMat);
		human_finger.Draw(shader_blinnPhong);

		trans *= glm::toMat4(finger1_quat);
		modelMat = glm::translate(glm::mat4(1.0f), finger1_position);
		shader_blinnPhong->SetMatrix("transform", trans);
		shader_blinnPhong->SetMatrix("modelMat", modelMat);
		shader_blinnPhong->SetMatrix("viewMat", viewMat);
		shader_blinnPhong->SetMatrix("projMat", projMat);
		shader_ground->SetUniform3f("viewPos", camera.Position);
		human_finger.Draw(shader_blinnPhong);
		
#pragma endregion

#pragma region Draw Cube (target)
		shader_cube->use();
		trans = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f, 0.1f, 0.1f));
		modelMat = glm::translate(glm::mat4(1.0f), cube_position);
		shader_cube->SetMatrix("transform", trans);
		shader_cube->SetMatrix("modelMat", modelMat);
		shader_cube->SetMatrix("viewMat", viewMat);
		shader_cube->SetMatrix("projMat", projMat);
		cube.Draw(shader_cube);
#pragma endregion


#pragma region Draw Skybox

		glDepthFunc(GL_LEQUAL);
		shader_skybox->use();
		viewMat = glm::mat4(glm::mat3(viewMat)); // remove translation from the view matrix
		modelMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.04f, 0.0f));

		shader_skybox->SetMatrix("modelMat", modelMat);
		shader_skybox->SetMatrix("viewMat", viewMat);
		shader_skybox->SetMatrix("projMat", projMat);
		shader_skybox->SetUniform1i("skybox", 0);

		glBindVertexArray(cubeVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);

#pragma endregion

		// Clean up, prepare for next render loop
		glfwSwapBuffers(window);
		glfwPollEvents();
		camera.UpdateCameraPos();
	}


}
