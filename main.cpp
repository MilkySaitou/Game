#include"DxLib.h"
#include "Game.h"
#include "character.h"
#include "player.h"
#include "DATA_LOAD.h"
#include "KeyConfig.h"

/*データオブジェクトインスタンス*/
Weapon weapon_data[40];
Armor armor_data[40];
Accessory accessory_data[40];
Magic magic_data[40];
int sound_data[100];
Player player_data;
Keymap key;
int EXP_table[99];
/*########################*/


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetWaitVSyncFlag(0);
	DxLib_Init();
	SetMainWindowText("VOICEROID たくてぃくす!");
	SetDrawScreen(DX_SCREEN_BACK);
	ChangeWindowMode(1);
	ChangeFont("メイリオ");
	LoadWeaponData();
	LoadArmorData();
	LoadAccessoryData();
	LoadMagicData();
	LoadSoundData();
	LoadEXPtable();
	player_data.PlayableCharacterNum(2);
	LoadPlayerData();
	KeymapInit(key);
	while (!ProcessMessage())
	{
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)
		{
			break;
		}
		START_MENU* start;
		BATTLE_SCENE* game;
		start = new START_MENU;
		start->Init();
		start->START_SCREEN();
		delete start;
		start = nullptr;
		game = new BATTLE_SCENE;
		game->BattleMain(2, 4, 20, 20, "DATA\\1\\MAIN_DATA.txt");
		game->End();
		delete game;
		game = nullptr;
		SetDrawBright(255, 255, 255);
	}
	
	return 0;
}