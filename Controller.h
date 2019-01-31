#pragma once
#include <GL/glew.h>
#include <glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include "View.hpp"

class Controller {
private:
	View* view;
	Model* model;
	float speed = 0.1f;
public:
	Controller(View* myview, Model* mymodel);
	~Controller();
	void loop();
};