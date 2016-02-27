#include "test.h"
#include <functional>

//#include "include\network\NetIncludes.h"

using namespace std;

void update();
void render(RenderManager*);
long double getCurrentTime();

typedef float(*ease_function)(float);

float ease_linear(float i){
	return i;
}
float ease_QuadIn(float i){
	return pow(i, 2);
}
float ease_QuadOut(float i){
	return i*(2-i);
}
float ease_QuadInOut(float i){
	return (i < 0.5) ? 2 * i*i : -1 + (4 - 2 * i)*i;
}
std::function<float(float)> getBezier(float x0, float x1, float x2, float x3){
	return [=](float i) {
		return x0 + i*(3 * x1 - 3 * x0) + i*i*(3 * x2 - 6 * x1 + 3 * x0) + i*i*i*(x3 - 3 * x2 + 3 * x1 - x0);
	};
}
std::function<void(float)> rotateTransform(SDLRenderObject* obj, double start, double end){
	return [=](float i) {obj->rotation = (end - start)*i + start; };
}

std::function<void(float)> moveCircArc(SDLRenderObject* obj, int centerx, int centery, double rad, double start_angle, double end_angle){
	return [=](float i) {
		obj->posX = centerx + rad * cos(M_PI*(end_angle*i + start_angle) / 180);
		obj->posY = centery + rad * sin(M_PI*(end_angle*i + start_angle) / 180);
	};
}
std::function<void(float)> moveEllipseArc(SDLRenderObject* obj, int centerx, int centery, double height, double width, double start_angle, double end_angle){
	return [=](float i) {
		obj->posX = centerx + width / 2 * cos(M_PI*(end_angle*i + start_angle) / 180);
		obj->posY = centery + height / 2 * sin(M_PI*(end_angle*i + start_angle) / 180);
	};
}
//float bezier()
struct motion{
	std::function<void(float)> trans;
	std::function<float(float)> ease;
	int start; //a value from 0 to 1
	int duration; //a value from 1-start to 0
};
struct animation{
	list <motion> motions;
	bool animate(float i){
		for (auto mot = motions.begin(); mot != motions.end(); mot++){
			float place;
			if (i >= mot->start + mot->duration){ place = 1.0; }//so durations of 0 are assumed to have finished
			else if (i < mot->start)			{ place = 0.0; }
			else								{ place = (i - mot->start) / mot->duration; }
			mot->trans(mot->ease(place));//apply transformation, place should be between 0 and 1
		}
		return ((i>1.0) || (i<0.0));
	}
};

int main() {

	return 0;
}

