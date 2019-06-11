#ifndef KEYCONFIG_H
#define KEYCONFIG_H

#include "DxLib.h"
//�Q�[���p�b�h�̂��̃{�^����������Ă���Ƃ�������Ő��삷��
struct Keymap
{
	int button_circle;		//�Z
	int button_cross;		//�~
	int button_rectangle;	//��
	int button_triangle;	//��
	int button_select;		//�Z���N�g
	int button_start;		//�X�^�[�g
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