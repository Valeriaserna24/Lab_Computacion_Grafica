/*
Nombre: Chavez Serna Joana Valeria
No. Cuenta: 320267561
Actividad: Práctica 7, texturizado
Fecha de entrega: 27 de Marzo del 2026
*/

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Shader.h"
#include "Camera.h"

// Prototipos originales del Previo 7
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

const GLuint WIDTH = 800, HEIGHT = 600;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0, lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
GLfloat deltaTime = 0.0f, lastFrame = 0.0f;

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Chavez Serna Joana Valeria-Cubo Texturizado", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_DEPTH_TEST);

	// Shaders 
	Shader ourShader("Shader/lamp.vs", "Shader/lamp.frag");

	// Arreglo de vértices: Posición(3), Color(3), UV(2)
	float vertices[] = {
		// Posiciones          // Colores            // UV 
		// Frente Num: 5
		-0.5f, -0.5f,  0.5f,     1.0f, 1.0f, 1.0f,     0.425f, 0.425f, // Inf. Izq
		 0.5f, -0.5f,  0.5f,     1.0f, 1.0f, 1.0f,     0.588f, 0.425f, // Inf. Der
		 0.5f,  0.5f,  0.5f,     1.0f, 1.0f, 1.0f,     0.589f, 0.588f, // Sup. Der

		  0.5f,  0.5f,  0.5f,     1.0f, 1.0f, 1.0f,     0.589f, 0.588f, // Sup. Der 
		 -0.5f,  0.5f,  0.5f,     1.0f, 1.0f, 1.0f,     0.425f, 0.588f, // Sup. Izq
		 -0.5f, -0.5f,  0.5f,     1.0f, 1.0f, 1.0f,     0.425f, 0.425f, // inf. Izq
		
		 // Atrás Num: 2
		-0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,     0.426f, 0.10f,
		 0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,     0.589f, 0.10f,
		 0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,     0.589f, 0.262f,

		 0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,     0.589f, 0.262f,
		-0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,     0.426f, 0.262f,
		-0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,     0.426f, 0.10f,
	
		// Izquierda Num: 4
		-0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,     0.262f, 0.425f,
		-0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,     0.426f, 0.425f,
		-0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,     0.426f, 0.588f,
	 
		-0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,     0.426f, 0.588f,
		-0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,     0.262f, 0.588f,
		-0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,     0.262f, 0.425f,
	
		// Derecha Num: 3
		 0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,     0.588f, 0.425f,
		 0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,     0.749f, 0.425f,
		 0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,     0.749f, 0.588f,

		 0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,     0.749f, 0.588f,
		 0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,     0.589f, 0.588f,
		 0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,     0.588f, 0.425f,

		 // arriba Num: 6
		 -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,     0.425f, 0.588f,
		  0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,     0.589f, 0.588f,
		  0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,     0.588f, 0.749f,
	 
		  0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,     0.588f, 0.749f,
		 -0.5f,  0.5f,  0.5f,   1.0f, 1.0f, 1.0f,     0.425f, 0.749f,
		 -0.5f,  0.5f, -0.5f,   1.0f, 1.0f, 1.0f,     0.425f, 0.588f,
	
		 // abajo Num: 1
		 -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,     0.426f, 0.264f,
		  0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,     0.588f, 0.264f,
		  0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,     0.588f, 0.425f,

		  0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,     0.588f, 0.425f,
		 -0.5f, -0.5f,  0.5f,   1.0f, 1.0f, 1.0f,     0.425f, 0.425f,
		 -0.5f, -0.5f, -0.5f,   1.0f, 1.0f, 1.0f,     0.426f, 0.264f,
	};

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Atributos: Posición(0), Color(1), UV(2). Stride de 8 floats.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	// Carga de Textura (Lógica Previo 7)
	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* image = stbi_load("images/Dado.png", &width, &height, &nrChannels, 0);
	if (image) {
		GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(image);

	while (!glfwWindowShouldClose(window)) {
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		DoMovement();

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ourShader.Use();

		// Matrices (Estructura Previo 7)
		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)WIDTH / HEIGHT, 0.1f, 100.0f);
		glm::mat4 model = glm::mat4(1.0f);

		glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(ourShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindTexture(GL_TEXTURE_2D, texture1);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();
	return 0;
}

// Funciones de control (Sin cambios)
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