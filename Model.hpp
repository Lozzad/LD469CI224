#pragma once
// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
#include <vector>

#include "Model.hpp"
#include <cmath>

// Include GLM
#include <glm/glm.hpp>
using namespace glm;

#include "GameObject.hpp"

class Model {
private:
	GameObject* board;
	GameObject* player;
	std::vector<GameObject*> walls;
	glm::vec3 startpos;

	void Model::createGameObjects();

public:
	Model();
	~Model();
	//getters n setters
	GameObject* getPlayer();
	GameObject* getBoard();
	void updatePlayer(glm::vec3 move);
	void updateCollectables();
	bool checkCollisions(GameObject * a, GameObject * b);
	bool playerTouchingWall();

};

