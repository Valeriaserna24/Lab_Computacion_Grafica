/*
Nombre: Chavez Serna Joana Valeria
No. Cuenta: 320267561
Actividad: Practica 4, Modelado 3d
Fecha de entrega: 06 de Marzo del 2026
*/

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

void Inputs(GLFWwindow* window);

const GLint WIDTH = 800, HEIGHT = 600;
float movX = 0.0f;
float movY = 0.0f;
float movZ = -5.0f;
float rot = 0.0f;

int main() {
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Chavez Serna Joana Valeria - Pollo Crossy Road", nullptr, nullptr);
    if (nullptr == window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) return EXIT_FAILURE;

    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    /* EXPLICACIÓN DEL ARREGLO:
       Cada bloque de 36 vértices representa un cubo completo de un solo color.
       Como no podemos usar varios shaders, pre-coloreamos los cubos aquí.
    */

   //Formato: Posición (x, y, z), Color (r, g, b)
    float vertices[] = {
        // --- CUBO 0: BLANCO PURO (Cuerpo, Pecho, Cabeza) ---
        // Frente
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,   0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,   0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
         // Atrás
         -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
          0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
          // Izquierda
          -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
          -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
          // Derecha
           0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,   0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
           0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,   0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
           // Abajo
           -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
            // Arriba
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,   0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,  -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,

             // --- CUBO 36: NARANJA (Pico, Patas) ---
             -0.5f, -0.5f,  0.5f,  1.0f, 0.5f, 0.0f,   0.5f, -0.5f,  0.5f,  1.0f, 0.5f, 0.0f,   0.5f,  0.5f,  0.5f,  1.0f, 0.5f, 0.0f,
              0.5f,  0.5f,  0.5f,  1.0f, 0.5f, 0.0f,  -0.5f,  0.5f,  0.5f,  1.0f, 0.5f, 0.0f,  -0.5f, -0.5f,  0.5f,  1.0f, 0.5f, 0.0f,
             -0.5f, -0.5f, -0.5f,  1.0f, 0.4f, 0.0f,   0.5f, -0.5f, -0.5f,  1.0f, 0.4f, 0.0f,   0.5f,  0.5f, -0.5f,  1.0f, 0.4f, 0.0f,
              0.5f,  0.5f, -0.5f,  1.0f, 0.4f, 0.0f,  -0.5f,  0.5f, -0.5f,  1.0f, 0.4f, 0.0f,  -0.5f, -0.5f, -0.5f,  1.0f, 0.4f, 0.0f,
              0.5f, -0.5f,  0.5f,  1.0f, 0.5f, 0.0f,   0.5f, -0.5f, -0.5f,  1.0f, 0.5f, 0.0f,   0.5f,  0.5f, -0.5f,  1.0f, 0.5f, 0.0f,
              0.5f,  0.5f, -0.5f,  1.0f, 0.5f, 0.0f,   0.5f,  0.5f,  0.5f,  1.0f, 0.5f, 0.0f,   0.5f, -0.5f,  0.5f,  1.0f, 0.5f, 0.0f,
             -0.5f,  0.5f,  0.5f,  1.0f, 0.5f, 0.0f,  -0.5f,  0.5f, -0.5f,  1.0f, 0.5f, 0.0f,  -0.5f, -0.5f, -0.5f,  1.0f, 0.5f, 0.0f,
             -0.5f, -0.5f, -0.5f,  1.0f, 0.5f, 0.0f,  -0.5f, -0.5f,  0.5f,  1.0f, 0.5f, 0.0f,  -0.5f,  0.5f,  0.5f,  1.0f, 0.5f, 0.0f,
             -0.5f, -0.5f, -0.5f,  1.0f, 0.4f, 0.0f,   0.5f, -0.5f, -0.5f,  1.0f, 0.4f, 0.0f,   0.5f, -0.5f,  0.5f,  1.0f, 0.4f, 0.0f,
              0.5f, -0.5f,  0.5f,  1.0f, 0.4f, 0.0f,  -0.5f, -0.5f,  0.5f,  1.0f, 0.4f, 0.0f,  -0.5f, -0.5f, -0.5f,  1.0f, 0.4f, 0.0f,
             -0.5f,  0.5f, -0.5f,  1.0f, 0.6f, 0.0f,   0.5f,  0.5f, -0.5f,  1.0f, 0.6f, 0.0f,   0.5f,  0.5f,  0.5f,  1.0f, 0.6f, 0.0f,
              0.5f,  0.5f,  0.5f,  1.0f, 0.6f, 0.0f,  -0.5f,  0.5f,  0.5f,  1.0f, 0.6f, 0.0f,  -0.5f,  0.5f, -0.5f,  1.0f, 0.6f, 0.0f,

              // --- CUBO 72: ROJO (Cresta, Papada) ---
              -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,   0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,   0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
               0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
              -0.5f, -0.5f, -0.5f,  0.8f, 0.0f, 0.0f,   0.5f, -0.5f, -0.5f,  0.8f, 0.0f, 0.0f,   0.5f,  0.5f, -0.5f,  0.8f, 0.0f, 0.0f,
               0.5f,  0.5f, -0.5f,  0.8f, 0.0f, 0.0f,  -0.5f,  0.5f, -0.5f,  0.8f, 0.0f, 0.0f,  -0.5f, -0.5f, -0.5f,  0.8f, 0.0f, 0.0f,
               0.5f, -0.5f,  0.5f,  0.9f, 0.0f, 0.0f,   0.5f, -0.5f, -0.5f,  0.9f, 0.0f, 0.0f,   0.5f,  0.5f, -0.5f,  0.9f, 0.0f, 0.0f,
               0.5f,  0.5f, -0.5f,  0.9f, 0.0f, 0.0f,   0.5f,  0.5f,  0.5f,  0.9f, 0.0f, 0.0f,   0.5f, -0.5f,  0.5f,  0.9f, 0.0f, 0.0f,
              -0.5f,  0.5f,  0.5f,  0.9f, 0.0f, 0.0f,  -0.5f,  0.5f, -0.5f,  0.9f, 0.0f, 0.0f,  -0.5f, -0.5f, -0.5f,  0.9f, 0.0f, 0.0f,
              -0.5f, -0.5f, -0.5f,  0.9f, 0.0f, 0.0f,  -0.5f, -0.5f,  0.5f,  0.9f, 0.0f, 0.0f,  -0.5f,  0.5f,  0.5f,  0.9f, 0.0f, 0.0f,
              -0.5f, -0.5f, -0.5f,  0.7f, 0.0f, 0.0f,   0.5f, -0.5f, -0.5f,  0.7f, 0.0f, 0.0f,   0.5f, -0.5f,  0.5f,  0.7f, 0.0f, 0.0f,
               0.5f, -0.5f,  0.5f,  0.7f, 0.0f, 0.0f,  -0.5f, -0.5f,  0.5f,  0.7f, 0.0f, 0.0f,  -0.5f, -0.5f, -0.5f,  0.7f, 0.0f, 0.0f,
              -0.5f,  0.5f, -0.5f,  1.0f, 0.1f, 0.1f,   0.5f,  0.5f, -0.5f,  1.0f, 0.1f, 0.1f,   0.5f,  0.5f,  0.5f,  1.0f, 0.1f, 0.1f,
               0.5f,  0.5f,  0.5f,  1.0f, 0.1f, 0.1f,  -0.5f,  0.5f,  0.5f,  1.0f, 0.1f, 0.1f,  -0.5f,  0.5f, -0.5f,  1.0f, 0.1f, 0.1f,

               // --- CUBO 108: NEGRO (Ojos) ---
               -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,   0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,   0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
               -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,   0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,   0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
                0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
                0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,   0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,   0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
                0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,   0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,   0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
               -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
               -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
               -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,   0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,   0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
                0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
               -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,   0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,   0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
                0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,  -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,

                // --- CUBO 144: GRIS CLARO (Contraste para Alas y Colita) ---
                -0.5f, -0.5f,  0.5f,  0.9f, 0.9f, 0.9f,   0.5f, -0.5f,  0.5f,  0.9f, 0.9f, 0.9f,   0.5f,  0.5f,  0.5f,  0.9f, 0.9f, 0.9f,
                 0.5f,  0.5f,  0.5f,  0.9f, 0.9f, 0.9f,  -0.5f,  0.5f,  0.5f,  0.9f, 0.9f, 0.9f,  -0.5f, -0.5f,  0.5f,  0.9f, 0.9f, 0.9f,
                -0.5f, -0.5f, -0.5f,  0.85f,0.85f,0.85f,  0.5f, -0.5f, -0.5f,  0.85f,0.85f,0.85f,  0.5f,  0.5f, -0.5f,  0.85f,0.85f,0.85f,
                 0.5f,  0.5f, -0.5f,  0.85f,0.85f,0.85f, -0.5f,  0.5f, -0.5f,  0.85f,0.85f,0.85f, -0.5f, -0.5f, -0.5f,  0.85f,0.85f,0.85f,
                 0.5f, -0.5f,  0.5f,  0.88f,0.88f,0.88f,  0.5f, -0.5f, -0.5f,  0.88f,0.88f,0.88f,  0.5f,  0.5f, -0.5f,  0.88f,0.88f,0.88f,
                 0.5f,  0.5f, -0.5f,  0.88f,0.88f,0.88f,  0.5f,  0.5f,  0.5f,  0.88f,0.88f,0.88f,  0.5f, -0.5f,  0.5f,  0.88f,0.88f,0.88f,
                -0.5f,  0.5f,  0.5f,  0.9f, 0.9f, 0.9f,  -0.5f,  0.5f, -0.5f,  0.9f, 0.9f, 0.9f,  -0.5f, -0.5f, -0.5f,  0.9f, 0.9f, 0.9f,
                -0.5f, -0.5f, -0.5f,  0.9f, 0.9f, 0.9f,  -0.5f, -0.5f,  0.5f,  0.9f, 0.9f, 0.9f,  -0.5f,  0.5f,  0.5f,  0.9f, 0.9f, 0.9f,
                -0.5f, -0.5f, -0.5f,  0.82f,0.82f,0.82f,  0.5f, -0.5f, -0.5f,  0.82f,0.82f,0.82f,  0.5f, -0.5f,  0.5f,  0.82f,0.82f,0.82f,
                 0.5f, -0.5f,  0.5f,  0.82f,0.82f,0.82f, -0.5f, -0.5f,  0.5f,  0.82f,0.82f,0.82f, -0.5f, -0.5f, -0.5f,  0.82f,0.82f,0.82f,
                -0.5f,  0.5f, -0.5f,  0.92f,0.92f,0.92f,  0.5f,  0.5f, -0.5f,  0.92f,0.92f,0.92f,  0.5f,  0.5f,  0.5f,  0.92f,0.92f,0.92f,
                 0.5f,  0.5f,  0.5f,  0.92f,0.92f,0.92f, -0.5f,  0.5f,  0.5f,  0.92f,0.92f,0.92f, -0.5f,  0.5f, -0.5f,  0.92f,0.92f,0.92f,
    };

    // (Aquí va la inicialización de VAO y VBO que ya tenías, es idéntica)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window)) {
        Inputs(window);
        glfwPollEvents();
        glClearColor(0.5f, 0.7f, 0.3f, 1.0f); // Fondo verde como pasto
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.Use();
        glm::mat4 view = glm::translate(glm::mat4(1), glm::vec3(movX, movY, movZ));
        view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");

        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));



        // --- DIBUJANDO AL PERSONAJE (JERARQUÍA DE CUBOS) ---

       
        glBindVertexArray(VAO);

        // 1. CUERPO Y CUELLO (Blanco Puro - Cubo 0)
        // Pecho y Cabeza (Alargado)
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.4f, 0.15f));
        model = glm::scale(model, glm::vec3(1.0f, 2.0f, 0.85f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Espalda (Escalón)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.05f, -0.35f));
        model = glm::scale(model, glm::vec3(1.0f, 0.9f, 0.7f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // 2. ALITAS Y COLITA (Gris Claro para contraste - Cubo 144)
        // Ala Derecha 
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.55f, -0.1f, 0.0f));
        model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.6f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 144, 36);

        // Ala Izquierda
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.55f, -0.1f, 0.0f));
        model = glm::scale(model, glm::vec3(0.25f, 0.5f, 0.6f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 144, 36);

        // Colita 
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.2f, -0.75f));
        model = glm::scale(model, glm::vec3(0.7f, 0.4f, 0.35f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 144, 36);

        // 3. PATAS DE "TENEDOR" (Naranja - Cubo 36)
        // Pata Derecha
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.25f, -0.7f, 0.1f));
        model = glm::scale(model, glm::vec3(0.12f, 0.4f, 0.12f)); // <--Aquí reduje la altura a 0.4f
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 36, 36);

        // Dedos (Varitas)
        for (float xOff : {0.08f, -0.08f}) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.25f + xOff, -0.9f, 0.25f));
            model = glm::scale(model, glm::vec3(0.08f, 0.05f, 0.35f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 36, 36);
        }

        // Pata Izquierda
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-0.25f, -0.7f, 0.1f));
        model = glm::scale(model, glm::vec3(0.12f, 0.4f, 0.12f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 36, 36);

        // Dedos
        for (float xOff : {0.08f, -0.08f}) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-0.25f + xOff, -0.9f, 0.25f));
            model = glm::scale(model, glm::vec3(0.08f, 0.05f, 0.35f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 36, 36);
        }

        // 4. CARA Y OJOS 
        // Ojos Negros (Cubo 108)
        for (float side : {0.52f, -0.52f}) { // El 0.52 hace que salgan del cuerpo blanco para verlos estando de frente el modelo.
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(side, 0.9f, 0.35f));
            model = glm::scale(model, glm::vec3(0.08f, 0.15f, 0.15f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 108, 36);
        }

        // Pico (Naranja)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.65f));
        model = glm::scale(model, glm::vec3(0.45f, 0.35f, 0.4f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 36, 36);

        // Papada (Roja)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.25f, 0.6f));
        model = glm::scale(model, glm::vec3(0.2f, 0.35f, 0.2f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 72, 36);

        // Cresta (Roja)
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 1.4f, 0.15f));
        model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 72, 36);

        glBindVertexArray(0);


        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

// (Función Inputs igual al previo para mover la cámara)
void Inputs(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        movX += 0.08f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        movX -= 0.08f;
    if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
        movY += 0.08f;
    if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
        movY -= 0.08f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        movZ -= 0.08f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        movZ += 0.08f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        rot += 0.4f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        rot -= 0.4f;
}
