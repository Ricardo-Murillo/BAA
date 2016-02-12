#include "test.h"



//#include "include\network\NetIncludes.h"

using namespace std;

void update();
void render(RenderManager*);
long double getCurrentTime();


int main() {

	return 0;
}

int _tmain(int argc, _TCHAR* argv[]){
	LogManager* log = LogManager::GetLogManager();
	log->create("log.txt");

	//RandGen::StaticInit();
	// need to initialize Steam before SDL so the overlay works*/

	LogManager* log = LogManager::GetLogManager();
	log->create("log.txt");

	if (!GamerServices::StaticInit())
		std::cout << "Failed to initialize Steam" << "\n";

	if (!NetworkManager::StaticInit())
		std::cout << "NetworkManager::StaticInit() failed!" << "\n";

	while (true){
		GamerServices::sInstance->Update();
		int x;
		cout << "Press 1 for player count" << endl
			 << "Press 2 for Player ID" << endl
			 << "Press 3 for text test" << endl
			 << "Press 4 for all players in lobby" << endl;
		cin >> x;
		uint32_t hello = 2;
		switch (x){
		case 1:
			cout << NetworkManager::sInstance->GetPlayerCount() << endl;
			if (NetworkManager::sInstance->IsMasterPeer()){
				cout << "You are master peer." << endl;
			}
			break;
		case 2:
			cout << GamerServices::sInstance->GetLocalPlayerName() << endl;
			break;
		case 3:
			while (hello < 10001){
				NetworkManager::sInstance->SendHelloWorld(hello);
				NetworkManager::sInstance->ProcessIncomingPackets();
				hello++;
			}
			break;
		case 4:
			NetworkManager::sInstance->GetAllPlayersInLobby();
			break;
		
		case 5:
			uint32_t h = 1;
			NetworkManager::sInstance->SendHelloWorld(h);
		}
	}
	/*
		NetworkManager::sInstance->ProcessIncomingPackets();
		//cout << "state: " << NetworkManager::sInstance->GetState() << endl;
		if (NetworkManager::sInstance->GetState() == 4)
			break;
		if (NetworkManager::sInstance->GetPlayerCount() == 2){
			//NetworkManager::sInstance->GetAllPlayersInLobby();
			NetworkManager::sInstance->TryReadyGame();
		}
	}

	InputManager* input = InputManager::getInstance();
	RenderManager* renderMan = RenderManager::getRenderManager();
	ResourceManager* resourceMan = ResourceManager::GetResourceManager();
	SceneManager* sceneMan = SceneManager::GetSceneManager();
	renderMan->init(700, 700, false, "Birds At Arms");
	resourceMan->loadFromXMLFile("source.xml");

	resourceMan->setCurrentScope(0);
	std::cout << "resource count : " << resourceMan->getResourceCount() << "\n";

	sceneMan->loadFromXMLFile("SceneTree.xml");

	InputListener* listen = new InputListener();


	Square* player1 = new Square(100, 100, 1);
	player1->obj = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 2, player1->x, player1->y);
	Square* player2 = new Square(200, 200, 2);
	player2->obj = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 12, player2->x, player2->y);


	/////////////////////////////////////////////////////
	/*              * * * GAME LOOP * * *              */
	/////////////////////////////////////////////////////
	bool gameloop = true;

	while (gameloop) {
		NetworkManager::sInstance->ProcessIncomingPackets();
		listen->getInput();

		player1->x += listen->input_x;
		player1->y += listen->input_y;

		//cout << player1->obj->posX << "," << player2->obj->posX<< endl;

		player1->update();

		OutputMemoryBitStream outData;
		outData.Write(NetworkManager::sInstance->kPosCC);
		player1->Write(outData);
		NetworkManager::sInstance->sendPacketToAllPeers(outData);
		//cout << "test size: " << NetworkManager::sInstance->test.size() << endl;
		for (int i = 0; i < NetworkManager::sInstance->test.size(); ++i){
			player2->Read(NetworkManager::sInstance->test.front());
			player2->update();
			NetworkManager::sInstance->test.pop();
		}

		if (input->isKeyDown(KEY_ESCAPE))
			gameloop = false;

		input->update();
		

		sceneMan->AssembleScene();

		//render(renderMan);
	}
	std::cout << renderMan << endl;*/
	/////////////////////////////////////////////////////
	/////////////////////////////////////////////////////
	/////////////////////////////////////////////////////
	std::cout << renderMan << endl;

	log->close();
	return 0;
}


void init(){

}





void render(RenderManager* renderMan) {
	renderMan->update();
}

long double getCurrentTime(){
	long double sysTime = time(0);
	long double sysTimeMS = sysTime * 1000;

	return sysTimeMS;
}