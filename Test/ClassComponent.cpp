#include "ClassComponent.h"

ClassComponent::ClassComponent()
{
	isChicken = false;
	isEagle = false;
	isQuail = false;
	isPeacock = false;
	isTurkey = false;
	isFlamingo = false;
	speed = 16;
	width = 1.33f;
	height = 1.35f;
	seedRequired = 8;
	featherSpeed = 1.0f / 7.0f;
	featherWidth = 1;
	featherHeight = 1;
	abilityCooldown = 15;
}

ClassComponent::~ClassComponent()
{
}


void ClassComponent::Update()
{

}

int ClassComponent::useAbility(){
	return 0;
}

int ClassComponent::getClass(){
	return 0;
}

//void ClassComponent::animation(SDLRenderObject** objRef, map_obj& allObjs, map_anim& animations){
//
//}