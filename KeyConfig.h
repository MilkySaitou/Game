#ifndef KEYCONFIG_H
#define KEYCONFIG_H

#include "DxLib.h"
//ゲームパッドのこのボタンが押されているという仮定で制作する
struct Keymap
{
	int button_circle;		//〇
	int button_cross;		//×
	int button_rectangle;	//△
	int button_triangle;	//□
	int button_select;		//セレクト
	int button_start;		//スタート
	int trigger_left1;		//L1
	int trigger_right1;		//R1
	int trigger_left2;		//L2
	int trigger_right2;		//R2
};

int KeymapInit(Keymap&);
int MakeDefaultKeyConfig();
int MakeKeyConfig(Keymap);
int KeyFunc(int, int&);
int InputJoyPad(int, int&);
int KeyConfigWait(int&);

#endif