int _tmain(int argc, _TCHAR* argv[]){
	int numPlayers = 1;
	LogManager* log = LogManager::GetLogManager();
	log->create("log.txt");

	if (!GamerServices::StaticInit())
		std::cout << "Failed to initialize Steam" << "\n";

	if (!NetworkManager::StaticInit())
		std::cout << "NetworkManager::StaticInit() failed!" << "\n";

	while (true){
		GamerServices::sInstance->Update();
		NetworkManager::sInstance->ProcessIncomingPackets();
		//cout << "state: " << NetworkManager::sInstance->GetState() << endl;
		if (NetworkManager::sInstance->GetState() == 4)
			break;
		if (NetworkManager::sInstance->GetPlayerCount() == numPlayers){
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
	renderMan->zoom = 0.25;
	resourceMan->setCurrentScope(0);
	std::cout << "resource count : " << resourceMan->getResourceCount() << "\n";

	sceneMan->loadFromXMLFile("SceneTree.xml");

	/////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////////////

	///  SYSTEMS

	SystemNetworkUpdater sysNetwork;
	SystemRenderUpdater sysRenderer;
	SystemInputUpdater sysInput;
	SystemLogicUpdater sysLogic;
	SystemPhysicsUpdater sysPhysics;


	SystemGameObjectQueue world;

	/// ENTITIES


	PlayerObjectFactory pFactory;

	world.AddObject(pFactory.Spawn(1));

	//vector<Player*> players;

	//Player* localPlayer;
	map< uint64_t, string > loby = NetworkManager::sInstance->getLobbyMap();

	/*

	int i = 0;
	for (auto &iter : loby)
	{
	//cout << iter.second << endl;
	Player* player = new Player(iter.first, 50 * i - 50, 50 * i - 50);
	if (player->ID == NetworkManager::sInstance->GetMyPlayerId()){
	localPlayer = player;
	player->isNetworkControlled = false;
	}
	player->objRef = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), something[i], player->posX, player->posY);
	players.push_back(player);
	}*/
	SDLRenderObject* base = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 0, 0, 0);
	base->anchor = { 0.5, 0.5 };
	//base->setVisible(false);
	SDLRenderObject* leg = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 103, -5, 30);
	leg->anchor = { 42 / float(leg->renderRect.w), 2 / float(leg->renderRect.h) };
	SDLRenderObject* armor = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 101, 0, 0);
	armor->anchor = { 0.5, 0.5 };
	SDLRenderObject* arm = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 102, 31, 43);
	arm->anchor = { 14 / float(arm->renderRect.w), 3 / float(arm->renderRect.h) };

	leg->setParent(base);
	armor->setParent(base);
	arm->setParent(armor);

	SDLRenderObject* tenta1 = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 151, 0, 50);
	tenta1->anchor = { 74 / float(tenta1->getWidth()), 200 / float(tenta1->getHeight()) };
	tenta1->setScale(0.25);

	SDLRenderObject* tenta2 = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 151, 0 , tenta1->getHeight() - 200);
	tenta2->anchor = { 74 / float(tenta2->getWidth()), 200 / float(tenta2->getHeight()) };
	tenta2->setScale(0.90);
	tenta2->setParent(tenta1);


	SDLRenderObject* tenta3 = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 151, 0, tenta2->getHeight() - 200);
	tenta3->anchor = { 74 / float(tenta3->getWidth()), 200 / float(tenta3->getHeight()) };
	tenta3->setScale(0.90);
	tenta3->setParent(tenta2);


	SDLRenderObject* tenta4 = sceneMan->InstantiateObject(sceneMan->findLayer("layer1"), 151, 0, tenta3->getHeight() - 200);
	tenta4->anchor = { 74 / float(tenta4->getWidth()), 200 / float(tenta4->getHeight()) };
	tenta4->setScale(0.90);
	tenta4->setParent(tenta3);

	SDLRenderObject* tenta[4];
	tenta[0] = tenta1;
	tenta[1] = tenta2;
	tenta[2] = tenta3;
	tenta[3] = tenta4;
	animation squirm;
	for (SDLRenderObject * iter : tenta){
		motion m = { rotateTransform(iter, 90, -90), ease_QuadInOut, 0, 1 };
		squirm.motions.push_back(m);
	}
	/*
	for (int i = 0; i < players.size(); ++i){
	if (!players[i]->isNetworkControlled)
	cout << players[i]->ID << endl;
	}

	*/
	/////////////////////////////////////////////////////
	/*              * * * GAME LOOP * * *              */
	/////////////////////////////////////////////////////
	bool gameloop = true;
	int var = 0;

	auto up = rotateTransform(arm, 0, 180);
	auto down = rotateTransform(arm, 180, 0);

	auto arcarm = moveEllipseArc(arm, 12, 14, 0, 4, -180, 360);
	auto arcbody = moveEllipseArc(armor, 0, 0, 5, 2, 0, -360);
	renderMan->zoom = 0.5;
	float size = 6;
	float ratio = 0.7;
	int armswing = size;
	int moveSpd = 1;
	int pressed = 0;
	int pressedTime = 3;
	int rotation = 0;
	while (gameloop) {
		var += 1;
		input->update();
		if (var % 40 < 20){
			squirm.animate(float(var % 40) / 20);
		}
		else{
			squirm.animate(2-float(var % 40) / 20);
		}
		//inputMan->update();
		//listen->getInput();
		NetworkManager::sInstance->ProcessIncomingPackets();
		NetworkManager::sInstance->UpdateDelay();

		//arm->rotation = var * 2;
		//base->posX += listen->input_x;
		//base->posY += listen->input_y;

		if (input->isKeyDown(KEY_DOWN)){
			base->posY += moveSpd;
		}
		if (input->isKeyDown(KEY_UP)){
			base->posY -= moveSpd;
		}
		if (input->isKeyDown(KEY_LEFT)){
			base->posX -= moveSpd;
		}
		if (input->isKeyDown(KEY_RIGHT)){
			base->posX += moveSpd;
		}
		if (input->isKeyDown(KEY_A) && !pressed){
			renderMan->flippedScreen = !renderMan->flippedScreen;
			pressed = pressedTime;
		}
		if (input->isKeyDown(KEY_Q) && !pressed){
			//base->setVisible(!base->isVisible());
			base->setFlippedH(!base->isFlippedH());
			pressed = pressedTime;
		}
		if (input->isKeyDown(KEY_W) && !pressed){
			//base->setVisible(!base->isVisible());
			base->setFlippedV(!base->isFlippedV());
			pressed = pressedTime;
		}
		if (pressed > 0)pressed--;
		if (input->isKeyDown(KEY_1)){
			base->setRotation(++rotation);
		}
		if (input->isKeyDown(KEY_2)){
			base->setRotation(--rotation);
		}
		if (input->isKeyDown(KEY_3)){
			base->setScale(2.0);
		}
		else if (input->isKeyDown(KEY_4)){
			base->setScale(0.5);
		}
		else if (input->isKeyDown(KEY_5)){
			base->setScale(2.0, 1.0);
		}
		else base->setScale(1.0);
		if (armswing > size && input->isKeyDown(KEY_Z)){
			armswing = 0;
		}
		if (armswing <= size){
			if (armswing < size*ratio)up(ease_QuadIn(float(armswing) / (size*ratio)));
			else down(ease_QuadOut(float(armswing - (size*ratio)) / (size*(1 - ratio))));
			cout << float(armswing) << endl;
			armswing += 1;
		}
		else{
			arcarm(float(var % 12) / 12);
		}
		arcbody(float(var % 12) / 12);

		//arm->posX = 31 + armor->posX;
		//arm->posY = 43 + armor->posY;

		int length = 20;
		float loop = (var % length);


		sysInput.InputUpdate(world.alive_object);
		sysRenderer.RenderUpdate(world.alive_object);
		sysLogic.LogicUpdate(world.alive_object);
		sysNetwork.NetworkUpdate(world.alive_object);
		sysPhysics.PhysicsUpdate(world.alive_object);

		//localPlayer->update();

		/*
		for (int i = 0; i < NetworkManager::sInstance->test.size(); ++i){
		//iterate though the queue, pop off packets, and create
		//commands to give to gameobjects
		int UID;
		NetworkManager::sInstance->test.front().Read(UID);
		//process packet here
		NetworkManager::sInstance->test.pop();
		}

		if (input->isKeyDown(KEY_ESCAPE))
		gameloop = false;
		for (int i = 0; i < players.size(); ++i){
		players[i]->update();
		}

		*/

		sceneMan->AssembleScene();

		//render(renderMan);
	}
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