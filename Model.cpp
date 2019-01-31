#include "Model.hpp"


void Model::createGameObjects() {
	//create player
	player = new GameObject();
	player->loadObject("ball.obj");
	player->createBoundingBox();
	player->setPosition(glm::vec3(3, 0, 3));
	player->fillColour(glm::vec3(1, 0, 1));
	//create the stage
	board = new GameObject();
	board->loadObject("box.obj");
	board->createBoundingBox();
	board->setPosition(glm::vec3(0, 0, 0));
	board->fillColour(glm::vec3(0, 1, 0));
}

Model::Model() {
	createGameObjects();
}
Model::~Model() {

}

GameObject* Model::getPlayer()
{
	return player;
}

GameObject * Model::getBoard()
{
	return board;
}

void Model::updatePlayer(glm::vec3 move){
	player->updatePosition(move);
}

void Model::updateCollectables(){
}

bool Model::checkCollisions(GameObject* a, GameObject* b) {
	
	//tried this but didnt work v well
	if (a->getPosition().x + a->getBoundingBox()->getMax().x >= b->getPosition().x + b->getBoundingBox()->getMin().x		//if max x of a is greater than min x of b
		&& a->getPosition().x + a->getBoundingBox()->getMin().x <= b->getPosition().x + b->getBoundingBox()->getMax().x) 	//and min x of a is less than max x of b
	{
		if (a->getPosition().y + a->getBoundingBox()->getMax().y >= b->getPosition().y + b->getBoundingBox()->getMin().y		//if max y of a is greater than min y of b
			&& a->getPosition().y + a->getBoundingBox()->getMin().y <= b->getPosition().y + b->getBoundingBox()->getMax().y)	//and min y of a is less than max y of b
		{
			if (a->getPosition().z + a->getBoundingBox()->getMax().z >= b->getPosition().z + b->getBoundingBox()->getMin().z		//if max z of a is greater than min x of b
				&& a->getPosition().z + a->getBoundingBox()->getMin().z <= b->getPosition().z + b->getBoundingBox()->getMax().z)	//and min z of a is less than max x of b
			{
				return true;
			}
		}
	}
	return false; 
}

bool Model::playerTouchingWall()
{
	return checkCollisions(player, board);
}
