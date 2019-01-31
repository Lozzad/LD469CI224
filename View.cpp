#include "View.hpp"

View::View(Model* mymodel, int widtht, int heightt) {
	width = widtht;
	height = heightt;
	model = mymodel;
}
View::~View() {
	//cleanup VBO and shader
	glDeleteBuffers(1, model->getPlayer()->getVertexBuffer());
	glDeleteBuffers(1, model->getBoard()->getVertexBuffer());
	glDeleteProgram(programID);
	glDeleteVertexArrays(1, &VertexArrayID);
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

glm::mat4 View::getMVPMatrix(glm::mat4 ModelMatrix) {
	glm::mat4 Projection = glm::perspective(
		glm::radians(45.0f),
		1.0f,
		0.1f,
		100.0f
		);
	glm::mat4 Model = ModelMatrix;
	glm::mat4 mvp = Projection * viewM * Model;

	return mvp;
}

int View::initialise() {

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "My Game", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//for getting the esc key
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	//hide mouse and enable unlimited movement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//set mouse at center screen
	glfwPollEvents();
	glfwSetCursorPos(window, width / 2, height / 2);

	// Initialize GLEW
	glewExperimental = GL_TRUE; //needed for core profile

	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	
	//we set the cursor at the centre so that it always start on our origin (0,0,0)
	glfwSetCursorPos(window, width / 2, height / 2);


	// blue? background
	glClearColor(0.9f, 0.9f, 0.9f, 0.0f);

	//define vertex array
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	//send vert info for player to GL using vertex buffer
	glGenBuffers(1, model->getPlayer()->getVertexBuffer());
	glBindBuffer(GL_ARRAY_BUFFER, *model->getPlayer()->getVertexBuffer());
	glBufferData(GL_ARRAY_BUFFER, model->getPlayer()->getVertices().size() * sizeof(glm::vec3), &model->getPlayer()->getVertices()[0], GL_STATIC_DRAW);
	//normal info 
	glGenBuffers(1, model->getPlayer()->getNormalBuffer());
	glBindBuffer(GL_ARRAY_BUFFER, *model->getPlayer()->getNormalBuffer());
	glBufferData(GL_ARRAY_BUFFER, model->getPlayer()->getNormals().size() * sizeof(glm::vec3), &model->getPlayer()->getNormals()[0], GL_STATIC_DRAW);
	//colors 
	glGenBuffers(1, model->getPlayer()->getColorBuffer());
	glBindBuffer(GL_ARRAY_BUFFER, *model->getPlayer()->getColorBuffer());
	glBufferData(GL_ARRAY_BUFFER, model->getPlayer()->getColors().size() * sizeof(glm::vec3), &model->getPlayer()->getColors()[0], GL_STATIC_DRAW);
	
	//same 4 the maze
	glGenBuffers(1, model->getBoard()->getVertexBuffer());
	glBindBuffer(GL_ARRAY_BUFFER, *model->getBoard()->getVertexBuffer());
	glBufferData(GL_ARRAY_BUFFER, model->getBoard()->getVertices().size() * sizeof(glm::vec3), &model->getBoard()->getVertices()[0], GL_STATIC_DRAW);
	//normals
	glGenBuffers(1, model->getBoard()->getNormalBuffer());
	glBindBuffer(GL_ARRAY_BUFFER, *model->getBoard()->getNormalBuffer());
	glBufferData(GL_ARRAY_BUFFER, model->getBoard()->getNormals().size() * sizeof(glm::vec3), &model->getBoard()->getNormals()[0], GL_STATIC_DRAW);
	//colors 
	glGenBuffers(1, model->getBoard()->getColorBuffer());
	glBindBuffer(GL_ARRAY_BUFFER, *model->getBoard()->getColorBuffer());
	glBufferData(GL_ARRAY_BUFFER, model->getBoard()->getColors().size() * sizeof(glm::vec3), &model->getBoard()->getColors()[0], GL_STATIC_DRAW);


	programID = LoadShaders("SimpleVertexShader.hlsl", "SimpleFragmentShader.hlsl");

	MatrixID = glGetUniformLocation(programID, "MVP");
	ViewMatrixID = glGetUniformLocation(programID, "M");
	ModelMatrixID = glGetUniformLocation(programID, "M");
	LightID = glGetUniformLocation(programID, "LightPosition_ws");
	lightPosition = glm::vec3(-4, 9, 0);

	return 1;
}

void View::update() {
		// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		glEnable(GL_DEPTH_TEST);
		
		//use our shaders
		glUseProgram(programID);
		glUniform3f(LightID, lightPosition.x, lightPosition.y, lightPosition.z);
		
		glm::mat4 ModelMatrixPlayer = glm::translate(model->getPlayer()->getPosition()) * glm::scale(model->getPlayer()->getScale());

		glm::mat4 mvp1 = getMVPMatrix(ModelMatrixPlayer);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp1[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &viewM[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrixPlayer[0][0]);


		//verts
		EnableBindPointer(0, *model->getPlayer()->getVertexBuffer());
		//colors
		EnableBindPointer(1, *model->getPlayer()->getColorBuffer());
		//normals
		EnableBindPointer(2, *model->getPlayer()->getNormalBuffer());
		//the mesh is then drawn
		glDrawArrays(GL_TRIANGLES, 0, model->getPlayer()->getVertices().size());

		//end of drawing the player

		glm::mat4 ModelMatrixMaze = glm::translate(model->getBoard()->getPosition()) * glm::scale(model->getBoard()->getScale());

		glm::mat4 mvp2 = getMVPMatrix(ModelMatrixMaze);

		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &mvp2[0][0]);
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &viewM[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrixMaze[0][0]);
		//verts
		EnableBindPointer(0, *model->getBoard()->getVertexBuffer());
		//colors
		EnableBindPointer(1, *model->getBoard()->getColorBuffer());
		//normals
		EnableBindPointer(2, *model->getBoard()->getNormalBuffer());

		glDrawArrays(GL_TRIANGLES, 0, model->getBoard()->getVertices().size());

		//end of drawing stage
		
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
}

//cutting down on code reuse
void View::EnableBindPointer(int key, GLuint buffer) {
	glEnableVertexAttribArray(key);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glVertexAttribPointer(key, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}


