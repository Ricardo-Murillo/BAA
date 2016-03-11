#include "FeatherPhysicsComponent.h"


FeatherPhysicsComponent::FeatherPhysicsComponent(GameObject* feather)
{
	gameObjectRef = feather;
	gameObjectRef->AddComponent(COMPONENT_PHYSICS, this);
	init();
}

FeatherPhysicsComponent::~FeatherPhysicsComponent(){}

void FeatherPhysicsComponent::init(){
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(gameObjectRef->posX, gameObjectRef->posY);
	bodyDef.angle = 0;// ... which direction it's facing

	GameWorld* gameWorld = GameWorld::getInstance();
	mBody = gameWorld->getPhysicsWorld()->CreateBody(&bodyDef);

	b2PolygonShape box;
	box.SetAsBox(32, 16); // look up other functions for polygons
	boxFixtureDef.shape = &box;
	boxFixtureDef.density = 1;
	mFixture = mBody->CreateFixture(&boxFixtureDef);
	mBody->SetUserData(gameObjectRef);
	mBody->SetTransform(b2Vec2(gameObjectRef->posX, gameObjectRef->posY), gameObjectRef->rotation / 180.0 * M_PI);

	setCollisionFilter(COLLISION_FEATHER, COLLISION_MINION);
}

void FeatherPhysicsComponent::handleCollision(GameObject* otherObj){
	//std::cout << "FEATHER handling collision with object ID: " << otherObj->ID << std::endl;
	//probably destroy itself, maybe trigger a sound?
	switch (otherObj->type){
		case GAMEOBJECT_TYPE::OBJECT_MINION:
		{
			gameObjectRef->setPos(-10000, 0);
			//destroy self or return to object pool
			
			gameObjectRef->isAlive = false;
			
			break;
		}
		case GAMEOBJECT_TYPE::OBJECT_PLAYER:
			//destroy self or return to object pool
			break;
		default:
			break;
	}
}

void FeatherPhysicsComponent::Update() {
	mBody->SetTransform(b2Vec2(gameObjectRef->posX, gameObjectRef->posY), gameObjectRef->rotation / 180.0 * M_PI);
}