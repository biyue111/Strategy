#include "player.h"

Player::Player(int i_id, bool i_NPCPlayer)
{
    id = i_id;
    is_NPCPlayer = i_NPCPlayer;
	//ai = new NPCPlayerAI();
}

void Player::enableInteraction(){
	for(int i=0;i<armyVList.size();i++){
		armyVList[i]->enableInteraction();
	}
}	

void Player::disableInteraction(){
    for(int i=0;i<armyVList.size();i++){
        armyVList[i]->disableInteraction();
    }
}

void Player::addArmy(Army *army){
    armyVList.append(army);
}
