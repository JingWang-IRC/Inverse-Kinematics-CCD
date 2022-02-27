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

int screenWidth = 1001;
int screenHeight = 1001;
int cursorPosX = screenWidth / 2;
int cursorPosY = screenHeight / 2;

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
#pragma endregion

#pragma region Light Declaration
LightDirectional lightD(glm::vec3(10.0f, 30.0f, 20.0f), glm::vec3(glm::radians(110.0f), glm::radians(30.0f), 0), glm::vec3(1.0f, 0.95f, 0.8f));
#pragma endregion

#pragma region Camera Declaration
// Initialize camera class
//Camera camera(glm::vec3(0, 0, 3.0f), glm::vec3(0, -1.0f, 0), glm::vec3(0, 1.0f, 0));
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::radians(0.0f), glm::radians(-90.0f), glm::vec3(0, 1.0f, 0));
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
	//camera.ProcessMouseMovement(Delta_x, Delta_y);
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
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
#pragma endregion 

#pragma region Pass light to shaders;
	shader_blinnPhong->use();
	shader_blinnPhong->SetUniform3f("lightD.pos", glm::vec3(lightD.position.x, lightD.position.y, lightD.position.z));
	shader_blinnPhong->SetUniform3f("lightD.color", glm::vec3(lightD.color.x, lightD.color.y, lightD.color.z));
	shader_blinnPhong->SetUniform3f("lightD.dirToLight", glm::vec3(lightD.direction.x, lightD.direction.y, lightD.direction.z));
#pragma endregion

#pragma region Load Models

	Model bunny(exePath.substr(0, exePath.find_last_of('\\')) + "\\bunny\\bunny.obj");
	Model bone(exePath.substr(0, exePath.find_last_of('\\')) + "\\bone\\bone.obj");
	Model cube(exePath.substr(0, exePath.find_last_of('\\')) + "\\cube\\cube.obj");

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
#pragma endregion

#pragma region Init and Load Texture;
	stbi_set_flip_vertically_on_load(true);
	//unsigned int TexBuffer_ground;
	//TexBuffer_ground = LoadTextureImageToGPU("ground.jpg", GL_RGB, GL_RGB, 0);
	//unsigned int TexBufferB;
	//TexBufferB = LoadTextureImageToGPU("awesomeface.png", GL_RGBA, GL_RGBA, 1);
#pragma endregion

#pragma region Prepare MVP matrices
	//glm::mat4 scale;  glm::scale(transMatrix, glm::vec3(100.0f, 100.0f, 100.0f));
	glm::mat4 boneScale;
	glm::mat4 trans;
	glm::mat4 modelMat;
	//modelMat = glm::rotate(modelMat, glm::radians(0.0f), glm::vec3(1.0f, 0, 0));
	glm::mat4 viewMat;
	glm::mat4 projMat;
	//projMat = glm::perspective(glm::radians(45.0f), 1400.0f / 1000.0f, 0.1f, 300.0f);
	projMat = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -5.0f, 5.0f);
