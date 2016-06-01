#include "GameOver.h"

GameOver::GameOver()
{
	RenderManager::getRenderManager()->setCameraPoint(0, 0);
	RenderManager::getRenderManager()->setBackground("Menu_bg.png");
	renderMan = RenderManager::getRenderManager();
	sceneMan  = SceneManager::GetSceneManager();
	statsText = new TextAlignment(.25);
	createButtons();
	createText();
	sceneMan->AssembleScene();
}

GameOver::~GameOver()
{
	removeButtons();
	delete statsText;
}

int GameOver::runScene(){
	RenderManager::getRenderManager()->toggleCursor(true);
	int buttonPressed = -2;
	while (true){
		buttonPressed = checkButtons();
		if (buttonPressed == BUTTON_MENU || InputManager::getInstance()->isKeyDown(KEY_ESCAPE))
			return SCENE_MENU;
		sceneMan->AssembleScene();
	}
}

void GameOver::createButtons(){
	

	int w, h;
	float x, y;
	renderMan->getWindowSize(&w, &h);

	// menu button
	
	renderMan->windowCoordToWorldCoord(x, y, (int)(w*(0.91f)), (int)(h*(0.85f)));
	mainMenuButt = bFactory.Spawn(buttonID++, x, y, 23, 75.0f, 75.0f, 0.3f);
	if (RenderManager::getRenderManager()->flippedScreen)mainMenuButt->rotation = 180;
	GameObjects.AddObject(mainMenuButt);

}

int GameOver::checkButtons(){
	InputManager::getInstance()->update();
	if (dynamic_cast<ButtonLogicComponent*>(mainMenuButt->GetComponent(COMPONENT_LOGIC))->isButtonPressed())
		return BUTTON_MENU;
	return BUTTON_NONE;
}

void GameOver::removeButtons(){
	GameObjects.DeleteObjects();
}

void GameOver::createText(){
	std::string text = "ERROR!";
	TextAlignment::COLOR *color = nullptr;

	std::cout << "team: " << Stats::getLocalTeam() << std::endl;
	std::cout << "purple: " << Stats::baseHealth_purple() << std::endl;
	std::cout << "yellow: " << Stats::baseHealth_yellow() << std::endl;
	if (Stats::baseHealth_purple() == Stats::baseHealth_yellow()){
		text = "No Contest!";
		color = new TextAlignment::COLOR(255, 255, 0);
	}
	else if (Stats::baseHealth_purple() < Stats::baseHealth_yellow()){
		if (Stats::getLocalTeam() == TEAM_PURPLE){
			text = "Victory!";
			color = new TextAlignment::COLOR(0, 153, 0);
		}
		else if (Stats::getLocalTeam() == TEAM_YELLOW){
			text = "Defeat!";
			color = new TextAlignment::COLOR(204, 0, 0);
		}
	}
	else if (Stats::baseHealth_purple() > Stats::baseHealth_yellow()){
		if (Stats::getLocalTeam() == TEAM_PURPLE){
			text = "Defeat!";
			color = new TextAlignment::COLOR(204, 0, 0);
		}
		else if(Stats::getLocalTeam() == TEAM_YELLOW){
			text = "Victory!";
			color = new TextAlignment::COLOR(0, 153, 0);
		}
	}
	// victory and defeat text
	statsText->setFontSize(110);
	statsText->setStartingYPos(1 / 8.0);
	statsText->createText(text, color, TextAlignment::MIDDLE);
	delete color;

	createStatsText();
	
}

void GameOver::createStatsText(){
	TextAlignment::COLOR *yellowColor = new TextAlignment::COLOR(255, 255, 0);
	TextAlignment::COLOR *purpleColor = new TextAlignment::COLOR(200, 0, 200);
	statsText->setStartingYPos(1 / 3.0f);
	statsText->setFontSize(30);
	// yellow team
	statsText->createText(std::to_string(Stats::baseHealth_yellow()) + " nest health lost", yellowColor, 1 / 4.0);
	statsText->createText(std::to_string(Stats::otherTeamMinionsKilled_yellow()) + " minions killed", yellowColor, 1 / 4.0);
	statsText->createText(std::to_string(Stats::otherTeamPlayersKilled_yellow()) + " enemies killed", yellowColor, 1 / 4.0);
	statsText->createText(std::to_string(Stats::feathersFired_yellow()) + " feathers thrown", yellowColor, 1 / 4.0);
	statsText->createText(std::to_string(Stats::abilitiesUsed_yellow()) + " abilities used", yellowColor, 1 / 4.0);
	// reset the offset
	statsText->resetOffset();
	// purple team
	statsText->createText(std::to_string(Stats::baseHealth_purple()) + " nest health lost", purpleColor, 3 / 4.0);
	statsText->createText(std::to_string(Stats::otherTeamMinionsKilled_purple()) + " minions killed", purpleColor, 3 / 4.0);
	statsText->createText(std::to_string(Stats::otherTeamPlayersKilled_purple()) + " enemies killed", purpleColor, 3 / 4.0);
	statsText->createText(std::to_string(Stats::feathersFired_purple()) + " feathers thrown", purpleColor, 3 / 4.0);
	statsText->createText(std::to_string(Stats::abilitiesUsed_purple()) + " abilities used", purpleColor, 3 / 4.0);

	delete yellowColor;
	delete purpleColor;
}