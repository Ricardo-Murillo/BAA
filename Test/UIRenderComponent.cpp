#include "UIRenderComponent.h"
#include "config.h"



UIRenderComponent::UIRenderComponent(){
}

UIRenderComponent::~UIRenderComponent(){
}

void UIRenderComponent::createUIType(UIType ID){

	SceneManager* sceneMan = SceneManager::GetSceneManager();
	SDLRenderObject *play;

	switch (ID){
	case PLAY_BUTTON:
		play = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 19, uiObjectRef->posX, uiObjectRef->posY, true);
		objRef = play;
		allObjs.push_back(play);
		break;
	case CANCEL_BUTTON:
		play = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 22, uiObjectRef->posX, uiObjectRef->posY, true);
		objRef = play;
		allObjs.push_back(play);
		break;
	case BACK_BUTTON:
		play = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 23, uiObjectRef->posX, uiObjectRef->posY, true);
		objRef = play;
		allObjs.push_back(play);
		break;
	case JOIN_BUTTON:
		play = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 24, uiObjectRef->posX, uiObjectRef->posY, true);
		objRef = play;
		allObjs.push_back(play);
		break;
	case OPTIONS_BUTTON:
		play = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 29, uiObjectRef->posX, uiObjectRef->posY, true);
		objRef = play;
		allObjs.push_back(play);
		break;
	case READY_BUTTON:
		play = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 26, uiObjectRef->posX, uiObjectRef->posY, true);
		objRef = play;
		allObjs.push_back(play);
		break;
	case PLAYER_SLOT:
		play = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 28, uiObjectRef->posX, uiObjectRef->posY, true);
		objRef = play;
		allObjs.push_back(play);
		break;
	case CHICKEN:
		play = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), CHICKEN, uiObjectRef->posX, uiObjectRef->posY, true);
		uiObjectRef->setWidthAndHeight(play->getWidth(), play->getHeight());
		objRef = play;
		allObjs.push_back(play);
		break;
	case PEACOCK:
		play = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), PEACOCK, uiObjectRef->posX, uiObjectRef->posY, true);
		uiObjectRef->setWidthAndHeight(play->getWidth(), play->getHeight());
		objRef = play;
		allObjs.push_back(play);
		break;
	case FLAMINGO:
		play = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), FLAMINGO, uiObjectRef->posX, uiObjectRef->posY, true);
		uiObjectRef->setWidthAndHeight(play->getWidth(), play->getHeight());
		objRef = play;
		allObjs.push_back(play);
		break;
	case QUAIL:
		play = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), QUAIL, uiObjectRef->posX, uiObjectRef->posY, true);
		uiObjectRef->setWidthAndHeight(play->getWidth(), play->getHeight());
		objRef = play;
		allObjs.push_back(play);
		break;
	case TURKEY:
		play = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), TURKEY, uiObjectRef->posX, uiObjectRef->posY, true);
		uiObjectRef->setWidthAndHeight(play->getWidth(), play->getHeight());
		objRef = play;
		allObjs.push_back(play);
		break;
	case EAGLE:
		play = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), EAGLE, uiObjectRef->posX, uiObjectRef->posY, true);		
		uiObjectRef->setWidthAndHeight(play->getWidth(), play->getHeight());
		objRef = play;
		allObjs.push_back(play);
		break;
	case INVITE_BUTTON:
		play = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 29, uiObjectRef->posX, uiObjectRef->posY, true);
		objRef = play;
		allObjs.push_back(play);
		break;
	case TIMER:{
		RenderManager* renderMan = RenderManager::getRenderManager();
		uiObjectRef->posX = SCREEN_WIDTH - 200;
		uiObjectRef->posY = 30;
		play = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), -1, uiObjectRef->posX, uiObjectRef->posY, true);
		play->setResourceObject(renderMan->renderText("Timer", 255, 0, 255, 50, "BowlbyOneSC-Regular"));

		objRef = play;
		allObjs.push_back(play);
		break;
	}
	case LOBBY_TIMER:{
		RenderManager* renderMan = RenderManager::getRenderManager();

		int x, y;
		renderMan->getWindowSize(&x, &y);

		play = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), -1, x / 2, y / 2, true);
		play->setResourceObject(renderMan->renderText("", 255, 0, 255, 50, "BowlbyOneSC-Regular"));

		objRef = play;
		allObjs.push_back(play);
		break;
	}
	case BIRDSEED_BAR:
		//TODO: these hardcoded positions are to put it a bit away from the borders of the screen.
		//We'll need to store them as variables, since PlayerLogicComponent->update has hardcoded
		//values to match
		uiObjectRef->posX = 30;
		uiObjectRef->posY = 30;
		play = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 1101, uiObjectRef->posX, uiObjectRef->posY, true);
		objRef = play;
		allObjs.push_back(play);
		break;
	case BIRDSEED_SHELL:
		uiObjectRef->posX = 30;
		uiObjectRef->posY = 30;
		play = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 1100, uiObjectRef->posX, uiObjectRef->posY, true);
		objRef = play;
		allObjs.push_back(play);
		break;
	case CROSSHAIR:
		play = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 1109, 0, 0, true);
		objRef = play;
		allObjs.push_back(play);
		break;
	case MENU_NAME:{
		std::string playerName = GamerServices::sInstance->GetLocalPlayerName();
		RenderManager* renderMan = RenderManager::getRenderManager();
		SDLRenderObject * name = sceneMan->InstantiateBlankObject(sceneMan->findLayer("layer2"), 0, 0, 0, 0);
		int x, y;
		renderMan->getWindowSize(&x, &y);
		play = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), -1, x / 2, y / 2, true);
		play->setResourceObject(renderMan->renderText(playerName.c_str(), 255, 0, 255, 50, "BowlbyOneSC-Regular"));
		break;
	}
	// CHARGE BAR needs to follow player posX/posY at a slight offset
	// using 1102 and 1103 for object IDs
	// position is currently hardcoded, need to move to follow the player sprite location
	case CHARGE_BAR:
		play = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 1103, (SCREEN_WIDTH - 300), 0, true);
		objRef = play;
		allObjs.push_back(play);
		break;
	case CHARGE_SHELL:
		play = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 1102, (SCREEN_WIDTH - 300), 0, true);
		objRef = play;
		allObjs.push_back(play);
		break;
	case KILL_NOTIFICATION:
		play = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), -1, (SCREEN_WIDTH - 300), 130, true);
		objRef = play;
		RenderManager* renderMan = RenderManager::getRenderManager();
		objRef->setResourceObject(renderMan->renderText("test", 255, 0, 255, 30, "BowlbyOneSC-Regular"));
		allObjs.push_back(play);
	}
}

void UIRenderComponent::Update(){
	RenderUIComponent::Update();
}
