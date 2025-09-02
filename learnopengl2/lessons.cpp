#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//vertex shader stored in a const C string (GLSL)
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";
//fragment shader stored in a const C string (GLSL)
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";


void framebuffer_size_callback(GLFWwindow*, int width, int height);
void processInput(GLFWwindow* window);

int main() //int tells us that the main function returns a value of type int
{
	glfwInit(); //initializes glfw 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //configs glfw
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//first argument = option we want to configure
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // second argument = an int that sets the value of our option
	//We're telling GLFW that version 3.3 is the OpenGL version we want to use
	//We set both major and minor versions to 3
	//We also tell GLFW we want to explicitly use the core profile

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}



	//VERTEX SHADER//
	//store shader object
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//attach the shader source code to the shader object and compile shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //(shader_obj_to_compile, how_man_str_we_pass_as_src_code, actual_src_code_of_vertex_shader, NULL)
	glCompileShader(vertexShader);


	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//FRAGMENT SHADER//
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}



	//LINK SHADERS //
	//linking vertex and fragment shaders to shader program
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram(); //creates a program and teturns the ID rederence to the newly created program object
	//attaching compiled shaders to the program object then link them to glLinkProgram
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	//delete shader objects once we've linked them into the program object
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);



	//SET UP VERTEX DATA (AND BUFFER(S)) AND CONFIGURE VERTEX ATTRIBUTES

	//VERTEX DATA
	//define the vertices of our triangle in normalized device coordinates
	/*  
	float vertices[15] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.0f, -0.5f, 0.0f,
		-0.25f, 0.5f, 0.0f,
		 0.25f, 0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f

	};
	*/
	float vertices[] = {
		// first triangle
		-0.9f, -0.5f, 0.0f,  // left 
		-0.0f, -0.5f, 0.0f,  // right
		-0.45f, 0.5f, 0.0f,  // top 
		// second triangle
		 0.0f, -0.5f, 0.0f,  // left
		 0.9f, -0.5f, 0.0f,  // right
		 0.45f, 0.5f, 0.0f   // top 
	};
	
	/* 
	float vertices[] = {
	 0.5f,  0.5f, 0.0f,  // top right
	 0.5f, -0.5f, 0.0f,  // bottom right
	-0.5f, -0.5f, 0.0f,  // bottom left
	-0.5f,  0.5f, 0.0f   // top left 
	};
	*/
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};
	
	//
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	//bind Vertex Array Object first, then bind and set vertex buffer(s),
	//and then configure vertex attribue(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO); //binding new buffer to the GL_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //copying the previously defined vertex data into the buffer's memory
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//telling OpenGL how to interpret the vertex data (per vertex attribute) (VAO)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	


	while (!glfwWindowShouldClose(window)) // "render loop" ~ so that program doessn't draw a single image and then immediately quit and close the window
	{
		//input
		processInput(window);
		//rendering commands (what we wantt to happen every frame of the loop

		//at start of frame we want to clear the screen, otherwise we'd still see results form the previous frams

		glClearColor(0.0f, 0.0f, 0.3f, 1.0f); //state-setting function //specifies which color is used after the buffers are clear
		glClear(GL_COLOR_BUFFER_BIT);         //state-using function //clears buffer
		//buffers start off default (sayblack), we set color it will be when clear (red), we clear, now the "default" is red. Set it to red, clear, set, clear, etcc...
		
		//the triangle we've all been waiting for
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//check and call event and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	// optional: de-allocate all resources once they've outlived their purpose:
   // ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}