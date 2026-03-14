/*
Nombre: Chavez Serna Joana Valeria
No. Cuenta: 320267561
Actividad: Práctica 5, Modelado Jerárquico
Fecha de entrega: 13 de Marzo del 2026
*/

#include<iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

void Inputs(GLFWwindow* window);

const GLint WIDTH = 1200, HEIGHT = 800;

// Variables de cámara y vista
float movX = 0.0f, movY = 0.0f, movZ = -10.0f, rot = 0.0f;

// Variables de articulación del Brazo
float hombro = 0.0f, codo = 0.0f, muneca = 0.0f, rotPalma = 0.0f;

// Variables para los 4 Dedos (3 falanges por dedo)
float dedo1A = 0.0f, dedo1B = 0.0f, dedo1C = 0.0f;
float dedo2A = 0.0f, dedo2B = 0.0f, dedo2C = 0.0f;
float dedo3A = 0.0f, dedo3B = 0.0f, dedo3C = 0.0f;
float dedo4A = 0.0f, dedo4B = 0.0f, dedo4C = 0.0f;
float dedo5A = 0.0f, dedo5B = 0.0f, dedo5C = 0.0f;
int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Chavez Serna Joana Valeria - Mano Jerarquica", nullptr, nullptr);
    if (nullptr == window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    int screenWidth, screenHeight;
    glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
    glViewport(0, 0, screenWidth, screenHeight);
    glEnable(GL_DEPTH_TEST);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

    float vertices[] = {
        -0.5f, -0.5f, 0.5f,  0.5f, -0.5f, 0.5f,  0.5f,  0.5f, 0.5f,  0.5f,  0.5f, 0.5f, -0.5f,  0.5f, 0.5f, -0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f,-0.5f,  0.5f, -0.5f,-0.5f,  0.5f,  0.5f,-0.5f,  0.5f,  0.5f,-0.5f, -0.5f,  0.5f,-0.5f, -0.5f, -0.5f,-0.5f,
         0.5f, -0.5f, 0.5f,  0.5f, -0.5f,-0.5f,  0.5f,  0.5f,-0.5f,  0.5f,  0.5f,-0.5f,  0.5f,  0.5f, 0.5f,  0.5f, -0.5f, 0.5f,
        -0.5f,  0.5f, 0.5f, -0.5f,  0.5f,-0.5f, -0.5f, -0.5f,-0.5f, -0.5f, -0.5f,-0.5f, -0.5f, -0.5f, 0.5f, -0.5f,  0.5f, 0.5f,
        -0.5f, -0.5f,-0.5f,  0.5f, -0.5f,-0.5f,  0.5f, -0.5f, 0.5f,  0.5f, -0.5f, 0.5f, -0.5f, -0.5f, 0.5f, -0.5f, -0.5f,-0.5f,
        -0.5f,  0.5f,-0.5f,  0.5f,  0.5f,-0.5f,  0.5f,  0.5f, 0.5f,  0.5f,  0.5f, 0.5f, -0.5f,  0.5f, 0.5f, -0.5f,  0.5f,-0.5f
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)screenWidth / (GLfloat)screenHeight, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window)) {
        Inputs(window);
        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ourShader.Use();
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 modelTemp, modelTemp2, f1_to_f2, f2_to_f3;

        view = glm::translate(view, glm::vec3(movX, movY, movZ));
        view = glm::rotate(view, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));

        GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
        GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
        GLint uniformColor = glGetUniformLocation(ourShader.Program, "color");

        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glBindVertexArray(VAO);

        // --- BRAZO ---
        model = glm::rotate(model, glm::radians(hombro), glm::vec3(0, 0, 1));
        modelTemp = model = glm::translate(model, glm::vec3(1.5f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(3.0f, 1.0f, 1.0f));
        glUniform3f(uniformColor, 0.0f, 1.0f, 0.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // --- ANTEBRAZO (Rojo) ---
        // 1. Posicionamos al final del bicep
        model = glm::translate(modelTemp, glm::vec3(1.5f, 0.0f, 0.0f));
        // 2. Flexión del codo (Eje Z para que suba y baje)
        model = glm::rotate(model, glm::radians(codo), glm::vec3(0.0f, 0.0f, 1.0f));
        // Se guarda la base del antebrazo para la jerarquía de la mano
        modelTemp = model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));

        // 3. Dibujamos el antebrazo 
        glm::mat4 modelAntebrazo = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f));
        glUniform3f(uniformColor, 1.0f, 0.0f, 0.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelAntebrazo));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // --- MUÑECA Y PALMA (Blanca) ---
        // 1. Nos movemos al final del antebrazo (pivote de la muñeca)
        model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));

        // 2. APLICAMOS LAS ROTACIONES DE LA MUÑECA 
        // Rotación Eje X: Pronación/Supinación
        model = glm::rotate(model, glm::radians(rotPalma), glm::vec3(1.0f, 0.0f, 0.0f));
        // Flexión normal de la muñeca (Eje Z)
        model = glm::rotate(model, glm::radians(muneca), glm::vec3(0.0f, 0.0f, 1.0f));

        // 3. Dibujamos la palma
        modelTemp2 = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
        glm::mat4 modelPalma = glm::scale(model, glm::vec3(0.5f, 1.0f, 1.0f));
        glUniform3f(uniformColor, 1.0f, 1.0f, 1.0f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPalma));
        glDrawArrays(GL_TRIANGLES, 0, 36);

      
        // ================= DEDO 1 (ÍNDICE) =================
        model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, 0.375f));
        model = glm::rotate(model, glm::radians(-dedo1A), glm::vec3(0.0f, 0.0f, 1.0f));
        f1_to_f2 = model = glm::translate(model, glm::vec3(0.3f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.6f, 0.28f, 0.22f));
        glUniform3f(uniformColor, 0.0f, 1.0f, 1.0f); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::translate(f1_to_f2, glm::vec3(0.3f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-dedo1B), glm::vec3(0.0f, 0.0f, 1.0f));
        f2_to_f3 = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.25f, 0.2f));
        glUniform3f(uniformColor, 1.0f, 0.0f, 1.0f); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::translate(f2_to_f3, glm::vec3(0.25f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-dedo1C), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(0.2f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.22f, 0.18f));
        glUniform3f(uniformColor, 0.0f, 0.5f, 0.5f); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // ================= DEDO 2 (MEDIO) =================
        model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, 0.13f));
        model = glm::rotate(model, glm::radians(-dedo2A), glm::vec3(0.0f, 0.0f, 1.0f));
        f1_to_f2 = model = glm::translate(model, glm::vec3(0.35f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.7f, 0.28f, 0.22f));
        glUniform3f(uniformColor, 1.0f, 1.0f, 0.0f); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::translate(f1_to_f2, glm::vec3(0.35f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-dedo2B), glm::vec3(0.0f, 0.0f, 1.0f));
        f2_to_f3 = model = glm::translate(model, glm::vec3(0.3f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.6f, 0.25f, 0.2f));
        glUniform3f(uniformColor, 1.0f, 0.5f, 0.0f); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::translate(f2_to_f3, glm::vec3(0.3f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-dedo2C), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.22f, 0.18f));
        glUniform3f(uniformColor, 0.4f, 0.2f, 0.0f); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // ================= DEDO 3 (ANULAR) =================
        model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, -0.12f));
        model = glm::rotate(model, glm::radians(-dedo3A), glm::vec3(0.0f, 0.0f, 1.0f));
        f1_to_f2 = model = glm::translate(model, glm::vec3(0.3f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.6f, 0.28f, 0.22f));
        glUniform3f(uniformColor, 1.0f, 0.0f, 0.5f); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::translate(f1_to_f2, glm::vec3(0.3f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-dedo3B), glm::vec3(0.0f, 0.0f, 1.0f));
        f2_to_f3 = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.25f, 0.2f));
        glUniform3f(uniformColor, 0.6f, 0.0f, 0.6f); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::translate(f2_to_f3, glm::vec3(0.25f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-dedo3C), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(0.2f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.22f, 0.18f));
        glUniform3f(uniformColor, 0.8f, 0.4f, 0.8f); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // ================= DEDO 4 (MEÑIQUE) =================
        model = glm::translate(modelTemp2, glm::vec3(0.25f, 0.35f, -0.37f));
        model = glm::rotate(model, glm::radians(-dedo4A), glm::vec3(0.0f, 0.0f, 1.0f));
        f1_to_f2 = model = glm::translate(model, glm::vec3(0.25f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.25f, 0.2f));
        glUniform3f(uniformColor, 0.0f, 0.0f, 1.0f); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::translate(f1_to_f2, glm::vec3(0.25f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-dedo4B), glm::vec3(0.0f, 0.0f, 1.0f));
        f2_to_f3 = model = glm::translate(model, glm::vec3(0.2f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.4f, 0.2f, 0.15f));
        glUniform3f(uniformColor, 0.0f, 0.8f, 1.0f); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        model = glm::translate(f2_to_f3, glm::vec3(0.2f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-dedo4C), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.3f, 0.15f, 0.1f));
        glUniform3f(uniformColor, 0.0f, 0.2f, 0.5f); glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // ================= Dedo 5 (PULGAR) =================
        // Falange A (Base)
        model = glm::translate(modelTemp2, glm::vec3(0.1f, 0.0f, 0.6f));

        // Inclinación inicial y rotación de control en Y
        model = glm::rotate(model, glm::radians(-25.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(dedo5A), glm::vec3(0.0f, 1.0f, 0.0f));

        f1_to_f2 = model = glm::translate(model, glm::vec3(0.2f, 0.0f, 0.0f)); // Traslación más corta
        model = glm::scale(model, glm::vec3(0.4f, 0.22f, 0.18f)); // <--- MÁS DELGADO Y CORTO
        glUniform3f(uniformColor, 0.6f, 0.6f, 0.6f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Falange B (Punta)
        model = glm::translate(f1_to_f2, glm::vec3(0.2f, 0.0f, 0.0f)); // Se conecta exacto al final de A
        model = glm::rotate(model, glm::radians(dedo5B), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.35f, 0.2f, 0.15f)); // <--- PUNTA MUY FINA
        glUniform3f(uniformColor, 0.8f, 0.8f, 0.8f);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}

