#include"DxLib.h"
#include"Game.h"
#include"KeyConfig.h"

#define KeyConfigStringX 120
#define KeyConfigStringY 130
#define ConfigBackInitialMag 50
#define CENTERX 320
#define CENTERY 240

void Load_SAVEDATA();

class Fps {
	int mStartTime;         //����J�n����
	int mCount;             //�J�E���^
	float mFps;             //fps
	const int N = 60;		//���ς����T���v����
	const int FPS = 60;		//�ݒ肵��FPS

public:
	Fps() {
		mStartTime = 0;
		mCount = 0;
		mFps = 0;
	}

	bool Update() {
		if (mCount == 0) { //1�t���[���ڂȂ玞�����L��
			mStartTime = GetNowCount();
		}
		if (mCount == N) { //60�t���[���ڂȂ畽�ς��v�Z����
			int t = GetNowCount();
			mFps = 1000.f / ((t - mStartTime) / (float)N);
			mCount = 0;
			mStartTime = t;
		}
		mCount++;
		return true;
	}

	void Draw() {
		DrawFormatString(0, 0, GetColor(255, 255, 255), "%.1f", mFps);
	}

	void Wait() {
		int tookTime = GetNowCount() - mStartTime;	//������������
		int waitTime = mCount * 1000 / FPS - tookTime;	//�҂ׂ�����
		if (waitTime > 0) {
			Sleep(waitTime);	//�ҋ@
		}
	}
};



