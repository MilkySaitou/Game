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

void LoadPlayerData();  //�v���C���[�f�[�^�ǂݍ���
void LoadWeaponData();	//����f�[�^�ǂݍ���
void LoadArmorData();	//�A�[�}�[�f�[�^�ǂݍ���
void LoadAccessoryData();//�A�N�Z�T���[�f�[�^�ǂݍ���
void LoadMagicData();	//���@�f�[�^�ǂݍ���
void LoadSoundData();	//���f�[�^�ǂݍ���
void LoadEXPtable();

#endif