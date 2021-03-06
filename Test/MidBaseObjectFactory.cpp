#include "MidBaseObjectFactory.h"


MidBaseObjectFactory::MidBaseObjectFactory()
{
}


MidBaseObjectFactory::~MidBaseObjectFactory()
{
}


GameObject * MidBaseObjectFactory::Spawn(uint64_t PID, float posX, float posY, float length, int team){

	GameObject* base = new GameObject();

	base->ID = PID;
	base->setPos(posX, posY);
	base->isAlive = true;
	base->health = 0;
	base->team = team;
	base->type = GAMEOBJECT_TYPE::OBJECT_BASE;

	// Minion Specific Render Component. In future will have flag
	// for type of class,  which will instatiate based on flag

	MidBaseRenderComponent* rend = new MidBaseRenderComponent(base,team);
	if (team == TEAM_YELLOW){
		base->flipV = false;
		base->flipH = false;
	}
	else{
		base->flipV = true;
		base->flipH = true;
	}
	MidBasePhysicsComponent* physics = new MidBasePhysicsComponent(base);

	MidBaseLogicComponent* logic = new MidBaseLogicComponent(base);

	MidBaseNetworkComponent* net = new MidBaseNetworkComponent(base);

	return base;
}