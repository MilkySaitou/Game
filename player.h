#ifndef PLAYER_H
#define PLAYER_H

#include"character.h"
#include"item.h"

using namespace std;

class Player
{
private:
	int clear_stage_num;
	int playable_character_num;
	int ITEM[4];
	int have_experience;
	character player_character[20];
public:
	Player();
	int getClearNum();
	int getPlayableNum();
	void PlayableCharacterNum(int);
	character ret_player_character(int);
	character* get_player_character(int);
	void item_Init();
	void character_Init(int, int, int, int, int, int, int, int,
		ATTACKTYPE, string, string, string, int, int, int, int, int, int, int);
};

#endif