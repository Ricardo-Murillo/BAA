/**
*  ClassComponent.h
*  Authors: Ethan Shimooka
*  Date 2/25/2016
*  Description :
Class Component represents the abstreact base class from which all other
Class Components are derived.


*/

#pragma once
#ifndef CLASSCOMPONENT_H_INCLUDED
#define CLASSCOMPONENT_H_INCLUDED


#include "Component.h"
///#include "MovementComponent.h"
///#include "include\InputManager.h"
///#include "include\network\MemoryBitStream.h"
#include "GameObject.h"
#include "RenderComponent.h"


class ClassComponent :
	public Component
{
public:
	/// Constructor
	ClassComponent();
	/// Destructor
	~ClassComponent();

	/// GameObject Container Refrence. (Assigned upon GameObject Creation) 
	GameObject* gameObjectRef;

	/// update object Positions from Input
	virtual void Update();

	//virtual void animation(SDLRenderObject** objRef, map_obj& allObjs, map_anim& animations);
	/// returns true if the ability was used, else returns false
	virtual int useAbility();

	virtual int getClass();

	float speed;
	float width, height;
	int   seedRequired;
	float featherSpeed;
	float featherWidth, featherHeight;
	float abilityCooldown;

	int currBirdseed = 0;
	int maxsBirdseed = 8;

	int powerNum=0;
	bool isChicken, isEagle, isFlamingo, isPeacock, isQuail, isTurkey;
};

#endif