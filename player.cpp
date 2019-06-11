#include "player.h"
#include"character.h"

Player::Player():
	clear_stage_num(0),
	playable_character_num(0),
	have_experience(0)
{
}
int Player::getClearNum() 
{
	return clear_stage_num;
}
int Player::getPlayableNum()
{
	return playable_character_num;
}
void Player::item_Init()
{
	memset(ITEM, 0, sizeof(ITEM));
}
void Player::character_Init(int AT, int DF, int MAT, int MDF, int DIR, int MOVENUM, int HP, int MP, ATTACKTYPE ATTACK, 
	string glaphic_data_place, string glaphic_face_place, string name,
	int wepID,int armID,int acceID,int magicID1,int magicID2,int magicID3,int j)
{
	player_character[j].Init(0,0,1,AT, DF, MAT, MDF, DIR, MOVENUM, HP, MP, ATTACK, glaphic_data_place, glaphic_face_place, name);
	player_character[j].WeaponEquip(wepID);
	player_character[j].ArmorEquip(armID);
	player_character[j].AccessoryEquip(acceID);
	player_character[j].MagicEquip(magicID1, 0);
	player_character[j].MagicEquip(magicID2, 1);
	player_character[j].MagicEquip(magicID3, 2);
	player_character[j].flgInsert(1);
}
character Player::ret_player_character(int j)
{
	return player_character[j];
}
character* Player::get_player_character(int j)
{
	return &player_character[j];
}
void Player::PlayableCharacterNum(int n)
{
	playable_character_num = n;
}
