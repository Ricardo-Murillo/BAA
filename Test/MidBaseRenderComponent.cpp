#include "MidBaseRenderComponent.h"


MidBaseRenderComponent::MidBaseRenderComponent(GameObject * base)
{

	gameObjectRef = base;
	gameObjectRef->AddComponent(COMPONENT_RENDER, this);

	SceneManager* sceneMan = SceneManager::GetSceneManager();

	//objRef = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 9991, 0, 0);
	objRef = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 2007, 0, 0);
	SDLRenderObject * box = sceneMan->InstantiateBlankObject(sceneMan->findLayer("layer2"), 0, 0, 10, 10);
	box->setIfRenderRect(true);
	allObjs["box"] = box;
	allObjs["base"] = objRef;

}


MidBaseRenderComponent::~MidBaseRenderComponent()
{
}

void MidBaseRenderComponent::Update(){


	RenderComponent::Update();
	RenderBoundingBox((allObjs["box"]));

}