int START_MENU::START_SCREEN() 
{
	const int White = GetColor(255, 255, 255);
	const int Gray  = GetColor(150, 150, 150);
	const int Black = GetColor(0, 0, 0);
	const int titlelogo = LoadGraph("IMG\\STARTMENU\\startmenu.bmp", TRUE);
	const int glaphic_carsol = LoadGraph("IMG\\STARTMENU\\carsol.bmp", TRUE);
	int config_select_place = 0;
	enum MENU_STATE
	{
		TITLE = 0,
		START_BUTTON,
		KEY_CONF_BUTTON,
		KEY_CONF
	};
	MENU_STATE flg = TITLE;
	int key_wait = 0;
	Fps fps;

	DINPUT_JOYSTATE joypad;

	while (!ProcessMessage())
	{
		SetDrawBright(255, 255, 255);
		ClearDrawScreen();
		DrawGraph(0, 0, titlelogo, TRUE);
		if(key_wait > 0 && !GetJoypadInputState(DX_INPUT_KEY_PAD1)) key_wait--;
		GetJoypadDirectInputState(DX_INPUT_PAD1, &joypad);
		switch (flg)
		{
		case TITLE:
			DrawString(250, 300, "PUSH TO START", White);
			if (KeyFunc(KEY_INPUT_SPACE, key_wait) || InputJoyPad(key.button_start,  key_wait)) flg = START_BUTTON;
			break;
		case START_BUTTON:
			DrawString(250, 310, "START", White);
			DrawString(250, 340, "KeyConfig", Gray);
			if (KeyFunc(KEY_INPUT_SPACE, key_wait) || InputJoyPad(key.button_circle, key_wait)) return 1;
			if (KeyFunc(KEY_INPUT_DOWN,  key_wait) || InputJoyPad(PAD_INPUT_DOWN,    key_wait)) flg = KEY_CONF_BUTTON;
			if (KeyFunc(KEY_INPUT_C,	 key_wait) || InputJoyPad(key.button_cross,  key_wait)) flg = TITLE;
			
			break;
		case KEY_CONF_BUTTON:
			DrawString(250, 310, "START", Gray);
			DrawString(250, 340, "KeyConfig", White);
			if (KeyFunc(KEY_INPUT_SPACE, key_wait) || InputJoyPad(key.button_circle, key_wait)) flg = KEY_CONF;
			if (KeyFunc(KEY_INPUT_UP,	 key_wait) || InputJoyPad(PAD_INPUT_UP,		 key_wait)) flg = START_BUTTON;
			if (KeyFunc(KEY_INPUT_C,	 key_wait) || InputJoyPad(key.button_cross,  key_wait)) flg = TITLE;
			break;
		case KEY_CONF:
			static int magnification = 0;	//�w�i�̊g�嗦��\���̂Ɏg�p
			if (magnification < 150)
			{
				DrawBox(CENTERX - magnification * 2, CENTERY - magnification, CENTERX + magnification * 2, CENTERY + magnification, Gray, TRUE);
				magnification += 8;
			}
			else
			{
				DrawBox(CENTERX - magnification * 2, CENTERY - magnification, CENTERX + magnification * 2, CENTERY + magnification, Gray, TRUE);
				DrawString(KeyConfigStringX - 100, KeyConfigStringY - 30, "SPACE�L�[�������Ȃ���R���g���[���[�̃{�^�����������ƂŔz�u��ύX�ł��܂�", White);
				DrawString(KeyConfigStringX - 100, KeyConfigStringY + 200, "�~�{�^���AC�L�[�ŕۑ����ďI�����܂�", White);
				DrawString(KeyConfigStringX, KeyConfigStringY, "�Z�{�^��", White);
				DrawFormatString(KeyConfigStringX + 300, KeyConfigStringY, White, "0x%06x", key.button_circle);
				DrawString(KeyConfigStringX, KeyConfigStringY + 20, "�~�{�^��", White);
				DrawFormatString(KeyConfigStringX + 300, KeyConfigStringY + 20, White, "0x%06x", key.button_cross);
				DrawString(KeyConfigStringX, KeyConfigStringY + 40, "���{�^��", White);
				DrawFormatString(KeyConfigStringX + 300, KeyConfigStringY + 40, White, "0x%06x", key.button_rectangle);
				DrawString(KeyConfigStringX, KeyConfigStringY + 60, "���{�^��", White);
				DrawFormatString(KeyConfigStringX + 300, KeyConfigStringY + 60, White, "0x%06x", key.button_triangle);
				DrawString(KeyConfigStringX, KeyConfigStringY + 80, "�Z���N�g�{�^��", White);
				DrawFormatString(KeyConfigStringX + 300, KeyConfigStringY + 80, White, "0x%06x", key.button_select);
				DrawString(KeyConfigStringX, KeyConfigStringY + 100, "�X�^�[�g�{�^��", White);
				DrawFormatString(KeyConfigStringX + 300, KeyConfigStringY + 100, White, "0x%06x", key.button_start);
				DrawString(KeyConfigStringX, KeyConfigStringY + 120, "L1�{�^��", White);
				DrawFormatString(KeyConfigStringX + 300, KeyConfigStringY + 120, White, "0x%06x", key.trigger_left1);
				DrawString(KeyConfigStringX, KeyConfigStringY + 140, "R1�{�^��", White);
				DrawFormatString(KeyConfigStringX + 300, KeyConfigStringY + 140, White, "0x%06x", key.trigger_right1);
				DrawString(KeyConfigStringX, KeyConfigStringY + 160, "L2�{�^��", White);
				DrawFormatString(KeyConfigStringX + 300, KeyConfigStringY + 160, White, "0x%06x", key.trigger_left2);
				DrawString(KeyConfigStringX, KeyConfigStringY + 180, "R2�{�^��", White);
				DrawFormatString(KeyConfigStringX + 300, KeyConfigStringY + 180, White, "0x%06x", key.trigger_right2);
				DrawGraph(KeyConfigStringX - 30, KeyConfigStringY + 3 + 20 * config_select_place, glaphic_carsol, TRUE);
				switch (config_select_place)
				{
				case 0:
					if (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait)) config_select_place++;
					if (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait)) config_select_place = 9;
					if (CheckHitKey(KEY_INPUT_SPACE))
					{
						KeyConfigWait(key.button_circle);
					}
					break;
				case 1:
					if (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait)) config_select_place++;
					if (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait)) config_select_place--;
					if (CheckHitKey(KEY_INPUT_SPACE))
					{
						KeyConfigWait(key.button_cross);
					}
					break;
				case 2:
					if (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait)) config_select_place++;
					if (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait)) config_select_place--;
					if (CheckHitKey(KEY_INPUT_SPACE))
					{
						KeyConfigWait(key.button_rectangle);
					}
					break;
				case 3:
					if (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait)) config_select_place++;
					if (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait)) config_select_place--;
					if (CheckHitKey(KEY_INPUT_SPACE))
					{
						KeyConfigWait(key.button_triangle);
					}
					break;
				case 4:
					if (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait)) config_select_place++;
					if (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait)) config_select_place--;
					if (CheckHitKey(KEY_INPUT_SPACE))
					{
						KeyConfigWait(key.button_select);
					}
					break;
				case 5:
					if (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait)) config_select_place++;
					if (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait)) config_select_place--;
					if (CheckHitKey(KEY_INPUT_SPACE))
					{
						KeyConfigWait(key.button_start);
					}
					break;
				case 6:
					if (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait)) config_select_place++;
					if (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait)) config_select_place--;
					if (CheckHitKey(KEY_INPUT_SPACE))
					{
						KeyConfigWait(key.trigger_left1);
					}
					break;
				case 7:
					if (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait)) config_select_place++;
					if (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait)) config_select_place--;
					if (CheckHitKey(KEY_INPUT_SPACE))
					{
						KeyConfigWait(key.trigger_right1);
					}
					break;
				case 8:
					if (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait)) config_select_place++;
					if (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait)) config_select_place--;
					if (CheckHitKey(KEY_INPUT_SPACE))
					{
						KeyConfigWait(key.trigger_left2);
					}
					break;
				case 9:
					if (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait)) config_select_place = 0;
					if (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait)) config_select_place--;
					if (CheckHitKey(KEY_INPUT_SPACE))
					{
						KeyConfigWait(key.trigger_right2);
					}
					break;
				}
				if (KeyFunc(KEY_INPUT_C, key_wait) || InputJoyPad(key.button_cross, key_wait) && !CheckHitKey(KEY_INPUT_SPACE))
				{
					magnification = 0;
					MakeKeyConfig(key);
					flg = KEY_CONF_BUTTON;
				}
			}
			break;
		}
		fps.Update();	//�X�V
		fps.Draw();		//�`��
		ScreenFlip();
		fps.Wait();		//�ҋ@
		
	}
	return 0;
}