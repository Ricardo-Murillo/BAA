#include "PlayerInputComponent.h"


PlayerInputComponent::PlayerInputComponent(GameObject* player)
{
	input = InputManager::getInstance();
	gameObjectRef = player;
	gameObjectRef->AddComponent(COMPONENT_INPUT, this);
}


PlayerInputComponent::~PlayerInputComponent()
{
}

void PlayerInputComponent::Update(){
	PlayerPhysicsComponent* physicsComp = (PlayerPhysicsComponent*)gameObjectRef->GetComponent(COMPONENT_PHYSICS);
	if (physicsComp){
		b2Body* body = physicsComp->mBody;
		InputManager* input = InputManager::getInstance();
		Controller* controller = input->controller;
		//handle input for moving
		bool moving = false;
		float speed = 60.0f;
<<<<<<< HEAD
		body->SetLinearVelocity(b2Vec2(controller->getLeftThumbX()*speed, body->GetLinearVelocity().y));
		if (input->isKeyDown(KEY_D)) {
=======
		if (input->isKeyDown(KEY_D) || input->isKeyDown(KEY_RIGHT)) {
>>>>>>> refs/remotes/origin/master
			body->SetLinearVelocity(b2Vec2(speed, body->GetLinearVelocity().y));
			moving = true;
			gameObjectRef->flipH = false;
		}
<<<<<<< HEAD
		if (input->isKeyDown(KEY_A) || controller->isJoystickDown(JOYSTICK_X)) {
=======
		if (input->isKeyDown(KEY_A) || input->isKeyDown(KEY_LEFT)) {
>>>>>>> refs/remotes/origin/master
			body->SetLinearVelocity(b2Vec2(-speed, body->GetLinearVelocity().y));
			moving = true;		
			gameObjectRef->flipH = true;
		}
<<<<<<< HEAD
		if (input->isKeyDown(KEY_SPACE) || controller->isJoystickPressed(JOYSTICK_A)) {
			body->SetLinearVelocity(b2Vec2(15*body->GetLinearVelocity().x, -speed));
=======
		if (input->isKeyDown(KEY_SPACE)) {
			body->SetLinearVelocity(b2Vec2(10*body->GetLinearVelocity().x, -speed));
			cout << body->GetLinearVelocity().x << endl;
>>>>>>> refs/remotes/origin/master
			//moving = true;
		}
		if (input->isKeyDown(KEY_S) || input->isKeyDown(KEY_DOWN)) {
			body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x, speed));
			moving = true;
		}
		if (!moving)body->SetLinearVelocity(b2Vec2(body->GetLinearVelocity().x*0.8, body->GetLinearVelocity().y));
		if (input->isMouseDown(MOUSE_LEFT)){
			PlayerLogicComponent* logic = dynamic_cast<PlayerLogicComponent*>(gameObjectRef->GetComponent(COMPONENT_LOGIC));
			uint64_t id = logic->spawnFeather(input->getMouseX(), input->getMouseY());
			PlayerNetworkComponent* net = dynamic_cast<PlayerNetworkComponent*>(gameObjectRef->GetComponent(COMPONENT_NETWORK));
			net->createFeatherPacket(id, input->getMouseX(), input->getMouseY());
		}
		if (body->GetLinearVelocity().x<0)gameObjectRef->flipH = true;
		else if (body->GetLinearVelocity().x>0)gameObjectRef->flipH = false;
	}
}

