#include "MinionObjectFactory.h"


MinionObjectFactory::MinionObjectFactory()
{
}


MinionObjectFactory::~MinionObjectFactory()
{
}


/// Spawn() assembles nessasary Components and throws them into
/// a "GameObject" Container.

GameObject* MinionObjectFactory::Spawn(uint64_t PID)
{

	GameObject* minion = new GameObject();

	minion->ID = PID;

	// Minion Specific Render Component. In future will have flag
	// for type of class,  which will instatiate based on flag

	MinionRenderComponent* rend = new MinionRenderComponent();
	rend->gameObjectRef = minion; //set components container refrence to this gameObject
	minion->AddComponent(COMPONENT_RENDER, rend);


	MinionNetworkComponent* net = new MinionNetworkComponent();
	net->gameObjectRef = minion; //set components container refrence to this gameObject
	minion->AddComponent(COMPONENT_NETWORK, net);


	MinionLogicComponent* logic = new MinionLogicComponent();
	logic->gameObjectRef = minion; //set components container refrence to this gameObject
	minion->AddComponent(COMPONENT_LOGIC, logic);

	MinionPhysicsComponent* physics = new MinionPhysicsComponent();
	physics->gameObjectRef = minion; //set components container refrence to this gameObject
	minion->AddComponent(COMPONENT_PHYSICS, physics);

	return minion;
}