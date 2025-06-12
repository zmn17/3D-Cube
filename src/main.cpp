#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <ostream>
#include "../external/include/WindowManager.h"
#include "../external/include/Utils.h"
#include "../external/include/Imgui_lib.h"

#define numVao 1
#define numVbo 1
#define numEbo 1

// window view port
const int width = 800;
const int height = 600;

// Buffers
GLuint vao[numVao];
GLuint vbo[numVbo];
GLuint ebo[numEbo];

GLuint shaderProgram;

// uniform variable
GLuint uniScale;
GLuint uniTex;

// uniform variable
GLuint projLoc, mvLoc;

// cammera nd object location
float cameraX, cameraY, cameraZ;
float cubeX, cubeY, cubeZ;

// perspective matrix attribute
float aspect;
float fov = 1.0472f;
float zNear = 0.1f;
float zFar = 1000.0f;
int h, w;

// matrices
glm::mat4 pMat, mMat, vMat, mvMat;


GLuint tex;

// shader source file
const char* vp = "/home/zee/dev/OpenGL_series/5_cube/shaders/vShader.glsl";
const char* fp = "/home/zee/dev/OpenGL_series/5_cube/shaders/fShader.glsl";

void setupRectangle(){
	// setup vertices of the rectangle
	// 200, 150 -> top left
	// 300, 150 -> top right
	// 300, 250 -> bottom right
	// 200, 250 -> bottom left
	// NDC_x = (pixel_x / width) * 2.0f - 1.0f
	// NDC_x = (pixel_y / height) * 2.0f
	
	// ======================= Rect vertex positions without EBO
	// float rec[] = {
	// 	// Triangle 1
	// 	-0.5f,  0.5f, 0.0f,			// bottom left		index = 0
	// 	-0.25f, 0.5f, 0.0f,			// bottom right		index = 1
	// 	-0.25f, 0.1667f, 0.0f,		// top right		index = 2
	//
	// 	// Triangle 2
	// 	-0.25f, 0.1667f, 0.0f,    // top right		index = 2
	// 	-0.5f,  0.1667f, 0.0f,   // top left			index = 3
	// 	-0.5f,  0.5f, 0.0f		// bottom left		index = 0
	// };
	
	float rec[] = {
		//           COORDINATES														COLORS											
		-0.5f,  0.5f, 0.0f,			/* bottom left		index = 0*/		0.78f, 0.62f, 0.91f,			 
		-0.25f, 0.5f, 0.0f,		/* bottom right		index = 1*/		0.73f, 0.48f, 0.48f,			
		-0.25f, 0.1667f, 0.0f,	/* top right		index = 2*/		0.73f, 0.67f, 0.47f,		
		-0.5f,  0.1667f, 0.0f,	/* top left			index = 3*/		0.98f, 0.74f, 0.0f,			
	};

	GLuint indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	// generate VAO
	glGenVertexArrays(numVao, vao);
	// bind 
	glBindVertexArray(vao[0]);
	
	// generate VBO
	glGenBuffers(numVbo, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rec), rec, GL_STATIC_DRAW);

	// generate EBO
	glGenBuffers(numEbo, ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Specifies how OpenGL should interpret the vertex data in your VBO (Vertex Buffer Object) for one attribute.
	// Tell OpenGL how to walk through your VBO data — e.g., every 6 floats: first 3 are position, next 3 are color.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	// Enables a specific attribute index for use during rendering.
	glEnableVertexAttribArray(0); // pos -> location = 0
	
	// for color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void * )(6 * sizeof(float)));
	// glEnableVertexAttribArray(2);
	
	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void setupCube(){
	float vertexPositions[216] = {
		//        COORDINATES           //     COLORS (randomized)
		-1.0f,  1.0f, -1.0f,            0.87f, 0.23f, 0.45f,
		-1.0f, -1.0f, -1.0f,           0.12f, 0.98f, 0.66f,
		 1.0f, -1.0f, -1.0f,           0.74f, 0.33f, 0.88f,
		 1.0f, -1.0f, -1.0f,           0.92f, 0.55f, 0.10f,
		 1.0f,  1.0f, -1.0f,           0.04f, 0.78f, 0.60f,
		-1.0f,  1.0f, -1.0f,           0.31f, 0.20f, 0.91f,

		 1.0f, -1.0f, -1.0f,           0.50f, 0.89f, 0.41f,
		 1.0f, -1.0f,  1.0f,           0.75f, 0.14f, 0.62f,
		 1.0f,  1.0f, -1.0f,           0.15f, 0.49f, 0.94f,
		 1.0f, -1.0f,  1.0f,           0.27f, 0.88f, 0.12f,
		 1.0f,  1.0f,  1.0f,           0.63f, 0.40f, 0.77f,
		 1.0f,  1.0f, -1.0f,           0.93f, 0.71f, 0.16f,

		 1.0f, -1.0f,  1.0f,           0.09f, 0.24f, 0.98f,
		-1.0f, -1.0f,  1.0f,           0.43f, 0.84f, 0.37f,
		 1.0f,  1.0f,  1.0f,           0.68f, 0.30f, 0.51f,
		-1.0f, -1.0f,  1.0f,           0.56f, 0.12f, 0.89f,
		-1.0f,  1.0f,  1.0f,           0.80f, 0.56f, 0.17f,
		 1.0f,  1.0f,  1.0f,           0.03f, 0.95f, 0.29f,

		-1.0f, -1.0f,  1.0f,           0.21f, 0.67f, 0.85f,
		-1.0f, -1.0f, -1.0f,           0.59f, 0.22f, 0.48f,
		-1.0f,  1.0f,  1.0f,           0.98f, 0.08f, 0.35f,
		-1.0f, -1.0f, -1.0f,           0.13f, 0.72f, 0.44f,
		-1.0f,  1.0f, -1.0f,           0.40f, 0.31f, 0.90f,
		-1.0f,  1.0f,  1.0f,           0.25f, 0.86f, 0.05f,

		-1.0f, -1.0f,  1.0f,           0.60f, 0.02f, 0.99f,
		 1.0f, -1.0f,  1.0f,           0.70f, 0.77f, 0.34f,
		 1.0f, -1.0f, -1.0f,           0.96f, 0.17f, 0.28f,
		 1.0f, -1.0f, -1.0f,           0.18f, 0.52f, 0.92f,
		-1.0f, -1.0f, -1.0f,           0.88f, 0.61f, 0.11f,
		-1.0f, -1.0f,  1.0f,           0.46f, 0.38f, 0.73f,

		-1.0f,  1.0f, -1.0f,           0.83f, 0.26f, 0.54f,
		 1.0f,  1.0f, -1.0f,           0.19f, 0.69f, 0.36f,
		 1.0f,  1.0f,  1.0f,           0.39f, 0.08f, 0.97f,
		 1.0f,  1.0f,  1.0f,           0.66f, 0.99f, 0.20f,
		-1.0f,  1.0f,  1.0f,           0.54f, 0.13f, 0.81f,
		-1.0f,  1.0f, -1.0f,           0.77f, 0.91f, 0.07f
	};

	glGenVertexArrays(numVao, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(numVbo, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), &vertexPositions, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void init(){
	shaderProgram = Utils::createShaderProgram(vp, fp);

	// get the location of uniform variable
	// uniScale = glGetUniformLocation(shaderProgram, "scale");

	cameraX = 0.0f;
	cameraY = 0.0f;
	cameraZ = 8.0f;
	
	cubeX = 0.0f;
	cubeY = -2.0f;
	cubeZ = 0.0f;

	mvLoc = glGetUniformLocation(shaderProgram, "mvMat");
	projLoc = glGetUniformLocation(shaderProgram, "projMat");

	setupCube();
}

void display(GLFWwindow* win){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// rgba(123, 65, 168, 1)
	glClearColor(123.0f/255.0f, 65.0f/255.0f, 168.0f/255.0f, 1.0f);

	// use the shadder program
	glUseProgram(shaderProgram);
	
	// pass the data to the uniform variable in shaders
	glUniform1f(uniScale, 1.5f);

	// build the perspective matrix
	glfwGetFramebufferSize(win,&w, &h);
	aspect = (float)w / (float)h;
	pMat = glm::perspective(fov, aspect, zNear, zFar);
	
	// pass the perspective matrix to shader
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	// build the view,model,view-model matrices
	glm::mat4 model = glm::mat4(1.0f);
	vMat = glm::translate(model, glm::vec3(-cameraX, -cameraY, -cameraZ));
	mMat = glm::translate(model, glm::vec3(cubeX, cubeY, cubeZ));
	mvMat = vMat * mMat;
	
	// pass the mv matrix to shader
	glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(mvMat));

	// glBindTexture(GL_TEXTURE_2D, gothTexture);
	glBindVertexArray(vao[0]);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	// glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
 }

int main(){
	// GLFW init and window creation 
	WindowManager* winmanager = new WindowManager(width, height, "CUBE");
	GLFWwindow* win = winmanager->getWindow();

	// ImGui init
	Imgui imgui(win, "#version 460");

	glViewport(0,0, 800,800);

	init();

	GLint numAttribs;
	glGetProgramiv(shaderProgram, GL_ACTIVE_ATTRIBUTES, &numAttribs);
	for(int i = 0; i < numAttribs; i++){
		char name[64];
		GLsizei len;
		GLint size;
		GLenum type;
		glGetActiveAttrib(shaderProgram, 0, sizeof(name), &len, &size, &type, name);
		std::cout<<"Attribute: " << i << ": " << name << std::endl;
	}


	while(!glfwWindowShouldClose(win)){
		glfwPollEvents();

		// add new frame
		imgui.NewFrame();

		ImGui::Begin("Cube Control");
		// Cube controls
		ImGui::SliderFloat3("Position X", &cubeX, 0.0f, 5.0f);
		ImGui::SliderFloat3("Position Y", &cubeY, 0.0f, 0.0f);
		ImGui::SliderFloat3("Position Z", &cubeZ, 0.0f, 0.0f);

		// Camera controls
		ImGui::SliderFloat("Camera X", &cameraX, 0.0f, 0.0f);
		ImGui::SliderFloat("Camera Y", &cameraY, 0.0f, 0.0f);
		ImGui::SliderFloat("Camera Z", &cameraZ, 0.0f, 0.0f);

		display(win);

		imgui.render_imgui();

		glfwSwapBuffers(win);
	}

	winmanager->destroy(win);
	delete winmanager;
	imgui.Shutdown();

	return 0;
}
