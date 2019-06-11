#include "DxLib.h"
#include "KeyConfig.h"
#include <iostream>
#include <fstream>
#include <string>

int KeymapInit(Keymap &keymap)
{
	std::fstream fs("DATA\\KeyConfig.txt");
	try
	{
		if (!fs) throw 1;
	}
	catch (int)
	{
		MessageBox(nullptr, TEXT("KeyConfig.txt no available."), TEXT("ERROR"), MB_OK);
		MakeDefaultKeyConfig();
	}
	std::string buf;
	try
	{
		fs >> buf;
		keymap.button_circle = std::stoi(buf.c_str());
		fs >> buf;
		keymap.button_cross = std::stoi(buf.c_str());
		fs >> buf;
		keymap.button_rectangle = std::stoi(buf.c_str());
		fs >> buf;
		keymap.button_triangle = std::stoi(buf.c_str());
		fs >> buf;
		keymap.button_select = std::stoi(buf.c_str());
		fs >> buf;
		keymap.button_start = std::stoi(buf.c_str());
		fs >> buf;
		keymap.trigger_left1 = std::stoi(buf.c_str());
		fs >> buf;
		keymap.trigger_right1 = std::stoi(buf.c_str());
		fs >> buf;
		keymap.trigger_left2 = std::stoi(buf.c_str());
		fs >> buf;
		keymap.trigger_right2 = std::stoi(buf.c_str());
	}
	catch (std::invalid_argument e)
	{
		MessageBox(nullptr, TEXT("Incorrect Value.(KeyConfig)"), TEXT("ERROR"), MB_OK);
		MessageBox(nullptr, TEXT("Create default config file."), TEXT("ERROR"), MB_OK);
		MakeDefaultKeyConfig();
	}
	return 0;
}
//デフォルトのキーコンフィグファイルを作成
int MakeDefaultKeyConfig()
{
	std::ofstream ofs("DATA\\KeyConfig.txt");
	ofs << PAD_INPUT_4;
	ofs << std::endl;
	ofs << PAD_INPUT_3;
	ofs << std::endl;
	ofs << PAD_INPUT_1;
	ofs << std::endl;
	ofs << PAD_INPUT_2;
	ofs << std::endl;
	ofs << PAD_INPUT_11;
	ofs << std::endl;
	ofs << PAD_INPUT_12;
	ofs << std::endl;
	ofs << PAD_INPUT_5;
	ofs << std::endl;
	ofs << PAD_INPUT_6;
	ofs << std::endl;
	ofs << PAD_INPUT_7;
	ofs << std::endl;
	ofs << PAD_INPUT_8;
	return 0;
}
//キーコンフィグファイルを更新
int MakeKeyConfig(Keymap key)
{
	std::ofstream ofs("DATA\\KeyConfig.txt");
	ofs << key.button_circle;
	ofs << std::endl;
	ofs << key.button_cross;
	ofs << std::endl;
	ofs << key.button_rectangle;
	ofs << std::endl;
	ofs << key.button_triangle;
	ofs << std::endl;
	ofs << key.button_select;
	ofs << std::endl;
	ofs << key.button_start;
	ofs << std::endl;
	ofs << key.trigger_left1;
	ofs << std::endl;
	ofs << key.trigger_right1;
	ofs << std::endl;
	ofs << key.trigger_left2;
	ofs << std::endl;
	ofs << key.trigger_right2;
	return 0;
}
//キー入力処理関数
int KeyFunc(int Key, int &key_input_wait)
{
	if (CheckHitKey(Key) && key_input_wait == 0) //特定のキーをチェック
	{
		key_input_wait = 1; //キーの重み
		return 1;
	}
	return 0;
}
//ジョイパッド入力処理
int InputJoyPad(int Button, int &key_input_wait)
{
	if ((GetJoypadInputState(DX_INPUT_PAD1) & Button) && key_input_wait == 0)
	{
		key_input_wait = 1;
		return 1;
	}
	return 0;
}
//キーコンフィグ入力待ち
//キー入力があれば１を返す。それ以外なら０。
int KeyConfigWait(int& key)
{
	if (GetJoypadInputState(DX_INPUT_PAD1) != 0)
	{
		key = GetJoypadInputState(DX_INPUT_PAD1);
		return 1;
	}
	return 0;
}