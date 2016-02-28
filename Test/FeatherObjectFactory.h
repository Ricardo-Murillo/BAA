#pragma once
#ifndef FEATHEROBJECTFACTORY_H_INCLUDED
#define FEATHEROBJECTFACTORY_H_INCLUDED

#include "GameObject.h"
#include "GameObjectFactory.h"


// Component Includes
#include "Component.h"
#include "FeatherRenderComponent.h"
#include "FeatherNetworkComponent.h"
#include "FeatherLogicComponent.h"
#include "FeatherPhysicsComponent.h"



class FeatherObjectFactory : public GameObjectFactory
{
public:
	/// Constructor
	FeatherObjectFactory();
	/// Destructor
	~FeatherObjectFactory();

	/// Creates a new feather GameObject
	GameObject * Spawn(uint64_t PID);

};

#endif