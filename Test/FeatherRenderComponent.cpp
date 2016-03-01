#include "FeatherRenderComponent.h"

FeatherRenderComponent::FeatherRenderComponent(){
	SceneManager* sceneMan = SceneManager::GetSceneManager();

	objRef = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 21, 0, 0);
}


FeatherRenderComponent::~FeatherRenderComponent()
{
}

void FeatherRenderComponent::Update(){
	RenderComponent::Update();
}