#include "FeatherObjectFactory.h"


FeatherObjectFactory::FeatherObjectFactory()
{
}


FeatherObjectFactory::~FeatherObjectFactory()
{
}

/// Spawn() assembles nessasary Components and throws them into
/// a "GameObject" Container.

GameObject* FeatherObjectFactory::Spawn(uint64_t PID)
{

	GameObject* feather = new GameObject();

	feather->ID = PID;

	// Feather Specific Render Component. In future will have flag
	// for type of class,  which will instatiate based on flag

	FeatherRenderComponent* rend = new FeatherRenderComponent();
	rend->gameObjectRef = feather; //set components container refrence to this gameObject
	feather->AddComponent(COMPONENT_RENDER, rend);


	FeatherNetworkComponent* net = new FeatherNetworkComponent();
	net->gameObjectRef = feather; //set components container refrence to this gameObject
	feather->AddComponent(COMPONENT_NETWORK, net);


	FeatherLogicComponent* logic = new FeatherLogicComponent();
	logic->gameObjectRef = feather; //set components container refrence to this gameObject
	feather->AddComponent(COMPONENT_LOGIC, logic);

	FeatherPhysicsComponent* physics = new FeatherPhysicsComponent();
	physics->gameObjectRef = feather; //set components container refrence to this gameObject
	feather->AddComponent(COMPONENT_PHYSICS, physics);

	return feather;
}