/*
Nombre: Chavez Serna Joana Valeria
No. Cuenta: 320267561
Actividad: Práctica 8 ,Materiales e iluminación
Fecha de entrega: 10 de Abril del 2026
*/

#include <iostream>
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Propiedades de la ventana
const GLuint WIDTH = 800, HEIGHT = 600;
//const GLuint WIDTH = 1800, HEIGHT = 1000; //Para el video
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Prototipos
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Cámara y Control
Camera camera(glm::vec3(0.0f, 10.0f, 50.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

// --- VARIABLES DE ANIMACIÓN (NUEVAS) ---
bool startAnim = false;     // Controla si la animación corre sola
float angle = 0.0f;         // Ángulo de rotación para Sol/Luna
float radius = 60.0f;       // Radio de la órbita circular
glm::vec3 sunPos, moonPos;  // Posiciones dinámicas

int main()
{
    // 1. Inicialización de GLFW y Ventana
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Chavez Serna Joana Valeria - Materiales e iluminación", nullptr, nullptr);
    if (nullptr == window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Callbacks
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) return EXIT_FAILURE;

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_DEPTH_TEST);

    // 2. Carga de Shaders
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");

    // 3. Carga de Modelos (Escenario + Astros)
    // --- Modelos Escenario ---
    Model dog((char*)"Models/RedDog.obj");
    Model Carrito((char*)"Models/p6/Carrito/carrito.obj");
    Model Menu((char*)"Models/p6/Menu/menu.obj");
    Model Dulces((char*)"Models/p6/Candy/objCandtBowl.obj");
    Model Letrero((char*)"Models/p6/LetreroCirc/signboard_02.obj");
    Model HeladoIndividual((char*)"Models/p6/Helado/10861 Ice Cream_v1_LOD1.obj");

    // --- Modelos Luces ---
    Model SunModel((char*)"Models/p8/sol/13913_Sun_v2_l3.obj");

    Model MoonModel((char*)"Models/p8/luna/moon.obj");

    // Proyección (una sola para todo)
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 500.0f);

    // --- INICIO DEL GAME LOOP ---
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        DoMovement();

        // 1. ANIMACIÓN DE TRAYECTORIA
        if (startAnim) {
            angle += 0.8f * deltaTime;
        }

        float radius = 100.0f; // Radio amplio para un arco bonito
        // Z en 20.0f para que la luz pegue de frente al carrito (-25.0f)
        sunPos = glm::vec3(cos(angle) * radius, sin(angle) * radius, 20.0f);
        moonPos = glm::vec3(cos(angle + glm::pi<float>()) * radius, sin(angle + glm::pi<float>()) * radius, 20.0f);

        // Cambio de cielo
        if (sunPos.y > 0)
            glClearColor(0.4f, 0.6f, 0.9f, 1.0f); // Día
        else
            glClearColor(0.02f, 0.02f, 0.1f, 1.0f); // Noche

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 2. CONFIGURACIÓN DEL SHADER DE ILUMINACIÓN
        lightingShader.Use();
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model(1.0f);
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


        // --- CONFIGURAR LUCES DINÁMICAS (SOL Y LUNA) ---
        lightingShader.Use();

        // 1. Lógica para el SOL
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.position"), sunPos.x, sunPos.y, sunPos.z);

        if (sunPos.y > 0) {
            // Sol encendido (Día)
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.3f, 0.3f, 0.3f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 1.2f, 1.2f, 1.2f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 1.0f, 1.0f, 1.0f);
        }
        else {
            // Sol apagado 
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.3f, 0.3f, 0.3f); // con un mínimo de luz base para que no sea negro
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.0f, 0.0f, 0.0f);
        }

        // 2. Lógica para la LUNA
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.position"), moonPos.x, moonPos.y, moonPos.z);

        if (moonPos.y > 0) {
            // Luna encendida
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.15f, 0.15f, 0.3f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.4f, 0.6f, 1.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 1.2f, 1.2f, 1.5f);
        }
        else {
            // Luna apagada 
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.ambient"), 0.09f, 0.09f, 0.6f);// con un mínimo de luz base para que no sea negro
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.diffuse"), 0.0f, 0.0f, 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "light2.specular"), 0.0f, 0.0f, 0.0f);
        }

        // --- MATERIAL BASE PARA EL ESCENARIO ---
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.6f, 0.6f, 0.6f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.5f, 0.5f, 0.5f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

        // 3. DIBUJAR MODELOS 
        // --- 1. EL CARRITO ---
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -4.0f, -25.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -180.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Carrito.Draw(lightingShader);

        // --- 2. MENU
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-35.0f, 0.0f, 7.0f));
        model = glm::rotate(model, glm::radians(25.0f), glm::vec3(0.0f, -1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f, 0.1f, 0.2f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Menu.Draw(lightingShader);

        // --- 3. PERRITO (dog)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-35.0f, 10.0f, 30.0f));
        model = glm::scale(model, glm::vec3(30.5f, 30.5f, 25.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(lightingShader);

        // --- 4. DULCES (Candy)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-13.0f, 16.0f, -4.7f));
        model = glm::scale(model, glm::vec3(2.2f, 2.2f, 2.2f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Dulces.Draw(lightingShader);

        // --- 5. EL HELADO
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(40.0f, 20.0f, 10.0f));
        model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        HeladoIndividual.Draw(lightingShader);

        // --- 6. EL LETRERO ---
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-55.0f, 20.0f, -10.0f));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Letrero.Draw(lightingShader);

        // --- 7. EL HELADO DENTRO DEL LETRERO ---
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-58.0f, 28.0f, -10.0f));
        model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        HeladoIndividual.Draw(lightingShader);

        
        // 4. DIBUJAR ASTROS 
        // Sol
        float sunIntensity = (sunPos.y > 0) ? 5.0f : 0.0f;
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), sunIntensity, sunIntensity, sunIntensity);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), sunIntensity, sunIntensity, sunIntensity);

        model = glm::mat4(1.0f);
        model = glm::translate(model, sunPos);
        model = glm::scale(model, glm::vec3(0.05f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        SunModel.Draw(lightingShader);

        // Luna
        float moonIntensity = (moonPos.y > 0) ? 5.0f : 0.0f;
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), moonIntensity, moonIntensity, moonIntensity);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), moonIntensity, moonIntensity, moonIntensity);

        model = glm::mat4(1.0f);
        model = glm::translate(model, moonPos);
        model = glm::scale(model, glm::vec3(0.4f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        MoonModel.Draw(lightingShader);

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// Control de Teclado
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS) keys[key] = true;
        else if (action == GLFW_RELEASE) keys[key] = false;
    }

    // TECLA PARA INICIAR ANIMACIÓN (Ejemplo: Letra 'P' de Play)
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        startAnim = !startAnim;
    }
}

void DoMovement() {
    if (keys[GLFW_KEY_W]) camera.ProcessKeyboard(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S]) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A]) camera.ProcessKeyboard(LEFT, deltaTime);
    if (keys[GLFW_KEY_D]) camera.ProcessKeyboard(RIGHT, deltaTime);
}

void MouseCallback(GLFWwindow* window, double xPos, double yPos) {
    if (firstMouse) {
        lastX = xPos; lastY = yPos; firstMouse = false;
    }
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;
    lastX = xPos; lastY = yPos;
    camera.ProcessMouseMovement(xOffset, yOffset);
}