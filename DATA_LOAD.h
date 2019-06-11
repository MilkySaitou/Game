#ifndef DATA_LOAD_H
#define DATA_LOAD_H

#include"Item.h"
#include"character.h"
#include"player.h"
#include <string>
#include <fstream>

extern Player player_data;
extern Weapon weapon_data[40];
extern Armor armor_data[40];
extern Accessory accessory_data[40];
extern int sound_data[100];
extern Magic magic_data[40];
extern int EXP_table[99];

void LoadPlayerData();  //プレイヤーデータ読み込み
void LoadWeaponData();	//武器データ読み込み
void LoadArmorData();	//アーマーデータ読み込み
void LoadAccessoryData();//アクセサリーデータ読み込み
void LoadMagicData();	//魔法データ読み込み
void LoadSoundData();	//音データ読み込み
void LoadEXPtable();

#endif