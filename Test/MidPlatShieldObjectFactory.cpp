#include "MidPlatShieldObjectFactory.h"


MidPlatShieldObjectFactory::MidPlatShieldObjectFactory()
{



}


MidPlatShieldObjectFactory::~MidPlatShieldObjectFactory()
{


}


GameObject * MidPlatShieldObjectFactory::Spawn(uint64_t PID, float posX, float posY, float length){

	GameObject* shield = new GameObject();

	shield->ID = PID;
	shield->setPos(posX, posY);
	shield->isAlive = true;
	shield->type = GAMEOBJECT_TYPE::OBJECT_LAUNCHER;

	// Minion Specific Render Component. In future will have flag
	// for type of class,  which will instatiate based on flag

	MidPlatShieldRenderComponent* rend = new MidPlatShieldRenderComponent(shield);

	MidPlatShieldPhysicsComponent* physics = new MidPlatShieldPhysicsComponent(shield);

	MidPlatShieldLogicComponent* logic = new MidPlatShieldLogicComponent(shield);


	return shield;



}

GameObject * MidPlatShieldObjectFactory::Collider(uint64_t PID, float posX, float posY, float length){

	GameObject* shield = new GameObject();

	shield->ID = PID;
	shield->setPos(posX, posY);
	shield->isAlive = true;
	shield->type = GAMEOBJECT_TYPE::OBJECT_LAUNCHER;
	int size = 4;

	// Minion Specific Render Component. In future will have flag
	// for type of class,  which will instatiate based on flag

	MidPlatShieldPhysicsComponent* physics = new MidPlatShieldPhysicsComponent(shield);

	MidPlatShieldLogicComponent* logic = new MidPlatShieldLogicComponent(shield);


	return shield;

}