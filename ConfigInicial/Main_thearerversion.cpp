/* Nombre: Chavez Serna Joana Valeria
* Fecha de entrega: 13 de Febrero 2026
* No de cuenta: 320267561
* Práctica: 1 Configuración inicial
*/

#include<iostream>

// Incluimos GLEW para poder acceder a las funciones modernas de OpenGL
#include <GL/glew.h>
// Incluimos GLFW que es la que nos ayuda a crear la ventana y manejar el teclado/mouse
#include <GLFW/glfw3.h>

// Definimos el tamaño de nuestra ventana
const GLint WIDTH = 800, HEIGHT = 600;

// Shaders: Son pequeños programas que corren directo en la tarjeta de video (GPU)
// Este Vertex Shader se encarga de posicionar los puntos (vértices) en el espacio
const GLchar* vertexShaderSource =
{
	"#version 330 core\n"
	"layout (location = 0) in vec3 position;\n"
	"void main()\n"
	"{\n"
	"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
	"}\0"
};

// Este Fragment Shader le da color a los píxeles (en este caso un tono naranja/naranja rojizo)
const GLchar* fragmentShaderSource =
{
	"#version 330 core\n"
	"out vec4 color;\n"
	"void main()\n"
	"{\n"
	"color = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0"
};

// Prototipo de la función para compilar los shaders
void CrearShader(void);

int main() {
	// Paso 1: Inicializar GLFW
	glfwInit();

	// Configuramos OpenGL para usar la versión 3.3 en el perfil core (el más moderno y eficiente)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Necesario para Mac, pero bueno ponerlo siempre
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // Por ahora dejamos la ventana fija

	// Creamos la ventana con mi nombre como dice la práctica
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Chavez Serna Joana Valeria", nullptr, nullptr);

	int screenWidth, screenHeight;
	// Obtenemos el tamaño real del buffer para que el dibujo no se vea estirado
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	// Si no se pudo crear la ventana, cerramos el programa
	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return EXIT_FAILURE;
	}

	// Hacemos que la ventana actual sea el contexto donde OpenGL va a dibujar
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE; // Habilitamos características modernas de GLEW

	// Inicializamos GLEW y checamos que todo esté en orden
	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// imprimimos los datos de nuestra GPU y la versión que estamos usando
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	// Le decimos a OpenGL el área donde va a renderizar
	glViewport(0, 0, screenWidth, screenHeight);

	// Aquí se llamaría a CrearShader para compilar el código de arriba
	// CrearShader();

	// Definimos las coordenadas del triángulo (X, Y, Z) entre -1 y 1
	GLfloat vertices[] =
	{
		-0.5f, -0.5f, 0.0f, // Esquina Izquierda (Abajo)
		 0.5f, -0.5f, 0.0f, // Esquina Derecha (Abajo)
		 0.0f,  0.5f, 0.0f  // Arriba (Centro)
	};

	// VBO: Buffer que guarda los datos de los vértices en la GPU
	// VAO: Objeto que recuerda cómo están organizados esos datos
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Empezamos a configurar el VAO
	glBindVertexArray(VAO);

	// Pasamos los datos del arreglo de vértices al buffer de la GPU
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Le explicamos a OpenGL que los datos son de 3 en 3 (floats) y empiezan en el índice 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Desactivamos los buffers para no moverles por error después
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Ciclo principal: Mientras no cerremos la ventana...
	while (!glfwWindowShouldClose(window))
	{
		// Revisamos eventos como cerrar la ventana o teclas
		glfwPollEvents();

		// Limpiamos la pantalla con un color azul/verdoso oscuro (fondo)
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Usamos el VAO que configuramos antes
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3); // Dibuja el triángulo con 3 vértices
		glBindVertexArray(0);

		// Cambiamos el buffer (lo que se dibujó atrás pasa al frente para verse)
		glfwSwapBuffers(window);
	}

	// Limpieza final al cerrar
	glfwTerminate();
	return EXIT_SUCCESS;
}

// Función para compilar y enlazar los Shaders (nuestra pequeña fábrica de gráficos)
void CrearShader()
{
	// 1. Compilamos el Vertex Shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	// Checamos si hubo errores de compilación
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR AL COMPILAR VERTEX SHADER:\n" << infoLog << std::endl;
	}

	// 2. Compilamos el Fragment Shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// Checamos errores
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR AL COMPILAR FRAGMENT SHADER:\n" << infoLog << std::endl;
	}

	// 3. Unimos ambos shaders en un Programa de Shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Verificamos que el enlace (linking) haya funcionado
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR AL ENLAZAR PROGRAMA DE SHADER:\n" << infoLog << std::endl;
	}

	// Ya que están en el programa, borramos los shaders individuales para liberar espacio
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
