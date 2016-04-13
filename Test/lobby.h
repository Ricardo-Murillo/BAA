#pragma once
#ifndef LOBBY_H
#define LOBBY_H



#include "main.h"
#include "UIObject.h"
#include "WorldObjectFactory.h"
// Component Includes
#include "Component.h"
#include "UIRenderComponent.h"
#include "UIInputComponent.h"
#include "UIComponent.h"



enum TEAM{
	RED = 1,
	BLUE = 2,
	NOTEAM = 3,
};

struct player{
	uint64_t playerId;
	string name;
	bool ready = false;
	int x, y;
	TEAM team;
	UIObject* playerSlot = new UIObject();
	UIObjectFactory* readyButton = new UIObjectFactory();
	int playerChoice;
	UIObjectFactory* birdClass = new UIObjectFactory();
};



class Lobby{
public:
	Lobby();
	~Lobby();

	void runLobby();
	void countdown(SystemUIObjectQueue &q);
	void cleanUP(SystemUIObjectQueue &q);

private:
	vector<player*> players;
	void addPlayers(SystemUIObjectQueue &q);
	int playersReady;
	int numPlayers;
	int maxPlayers = 4;
	void assignPlayers(SceneManager* scene, RenderManager* rendMan);
	void updateLobby();
	void drawBirds(SystemUIObjectQueue &q);
	vector<UIObject*> Birds;
	int teamRed;
};

#endif