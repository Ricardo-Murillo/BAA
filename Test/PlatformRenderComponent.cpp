#include "PlatformRenderComponent.h"


PlatformRenderComponent::PlatformRenderComponent(GameObject* platform){
	gameObjectRef = platform;
	gameObjectRef->AddComponent(COMPONENT_RENDER, this);

	SceneManager* sceneMan = SceneManager::GetSceneManager();

	//objRef = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 9991, 0, 0);
	objRef = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 101001, 0, 0);
	SDLRenderObject * box = sceneMan->InstantiateBlankObject(sceneMan->findLayer("layer2"), 0, 0, 10, 10);
	box->setIfRenderRect(true);
	allObjs["box"] = box;
	allObjs["base"] = objRef;
}


PlatformRenderComponent::~PlatformRenderComponent()
{
}

void PlatformRenderComponent::Update(){
	RenderComponent::Update();
	RenderBoundingBox((allObjs["box"]));
}