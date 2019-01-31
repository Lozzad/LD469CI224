#pragma once
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
#include <vector>

// Include GLM
#include <glm/glm.hpp>
using namespace glm;
#include "common/objloader.hpp"
#include "common/vboindexer.hpp"
#include <glm/gtx/transform.hpp>
#include "common/shader.hpp"
#include "common/controls.hpp"
#include "common/text2D.hpp"
#include "common/texture.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "Model.hpp"

class View {
private:
    
    GLFWwindow* window;
    
    GLuint programID;
	Model* model;
    
    GLuint VertexArrayID;
	//GLuint colorbuffer;

	glm::mat4 viewM = glm::lookAt(
		glm::vec3(0, 20, 10),	//camera pos
		glm::vec3(0, 0, 0),		//look where?
		glm::vec3(0, 1, 0)		//orientation
	);
	

    //screen size
    int width;
    int height;
    
	GLuint MatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;
	GLuint LightID;
	glm::vec3 lightPosition;

    
public:
    View(Model* mymodel, int widtht, int heightt);
    ~View();
	glm::mat4 getMVPMatrix(glm::mat4 ModelMatrix);
    int initialise();
    void update();
	void EnableBindPointer(int key, GLuint buffer);
	GLFWwindow* getWindow() { return window; }
};

