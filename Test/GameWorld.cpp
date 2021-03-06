#include "GameWorld.h"

GameWorld GameWorld::gameWorld;

GameWorld::GameWorld(){
	//initialize the class here
	//Note: hardcoded gravity for testing
	const b2Vec2 gravity =  b2Vec2(0, 0);

	physicsWorld = new b2World(gravity);
	currTime = clock();
}
float worldScale = 20.0f;
bool konamiUnlocked = false;


GameWorld::~GameWorld(){
	//free the bodies
	b2Body* bodyList = physicsWorld->GetBodyList();
	int bodyCount = physicsWorld->GetBodyCount();
	for (int i = 0; i< bodyCount; i++){
		b2Body* bodyToBeGone = bodyList;
		bodyList = bodyList->GetNext();
		//we have the current body, now we delete it's fixtures
		for (b2Fixture* f = bodyToBeGone->GetFixtureList(); f;){
			b2Fixture* fixtureToDestroy = f;
			f = f->GetNext();
			bodyToBeGone->DestroyFixture(fixtureToDestroy);
		}
		physicsWorld->DestroyBody(bodyToBeGone);
	}
	//delete physicsWorld;
}

GameWorld* GameWorld::getInstance() {
	return &gameWorld;
}

b2World* GameWorld::getPhysicsWorld(){
	return physicsWorld;
}

void GameWorld::update(){
	//TODO: replace first hardcoded value of time length to actual time since last frame
	//the last two values are how many calculations are done within this time period. Higher is
	// more precise, but more computationally intensive
	double fps = clock() - currTime;
	if (fps == 0)fps++;//prevents dividing by zero
	fps/= (double)(CLOCKS_PER_SEC);
	currTime = clock();
	physicsWorld->Step((float32)fps,5,5);
}