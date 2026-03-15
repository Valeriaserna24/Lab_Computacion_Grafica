/*
Nombre: Chavez Serna Joana Valeria
No. Cuenta: 320267561
Actividad: Practica 2, Dibujo de primitivas (Unicornio en 2D)
Fecha de entrega: 18 de Febrero del 2026s
*/

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);
const GLint WIDTH = 800, HEIGHT = 600;

int main() {
    if (!glfwInit()) return EXIT_FAILURE;

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Chavez Serna Joana Valeria - Unicornio 2D", NULL, NULL);
    if (window == NULL) { glfwTerminate(); return EXIT_FAILURE; }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);
    glewExperimental = GL_TRUE;
    if (GLEW_OK != glewInit()) return EXIT_FAILURE;

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    // Arreglo de vértices basado en tus índices y coordenadas exactas
    // Estructura: X, Y, Z, R, G, B
    float vertices[] = {
        // --- CABEZA Y OREJA ---
        0.0f, 0.4f, 0.0f, 1.0f, 0.0f, 0.5f,  -0.2f, 0.7f, 0.0f, 1.0f, 0.0f, 0.5f,  -0.2f, 0.5f, 0.0f, 1.0f, 0.0f, 0.5f, // (0,1,2)
        0.0f, 0.4f, 0.0f, 0.8f, 0.0f, 0.4f,  -0.2f, 0.5f, 0.0f, 0.8f, 0.0f, 0.4f,  -0.2f, 0.4f, 0.0f, 0.8f, 0.0f, 0.4f, // (0,2,3)
        0.0f, 0.4f, 0.0f, 0.0f, 0.5f, 1.0f,   0.1f, 0.1f, 0.0f, 0.0f, 0.5f, 1.0f,  -0.2f, 0.4f, 0.0f, 0.0f, 0.5f, 1.0f, // (0,16,3)
        0.0f, 0.4f, 0.0f, 0.2f, 0.0f, 0.6f,   0.1f, 0.1f, 0.0f, 0.2f, 0.0f, 0.6f,   0.2f, 0.0f, 0.0f, 0.2f, 0.0f, 0.6f, // (0,16,17)
        0.0f, 0.4f, 0.0f, 0.5f, 0.0f, 0.5f,   0.2f, 0.0f, 0.0f, 0.5f, 0.0f, 0.5f,   0.3f, 0.3f, 0.0f, 0.5f, 0.0f, 0.5f, // (0,17,24)

        // --- CUERNO ---
        -0.2f, 0.4f, 0.0f, 1.0f, 1.0f, 0.0f, -0.4f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, -0.7f, 0.8f, 0.0f, 1.0f, 1.0f, 0.0f, // (3,4,5)
        -0.4f, 0.5f, 0.0f, 0.8f, 0.8f, 0.0f, -0.7f, 0.8f, 0.0f, 0.8f, 0.8f, 0.0f, -0.4f, 0.1f, 0.0f, 0.8f, 0.8f, 0.0f, // (4,5,6)

        // --- CARA ---
        -0.2f, 0.4f, 0.0f, 0.0f, 0.4f, 0.8f, -0.4f, 0.5f, 0.0f, 0.0f, 0.4f, 0.8f,  0.1f, 0.1f, 0.0f, 0.0f, 0.4f, 0.8f, // (3,4,16)
        -0.4f, 0.5f, 0.0f, 0.0f, 0.3f, 0.7f, -0.4f, 0.1f, 0.0f, 0.0f, 0.3f, 0.7f,  0.1f, 0.1f, 0.0f, 0.0f, 0.3f, 0.7f, // (4,6,16)
        -0.4f, 0.1f, 0.0f, 0.0f, 0.8f, 0.8f,  0.0f, 0.0f, 0.0f, 0.0f, 0.8f, 0.8f,  0.1f, 0.1f, 0.0f, 0.0f, 0.8f, 0.8f, // (6,15,16)
         0.0f, 0.0f, 0.0f, 0.3f, 0.6f, 0.9f,  0.1f, 0.1f, 0.0f, 0.3f, 0.6f, 0.9f,  0.2f, 0.0f, 0.0f, 0.3f, 0.6f, 0.9f, // (15,16,17)

         // --- NARIZ Y HOCICO ---
         -0.4f, 0.1f, 0.0f, 0.2f, 0.8f, 0.2f, -0.3f, -0.3f, 0.0f, 0.2f, 0.8f, 0.2f,  0.0f, 0.0f, 0.0f, 0.2f, 0.8f, 0.2f, // (6,13,15)
         -0.4f, 0.1f, 0.0f, 0.1f, 0.5f, 0.1f, -0.5f, -0.3f, 0.0f, 0.1f, 0.5f, 0.1f, -0.3f, -0.3f, 0.0f, 0.1f, 0.5f, 0.1f, // (6,7,13)
         -0.5f, -0.3f, 0.0f, 0.0f, 0.4f, 0.0f, -0.4f, -0.4f, 0.0f, 0.0f, 0.4f, 0.0f, -0.3f, -0.3f, 0.0f, 0.0f, 0.4f, 0.0f, // (7,12,13)
         -0.5f, -0.3f, 0.0f, 0.3f, 0.5f, 0.1f, -0.6f, -0.5f, 0.0f, 0.3f, 0.5f, 0.1f, -0.4f, -0.4f, 0.0f, 0.3f, 0.5f, 0.1f, // (7,8,12)

         // --- MANDÍBULA ---
         -0.6f, -0.5f, 0.0f, 1.0f, 0.5f, 0.0f, -0.5f, -0.7f, 0.0f, 1.0f, 0.5f, 0.0f, -0.4f, -0.4f, 0.0f, 1.0f, 0.5f, 0.0f, // (8,9,12)
         -0.5f, -0.7f, 0.0f, 0.9f, 0.4f, 0.0f, -0.25f,-0.65f,0.0f, 0.9f, 0.4f, 0.0f, -0.4f, -0.4f, 0.0f, 0.9f, 0.4f, 0.0f, // (9,10,12)
         -0.25f,-0.65f,0.0f, 0.8f, 0.3f, 0.0f, -0.2f, -0.5f, 0.0f, 0.8f, 0.3f, 0.0f, -0.4f, -0.4f, 0.0f, 0.8f, 0.3f, 0.0f, // (10,11,12)

         // --- CUELLO ---
         -0.4f, -0.4f, 0.0f, 0.6f, 0.1f, 0.8f, -0.3f, -0.3f, 0.0f, 0.6f, 0.1f, 0.8f,  0.1f, -0.3f, 0.0f, 0.6f, 0.1f, 0.8f, // (12,13,14)
         -0.4f, -0.4f, 0.0f, 0.5f, 0.0f, 0.7f,  0.1f, -0.3f, 0.0f, 0.5f, 0.0f, 0.7f, -0.2f, -0.5f, 0.0f, 0.5f, 0.0f, 0.7f, // (12,14,11)
         -0.3f, -0.3f, 0.0f, 0.4f, 0.0f, 0.5f,  0.1f, -0.3f, 0.0f, 0.4f, 0.0f, 0.5f,  0.0f, 0.0f, 0.0f, 0.4f, 0.0f, 0.5f, // (13,14,15)
          0.1f, -0.3f, 0.0f, 0.7f, 0.4f, 0.9f,  0.0f, 0.0f, 0.0f, 0.7f, 0.4f, 0.9f,   0.2f, 0.0f, 0.0f, 0.7f, 0.4f, 0.9f, // (14,15,17)

          // --- CRIN Y PARTE TRASERA ---
           0.1f, -0.3f, 0.0f, 1.0f, 0.2f, 0.2f,  0.2f, 0.0f, 0.0f, 1.0f, 0.2f, 0.2f,   0.4f, -0.3f, 0.0f, 1.0f, 0.2f, 0.2f, // (14,17,18)
           0.2f, 0.0f, 0.0f, 0.8f, 0.0f, 0.0f,  0.4f, -0.3f, 0.0f, 0.8f, 0.0f, 0.0f,   0.3f, 0.3f, 0.0f, 0.8f, 0.0f, 0.0f, // (17,18,24)
           0.4f, -0.3f, 0.0f, 0.6f, 0.0f, 0.0f,  0.8f, -0.1f, 0.0f, 0.6f, 0.0f, 0.0f,   0.3f, 0.3f, 0.0f, 0.6f, 0.0f, 0.0f, // (18,23,24)

           // --- PECHO Y CUERPO INFERIOR ---
            0.1f, -0.3f, 0.0f, 0.4f, 0.2f, 0.6f,  0.4f, -0.3f, 0.0f, 0.4f, 0.2f, 0.6f,   0.3f, -0.8f, 0.0f, 0.4f, 0.2f, 0.6f, // (14,18,19)
            0.4f, -0.3f, 0.0f, 0.3f, 0.1f, 0.5f,  0.3f, -0.8f, 0.0f, 0.3f, 0.1f, 0.5f,   0.45f,-0.8f, 0.0f, 0.3f, 0.1f, 0.5f, // (18,19,20)
            0.4f, -0.3f, 0.0f, 0.5f, 0.3f, 0.7f,  0.45f,-0.8f, 0.0f, 0.5f, 0.3f, 0.7f,   0.6f, -0.4f, 0.0f, 0.5f, 0.3f, 0.7f, // (18,20,21)
            0.4f, -0.3f, 0.0f, 0.2f, 0.2f, 0.5f,  0.6f, -0.4f, 0.0f, 0.2f, 0.2f, 0.5f,   0.8f, -0.1f, 0.0f, 0.2f, 0.2f, 0.5f, // (18,21,23)
            0.6f, -0.4f, 0.0f, 0.4f, 0.4f, 0.4f,  0.7f, -0.4f, 0.0f, 0.4f, 0.4f, 0.4f,   0.8f, -0.1f, 0.0f, 0.4f, 0.4f, 0.4f  // (21,22,23)
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Posición (Location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Color (Location 1)
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        ourShader.Use();
        glBindVertexArray(VAO);
        // Dibujamos todos los triángulos automáticamente
        glDrawArrays(GL_TRIANGLES, 0, sizeof(vertices) / (6 * sizeof(float)));
        glBindVertexArray(0);
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

void resize(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}