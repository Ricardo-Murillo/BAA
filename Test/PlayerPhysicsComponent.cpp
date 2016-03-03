#include "PlayerPhysicsComponent.h"


PlayerPhysicsComponent::PlayerPhysicsComponent(){}

PlayerPhysicsComponent::~PlayerPhysicsComponent(){}

void PlayerPhysicsComponent::init(){
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(gameObjectRef->posX, gameObjectRef->posY);
	bodyDef.angle = 0;// ... which direction it's facing

	GameWorld* gameWorld = GameWorld::getInstance();
	mBody = gameWorld->getPhysicsWorld()->CreateBody(&bodyDef);

	b2PolygonShape box;
	box.SetAsBox(1,1); // look up other functions for polygons
	b2FixtureDef boxFixtureDef;
	boxFixtureDef.shape = &box;
	boxFixtureDef.density = 1;
	mFixture=mBody->CreateFixture(&boxFixtureDef);
	cout << mFixture->GetFilterData().maskBits << endl;
	mBody->SetUserData(gameObjectRef);
	//hardcoded for debugging purposes
	mBody->SetTransform(b2Vec2(0,-5),0);
}



void PlayerPhysicsComponent::handleCollision(GameObject* otherObj){
	cout << "player handling collision with object ID: " << otherObj->ID<<endl;
}

void PlayerPhysicsComponent::Update(){
	gameObjectRef->posX = mBody->GetPosition().x*20.0f;
	gameObjectRef->posY = mBody->GetPosition().y*20.0f;
	//cout << "x=" << gameObjectRef->posX << "y=" << gameObjectRef->posY << endl;
}