void Inputs(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

    // Controles de Cámara y Brazo (Se mantienen igual)
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) movX += 0.08f;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) movX -= 0.08f;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) movY += 0.08f;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) movY -= 0.08f;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) movZ -= 0.08f;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) movZ += 0.08f;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) rot += 0.18f;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) rot -= 0.18f;
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) hombro += 0.18f;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) hombro -= 0.18f;
    // Controles de Codo (G y T)
    // Limitamos de 0 a 140 grados para una flexión natural
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) if (codo < 110.0f) codo += 0.5f;
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) if (codo > 0.0f) codo -= 0.5f;
    // Controles de Muñeca (H y Y)
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        if (muneca < 70.0f) muneca += 0.5f; // Flexión hacia arriba
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
        if (muneca > -80.0f) muneca -= 0.5f; // Extensión hacia abajo
    }
    // ROTACIÓN PALMA IZQUIERDA 
    if (glfwGetKey(window, GLFW_KEY_COMMA) == GLFW_PRESS) {
        if (rotPalma < 90.0f) rotPalma += 0.5f; // Gira palma hacia arriba/derecha
    }
    if (glfwGetKey(window, GLFW_KEY_PERIOD) == GLFW_PRESS) {
        if (rotPalma > -180.0f) rotPalma -= 0.5f; // Límite para que no gire de más hacia afuera
    }

    // RESTRICCIONES DEDOS (0 a 90 grados)
    // Dedo 1 (Índice) - Solo rotan falanges B y C
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) if (dedo1B < 90.0f) dedo1B += 0.5f;
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) if (dedo1B > 0.0f)  dedo1B -= 0.5f;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) if (dedo1C < 90.0f) dedo1C += 0.5f;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) if (dedo1C > 0.0f)  dedo1C -= 0.5f;

    // Dedo 2 (Medio)
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) if (dedo2B < 90.0f) dedo2B += 0.5f;
    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) if (dedo2B > 0.0f)  dedo2B -= 0.5f;
    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) if (dedo2C < 90.0f) dedo2C += 0.5f;
    if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS) if (dedo2C > 0.0f)  dedo2C -= 0.5f;

    // Dedo 3 (Anular)
    if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS) if (dedo3B < 90.0f) dedo3B += 0.5f;
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) if (dedo3B > 0.0f)  dedo3B -= 0.5f;
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) if (dedo3C < 90.0f) dedo3C += 0.5f;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) if (dedo3C > 0.0f)  dedo3C -= 0.5f;

    // Dedo 4 (Meñique)
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) if (dedo4B < 90.0f) dedo4B += 0.5f;
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) if (dedo4B > 0.0f)  dedo4B -= 0.5f;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) if (dedo4C < 90.0f) dedo4C += 0.5f;
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) if (dedo4C > 0.0f)  dedo4C -= 0.5f;

    // CONTROL PULGAR (Solo Punta para no chocar con palma)
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        if (dedo5B < 70.0f) dedo5B += 0.5f; // Cierra la punta
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        if (dedo5B > 0.0f) dedo5B -= 0.5f;  // Abre la punta
    }
}