#include "PlayerRenderComponent.h"

PlayerRenderComponent::PlayerRenderComponent(GameObject* player, function_t func)
{
	gameObjectRef = player;
	gameObjectRef->AddComponent(COMPONENT_RENDER, this);

	RenderComponent::RenderComponent();
	SceneManager* sceneMan = SceneManager::GetSceneManager();
	RenderManager* renderMan = RenderManager::getRenderManager();
	std::string playerName = GamerServices::sInstance->GetLocalPlayerName();

	SDLRenderObject * name = sceneMan->InstantiateBlankObject(sceneMan->findLayer("layer2"), 0, 0, 0, 0);
	name->setResourceObject(renderMan->renderText(playerName.c_str(), 200, 0, 200, 20, "BowlbyOneSC-Regular"));
	//name->setParent(base);
	name->setPos(0, -60);
	allObjs["name"] = name;

	//ChickenClassComponent* classComp = dynamic_cast<ChickenClassComponent*>(gameObjectRef->GetComponent(COMPONENT_CLASS));
	//classComp->animation(&objRef, allObjs, animations);

	func(&objRef, allObjs, animations);
}


PlayerRenderComponent::~PlayerRenderComponent()
{
	for (auto i = animations.begin(); i != animations.end(); i++){
		delete i->second;
	}
}


void PlayerRenderComponent::Update(){
	RenderComponent::Update();
	RenderBoundingBox((allObjs["box"]));
	ApplyPhysicsRotation(allObjs["base"]);
	RenderComponent::animate();
	allObjs["name"]->setPos(allObjs["base"]->getPosX(), -60 + allObjs["base"]->getPosY());
}


