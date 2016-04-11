#pragma once

#ifndef MIDBASEPHYSICSCOMPONENT_H_INCLUDED
#define MIDBASEPHYSICSCOMPONENT_H_INCLUDED


#include "PhysicsComponent.h"
#include "MidBaseLogicComponent.h"


class MidBasePhysicsComponent :
	public PhysicsComponent
{
public:
	MidBasePhysicsComponent(GameObject * base);
	~MidBasePhysicsComponent();
	
	bool base_hit = false;


	void handleCollision(GameObject* otherObj);

	void Init();

	void Update();
	
};

#endif