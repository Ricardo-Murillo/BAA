#include "main.h"
#include <functional>
#include <crtdbg.h>

//#include "include\network\NetIncludes.h"


using namespace std;

void update();


int main() {

	return 0;
}

int _tmain(int argc, _TCHAR* argv[]){
	
	

	int numPlayers = 1;
	if (!GamerServices::StaticInit())
		std::cout << "Failed to initialize Steam" << "\n";

	if (!NetworkManager::StaticInit()){
		std::cout << "NetworkManager::StaticInit() failed!" << "\n";
	}

	RenderManager* renderMan = RenderManager::getRenderManager();
	renderMan->init(SCREEN_WIDTH, SCREEN_HEIGHT, false, "Birds At Arms");


	LogManager* log = LogManager::GetLogManager();
	log->create("log.txt");


	Start mainMenu;
	mainMenu.mainMenu();

	// don't need to change this line
	int numLobbyPlayer = 0;

	/////////////////////////////////////////////////////
	// Loop freeing memory NOW MOVED TO DESTRUCTOROF SYSTEMGAMEOBJECTQUEUE
	//for (unsigned int i = 0; i < GameObjects.alive_objects.size(); i++){
		//GameObjects.DeleteObjects(GameObjects.alive_objects[i]->ID);
	//}

	log->close();

	return 0;
}