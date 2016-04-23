#include "MinionNetworkComponent.h"


MinionNetworkComponent::MinionNetworkComponent(GameObject* minion)
{
	gameObjectRef = minion;
	gameObjectRef->AddComponent(COMPONENT_NETWORK, this);
	logic = dynamic_cast<MinionLogicComponent*>(gameObjectRef->GetComponent(COMPONENT_LOGIC));
}


MinionNetworkComponent::~MinionNetworkComponent()
{
}

void MinionNetworkComponent::Update(){
	while (!incomingPackets.empty()){
		InputMemoryBitStream packet = incomingPackets.front();
		int mCommand;

		packet.Read(mCommand);
		switch (mCommand){
		case COMMAND_TYPE::CM_DIE:
			HandleMenionDeath(packet);
			break;
		}
	}
}

void MinionNetworkComponent::SendMenionDeath(uint64_t ID){
	OutputMemoryBitStream* deathPacket = new OutputMemoryBitStream();
	deathPacket->Write(NetworkManager::sInstance->kMinDeathCC);
	deathPacket->Write(ID);
	outgoingPackets.push(deathPacket);
}

void MinionNetworkComponent::HandleMenionDeath(InputMemoryBitStream& inPacket){
	
	uint64_t ID;
	inPacket.Read(ID);

	logic->DestroyMinion(ID);
}