#pragma endregion

	glm::vec3 bone1_position = glm::vec3(0.0f);
	glm::vec3 bone2_position = glm::vec3(0.0f);
	glm::vec3 bone3_position = glm::vec3(0.0f);
	glm::vec3 bone4_position = glm::vec3(0.0f);
	glm::vec3 bone5_position = glm::vec3(0.0f);
	glm::vec3 bonechain_endPosition = glm::vec3(0.0f);
	float bone1_length = 1.5;
	float bone2_length = 1.5;
	float bone3_length = 1.5;
	float bone4_length = 1.5;
	float bone5_length = 1.5;

	glm::quat bone1_quat = glm::angleAxis(glm::radians(0.0f), glm::vec3(1., 0., 0.));
	glm::quat bone2_quat = glm::angleAxis(glm::radians(0.0f), glm::vec3(1., 0., 0.));
	glm::quat bone3_quat = glm::angleAxis(glm::radians(0.0f), glm::vec3(1., 0., 0.));
	glm::quat bone4_quat = glm::angleAxis(glm::radians(0.0f), glm::vec3(1., 0., 0.));
	glm::quat bone5_quat = glm::angleAxis(glm::radians(0.0f), glm::vec3(1., 0., 0.));

	glm::quat rot1 = glm::angleAxis(glm::radians(1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::quat rot2 = glm::angleAxis(glm::radians(1.1f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::quat rot3 = glm::angleAxis(glm::radians(1.1f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::quat rot4 = glm::angleAxis(glm::radians(1.1f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::quat rot5 = glm::angleAxis(glm::radians(1.1f), glm::vec3(0.0f, 0.0f, 1.0f));
	bone1_quat = rot1 * bone1_quat;
	bone2_quat = rot2 * bone2_quat;
	bone3_quat = rot3 * bone3_quat;
	bone4_quat = rot4 * bone4_quat;
	bone5_quat = rot5 * bone5_quat;

	glm::vec3 rotAxis;
	glm::quat rot;

	// Start drawing
	while (!glfwWindowShouldClose(window))
	{
		// Mouse Action
		processInput_camera(window);
		
		// Cursor position to coordinate
		float positionX = (cursorPosX - (screenWidth / 2)) / float(screenWidth / 2) * 10.0f;
		float positionY = -(cursorPosY - (screenHeight / 2)) / float(screenHeight / 2) * 10.0f;

		// Clear Screen
		glClearColor(0.2f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glViewport(0, 0, screenWidth, screenHeight);
		viewMat = camera.GetViewMatrix();

#pragma region Draw Bone (analytic)

		glm::vec3 bone1_position = glm::vec3(0.0f, 0.0f, 0.0f);
		float bone1_length = 4;
		float bone2_length = 3;

		float angle_T;
		float angle_1;
		float bone1_angle;
		float bone2_angle;

		// when the target is over the distance the two bones can reach to
		if (std::sqrt(positionX * positionX + positionY * positionY) > (bone1_length + bone2_length))
		{
			if (positionY > 0) bone1_angle = std::acos(positionX / std::sqrt(positionX * positionX + positionY * positionY));
			else bone1_angle = -std::acos(positionX / std::sqrt(positionX * positionX + positionY * positionY));

			bone2_angle = 0;
		}
		// when the target is inside the area the two bones can reach to
		else if (std::sqrt(positionX * positionX + positionY * positionY) < std::abs(bone1_length - bone2_length))
		{
			if (positionY > 0) bone1_angle = std::acos(positionX / std::sqrt(positionX * positionX + positionY * positionY));
			else bone1_angle = -std::acos(positionX / std::sqrt(positionX * positionX + positionY * positionY));

			bone2_angle = M_PI;
		}
		// to avoid the bones flip when go beyond x_axis or y_axis, devide them to 2 cases
		else if (positionY > 0)
		{
			angle_T = std::acos(positionX / std::sqrt(positionX * positionX + positionY * positionY));
			angle_1 = std::acos((bone1_length * bone1_length + positionX * positionX + positionY * positionY - bone2_length * bone2_length) / (2 * bone1_length * std::sqrt(positionX * positionX + positionY * positionY)));
			bone1_angle = angle_1 + angle_T;
			bone2_angle = -std::acos(-(bone1_length * bone1_length + bone2_length * bone2_length - positionX * positionX - positionY * positionY) / (2 * bone1_length * bone2_length));
		}
		else
		{
			angle_T = std::acos((bone1_length * bone1_length + positionX * positionX + positionY * positionY - bone2_length * bone2_length) / (2 * bone1_length * std::sqrt(positionX * positionX + positionY * positionY)));
			angle_1 = std::acos(positionX / std::sqrt(positionX * positionX + positionY * positionY));
			bone1_angle = -(angle_1 - angle_T);
			bone2_angle = -std::acos(-(bone1_length * bone1_length + bone2_length * bone2_length - positionX * positionX - positionY * positionY) / (2 * bone1_length * bone2_length));
		}

		shader_blinnPhong->use();
		boneScale = glm::scale(glm::mat4(1.0f), glm::vec3(bone1_length, bone1_length, 1.0f));
		trans = glm::rotate(glm::mat4(1.0f), bone1_angle, glm::vec3(0.0f, 0.0f, 1.0f));
		modelMat = glm::translate(glm::mat4(1.0f), bone1_position);
		shader_blinnPhong->SetMatrix("transform", trans * boneScale);
		shader_blinnPhong->SetMatrix("modelMat", modelMat);
		shader_blinnPhong->SetMatrix("viewMat", viewMat);
		shader_blinnPhong->SetMatrix("projMat", projMat);

		bone.Draw(shader_blinnPhong);

		glm::vec3 bone2_position = glm::mat3(glm::rotate(glm::mat4(1.0f), bone1_angle, glm::vec3(0.0f, 0.0f, 1.0f))) 
									* (bone1_position + glm::vec3(bone1_length, 0.0f, 0.0f));
		boneScale = glm::scale(glm::mat4(1.0f), glm::vec3(bone2_length, bone2_length, 1.0f));
		trans = glm::rotate(trans, bone2_angle, glm::vec3(0.0f, 0.0f, 1.0f));
		modelMat = glm::translate(glm::mat4(1.0f), bone2_position);
		shader_blinnPhong->SetMatrix("transform", trans * boneScale);
		shader_blinnPhong->SetMatrix("modelMat", modelMat);
		shader_blinnPhong->SetMatrix("viewMat", viewMat);
		shader_blinnPhong->SetMatrix("projMat", projMat);

		bone.Draw(shader_blinnPhong);
#pragma endregion

#pragma region Draw Bone (ccd) 5 bones
		//trans = glm::toMat4(bone1_quat);
		//bone2_position = glm::mat3(trans) * glm::vec3(bone1_length, 0.0f, 0.0f) + bone1_position;
		//trans *= glm::toMat4(bone2_quat);
		//bone3_position = glm::mat3(trans) * glm::vec3(bone2_length, 0.0f, 0.0f) + bone2_position;
		//trans *= glm::toMat4(bone3_quat);
		//bone4_position = glm::mat3(trans) * glm::vec3(bone3_length, 0.0f, 0.0f) + bone3_position;
		//trans *= glm::toMat4(bone4_quat);
		//bone5_position = glm::mat3(trans) * glm::vec3(bone4_length, 0.0f, 0.0f) + bone4_position;
		//trans *= glm::toMat4(bone5_quat);
		//bonechain_endPosition = glm::mat3(trans) * glm::vec3(bone5_length, 0.0f, 0.0f) + bone5_position;

		//int counter = 10;

		//while (counter--)
		//{
		//	glm::vec3 p2e = glm::normalize(bonechain_endPosition - bone5_position);
		//	glm::vec3 p2t = glm::normalize(glm::vec3(positionX, positionY, 0.0f) - bone5_position);
		//	float angle = std::acos(std::max(std::min(glm::dot(p2e, p2t), 1.0f), -1.0f));

		//	if (angle >= 0.01)
		//	{
		//		rotAxis = glm::normalize(glm::cross(p2e, p2t));

		//		rot = glm::angleAxis(angle, rotAxis);
		//		bone5_quat = rot * bone5_quat;

		//		trans = glm::toMat4(bone1_quat);
		//		bone2_position = glm::mat3(trans) * glm::vec3(bone1_length, 0.0f, 0.0f) + bone1_position;
		//		trans *= glm::toMat4(bone2_quat);
		//		bone3_position = glm::mat3(trans) * glm::vec3(bone2_length, 0.0f, 0.0f) + bone2_position;
		//		trans *= glm::toMat4(bone3_quat);
		//		bone4_position = glm::mat3(trans) * glm::vec3(bone3_length, 0.0f, 0.0f) + bone3_position;
		//		trans *= glm::toMat4(bone4_quat);
		//		bone5_position = glm::mat3(trans) * glm::vec3(bone4_length, 0.0f, 0.0f) + bone4_position;
		//		trans *= glm::toMat4(bone5_quat);
		//		bonechain_endPosition = glm::mat3(trans) * glm::vec3(bone5_length, 0.0f, 0.0f) + bone5_position;

		//		glm::vec3 temp = glm::mat3(trans) * glm::vec3(bone5_length, 0.0f, 0.0f);
		//	}

		//	p2e = glm::normalize(bonechain_endPosition - bone4_position);
		//	p2t = glm::normalize(glm::vec3(positionX, positionY, 0.0f) - bone4_position);
		//	angle = std::acos(glm::dot(p2e, p2t));
		//	
		//	if (angle >= 0.01)
		//	{
		//		rotAxis = glm::cross(p2e, p2t);
		//		rot = glm::angleAxis(angle, glm::normalize(rotAxis));
		//		bone4_quat = rot * bone4_quat;

		//		trans = glm::toMat4(bone1_quat);
		//		bone2_position = glm::mat3(trans) * glm::vec3(bone1_length, 0.0f, 0.0f) + bone1_position;
		//		trans *= glm::toMat4(bone2_quat);
		//		bone3_position = glm::mat3(trans) * glm::vec3(bone2_length, 0.0f, 0.0f) + bone2_position;
		//		trans *= glm::toMat4(bone3_quat);
		//		bone4_position = glm::mat3(trans) * glm::vec3(bone3_length, 0.0f, 0.0f) + bone3_position;
		//		trans *= glm::toMat4(bone4_quat);
		//		bone5_position = glm::mat3(trans) * glm::vec3(bone4_length, 0.0f, 0.0f) + bone4_position;
		//		trans *= glm::toMat4(bone5_quat);
		//		bonechain_endPosition = glm::mat3(trans) * glm::vec3(bone5_length, 0.0f, 0.0f) + bone5_position;
		//	}

		//	p2e = glm::normalize(bonechain_endPosition - bone3_position);
		//	p2t = glm::normalize(glm::vec3(positionX, positionY, 0.0f) - bone3_position);
		//	angle = std::acos(glm::dot(p2e, p2t));

		//	if (angle >= 0.01)
		//	{
		//		rotAxis = glm::cross(p2e, p2t);
		//		rot = glm::angleAxis(angle, glm::normalize(rotAxis));
		//		bone3_quat = rot * bone3_quat;

		//		trans = glm::toMat4(bone1_quat);
		//		bone2_position = glm::mat3(trans) * glm::vec3(bone1_length, 0.0f, 0.0f) + bone1_position;
		//		trans *= glm::toMat4(bone2_quat);
		//		bone3_position = glm::mat3(trans) * glm::vec3(bone2_length, 0.0f, 0.0f) + bone2_position;
		//		trans *= glm::toMat4(bone3_quat);
		//		bone4_position = glm::mat3(trans) * glm::vec3(bone3_length, 0.0f, 0.0f) + bone3_position;
		//		trans *= glm::toMat4(bone4_quat);
		//		bone5_position = glm::mat3(trans) * glm::vec3(bone4_length, 0.0f, 0.0f) + bone4_position;
		//		trans *= glm::toMat4(bone5_quat);
		//		bonechain_endPosition = glm::mat3(trans) * glm::vec3(bone5_length, 0.0f, 0.0f) + bone5_position;
		//	}


		//	p2e = glm::normalize(bonechain_endPosition - bone2_position);
		//	p2t = glm::normalize(glm::vec3(positionX, positionY, 0.0f) - bone2_position);
		//	angle = std::acos(glm::dot(p2e, p2t));
		//	
		//	if (angle >= 0.01)
		//	{
		//		rotAxis = glm::cross(p2e, p2t);
		//		rot = glm::angleAxis(angle, glm::normalize(rotAxis));
		//		bone2_quat = rot * bone2_quat;

		//		trans = glm::toMat4(bone1_quat);
		//		bone2_position = glm::mat3(trans) * glm::vec3(bone1_length, 0.0f, 0.0f) + bone1_position;
		//		trans *= glm::toMat4(bone2_quat);
		//		bone3_position = glm::mat3(trans) * glm::vec3(bone2_length, 0.0f, 0.0f) + bone2_position;
		//		trans *= glm::toMat4(bone3_quat);
		//		bone4_position = glm::mat3(trans) * glm::vec3(bone3_length, 0.0f, 0.0f) + bone3_position;
		//		trans *= glm::toMat4(bone4_quat);
		//		bone5_position = glm::mat3(trans) * glm::vec3(bone4_length, 0.0f, 0.0f) + bone4_position;
		//		trans *= glm::toMat4(bone5_quat);
		//		bonechain_endPosition = glm::mat3(trans) * glm::vec3(bone5_length, 0.0f, 0.0f) + bone5_position;
		//	}


		//	p2e = glm::normalize(bonechain_endPosition - bone1_position);
		//	p2t = glm::normalize(glm::vec3(positionX, positionY, 0.0f) - bone1_position);
		//	angle = std::acos(glm::dot(p2e, p2t));
		//	
		//	if (angle >= 0.01)
		//	{
		//		rotAxis = glm::cross(p2e, p2t);
		//		rot = glm::angleAxis(angle, glm::normalize(rotAxis));
		//		bone1_quat = rot * bone1_quat;

		//		trans = glm::toMat4(bone1_quat);
		//		bone2_position = glm::mat3(trans) * glm::vec3(bone1_length, 0.0f, 0.0f) + bone1_position;
		//		trans *= glm::toMat4(bone2_quat);
		//		bone3_position = glm::mat3(trans) * glm::vec3(bone2_length, 0.0f, 0.0f) + bone2_position;
		//		trans *= glm::toMat4(bone3_quat);
		//		bone4_position = glm::mat3(trans) * glm::vec3(bone3_length, 0.0f, 0.0f) + bone3_position;
		//		trans *= glm::toMat4(bone4_quat);
		//		bone5_position = glm::mat3(trans) * glm::vec3(bone4_length, 0.0f, 0.0f) + bone4_position;
		//		trans *= glm::toMat4(bone5_quat);
		//		bonechain_endPosition = glm::mat3(trans) * glm::vec3(bone5_length, 0.0f, 0.0f) + bone5_position;
		//	}
		//}


		//shader_blinnPhong->use();
		//boneScale = glm::scale(glm::mat4(1.0f), glm::vec3(bone1_length, bone1_length, 1.0f));
		//trans = glm::toMat4(bone1_quat);
		//modelMat = glm::translate(glm::mat4(1.0f), bone1_position);
		//shader_blinnPhong->SetMatrix("transform", trans * boneScale);
		//shader_blinnPhong->SetMatrix("modelMat", modelMat);
		//shader_blinnPhong->SetMatrix("viewMat", viewMat);
		//shader_blinnPhong->SetMatrix("projMat", projMat);
		//bone.Draw(shader_blinnPhong);

		//boneScale = glm::scale(glm::mat4(1.0f), glm::vec3(bone2_length, bone2_length, 1.0f));
		//trans *= glm::toMat4(bone2_quat);
		//modelMat = glm::translate(glm::mat4(1.0f), bone2_position);
		//shader_blinnPhong->SetMatrix("transform", trans * boneScale);
		//shader_blinnPhong->SetMatrix("modelMat", modelMat);
		//bone.Draw(shader_blinnPhong);

		//boneScale = glm::scale(glm::mat4(1.0f), glm::vec3(bone3_length, bone3_length, 1.0f));
		//trans *= glm::toMat4(bone3_quat);
		//modelMat = glm::translate(glm::mat4(1.0f), bone3_position);
		//shader_blinnPhong->SetMatrix("transform", trans * boneScale);
		//shader_blinnPhong->SetMatrix("modelMat", modelMat);
		//bone.Draw(shader_blinnPhong);

		//boneScale = glm::scale(glm::mat4(1.0f), glm::vec3(bone4_length, bone4_length, 1.0f));
		//trans *= glm::toMat4(bone4_quat);
		//modelMat = glm::translate(glm::mat4(1.0f), bone4_position);
		//shader_blinnPhong->SetMatrix("transform", trans * boneScale);
		//shader_blinnPhong->SetMatrix("modelMat", modelMat);
		//bone.Draw(shader_blinnPhong);

		//boneScale = glm::scale(glm::mat4(1.0f), glm::vec3(bone5_length, bone5_length, 1.0f));
		//trans *= glm::toMat4(bone5_quat);
		//modelMat = glm::translate(glm::mat4(1.0f), bone5_position);
		//shader_blinnPhong->SetMatrix("transform", trans * boneScale);
		//shader_blinnPhong->SetMatrix("modelMat", modelMat);
		//bone.Draw(shader_blinnPhong);
#pragma endregion

		// Clean up, prepare for next render loop
		glfwSwapBuffers(window);
		glfwPollEvents();
		//camera.UpdateCameraPos();
	}


}
