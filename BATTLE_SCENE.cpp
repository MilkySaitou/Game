#include"DxLib.h"
#include<string>
#include<fstream>
#include"Game.h"
#include<cmath>
#include"item.h"

int X = 290;
int Y = 225;
int CHARACENTERX = 30;
int meirio;
int gothic;

//ゲーム終了処理
void Game::End() 
{
}

BATTLE_SCENE::BATTLE_SCENE():
	Enemy_Characternum(0),
	Player_Characternum(0),
	Characternum(0),
	key_wait(0),
	choice_carsol(0),
	choice(0),
	player_num(1),
	cx(0),
	cy(0),
	MAPWIDTH(0),
	MAPHEIGHT(0),
	last_move_x(0), 
	last_move_y(0),
	standbytemp(0),
	status_current_character_team(1),
	damage(0), //ダメージ数保持
	oldx(0), 
	oldy(0), //移動時の前のマスの座標保持
	ToDo(BATTLE_START),
	p_choice(nullptr),
	p_attack_target(nullptr),
	p_lastmovechara(nullptr),
	menu_X(-100),
	menu_X_animation(0),
	status_carsor(1),
	getEXP(0),
	moven(0), //ムーブアニメーションに必要　歩数を計る	
	bright_change(255),
	use_flame_time(0),//アニメーション表示時間制御
	camerax(0),
	cameray(0),
	current_magic(magic_data[0])
{
	std::memset(go_character, 0, sizeof(go_character));
	std::memset(p_magic_target, 0, sizeof(p_magic_target));
	std::memset(glaphics, 0, sizeof(glaphics));
	std::memset(flg, 0, sizeof(flg));
	for (int i = 0; i < sizeof(animation_glaphics) / sizeof(*animation_glaphics); i++)
	{
		for (int j = 0; j < sizeof(*animation_glaphics) / sizeof(**animation_glaphics); j++)
		{
			animation_glaphics[i][j] = 0;
		}
	}
}
void BATTLE_SCENE::BattleMain(int inPlayer_Characternum , int inEnemy_Characternum, int IN_MAPWIDTH, int IN_MAPHEIGHT,string data) 
{
	Player_Characternum = inPlayer_Characternum;
	Enemy_Characternum  = inEnemy_Characternum;
	Characternum = Player_Characternum + Enemy_Characternum;
	MAPWIDTH  = IN_MAPWIDTH;
	MAPHEIGHT = IN_MAPHEIGHT;
	MAP						= new int[MAPWIDTH * MAPHEIGHT];
	MAP_CHIP_GRAPHIC_ARRAY	= new int[MAPWIDTH * MAPHEIGHT];
	moveable				= new int[MAPWIDTH * MAPHEIGHT];
	moveable_inverse		= new int[MAPWIDTH * MAPHEIGHT];
	attack_able				= new int[MAPWIDTH * MAPHEIGHT];
	counter_attack_able		= new int[MAPWIDTH * MAPHEIGHT];
	magic_able				= new int[MAPWIDTH * MAPHEIGHT];
	magic_launch			= new int[MAPWIDTH * MAPHEIGHT];
	Battle_Chara			= new character[Player_Characternum + Enemy_Characternum];
	mapchip_glaphics		= new int[20];

	string map_data = "";
	string enemy_data = "";
	string mapchip_data = "";

	/*######################################################*/
	/*    攻撃選択配列、移動選択配列の初期化				*/
	/*######################################################*/
	memset(moveable,			0, sizeof(moveable));
	memset(moveable_inverse,	0, sizeof(moveable_inverse));
	memset(attack_able,			0, sizeof(attack_able));
	memset(counter_attack_able, 0, sizeof(counter_attack_able));
	memset(magic_able,			0, sizeof(magic_able));
	memset(magic_launch,		0, sizeof(magic_launch));
	/*######################################################*/
	LoadDataExtract		(data,map_data,enemy_data,mapchip_data);
	LoadMapChipData		(mapchip_data);
	LoadMapData			(map_data);
	LoadBattleMainData	();
	LoadEnemyData		(enemy_data);
	meirio = CreateFontToHandle("メイリオ", 28, 4);
	gothic = CreateFontToHandle("MSゴシック", 20, 14);

	//マップ配列にキャラクターフラグ埋め込み
	for (int i = 0; i < Enemy_Characternum; i++)
	{
		if (Battle_Chara[i].getflg()) 
		{
			int x = Battle_Chara[i].gety();
			MAP[Battle_Chara[i].gety() * MAPWIDTH + Battle_Chara[i].getx()] = Battle_Chara[i].getflg();
		}
	}
	int time = 0;
	int win = 0;

	class Fps {
		int mStartTime;         //測定開始時刻
		int mCount;             //カウンタ
		float mFps;             //fps
		const int N = 60;//平均を取るサンプル数
		const int FPS = 60;	//設定したFPS
	public:
		Fps() {
			mStartTime = 0;
			mCount = 0;
			mFps = 0;
		}
		bool Update() {
			if (mCount == 0) { //1フレーム目なら時刻を記憶
				mStartTime = GetNowCount();
			}
			if (mCount == N) { //60フレーム目なら平均を計算する
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
			int tookTime = GetNowCount() - mStartTime;	//かかった時間
			int waitTime = mCount * 1000 / FPS - tookTime;	//待つべき時間
			if (waitTime > 0) {
				Sleep(waitTime);	//待機
			}
		}
	};
	Fps fps;

	while (!ProcessMessage())
	{
		if (CheckHitKey(KEY_INPUT_ESCAPE)) break;

		ClearDrawScreen();
		DrawGraph(0, 0, glaphics[19], TRUE);
		Play();
		Draw();
		if (ToDo != BATTLE_START && ToDo != BATTLE_CHARACTER_SELECT_INIT && ToDo != BATTLE_CHARACTER_SELECT && ToDo != SELECT_TO_EXPLORE )
		{
			WinCheck();
		}

		//キーの重みを下げる
		if (key_wait > 0 && !GetJoypadInputState(DX_INPUT_KEY_PAD1))
		{
			key_wait--;
		}
		fps.Update();	//更新
		fps.Draw();		//描画
		ScreenFlip();
		fps.Wait();		//待機
		if ((ToDo == WIN || ToDo == LOSE) && use_flame_time > 300)
		{
			break;
		}
		
	}
	delete[] Battle_Chara;
	delete[] MAP;
	delete[] MAP_CHIP_GRAPHIC_ARRAY;
	delete[] moveable;
	delete[] moveable_inverse;
	delete[] attack_able;
	delete[] counter_attack_able;
	delete[] magic_able;
	delete[] magic_launch;
	delete[] mapchip_glaphics;

	Battle_Chara			= nullptr;
	MAP						= nullptr;
	MAP_CHIP_GRAPHIC_ARRAY	= nullptr;
	moveable				= nullptr;
	moveable_inverse		= nullptr;
	attack_able				= nullptr;
	counter_attack_able		= nullptr;
	magic_able				= nullptr;
	magic_launch			= nullptr;
	mapchip_glaphics		= nullptr;
}
/*バトルロジック部*/
void BATTLE_SCENE::Play() {
	switch (ToDo)
	{
	case BATTLE_START:
		use_flame_time++;
		if (use_flame_time > 40)
		{
			ToDo = BATTLE_CHARACTER_SELECT_INIT;
			use_flame_time = 0;
		}
		break;
	case BATTLE_CHARACTER_SELECT_INIT:
		for (int i = 0; i < player_data.getPlayableNum(); i++)
		{
			go_character[i] = 0;
		}
		ToDo = BATTLE_CHARACTER_SELECT;
		break;

	case BATTLE_CHARACTER_SELECT:
		/*#################################
		flg[0] = 0 → 参戦キャラクター選択
		flg[0] = 1 → チェック画面に遷移
		flg[1] = 0 → はい
		flg[1] = 1 → いいえ
		###################################*/
		switch (flg[0])
		{
		case 0:
			if ((KeyFunc(KEY_INPUT_RIGHT, key_wait) || InputJoyPad(PAD_INPUT_RIGHT, key_wait)) && choice_carsol < Characternum - Enemy_Characternum - 1)
			{
				DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
				choice_carsol++;
			}
			if ((KeyFunc(KEY_INPUT_LEFT, key_wait) || InputJoyPad(PAD_INPUT_LEFT, key_wait)) && choice_carsol > 0)
			{
				DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
				choice_carsol--;
			}
			if ((KeyFunc(KEY_INPUT_SPACE, key_wait) || InputJoyPad(key.button_circle, key_wait)) && choice <= Characternum - Enemy_Characternum - 1 && !go_character[choice_carsol])
			{
				go_character[choice_carsol] = 1;
				DxLib::PlaySoundMem(sound_data[1], DX_PLAYTYPE_BACK);
				choice++;
			}
			if ((KeyFunc(KEY_INPUT_C, key_wait) || InputJoyPad(key.button_cross, key_wait)) && go_character[choice_carsol])
			{
				go_character[choice_carsol] = 0;
				DxLib::PlaySoundMem(sound_data[3], DX_PLAYTYPE_BACK);
				choice--;
			}
			if ((KeyFunc(KEY_INPUT_T, key_wait) || InputJoyPad(key.button_start, key_wait)) && choice > 0)
			{
				DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
				flg[0] = 1;
				flg[1] = 1;
			}
			break;
		case 1:
			if (KeyFunc(KEY_INPUT_C, key_wait) || InputJoyPad(key.button_cross, key_wait))
			{
				DxLib::PlaySoundMem(sound_data[3], DX_PLAYTYPE_BACK);
				flg[0] = 0;
				flg[1] = 1;
			}
			if ((KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait)) && flg[1] == 0)
			{
				DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
				flg[1] = 1;
			}
			if ((KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait)) && flg[1] == 1)
			{
				DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
				flg[1] = 0;
			}
			
			if (KeyFunc(KEY_INPUT_SPACE, key_wait) || InputJoyPad(key.button_circle, key_wait))
			{
				//戦闘がスタートする
				if (flg[1] == 0)
				{
					for (int i = 0; i < player_data.getPlayableNum(); i++)
					{
						if (go_character[i])
						{
							Battle_Chara[Enemy_Characternum + i] = player_data.ret_player_character(i);
						}
					}
					//参戦キャラクターの初期配置を決める.
					for (int i = Enemy_Characternum; i < Characternum; i++)
					{
						if (Battle_Chara[i].getflg())
						{
							for (int j = 0; j < MAPWIDTH * MAPHEIGHT; j++)
							{
								if (MAP[j] == 3)
								{
									Battle_Chara[i].x_insert(j%MAPWIDTH);
									Battle_Chara[i].y_insert(j / MAPWIDTH);
									MAP[j] = 1;
									break;
								}
							}
						}
					}
					//マップ上の自キャラ格納可能位置を3から0にする.
					for (int j = 0; j < MAPWIDTH * MAPHEIGHT; j++)
					{
						if (MAP[j] == 3)
						{
							MAP[j] = 0;
						}
					}
					use_flame_time = 0;
					flg[0] = 2;
					DxLib::PlaySoundMem(sound_data[6], DX_PLAYTYPE_BACK);
					DxLib::PlaySoundMem(BGM, DX_PLAYTYPE_LOOP);
				}
				if (flg[1] == 1)
				{
					flg[0] = 0;
					flg[1] = 1;
				}
			}
			break;
		case 2:
			if (use_flame_time <= 60)
			{
				fade(10);
				use_flame_time++;
			}
			else
			{
				use_flame_time = 0;
				ToDo = SELECT_TO_EXPLORE;
			}
			break;
		}
		break;
	case SELECT_TO_EXPLORE:
		if (use_flame_time <= 60)
		{
			use_flame_time++;
		}
		else
		{
			use_flame_time = 0;
			//１番目の自キャラにカーソルを合わせてスタート
			cx = Battle_Chara[Enemy_Characternum].getx();
			cy = Battle_Chara[Enemy_Characternum].gety();
			CharaDrawCalc();
			ToDo = EXPLORE;
		}
		break;

	case TURN_CHANGE://ターン交代
		if (use_flame_time == 50)
		{
			if (player_num == 1) {
				player_num = 2;
			}
			else if (player_num == 2) {
				player_num = 1;
			}
			for (int i = 0; i < Characternum; i++)
			{
				if (Battle_Chara[i].getflg() == player_num)
				{
					cx = Battle_Chara[i].getx();
					cy = Battle_Chara[i].gety();
				}
			}
			for (int i = 0; i < Characternum; i++)
			{
				Battle_Chara[i].TurnInit();
			}
			p_lastmovechara = NULL;
			if (player_num == 1)
			{
				use_flame_time = 0;
				ToDo = EXPLORE;
			}
			else
			{
				use_flame_time = 0;
				ToDo = ENEMY_TURN_AI_THINK;
			}
		}
		use_flame_time++;
		break;

		/*## カーソルを移動してプレイヤーが選択する時間 ##*/
	case EXPLORE:
		if (flg[4] == 0 && flg[6] == 0)
		{
			CarsorMove();
			CharaDrawCalc();
			for (int i = 0; i < Characternum; i++)
			{
				if (Battle_Chara[i].getflg() == 1 || Battle_Chara[i].getflg() == 2)
				{
					if (Battle_Chara[i].getx() == cx && Battle_Chara[i].gety() == cy)
					{
						p_choice = &Battle_Chara[i];
					}
				}
			}
			if (MAP[cy * MAPWIDTH + cx] != 1 && MAP[cy * MAPWIDTH + cx] != 2)
			{
				p_choice = NULL;
			}
			if (MAP[cy * MAPWIDTH + cx] == player_num && (KeyFunc(KEY_INPUT_SPACE, key_wait) || InputJoyPad(key.button_circle, key_wait)))
			{
				menu_X_animation = 0;
				menu_X = -100;
				ToDo = MENU;
				DxLib::PlaySoundMem(sound_data[4], DX_PLAYTYPE_BACK);
				flg[2] = 1; //menu時に使用
			}
			if (menu_X > -100)
			{
				menu_X -= 25 - menu_X_animation;
				menu_X_animation++;
			}

			//敵キャラが選択されたときの処理

			if (MAP[cy * MAPWIDTH + cx] != player_num && p_choice != NULL && (KeyFunc(KEY_INPUT_SPACE, key_wait) || InputJoyPad(key.button_circle, key_wait)))
			{
				p_choice->MoveAbleInit(moveable, moveable_inverse, MAPWIDTH, MAPHEIGHT);
				MoveAbleZone(p_choice->getx(), p_choice->gety(), p_choice->getMOV());
				DxLib::PlaySoundMem(sound_data[1], DX_PLAYTYPE_BACK);
				ToDo = ENEMY_MOVE_ABLE;
			}
			//ヒントを表示
			if (KeyFunc(KEY_INPUT_H, key_wait) || InputJoyPad(key.button_select, key_wait))
			{
				flg[6] = 1;
			}
			//ターンチェンジ
			if (KeyFunc(KEY_INPUT_T, key_wait) || InputJoyPad(key.button_start, key_wait))
			{
				flg[4] = 1;
				flg[5] = 1;
			}
			if ((KeyFunc(KEY_INPUT_X, key_wait) || InputJoyPad(key.button_rectangle, key_wait)) && MAP[cy * MAPWIDTH + cx] == player_num && p_choice != NULL)
			{
				DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
				p_choice->DirectionChange();
			}
			//移動キャンセル処理
			if ((KeyFunc(KEY_INPUT_C, key_wait) || InputJoyPad(key.button_cross, key_wait)) && p_lastmovechara != NULL)
			{
				MAP[p_lastmovechara->gety() * MAPWIDTH + p_lastmovechara->getx()] = 0;
				p_lastmovechara->MoveCancel(last_move_x, last_move_y);
				MAP[last_move_y * MAPWIDTH + last_move_x] = p_lastmovechara->getflg();
				cx = last_move_x;
				cy = last_move_y;
				DxLib::PlaySoundMem(sound_data[3], DX_PLAYTYPE_BACK);
				p_lastmovechara = NULL;
			}
		}
		else if(flg[4] == 1 && flg[6] == 0)
		{
			//ターンチェンジ前のチェック画面
			if (KeyFunc(KEY_INPUT_C, key_wait) || InputJoyPad(key.button_cross, key_wait))
			{
				flg[4] = 0;
				flg[5] = 1;
				DxLib::PlaySoundMem(sound_data[3], DX_PLAYTYPE_BACK);
			}
			if(flg[5] == 0 && (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait)))
			{
				flg[5] = 1;
				DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
			}
			if (flg[5] == 1 && (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait)))
			{
				flg[5] = 0;
				DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
			}
			if (flg[5] == 1 && (KeyFunc(KEY_INPUT_SPACE, key_wait) || InputJoyPad(key.button_circle, key_wait)))
			{
				flg[4] = 0;
				flg[5] = 1;
				DxLib::PlaySoundMem(sound_data[3], DX_PLAYTYPE_BACK);
			}
			if (flg[5] == 0 && (KeyFunc(KEY_INPUT_SPACE, key_wait) || InputJoyPad(key.button_circle, key_wait)))
			{
				flg[4] = 0;
				use_flame_time = 0;
				DxLib::PlaySoundMem(sound_data[1], DX_PLAYTYPE_BACK);
				ToDo = TURN_CHANGE;
				break;
			}
		}
		else if(flg[4] == 0 && flg[6] == 1)
		{
			if (KeyFunc(KEY_INPUT_C, key_wait) || InputJoyPad(key.button_cross, key_wait))
			{
				flg[6] = 0;
			}
		}

		break;

		/*## 移動、攻撃などの選択 ##*/
	case MENU:
		/*############
		flg[2]で遷移 #
		1.移動		 #
		2.攻撃		 #
		3.魔法使用	 #
		4.道具		 #
		5.状態		 #
		6.待機型変更 #
		##############*/
		if (flg[2] != 7 && KeyFunc(KEY_INPUT_C, key_wait) || InputJoyPad(key.button_cross, key_wait))
		{
			menu_X_animation = 0;
			flg[2] = 1;
			DxLib::PlaySoundMem(sound_data[3], DX_PLAYTYPE_BACK);
			ToDo = EXPLORE;
		}
		if (menu_X < 20)
		{
			menu_X += 25 - menu_X_animation;
			menu_X_animation++;
		}
		if (menu_X > 20)
		{
			menu_X_animation = 0;
			menu_X = 20;
		}
		switch (flg[2])
		{
		case 1://移動
			if (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait))
			{
				DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
				flg[2]++;
			}
			if (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait))
			{
				DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
				flg[2] = 6;
			}
			if ((KeyFunc(KEY_INPUT_SPACE, key_wait) || InputJoyPad(key.button_circle, key_wait)) && p_choice->getmoveflg())
			{
				p_choice->MoveAbleInit(moveable, moveable_inverse, MAPWIDTH, MAPHEIGHT);
				MoveAbleZone(p_choice->getx(),p_choice->gety(),p_choice->getMOV());
				menu_X = -100;
				DxLib::PlaySoundMem(sound_data[1], DX_PLAYTYPE_BACK);
				ToDo = MOVE;
			}
			break;
		case 2://攻撃
			if (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait))
			{
				DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
				flg[2]++;
			}
			if (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait))
			{
				DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
				flg[2]--;
			}
			if ((KeyFunc(KEY_INPUT_SPACE, key_wait) || InputJoyPad(key.button_circle, key_wait)) && p_choice->getattackflg())
			{
				AttackAbleZone();
				menu_X = -100;
				DxLib::PlaySoundMem(sound_data[1], DX_PLAYTYPE_BACK);
				ToDo = ATTACK;
			}
			break;
		case 3://魔法
			if (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait))
			{
				DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
				flg[2]++;
			}
			if (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait))
			{
				DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
				flg[2]--;
			}
			if ((KeyFunc(KEY_INPUT_SPACE, key_wait) || InputJoyPad(key.button_circle, key_wait))
				&& p_choice->getattackflg() && !(p_choice->getMagicID(0) == 0 
					&& p_choice->getMagicID(1) == 0 && p_choice->getMagicID(2) == 0))
			{
				DxLib::PlaySoundMem(sound_data[1], DX_PLAYTYPE_BACK);
				flg[2] = 7;
				flg[3] = 1;
			}
			break;
		case 4:
			if (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait))
			{
				DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
				flg[2]++;
			}
			if (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait))
			{
				DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
				flg[2]--;
			}
			break;
		case 5://状態
			if (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait))
			{
				DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
				flg[2]++;
			}
			if (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait))
			{
				DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
				flg[2]--;
			}
			if (KeyFunc(KEY_INPUT_SPACE, key_wait) || InputJoyPad(key.button_circle, key_wait))
			{
				use_flame_time = 0;
				bright_change = 255;
				status_current_character_team = p_choice->getflg();
				DxLib::PlaySoundMem(sound_data[6], DX_PLAYTYPE_BACK);
				ToDo = MENU_TO_STATUS;
			}
			break;
		case 6://待機型
			if (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait))
			{
				DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
				flg[2] = 1;
			}
			if (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait))
			{
				DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
				flg[2]--;
			}
			if (KeyFunc(KEY_INPUT_SPACE, key_wait) || InputJoyPad(key.button_circle, key_wait)) p_choice->StandByChange();
			break;
		case 7: //魔法選択
			if (KeyFunc(KEY_INPUT_C, key_wait) || InputJoyPad(key.button_cross, key_wait))
			{
				DxLib::PlaySoundMem(sound_data[3], DX_PLAYTYPE_BACK);
				flg[2] = 3;
			}
			switch(flg[3])
			{
			case 1:
				if (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					flg[3]++;
				}
				if (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					flg[3] = 3;
				}
				if ((KeyFunc(KEY_INPUT_SPACE, key_wait) || InputJoyPad(key.button_circle, key_wait))
					&& p_choice->getMagicID(0) != 0 && p_choice->getMP() - magic_data[p_choice->getMagicID(0)].getUseMP() >= 0)
				{
					current_magic = magic_data[p_choice->getMagicID(0)];
					//魔法攻撃範囲初期化
					for (int i = 0; i < MAPHEIGHT; i++)
					{
						for (int j = 0; j < MAPWIDTH; j++)
						{
							magic_able[j + i * MAPWIDTH] = 0;
						}
					}
					
					MagicAbleZone(p_choice->getx(),p_choice->gety(),current_magic.getFireDistance());
					ToDo = MAGIC;
				}
			case 2:
				if (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					flg[3]++;
				}
				if (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					flg[3]--;
				}
				//スペースが押されたときの反応
				if ((KeyFunc(KEY_INPUT_SPACE, key_wait) || InputJoyPad(key.button_circle, key_wait)) 
					&& p_choice->getMagicID(1) != 0 && p_choice->getMP() - magic_data[p_choice->getMagicID(1)].getUseMP() >= 0)
				{
					current_magic = magic_data[p_choice->getMagicID(1)];
					//魔法攻撃範囲初期化
					for (int i = 0; i < MAPHEIGHT; i++)
					{
						for (int j = 0; j < MAPWIDTH; j++)
						{
							magic_able[j + i * MAPWIDTH] = 0;
						}
					}
					MagicAbleZone(p_choice->getx(), p_choice->gety(), current_magic.getFireDistance());
					ToDo = MAGIC;
				}
			case 3:
				if (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					flg[3] = 1;
				}
				if (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					flg[3]--;
				}
				if ((KeyFunc(KEY_INPUT_SPACE, key_wait) || InputJoyPad(key.button_circle, key_wait))
					&& p_choice->getMagicID(2) != 0 && p_choice->getMP() - magic_data[p_choice->getMagicID(2)].getUseMP() >= 0)
				{
					current_magic = magic_data[p_choice->getMagicID(2)];
					
					//魔法攻撃範囲初期化
					for (int i = 0; i < MAPHEIGHT; i++)
					{
						for (int j = 0; j < MAPWIDTH; j++)
						{
							magic_able[j + i * MAPWIDTH] = 0;
						}
					}
					MagicAbleZone(p_choice->getx(), p_choice->gety(), current_magic.getFireDistance());
					DxLib::PlaySoundMem(sound_data[1], DX_PLAYTYPE_BACK);
					ToDo = MAGIC;
				}
			}
			break;
		}
		break;

/*##################################################### 移動処理 ###########################################################*/

	case MOVE:
		CarsorMove();
		CharaDrawCalc();
		if ((KeyFunc(KEY_INPUT_SPACE, key_wait) || InputJoyPad(key.button_circle, key_wait)) && !MAP[cy * MAPWIDTH + cx])
		{
			if (p_choice->MovAbleFlgFunc(cx, cy, moveable, MAPWIDTH))
			{
				p_choice->MoveRouteCalculation(cx, cy, moveable, moveable_inverse, MAPWIDTH, MAPHEIGHT);
				p_choice->DrawModeInsert(DRAW_MODE_MOVE);
				/* 移動後のキャンセル時に利用する */
				p_lastmovechara = p_choice;
				last_move_x = p_choice->getx();
				last_move_y = p_choice->gety();
				MAP[p_choice->gety() * MAPWIDTH + p_choice->getx()] = 0;
				MAP[cy * MAPWIDTH + cx] = p_choice->getflg();
				use_flame_time = 20;
				moven = 0;
				DxLib::PlaySoundMem(sound_data[8], DX_PLAYTYPE_BACK);
				ToDo = MOVE_ANIMATION;
			}
		}
		if (KeyFunc(KEY_INPUT_C, key_wait) || InputJoyPad(key.button_cross, key_wait))
		{
			cx = p_choice->getx();
			cy = p_choice->gety();
			CharaDrawCalc();
			DxLib::PlaySoundMem(sound_data[3], DX_PLAYTYPE_BACK);
			ToDo = MENU;
		}
		break;
		/*## 移動アニメーション処理 ##*/
	case MOVE_ANIMATION:
		if (use_flame_time == 20)
		{
			moven++;
			if (p_choice->getx() == cx && p_choice->gety() == cy)
			{
				use_flame_time = 0;
				ToDo = MOVE_POST;
				break;
			}
			p_choice->MoveShow(moven,moveable_inverse, MAPHEIGHT, MAPWIDTH);
			use_flame_time = 0;
		}
		p_choice->MoveCharacterCordinateCalc(cx, cy, use_flame_time);
		use_flame_time++;
		break;
		/*## 移動後処理 ##*/
	case MOVE_POST:
		
		if (p_choice->getdownflg() == 1)
		{
			p_choice->DrawModeInsert(DRAW_MODE_DOWN);
		}
		else
		{
			p_choice->DrawModeInsert(DRAW_MODE_NORMAL);
		}
		if (use_flame_time < 15)
		{
			use_flame_time++;
		}
		else
		{
			use_flame_time = 0;
			ToDo = EXPLORE;
		}
		CharaDrawCalc();
		break;

/*##################################################### 攻撃 ##############################################################*/

	case ATTACK://攻撃時処理(完成)
		for (int i = 0; i < Characternum; i++)
		{
			if (Battle_Chara[i].getflg() == 1 || Battle_Chara[i].getflg() == 2)
			{
				if (Battle_Chara[i].getx() == cx && Battle_Chara[i].gety() == cy)
				{
					p_attack_target = &Battle_Chara[i];
				}
			}
		}
		if (MAP[cy * MAPWIDTH + cx] != 1 && MAP[cy * MAPWIDTH + cx] != 2)
		{
			p_attack_target = NULL;
		}
		use_flame_time++;
		CarsorMove();
		CharaDrawCalc();
		if (!MAP[cy * MAPWIDTH + cx])
		{
			p_attack_target = NULL;
		}
		if (KeyFunc(KEY_INPUT_C, key_wait) || InputJoyPad(key.button_cross, key_wait))
		{
			ToDo = MENU;
			cx = p_choice->getx();
			cy = p_choice->gety();
			CharaDrawCalc();
			p_attack_target = NULL;
			DxLib::PlaySoundMem(sound_data[3], DX_PLAYTYPE_BACK);
			break;
		}
		if ((KeyFunc(KEY_INPUT_SPACE, key_wait) || InputJoyPad(key.button_circle, key_wait))
			&& p_choice->AttackableFlgFunc(p_attack_target, attack_able, MAPWIDTH, MAPHEIGHT))
		{
			p_lastmovechara = NULL;
			standbytemp = p_choice->getstandbyflg();
			flg[2] = 1;
			p_choice->Attack(p_attack_target, attack_able, MAPWIDTH, MAPHEIGHT);
			damage = p_choice->AttackDamageCalculation(p_attack_target);
			p_choice->DrawModeInsert(DRAW_MODE_ATTACK);
			cx = p_choice->getx();
			cy = p_choice->gety();
			CharaDrawCalc();
			use_flame_time = 0;
			DxLib::PlaySoundMem(sound_data[8], DX_PLAYTYPE_BACK);
			ToDo = ATTACK_ANIMATION;
		}
		break;

	case ATTACK_ANIMATION://攻撃アニメーション
		if (use_flame_time < ATTACKANIMATIONTIME) {
			if (use_flame_time == 42)
			{
				DxLib::PlaySoundMem(sound_data[0], DX_PLAYTYPE_BACK);
				p_attack_target->DrawModeInsert(DRAW_MODE_DAMAGE);
			}
			if (use_flame_time == 80)
			{
				p_attack_target->DownCheck();
			}
			use_flame_time++;
		}
		else {
			if (p_choice->getdownflg() == 1)
			{
				p_choice->DrawModeInsert(DRAW_MODE_DOWN);
			}
			else
			{
				p_choice->DrawModeInsert(DRAW_MODE_NORMAL);
			}
			if (p_attack_target->getdownflg() == 1)
			{
				p_attack_target->DrawModeInsert(DRAW_MODE_DOWN);
			}
			else
			{
				p_attack_target->DrawModeInsert(DRAW_MODE_NORMAL);
			}
			ToDo = ATTACK_POST;
		}
		break;

	case ATTACK_POST://攻撃後処理
		if (p_attack_target->getHP() <= 0)
		{
			MAP[p_attack_target->gety() * MAPWIDTH + p_attack_target->getx()] = 0;
			p_attack_target->removefunc(getEXP);
			cx = p_choice->getx();
			cy = p_choice->gety();
			CharaDrawCalc();
			p_attack_target = NULL;
			ToDo = EXPLORE;
			break;
		}
		if (p_attack_target->getstandbyflg() == 0 && p_attack_target->getflg() != p_choice->getflg())
		{
			ToDo = COUNTER;
		}
		else 
		{
			p_choice->DrawModeInsert(DRAW_MODE_NORMAL);
			cx = p_choice->getx();
			cy = p_choice->gety();
			ToDo = EXPLORE;
		}
		break;

	case COUNTER://カウンター処理
		if (p_attack_target->getflg() && !(p_attack_target->getstandbyflg()))
		{
			CounterAbleZone();
			if (p_attack_target->AttackableFlgFunc(p_choice, counter_attack_able, MAPWIDTH, MAPHEIGHT))
			{
				p_attack_target->Counter(p_choice, counter_attack_able, MAPWIDTH, MAPHEIGHT);
				damage = p_attack_target->AttackDamageCalculation(p_choice);
				cx = p_attack_target->getx();
				cy = p_attack_target->gety();
				CharaDrawCalc();
				p_attack_target->DrawModeInsert(DRAW_MODE_ATTACK);
				use_flame_time = 0;
				ToDo = COUNTER_ANIMATION;
			}
			else
			{
				ToDo = COUNTER_POST;
			}
			break;
		}
		ToDo = COUNTER_POST;
		break;

	case COUNTER_ANIMATION://カウンター攻撃アニメーション
		if (use_flame_time < ATTACKANIMATIONTIME) {
			if (use_flame_time == 42)
			{
				DxLib::PlaySoundMem(sound_data[0], DX_PLAYTYPE_BACK);
				p_choice->DrawModeInsert(DRAW_MODE_DAMAGE);
			}
			if (use_flame_time == 80)
			{
				p_choice->DownCheck();
			}
			use_flame_time++;
		}
		else 
		{
			if (p_choice->getdownflg() == 1)
			{
				p_choice->DrawModeInsert(DRAW_MODE_DOWN);
			}
			else
			{
				p_choice->DrawModeInsert(DRAW_MODE_NORMAL);
			}
			if (p_attack_target->getdownflg() == 1)
			{
				p_attack_target->DrawModeInsert(DRAW_MODE_DOWN);
			}
			else
			{
				p_attack_target->DrawModeInsert(DRAW_MODE_NORMAL);
			}
			use_flame_time = 0;
			ToDo = COUNTER_POST;
		}
		break;

	case COUNTER_POST://カウンター後処理
		if (p_choice->getHP() <= 0)
		{
			MAP[p_choice->gety() * MAPWIDTH + p_choice->getx()] = 0;
			p_choice->removefunc(getEXP);
		}
		if (standbytemp != p_choice->getstandbyflg())
		{
			p_choice->StandByChange();
		}
		if (use_flame_time < 15)
		{
			use_flame_time++;
		}
		else
		{
			cx = p_choice->getx();
			cy = p_choice->gety();
			CharaDrawCalc();
			p_attack_target = nullptr;
			ToDo = EXPLORE;
		}
		break;

/*################################################ 魔法 ###########################################################*/

	case MAGIC:
		CarsorMove();
		for (int i = 0; i < Characternum; i++)
		{
			if (Battle_Chara[i].getflg() == 1 || Battle_Chara[i].getflg() == 2)
			{
				if (Battle_Chara[i].getx() == cx && Battle_Chara[i].gety() == cy)
				{
					p_attack_target = &Battle_Chara[i];
				}
			}
		}
		if (MAP[cy * MAPWIDTH + cx] != 1 && MAP[cy * MAPWIDTH + cx] != 2)
		{
			p_attack_target = NULL;
		}
		CharaDrawCalc();
		if (KeyFunc(KEY_INPUT_C, key_wait) || InputJoyPad(key.button_cross, key_wait))
		{
			cx = p_choice->getx();
			cy = p_choice->gety();
			CharaDrawCalc();
			DxLib::PlaySoundMem(sound_data[3], DX_PLAYTYPE_BACK);
			ToDo = MENU;
		}
		if (magic_able[cx + cy * MAPWIDTH] == 1 && (KeyFunc(KEY_INPUT_SPACE, key_wait) || InputJoyPad(key.button_circle, key_wait)))
		{
			for (int i = 0; i < MAPHEIGHT; i++)
			{
				for (int j = 0; j < MAPWIDTH; j++)
				{
					magic_launch[j + i * MAPWIDTH] = 0;
				}
			}
			MagicLaunchZone(cx, cy, current_magic.getDamageRange());
			DxLib::PlaySoundMem(sound_data[1], DX_PLAYTYPE_BACK);
			ToDo = MAGIC_LAUNCH;
		}
		break;
	case MAGIC_LAUNCH:
		CarsorMove();
		CharaDrawCalc();
		for (int i = 0; i < MAPHEIGHT; i++)
		{
			for (int j = 0; j < MAPWIDTH; j++)
			{
				magic_launch[j + i * MAPWIDTH] = 0;
			}
		}
		if (MAP[cy * MAPWIDTH + cx] != 1 && MAP[cy * MAPWIDTH + cx] != 2)
		{
			p_attack_target = NULL;
		}
		if (magic_able[cx + cy * MAPWIDTH] == 1)
		{
			magic_launch[cx + cy * MAPWIDTH] = 1;
			MagicLaunchZone(cx, cy, current_magic.getDamageRange());
		}
		//魔法攻撃先決定時処理
		if (magic_able[cx + cy * MAPWIDTH] == 1 && (KeyFunc(KEY_INPUT_SPACE, key_wait) || InputJoyPad(key.button_circle, key_wait)))
		{
			magic_target_num = 0;
			for (int i = 0; i < Characternum; i++)
			{
				if (magic_launch[Battle_Chara[i].getx() + Battle_Chara[i].gety() * MAPWIDTH] == 1)
				{
					p_magic_target[magic_target_num] = &Battle_Chara[i];
					p_choice->MagicAttack(p_magic_target[magic_target_num], current_magic);
					magic_target_num++;
				}
			}
			p_choice->MagicUseMP(current_magic);
			use_flame_time = 0;
			p_choice->DrawModeInsert(DRAW_MODE_MAGIC);
			menu_X = -100;
			DxLib::PlaySoundMem(sound_data[8], DX_PLAYTYPE_BACK);
			ToDo = MAGIC_ANIMATION;
			break;
		}
		//キャンセル
		if (KeyFunc(KEY_INPUT_C, key_wait) || InputJoyPad(key.button_cross, key_wait))
		{
			DxLib::PlaySoundMem(sound_data[3], DX_PLAYTYPE_BACK);
			ToDo = MAGIC;
		}
		break;

	/*_________________________________________________________________________*/

	case MAGIC_ANIMATION:
		/*##################
		0-40フレーム詠唱描画
		40-80魔法描画
		80-120ダメージ描画
		###################*/
		if (use_flame_time == 40)
		{
			if (p_choice->getdownflg() == 0)
			{
				p_choice->DrawModeInsert(DRAW_MODE_NORMAL);
			}
			else
			{
				p_choice->DrawModeInsert(DRAW_MODE_DOWN);
			}
			for (int i = 0; i < magic_target_num; i++)
			{
				if (p_magic_target[i] != NULL)
				{
					p_magic_target[i]->DrawModeInsert(DRAW_MODE_DAMAGE);
				}
			}
			current_magic.SoundMagic();
		}
		if (use_flame_time == 80)
		{
			DxLib::PlaySoundMem(sound_data[0], DX_PLAYTYPE_BACK);
		}
		if (use_flame_time == 120) 
		{
			for (int i = 0; i < magic_target_num; i++)
			{
				if (p_magic_target[i] != NULL)
				{
					p_magic_target[i]->DownCheck();
				}
			}
			use_flame_time = 0;
			ToDo = MAGIC_POST;
			break;
		}
		use_flame_time++;
		break;

	/*_______________________________________________________________*/

	case MAGIC_POST:
		if (use_flame_time == 0)
		{
			for (int i = 0; i < magic_target_num; i++)
			{
				if (p_magic_target[i] != NULL)
				{
					if (p_magic_target[i]->getdownflg() == 0)
					{
						p_magic_target[i]->DrawModeInsert(DRAW_MODE_NORMAL);
					}
					else
					{
						p_magic_target[i]->DrawModeInsert(DRAW_MODE_DOWN);
					}
				}
				if (p_magic_target[i]->getHP() <= 0)
				{
					p_magic_target[i]->removefunc(getEXP);
					MAP[p_magic_target[i]->getx() + p_magic_target[i]->gety() * MAPWIDTH] = 0;
				}
			}
		}
		if(use_flame_time == 40)
		{
			cx = p_choice->getx();
			cy = p_choice->gety();
			CharaDrawCalc();
			use_flame_time = 0;
			p_attack_target = nullptr;
			ToDo = EXPLORE;
		}
		use_flame_time++;
		break;

/*############################################## ステータス画面 #####################################################*/
	case MENU_TO_STATUS:
		if (use_flame_time <= 40)
		{
			SetDrawBright(bright_change, bright_change, bright_change);
			use_flame_time++;
		}

		if (use_flame_time > 40)
		{
			use_flame_time = 0;
			ToDo = STATUS;
			status_carsor = 1;
		}
		break;

	case STATUS:
		if (use_flame_time <= 40)
		{
			SetDrawBright(bright_change, bright_change, bright_change);
			use_flame_time++;
		}
		if (use_flame_time > 40)
		{
			if (KeyFunc(KEY_INPUT_C, key_wait) || InputJoyPad(key.button_cross, key_wait))
			{
				use_flame_time = 0;
				if (p_choice->getflg() == player_num)
				{
					DxLib::PlaySoundMem(sound_data[3], DX_PLAYTYPE_BACK);
					ToDo = STATUS_TO_MENU1;
				}
				else
				{
					DxLib::PlaySoundMem(sound_data[3], DX_PLAYTYPE_BACK);
					ToDo = STATUS_TO_EXPLORE1;
				}
			}
			if (KeyFunc(KEY_INPUT_R, key_wait) || InputJoyPad(key.trigger_right1, key_wait))
			{
				//ステータス画面に入ったのが敵キャラからの場合
				if (status_current_character_team == 2)
				{
					status_carsor = 1;
					for (int i = 0; i < Enemy_Characternum; i++)
					{
						if (p_choice == &Battle_Chara[i])
						{
							do {
								if (i == Enemy_Characternum - 1)
								{
									i = 0;
								}
								else
								{
									i++;
								}
								p_choice = &Battle_Chara[i];
							} while (p_choice->getflg() == 0);
							cx = p_choice->getx();
							cy = p_choice->gety();
							CharaDrawCalc();
							DxLib::PlaySoundMem(sound_data[9], DX_PLAYTYPE_BACK);
							break;
						}
					}
				}
				//ステータス画面に入ったのが味方キャラからの場合
				else
				{
					status_carsor = 1;
					for (int i = Enemy_Characternum; i < Characternum; i++)
					{
						if (p_choice == &Battle_Chara[i])
						{
							do{
								if (i == Characternum - 1)
								{
									i = Enemy_Characternum;
								}
								else
								{
									i++;
								}
								p_choice = &Battle_Chara[i];
							} while (p_choice->getflg() == 0);
							cx = p_choice->getx();
							cy = p_choice->gety();
							CharaDrawCalc();
							DxLib::PlaySoundMem(sound_data[9], DX_PLAYTYPE_BACK);
							break;
						}
					}
				}
			}
			if (KeyFunc(KEY_INPUT_Q, key_wait) || InputJoyPad(key.trigger_left1, key_wait))
			{
				//ステータス画面に入ったのが敵キャラからの場合
				if (status_current_character_team == 2)
				{
					status_carsor = 1;
					for (int i = 0; i < Enemy_Characternum; i++)
					{
						if (p_choice == &Battle_Chara[i])
						{
							do{
								if (i == 0)
								{
									i = Enemy_Characternum - 1;
								}
								else
								{
									i--;
								}
								p_choice = &Battle_Chara[i];
							} while (p_choice->getflg() == 0);
							cx = p_choice->getx();
							cy = p_choice->gety();
							CharaDrawCalc();
							DxLib::PlaySoundMem(sound_data[9], DX_PLAYTYPE_BACK);
							break;
						}
					}
				}
				//ステータス画面に入ったのが味方キャラからの場合
				else
				{
					status_carsor = 1;
					for (int i = Enemy_Characternum; i < Characternum; i++)
					{
						if (p_choice == &Battle_Chara[i])
						{
							do{
								if (i == Enemy_Characternum)
								{
									i = Characternum - 1;
								}
								else
								{
									i--;
								}
								p_choice = &Battle_Chara[i];
							} while (p_choice->getflg() == 0);
							cx = p_choice->getx();
							cy = p_choice->gety();
							CharaDrawCalc();
							DxLib::PlaySoundMem(sound_data[9], DX_PLAYTYPE_BACK);
							break;
						}
					}
				}
			}
			switch (status_carsor)
			{
			/*#################################
			　ステータス画面　カーソル番号情報
				1.装備武器		4.装備魔法1
				2.装備アーマー	5.装備魔法2
				3.装備アクセ	6.装備魔法3
			###################################*/
			case 1:
				if (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					status_carsor = 2;
				}
				else if (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					status_carsor = 3;
				}
				else if (KeyFunc(KEY_INPUT_RIGHT, key_wait) || InputJoyPad(PAD_INPUT_RIGHT, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					status_carsor = 4;
				}
				else if (KeyFunc(KEY_INPUT_LEFT, key_wait) || InputJoyPad(PAD_INPUT_LEFT, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					status_carsor = 4;
				}
				break;
			case 2:
				if (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					status_carsor = 3;
				}
				else if (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					status_carsor = 1;
				}
				else if (KeyFunc(KEY_INPUT_RIGHT, key_wait) || InputJoyPad(PAD_INPUT_RIGHT, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					status_carsor = 5;
				}
				else if (KeyFunc(KEY_INPUT_LEFT, key_wait) || InputJoyPad(PAD_INPUT_LEFT, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					status_carsor = 5;
				}
				break;
			case 3:
				if (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					status_carsor = 1;
				}
				else if (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					status_carsor = 2;
				}
				else if (KeyFunc(KEY_INPUT_RIGHT, key_wait) || InputJoyPad(PAD_INPUT_RIGHT, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					status_carsor = 6;
				}
				else if (KeyFunc(KEY_INPUT_LEFT, key_wait) || InputJoyPad(PAD_INPUT_LEFT, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					status_carsor = 6;
				}
				break;
			case 4:
				if (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					status_carsor = 5;
				}
				else if (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					status_carsor = 6;
				}
				else if (KeyFunc(KEY_INPUT_RIGHT, key_wait) || InputJoyPad(PAD_INPUT_RIGHT, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					status_carsor = 1;
				}
				else if (KeyFunc(KEY_INPUT_LEFT, key_wait) || InputJoyPad(PAD_INPUT_LEFT, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					status_carsor = 1;
				}
				break;
			case 5:
				if (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					status_carsor = 6;
				}
				else if (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					status_carsor = 4;
				}
				else if (KeyFunc(KEY_INPUT_RIGHT, key_wait) || InputJoyPad(PAD_INPUT_RIGHT, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					status_carsor = 2;
				}
				else if (KeyFunc(KEY_INPUT_LEFT, key_wait) || InputJoyPad(PAD_INPUT_LEFT, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					status_carsor = 2;
				}
				break;
			case 6:
				if (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					status_carsor = 4;
				}
				else if (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					status_carsor = 5;
				}
				else if (KeyFunc(KEY_INPUT_RIGHT, key_wait) || InputJoyPad(PAD_INPUT_RIGHT, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					status_carsor = 3;
				}
				else if (KeyFunc(KEY_INPUT_LEFT, key_wait) || InputJoyPad(PAD_INPUT_LEFT, key_wait))
				{
					DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
					status_carsor = 3;
				}
				break;
			}
		}
		break;

	case STATUS_TO_EXPLORE1:
		if (use_flame_time <= 40)
		{
			SetDrawBright(bright_change, bright_change, bright_change);
			use_flame_time++;
		}
		if (use_flame_time > 40)
		{
			use_flame_time = 0;
			ToDo = STATUS_TO_EXPLORE2;
		}
		break;

	case STATUS_TO_EXPLORE2:
		if (use_flame_time <= 40)
		{
			SetDrawBright(bright_change, bright_change, bright_change);
			use_flame_time++;
		}
		if (use_flame_time > 40)
		{
			ToDo = EXPLORE;
		}
		break;

	case STATUS_TO_MENU1:
		if (use_flame_time <= 40)
		{
			DxLib::SetDrawBright(bright_change, bright_change, bright_change);
			use_flame_time++;
		}
		if (use_flame_time > 40)
		{
			use_flame_time = 0;
			ToDo = STATUS_TO_MENU2;
		}
		break;

	case STATUS_TO_MENU2:
		if (use_flame_time <= 40)
		{
			DxLib::SetDrawBright(bright_change, bright_change, bright_change);
			use_flame_time++;
		}
		if (use_flame_time > 40)
		{
			ToDo = MENU;
		}
		break;
/*################################################ EXPLOREで敵キャラ選択時 ######################################################*/
	case ENEMY_MOVE_ABLE:
		if (KeyFunc(KEY_INPUT_C, key_wait) || InputJoyPad(key.button_cross, key_wait))
		{
			DxLib::PlaySoundMem(sound_data[3], DX_PLAYTYPE_BACK);
			ToDo = EXPLORE;
		}
		if (KeyFunc(KEY_INPUT_SPACE, key_wait) || InputJoyPad(key.button_circle, key_wait))
		{
			AttackAbleZone();
			DxLib::PlaySoundMem(sound_data[1], DX_PLAYTYPE_BACK);
			ToDo = ENEMY_ATTACK_ABLE;
		}
		break;

	case ENEMY_ATTACK_ABLE:
		if (KeyFunc(KEY_INPUT_C, key_wait) || InputJoyPad(key.button_cross, key_wait))
		{
			DxLib::PlaySoundMem(sound_data[3], DX_PLAYTYPE_BACK);
			ToDo = EXPLORE;
		}
		if (KeyFunc(KEY_INPUT_SPACE, key_wait) || InputJoyPad(key.button_circle, key_wait))
		{
			status_current_character_team = p_choice->getflg();
			use_flame_time = 0;
			DxLib::PlaySoundMem(sound_data[6], DX_PLAYTYPE_BACK);
			ToDo = MENU_TO_STATUS;
		}
		break;
//####################################################################################
	case ENEMY_TURN_AI_THINK:
		int max_move_x;
		int max_move_y;
		int max_attack_x;
		int max_attack_y;
		int temp_evaluation1;
		int temp_evaluation2;
		int attackflag;
		attackflag = 0;
		character* max_attack_target;
		character* max_close_target;
		character* move_close_target;
		max_attack_x = -1;
		max_attack_y = -1;
		temp_evaluation1 = 0;
		temp_evaluation2 = 0;
		max_close_target = nullptr;
		move_close_target = nullptr;
		max_attack_target = nullptr;
		int hantei;
		hantei = 0;
		int max;
		max = 0;
		CharaDrawCalc();
		
		use_flame_time++;
		if (use_flame_time < 40)
		{
			break;
		}

		for (int i = 0; i < Enemy_Characternum; i++)
		{
			//未行動敵キャラ判別
			if (Battle_Chara[i].getflg() == 2 && Battle_Chara[i].getmoveflg() == 1)
			{
				p_choice = &Battle_Chara[i];
				hantei++;
				break;
			}
		}
		if (hantei == 0)
		{
			use_flame_time = 0;
			ToDo = TURN_CHANGE; 
			break;
		}
		max_move_x = p_choice->getx();
		max_move_y = p_choice->gety();

			//移動可能マスを探す
		p_choice->MoveAbleInit(moveable, moveable_inverse, MAPWIDTH, MAPHEIGHT);
		MoveAbleZone(p_choice->getx(), p_choice->gety(), p_choice->getMOV());
		//移動の範囲探索しながら攻撃目標も探索する
		for (int move_y = 0; move_y < MAPHEIGHT; move_y++)
		{
			for (int move_x = 0; move_x < MAPWIDTH; move_x++)
			{
				if (moveable[move_x + move_y * MAPWIDTH] >= 0 && ( MAP[move_x + move_y * MAPWIDTH] == 0 || ( move_x == p_choice->getx() && move_y == p_choice->gety() ) ))
				{
					for (int i = Enemy_Characternum; i < Characternum; i++)
					{
						if (Battle_Chara[i].getflg() != 0)
						{
							if (abs(Battle_Chara[i].getx() - move_x) + abs(Battle_Chara[i].gety() - move_y) < 4)
							{
								temp_evaluation1 = 5;
								move_close_target = &Battle_Chara[i];
							}
							else if (abs(Battle_Chara[i].getx() - move_x) + abs(Battle_Chara[i].gety() - move_y) < 3)
							{
								temp_evaluation1 = 10;
								move_close_target = &Battle_Chara[i];
							}
							else if (abs(Battle_Chara[i].getx() - move_x) + abs(Battle_Chara[i].gety() - move_y) < 2)
							{
								temp_evaluation1 = 15;
								move_close_target = &Battle_Chara[i];
							}
						}
					}
					for (int i = 0; i < MAPWIDTH*MAPHEIGHT; i++)
					{
						attack_able[i] = 0;
					}
					//攻撃の探索　ここから総当たり
					AttackZonePattern(move_x, move_y);
					for (int attack_y = 0; attack_y < MAPHEIGHT; attack_y++)
					{
						for (int attack_x = 0; attack_x < MAPWIDTH; attack_x++)
						{
							if (attack_able[attack_x + attack_y * MAPWIDTH] == 1 && MAP[attack_x + attack_y * MAPWIDTH] == 1)
							{
								attackflag = 0;
								for (int i = Enemy_Characternum; i < Characternum; i++)
								{
									if (Battle_Chara[i].getflg() != 0)
									{
										if (Battle_Chara[i].getx() == attack_x && Battle_Chara[i].gety() == attack_y)
										{
											p_attack_target = &Battle_Chara[i];
										}
									}
								}
								
								int direction = 1;
								if (move_x - p_attack_target->getx() > 0)
								{
									direction = 1;
								}
								else if (move_y - p_attack_target->gety() > 0)
								{
									direction = 2;
								}
								else if (move_x - p_attack_target->getx() < 0)
								{
									direction = 3;
								}
								else if (move_y - p_attack_target->gety() < 0)
								{
									direction = 4;
								}
								attackflag = 1;
								temp_evaluation2 = p_choice->AttackDamageCalculation(p_attack_target, direction) * 1000;	//評価式  
							}
							else
							{
								temp_evaluation2 = 0;
							}
							//評価値が最高ならデータを保持する
							if (max < temp_evaluation2 + temp_evaluation1)
							{
								max = temp_evaluation2 + temp_evaluation1;
								max_move_x = move_x;
								max_move_y = move_y;
								max_close_target = move_close_target;
								if (attackflag == 1)
								{
									max_attack_x = attack_x;
									max_attack_y = attack_y;
									max_attack_target = p_attack_target;
								}
							}
						}
					}
				}
			}
		}
		MAP[p_choice->getx() + p_choice->gety() * MAPWIDTH] = 0;
		ai_move_x = max_move_x;
		ai_move_y = max_move_y;
		ai_attack_x = max_attack_x;
		ai_attack_y = max_attack_y;
		p_attack_target = max_attack_target;
		MAP[max_move_x + max_move_y * MAPWIDTH] = 2;
		p_choice->MoveFlgInsert();
		if (ai_move_x != p_choice->getx() || ai_move_y != p_choice->gety())
		{
			use_flame_time = 0;
			ToDo = ENEMY_TURN_MOVE;
		}
		else if (ai_attack_x != -1 && ai_attack_y != -1)
		{
			use_flame_time = 0;
			ToDo = ENEMY_TURN_ATTACK;
		}
		break;
	case ENEMY_TURN_MOVE:
		if (use_flame_time == 0)
		{
			cx = ai_move_x;
			cy = ai_move_y;
			CharaDrawCalc();
			moven = 0;
		}
		if (use_flame_time == 60)
		{
			use_flame_time = 20;
			ToDo = ENEMY_TURN_MOVE_ANIMATION;
			p_choice->MoveRouteCalculation(cx, cy, moveable, moveable_inverse, MAPWIDTH, MAPHEIGHT);
			p_choice->DrawModeInsert(DRAW_MODE_MOVE);
			break;
		}
		use_flame_time++;
		break;
	case ENEMY_TURN_MOVE_ANIMATION:
		if (use_flame_time == 20)
		{
			moven++;
			if (p_choice->getx() == cx && p_choice->gety() == cy)
			{
				use_flame_time = 0;
				ToDo = ENEMY_TURN_MOVE_POST;
				break;
			}
			p_choice->MoveShow(moven, moveable_inverse, MAPHEIGHT, MAPWIDTH);
			use_flame_time = 0;
		}
		p_choice->MoveCharacterCordinateCalc(cx, cy, use_flame_time);
		use_flame_time++;
		break;
	case ENEMY_TURN_MOVE_POST:
		if (use_flame_time == 0)
		{
			cx = p_choice->getx();
			cy = p_choice->gety();
			CharaDrawCalc();
			if (p_choice->getdownflg() == 1)
			{
				p_choice->DrawModeInsert(DRAW_MODE_DOWN);
			}
			else
			{
				p_choice->DrawModeInsert(DRAW_MODE_NORMAL);
			}
		}
		if (use_flame_time < 15)
		{
			use_flame_time++;
		}
		else
		{
			use_flame_time = 0;
			if (ai_attack_x != -1 && ai_attack_y != -1)
			{
				ToDo = ENEMY_TURN_ATTACK;
				break;
			}
			else
			{
				ToDo = ENEMY_TURN_AI_THINK;
				break;
			}
		}
		CharaDrawCalc();
		break;
	case ENEMY_TURN_ATTACK:
		if (use_flame_time == 0)
		{
			cx = p_choice->getx();
			cy = p_choice->gety();
		}
		if (use_flame_time == 20)
		{
			standbytemp = p_choice->getstandbyflg();
			AttackAbleZone();
			p_choice->Attack(p_attack_target, attack_able, MAPWIDTH, MAPHEIGHT);
			damage = p_choice->AttackDamageCalculation(p_attack_target);
			cx = ai_attack_x;
			cy = ai_attack_y;
		}
		if (use_flame_time == 40)
		{
			cx = p_choice->getx();
			cy = p_choice->gety();
			CharaDrawCalc();
			use_flame_time = 0;
			p_choice->DrawModeInsert(DRAW_MODE_ATTACK);
			ToDo = ENEMY_TURN_ATTACK_ANIMATION;
			break;
		}
		CharaDrawCalc();
		use_flame_time++;
		break;
	case ENEMY_TURN_ATTACK_ANIMATION:
		if (use_flame_time < ATTACKANIMATIONTIME) {
			if (use_flame_time == 42)
			{
				DxLib::PlaySoundMem(sound_data[0], DX_PLAYTYPE_BACK);
				p_attack_target->DrawModeInsert(DRAW_MODE_DAMAGE);
			}
			if (use_flame_time == 80)
			{
				p_attack_target->DownCheck();
			}
			use_flame_time++;
		}
		else {
			if (p_choice->getdownflg() == 1)
			{
				p_choice->DrawModeInsert(DRAW_MODE_DOWN);
			}
			else
			{
				p_choice->DrawModeInsert(DRAW_MODE_NORMAL);
			}
			if (p_attack_target->getdownflg() == 1)
			{
				p_attack_target->DrawModeInsert(DRAW_MODE_DOWN);
			}
			else
			{
				p_attack_target->DrawModeInsert(DRAW_MODE_NORMAL);
			}
			use_flame_time = 0;
			ToDo = ENEMY_TURN_ATTACK_POST;
			break;
		}
		break;
	case ENEMY_TURN_ATTACK_POST:
		if (p_attack_target->getHP() <= 0)
		{
			MAP[p_attack_target->gety() * MAPWIDTH + p_attack_target->getx()] = 0;
			p_attack_target->removefunc(getEXP);
			cx = p_choice->getx();
			cy = p_choice->gety();
			CharaDrawCalc();
			p_attack_target = NULL;
			use_flame_time = 0;
			ToDo = ENEMY_TURN_AI_THINK;
			break;
		}
		if (p_attack_target->getstandbyflg() == 0 && p_attack_target->getflg() != p_choice->getflg())
		{
			ToDo = ENEMY_TURN_COUNTER;
		}
		else
		{
			p_choice->DrawModeInsert(DRAW_MODE_NORMAL);
			cx = p_choice->getx();
			cy = p_choice->gety();
			use_flame_time = 0;
			ToDo = ENEMY_TURN_AI_THINK;
			break;
		}
		break;
	case ENEMY_TURN_COUNTER:
		if (p_attack_target->getflg() && !(p_attack_target->getstandbyflg()))
		{
			CounterAbleZone();
			if (p_attack_target->AttackableFlgFunc(p_choice, counter_attack_able, MAPWIDTH, MAPHEIGHT))
			{
				p_attack_target->Counter(p_choice, counter_attack_able, MAPWIDTH, MAPHEIGHT);
				damage = p_attack_target->AttackDamageCalculation(p_choice);
				cx = p_attack_target->getx();
				cy = p_attack_target->gety();
				CharaDrawCalc();
				p_attack_target->DrawModeInsert(DRAW_MODE_ATTACK);
				use_flame_time = 0;
				ToDo = ENEMY_TURN_COUNTER_ANIMATION;
			}
			else
			{
				ToDo = ENEMY_TURN_COUNTER_POST;
			}
			break;
		}
		ToDo = ENEMY_TURN_COUNTER_POST;
		break;
	case ENEMY_TURN_COUNTER_ANIMATION:
		if (use_flame_time < ATTACKANIMATIONTIME) {
			if (use_flame_time == 42)
			{
				DxLib::PlaySoundMem(sound_data[0], DX_PLAYTYPE_BACK);
				p_choice->DrawModeInsert(DRAW_MODE_DAMAGE);
			}
			if (use_flame_time == 80)
			{
				p_choice->DownCheck();
			}
			use_flame_time++;
		}
		else
		{
			if (p_choice->getdownflg() == 1)
			{
				p_choice->DrawModeInsert(DRAW_MODE_DOWN);
			}
			else
			{
				p_choice->DrawModeInsert(DRAW_MODE_NORMAL);
			}
			if (p_attack_target->getdownflg() == 1)
			{
				p_attack_target->DrawModeInsert(DRAW_MODE_DOWN);
			}
			else
			{
				p_attack_target->DrawModeInsert(DRAW_MODE_NORMAL);
			}
			use_flame_time = 0;
			ToDo = ENEMY_TURN_COUNTER_POST;
		}
		break;
	case ENEMY_TURN_COUNTER_POST:
		if (p_choice->getHP() <= 0)
		{
			MAP[p_choice->gety() * MAPWIDTH + p_choice->getx()] = 0;
			p_choice->removefunc(getEXP);
		}
		if (standbytemp != p_choice->getstandbyflg())
		{
			p_choice->StandByChange();
		}
		if (use_flame_time < 15)
		{
			use_flame_time++;
		}
		else
		{
			cx = p_choice->getx();
			cy = p_choice->gety();
			CharaDrawCalc();
			p_attack_target = NULL;
			ToDo = ENEMY_TURN_AI_THINK;
		}
		break;
	case WIN:
		if (use_flame_time == 0)
		{
			//経験値精算
			for (int i = Enemy_Characternum; i < Characternum; i++)
			{
				for (int j = 0; j < player_data.getPlayableNum(); j++)
				{
					if (player_data.get_player_character(j)->getname() == Battle_Chara[i].getname()) 
					{
						player_data.get_player_character(j)->EXPCalc(getEXP);
					}
				}
			}
		}
		break;
	case LOSE:
		break;
	}
}

/*バトル描画部*/
void BATTLE_SCENE::Draw()
{
	switch (ToDo)
	{
	case BATTLE_CHARACTER_SELECT:
		DrawGraph(23, 8, glaphics[21], TRUE);
		DrawString(45, 20, "出撃させるキャラクターを選んでください", GetColor(0, 0, 0));
		DrawFormatStringToHandle(30, 260, GetColor(255, 255, 255), meirio, "現在%d人 / 最大%d人", choice, Player_Characternum);
		DrawString(380, 20, "Tキーで戦闘を開始します", GetColor(0, 0, 0));
		DrawGraph(42 + 80 * choice_carsol, 355, glaphics[3], TRUE);
		player_data.ret_player_character(choice_carsol).DrawStatusHead(glaphics);
		for (int i = 0; i < player_data.getPlayableNum(); i++)
		{
			player_data.ret_player_character(i).SelectTimeShow(i, 0);
			if (go_character[i])
			{
				DrawGraph(45 + 80 * i, 345, glaphics[20], TRUE);
			}
		}
		switch (flg[0])
		{
		case 0:
			break;
		case 1:
			DrawGraph(X - 80, Y - 20, glaphics[18], TRUE);
			switch (flg[1])
			{
			case 0:
				DrawGraph(X - 40, Y + 25, glaphics[5], TRUE);
				break;
			case 1:
				DrawGraph(X - 40, Y + 48, glaphics[5], TRUE);
				break;
			}
			break;
		case 2:
			break;
		}
		break;

	case SELECT_TO_EXPLORE:
		SetDrawBright(bright_change, bright_change, bright_change);
		fade(-10);
		DrawString(X, Y, "戦闘開始", GetColor(255, 255, 255));
		break;

	case TURN_CHANGE:
		fade(-10);
		SetDrawBright(bright_change, bright_change, bright_change);
		MapDraw();
		CharaDraw(Characternum, 255);
		DrawBox(X - 10, Y - 10, X + 130, Y + 30, GetColor(255, 255, 255), TRUE);
		if (player_num == 1) DrawString(X, Y, "2Pのターンです", GetColor(0, 0, 0));
		if (player_num == 2) DrawString(X, Y, "1Pのターンです", GetColor(0, 0, 0));
		break;

	case EXPLORE:
		SetDrawBright(bright_change, bright_change, bright_change);
		fade(-10);
		MapDraw();
		DrawGraph(X, Y, glaphics[3], TRUE);
		SetDrawBright(255, 255, 255);
		CharaDraw(Characternum, 255);
		DrawCarsolAnimation(bright_change);
		DrawGraph(menu_X, 20, glaphics[4], TRUE);
		if (flg[6] == 0 && flg[4] == 0)
		{
			DrawString(480, 460, "Hキーでヘルプを表示", GetColor(255, 255, 255));
		}
		if (p_choice != nullptr)
		{
			StatusShow();
		}
		if (flg[4] == 1)
		{
			DrawGraph(X - 70,Y - 30,glaphics[18],TRUE);
			DrawGraph(X - 25, (Y + 15) + 25 * flg[5], glaphics[5], TRUE);
		}
		if (flg[6] == 1)
		{
			DrawHelp();
		}
		break;

	case MENU:
		SetDrawBright(bright_change, bright_change, bright_change);
		fade(-15, 255);
		MapDraw();
		DrawGraph(X, Y, glaphics[3], TRUE);
		SetDrawBright(255, 255, 255);
		CharaDraw(Characternum, 255);
		DrawCarsolAnimation(bright_change);
		StatusShow();
		DrawGraph(menu_X, 20, glaphics[4], TRUE);
		if (p_choice->getmoveflg())
		{
			DrawGraph(menu_X, 20, glaphics[38], TRUE);
		}
		else
		{
			DrawGraph(menu_X, 20, glaphics[39], TRUE);
		}
		if (p_choice->getattackflg())
		{
			DrawGraph(menu_X, 20, glaphics[40], TRUE);
			if (!(p_choice->getMagicID(0) == 0 && p_choice->getMagicID(1) == 0 && p_choice->getMagicID(2) == 0))
			{
				DrawGraph(menu_X, 20, glaphics[42], TRUE);
			}
			else
			{
				DrawGraph(menu_X, 20, glaphics[43], TRUE);
			}
			DrawGraph(menu_X, 20, glaphics[44], TRUE);
		}
		else
		{
			DrawGraph(menu_X, 20, glaphics[41], TRUE);
			DrawGraph(menu_X, 20, glaphics[43], TRUE);
			DrawGraph(menu_X, 20, glaphics[45], TRUE);
		}
		DrawGraph(menu_X, 20, glaphics[46], TRUE);
		DrawGraph(menu_X, 20, glaphics[47], TRUE);
		if (flg[2] != 7)
		{
			DrawGraph(3, (flg[2] - 1) * 37 + 31, glaphics[5], TRUE);
		}
		else
		{
			//使用魔法を選択する画面
			DrawGraph(menu_X + 100, 90, glaphics[53], TRUE);
			DrawGraph(menu_X + 105, 98, glaphics[22], TRUE);
			switch (magic_data[p_choice->getMagicID(0)].getMagicCate())
			{
			case 1:
				DrawGraph(menu_X + 105, 98, glaphics[33], TRUE);
				break;
			case 2:
				DrawGraph(menu_X + 105, 98, glaphics[34], TRUE);
				break;
			case 3:
				DrawGraph(menu_X + 105, 98, glaphics[35], TRUE);
				break;
			case 4:
				DrawGraph(menu_X + 105, 98, glaphics[36], TRUE);
				break;
			}
			DrawGraph(menu_X + 105, 148, glaphics[22], TRUE);
			switch (magic_data[p_choice->getMagicID(1)].getMagicCate())
			{
			case 1:
				DrawGraph(menu_X + 105, 98, glaphics[33], TRUE);
				break;
			case 2:
				DrawGraph(menu_X + 105, 98, glaphics[34], TRUE);
				break;
			case 3:
				DrawGraph(menu_X + 105, 98, glaphics[35], TRUE);
				break;
			case 4:
				DrawGraph(menu_X + 105, 98, glaphics[36], TRUE);
				break;
			}
			DrawGraph(menu_X + 105, 198, glaphics[22], TRUE);
			switch (magic_data[p_choice->getMagicID(2)].getMagicCate())
			{
			case 1:
				DrawGraph(menu_X + 105, 98, glaphics[33], TRUE);
				break;
			case 2:
				DrawGraph(menu_X + 105, 98, glaphics[34], TRUE);
				break;
			case 3:
				DrawGraph(menu_X + 105, 98, glaphics[35], TRUE);
				break;
			case 4:
				DrawGraph(menu_X + 105, 98, glaphics[36], TRUE);
				break;
			}
			p_choice->DrawEquipMagicName(menu_X + 150, 100, 0);
			p_choice->DrawEquipMagicName(menu_X + 150, 150, 1);
			p_choice->DrawEquipMagicName(menu_X + 150, 200, 2);
			DrawGraph(menu_X + 70, (flg[3] - 1) * 50 + 100, glaphics[5], TRUE);
		}
		break;

	case MOVE:
		SetDrawBright(bright_change, bright_change, bright_change);
		fade(15, 255, 60);
		MapDraw();
		DrawMoveable();
		DrawGraph(X, Y, glaphics[3], TRUE);
		CharaDraw(Characternum, 255);
		DrawCarsolAnimation(255);
		StatusShow();
		if (p_attack_target != nullptr)
		{
			StatusShowSub();
		}
		break;

	case MOVE_ANIMATION:
		SetDrawBright(bright_change, bright_change, bright_change);
		MapDraw();
		CharaDraw(Characternum, 255);
		break;

	case MOVE_POST:
		SetDrawBright(bright_change, bright_change, bright_change);
		MapDraw();
		CharaDraw(Characternum, 255);
		fade(-15);
		break;

	case ATTACK:
		SetDrawBright(bright_change, bright_change, bright_change);
		fade(15, 255, 60);
		MapDraw();
		DrawAttackable();
		DrawGraph(X, Y, glaphics[3], TRUE);
		CharaDraw(Characternum, 255);
		DrawCarsolAnimation(255);
		StatusShow();
		if (p_attack_target != NULL && p_choice != p_attack_target)
		{
			StatusShowSub();
		}
		break;

	case ATTACK_ANIMATION:
		SetDrawBright(bright_change, bright_change, bright_change);
		MapDraw();
		CharaDraw(Characternum, bright_change);
		DrawDamageAnimation();
		break;

	case ATTACK_POST:
		SetDrawBright(bright_change, bright_change, bright_change);
		MapDraw();
		CharaDraw(Characternum, bright_change);
		break;
	case COUNTER:
		SetDrawBright(bright_change, bright_change, bright_change);
		MapDraw();
		CharaDraw(Characternum, bright_change);
		break;
	case COUNTER_ANIMATION:
		SetDrawBright(bright_change, bright_change, bright_change);
		MapDraw();
		CharaDraw(Characternum, bright_change);
		DrawDamageAnimation();
		break;
	case COUNTER_POST:
		SetDrawBright(bright_change, bright_change, bright_change);
		MapDraw();
		DrawGraph(X, Y, glaphics[3], TRUE);
		CharaDraw(Characternum, bright_change);
		DrawCarsolAnimation(bright_change);
		fade(-15);
		break;
	case MAGIC:
		SetDrawBright(bright_change, bright_change, bright_change);
		fade(15, 255, 60);
		MapDraw();
		DrawMagicAble();
		DrawGraph(X, Y, glaphics[3], TRUE);
		CharaDraw(Characternum, 255);
		DrawCarsolAnimation(255);
		StatusShow();
		if (p_attack_target != NULL && p_choice != p_attack_target)
		{
			StatusShowSub();
		}
		break;
	case MAGIC_LAUNCH:
		MapDraw();
		DrawMagicAble();
		DrawMagicLaunch();
		DrawGraph(X, Y, glaphics[3], TRUE);
		CharaDraw(Characternum, 255);
		DrawCarsolAnimation(255);
		StatusShow();
		if (p_attack_target != NULL && p_choice != p_attack_target)
		{
			StatusShowSub();
		}
		break;
	case MAGIC_ANIMATION:
		MapDraw();
		CharaDraw(Characternum, 255);
		SetDrawBright(255, 255, 255);
		if (use_flame_time >= 40 && use_flame_time <= 80)
		{
			current_magic.DrawMagic(X + camerax,Y + cameray - 45, use_flame_time % 6);
		}
		if (use_flame_time > 80 && use_flame_time <= 120)
		{
			if (ToDo == MAGIC_ANIMATION)
			{
				for (int i = 0; i < magic_target_num; i++)
				{
					DrawFormatString(
						(p_magic_target[i]->getx() - cx)*(BLOCKWIDTH / 2) + (p_magic_target[i]->gety() - cy)*(BLOCKWIDTH / 2) + X + 38,
						(p_magic_target[i]->gety() - cy)*(BLOCKHEIGHT / 2) - (p_magic_target[i]->getx() - cx)*(BLOCKHEIGHT / 2) + Y - 60,
						GetColor(255, 255, 255), "%d", p_choice->MagicDamageCalculation(p_magic_target[i], current_magic));
				}
			}
		}
		SetDrawBright(bright_change, bright_change, bright_change);

		break;
	case MAGIC_POST:
		SetDrawBright(bright_change, bright_change, bright_change);
		fade(-15);
		MapDraw();
		CharaDraw(Characternum, 255);
		break;
	case MENU_TO_STATUS:
		SetDrawBright(bright_change, bright_change, bright_change);
		fade(10);
		MapDraw();
		DrawGraph(X, Y, glaphics[3], TRUE);
		CharaDraw(Characternum, bright_change);
		DrawCarsolAnimation(bright_change);
		StatusShow(bright_change);
		break;

	case STATUS:
		SetDrawBright(bright_change, bright_change, bright_change);
		fade(-10);
		DrawGraph(23, 10, glaphics[21], TRUE);
		p_choice->DrawStatusHead(glaphics);
		p_choice->DrawStatusBottom(glaphics);
		DrawGraph(580, 50, glaphics[31], TRUE);
		DrawGraph(30, 50, glaphics[32], TRUE);
		switch (status_carsor)
		{
		case 1:
			DrawGraph(30, 300, glaphics[5], TRUE);
			p_choice->WeaponExplanation(35, 23);
			break;
		case 2:
			DrawGraph(30, 350, glaphics[5], TRUE);
			p_choice->ArmorExplanation(35, 23);
			break;
		case 3:
			DrawGraph(30, 400, glaphics[5], TRUE);
			p_choice->AccessoryExplanation(35, 23);
			break;
		case 4:
			DrawGraph(350, 300, glaphics[5], TRUE);
			p_choice->MagicExplanation(0, 35, 23);
			break;
		case 5:
			DrawGraph(350, 350, glaphics[5], TRUE);
			p_choice->MagicExplanation(1, 35, 23);
			break;
		case 6:
			DrawGraph(350, 400, glaphics[5], TRUE);
			p_choice->MagicExplanation(2, 35, 23);
			break;
		}
		break;

	case STATUS_TO_EXPLORE1:
		fade(10);
		p_choice->DrawStatusHead(glaphics);
		p_choice->DrawStatusBottom(glaphics);
		CharaDrawCalc();
		break;

	case STATUS_TO_EXPLORE2:
		fade(-10);
		MapDraw();
		DrawGraph(X, Y, glaphics[3], TRUE);
		CharaDraw(Characternum, bright_change);
		DrawCarsolAnimation(bright_change);
		if (p_choice != NULL)
		{
			StatusShow(bright_change);
		}
		break;

	case STATUS_TO_MENU1:
		fade(10);
		p_choice->DrawStatusHead(glaphics);
		p_choice->DrawStatusBottom(glaphics);
		break;

	case STATUS_TO_MENU2:
		fade(-10);
		MapDraw();
		DrawGraph(X, Y, glaphics[3], TRUE);
		CharaDraw(Characternum, bright_change);
		DrawCarsolAnimation(bright_change);
		StatusShow(bright_change);
		DrawGraph(20, 20, glaphics[4], TRUE);
		if (p_choice->getmoveflg())
		{
			DrawGraph(menu_X, 20, glaphics[38], TRUE);
		}
		else
		{
			DrawGraph(menu_X, 20, glaphics[39], TRUE);
		}
		if (p_choice->getattackflg())
		{

			DrawGraph(menu_X, 20, glaphics[40], TRUE);
			DrawGraph(menu_X, 20, glaphics[42], TRUE);
			DrawGraph(menu_X, 20, glaphics[44], TRUE);
		}
		else
		{
			DrawGraph(menu_X, 20, glaphics[41], TRUE);
			DrawGraph(menu_X, 20, glaphics[43], TRUE);
			DrawGraph(menu_X, 20, glaphics[45], TRUE);
		}
		DrawGraph(menu_X, 20, glaphics[46], TRUE);
		DrawGraph(menu_X, 20, glaphics[47], TRUE);
		DrawGraph(3, (flg[2] - 1) * 37 + 31, glaphics[5], TRUE);
		break;

	case ENEMY_MOVE_ABLE:
		fade(15, 255, 60);
		MapDraw();
		DrawMoveable();
		CharaDraw(Characternum, 255);
		StatusShow();
		break;

	case ENEMY_ATTACK_ABLE:
		fade(15, 255, 60);
		MapDraw();
		DrawAttackable();
		CharaDraw(Characternum, 255);
		StatusShow();
		break;
	case ENEMY_TURN_AI_THINK:
		MapDraw();
		CharaDraw(Characternum, 255);
		break;
	case ENEMY_TURN_MOVE:
		MapDraw();

		if (use_flame_time > 20)
		{
			fade(15, 255, 60);
			DrawMoveable();
		}
		CharaDraw(Characternum, 255);
		StatusShow();
		break;
	case ENEMY_TURN_MOVE_ANIMATION:
		MapDraw();
		CharaDraw(Characternum, 255);
		break;
	case ENEMY_TURN_MOVE_POST:
		SetDrawBright(bright_change, bright_change, bright_change);
		MapDraw();
		CharaDraw(Characternum, 255);
		fade(-15);
		break;
	case ENEMY_TURN_ATTACK:
		fade(15, 255, 60);
		MapDraw();
		if (use_flame_time >= 20)
		{
			DrawAttackable();
		}
		CharaDraw(Characternum, 255);
		StatusShow();
		break;
	case ENEMY_TURN_ATTACK_ANIMATION:
		DxLib::SetDrawBright(bright_change, bright_change, bright_change);
		MapDraw();
		CharaDraw(Characternum, bright_change);
		DrawDamageAnimation();
		break;
	case ENEMY_TURN_ATTACK_POST:
		SetDrawBright(bright_change, bright_change, bright_change);
		MapDraw();
		CharaDraw(Characternum, bright_change);
		break;
	case ENEMY_TURN_COUNTER:
		SetDrawBright(bright_change, bright_change, bright_change);
		MapDraw();
		CharaDraw(Characternum, bright_change);
		break;
	case ENEMY_TURN_COUNTER_ANIMATION:
		SetDrawBright(bright_change, bright_change, bright_change);
		MapDraw();
		CharaDraw(Characternum, bright_change);
		DrawDamageAnimation();
		break;
	case ENEMY_TURN_COUNTER_POST:
		SetDrawBright(bright_change, bright_change, bright_change);
		MapDraw();
		DrawGraph(X, Y, glaphics[3], TRUE);
		CharaDraw(Characternum, bright_change);
		DrawCarsolAnimation(bright_change);
		fade(-15);
		break;
	case WIN:
		MapDraw();
		CharaDraw(Characternum, bright_change);
		DrawGraph(0, 200, glaphics[54], TRUE);
		DrawString(265, 300, "タイトルに戻ります", GetColor(0, 0, 0));
		use_flame_time++;
		if (use_flame_time <= 120)
		{
			fade(-10);
		}
		else
		{
			fade(5);
			StopSoundMem(BGM);
		}
		break;
	case LOSE:
		MapDraw();
		CharaDraw(Characternum, 255);
		DrawString(X, Y, "負けました", GetColor(255, 255, 255));
		use_flame_time++;
		if (use_flame_time <= 120)
		{
			fade(-10);
		}
		else
		{
			fade(10);
			StopSoundMem(BGM);
		}
		break;
	default:
		break;
	}
}
//データをロードするためのデータを読み取る
void BATTLE_SCENE::LoadDataExtract(string data, string& mapdata, string& enemydata, string& mapchipdata)
{
	ifstream ifs(data);
	string buf;

	ifs >> buf;
	mapdata = buf;
	ifs >> buf;
	enemydata = buf;
	ifs >> buf;
	mapchipdata = buf;
	ifs >> buf;
	BGM = LoadSoundMem(buf.c_str());
}
//マップチップデータ読み込み
void BATTLE_SCENE::LoadMapChipData(string mapchipdata)
{
	string buf;
	ifstream ifs(mapchipdata);
	try
	{
		if (!ifs)throw 1;
	}
	catch (int)
	{
		MessageBox(nullptr, TEXT(""), TEXT("ERROR"), MB_OK);
		return;
	}
	int i = 0;
	while (1)
	{
		ifs >> buf;
		mapchip_glaphics[i] = LoadGraph(buf.c_str(), TRUE);
		if (ifs.eof()) break;
		i++;
	}
}
void BATTLE_SCENE::LoadMapData(string mapdata)
{
	char buf;
	ifstream ifs(mapdata);
	if (!ifs)return;
	for (int i = 0; i < MAPWIDTH; i++)
	{
		for (int j = 0; j < MAPHEIGHT; j++)
		{
			ifs >> buf;
			if (buf == '1')
			{
				MAP[j + i * MAPWIDTH] = 0;
				MAP_CHIP_GRAPHIC_ARRAY[j + i * MAPWIDTH] = 1;
			}
			else if (buf == '2')
			{
				MAP[j + i * MAPWIDTH] = -10;
				MAP_CHIP_GRAPHIC_ARRAY[j + i * MAPWIDTH] = 2;
			}
			else if (buf == '3')
			{
				MAP[j + i * MAPWIDTH] = -10;
				MAP_CHIP_GRAPHIC_ARRAY[j + i * MAPWIDTH] = 3;
			}
			else if (buf == '4')
			{
				MAP[j + i * MAPWIDTH] = 0;
				MAP_CHIP_GRAPHIC_ARRAY[j + i * MAPWIDTH] = 4;
			}
			else if (buf == '5')
			{
				MAP[j + i * MAPWIDTH] = 0;
				MAP_CHIP_GRAPHIC_ARRAY[j + i * MAPWIDTH] = 5;
			}
			else if (buf == '6')
			{
				MAP[j + i * MAPWIDTH] = 0;
				MAP_CHIP_GRAPHIC_ARRAY[j + i * MAPWIDTH] = 6;
			}
			else if (buf == '7')
			{
				MAP[j + i * MAPWIDTH] = 0;
				MAP_CHIP_GRAPHIC_ARRAY[j + i * MAPWIDTH] = 7;
			}
			else if (buf == '8')
			{
				MAP[j + i * MAPWIDTH] = -10;
				MAP_CHIP_GRAPHIC_ARRAY[j + i * MAPWIDTH] = 8;
			}
			else if (buf == 'F')
			{
				MAP[j + i * MAPWIDTH] = 3;
				MAP_CHIP_GRAPHIC_ARRAY[j + i * MAPWIDTH] = 1;
			}
		}
	}

}
void BATTLE_SCENE::LoadEnemyData(string enemy_data)
{
	ifstream ifs(enemy_data);
	if (!ifs) exit(1);

	int a;
	int b;
	int LV;
	int AT;
	int DF;
	int MAT;
	int MDF;
	int DIR;
	int MOVENUM;
	int HP;
	int MP;
	int weaponID;
	int armorID;
	int accessoryID;
	ATTACKTYPE ATTACK = TATE;
	string glaphic_data_place;
	string glaphic_face_place;
	string name;
	int EXP;
	string buf;
	for (int i = 0; i < Enemy_Characternum; i++)
	{
		ifs >> buf;
		a = stoi(buf);
		ifs >> buf;
		b = stoi(buf);
		ifs >> buf;
		LV = stoi(buf);
		ifs >> buf;
		AT = stoi(buf);
		ifs >> buf;
		DF = stoi(buf);
		ifs >> buf;
		MAT = stoi(buf);
		ifs >> buf;
		MDF = stoi(buf);
		ifs >> buf;
		DIR = stoi(buf);
		ifs >> buf;
		MOVENUM = stoi(buf);
		ifs >> buf;
		HP = stoi(buf);
		ifs >> buf;
		MP = stoi(buf);
		ifs >> buf;
		if (1 == stoi(buf))
		{
			ATTACK = TATE;
		}
		else if (2 == stoi(buf)) {
			ATTACK = YOKO;
		}
		else if (3 == stoi(buf)) {
			ATTACK = YUMI;
		}
		ifs >> glaphic_data_place;
		ifs >> name;
		ifs >> buf;
		weaponID = stoi(buf);
		ifs >> buf;
		armorID = stoi(buf);
		ifs >> buf;
		accessoryID = stoi(buf);
		ifs >> buf;
		EXP = stoi(buf);
		Battle_Chara[i].Init(a, b, LV, AT, DF, MAT, MDF, DIR,
			MOVENUM, HP, MP, ATTACK, glaphic_data_place.c_str(), glaphic_face_place.c_str(), name.c_str());
		Battle_Chara[i].enemyEXPInsert(EXP);
		Battle_Chara[i].WeaponEquip(weaponID);
		Battle_Chara[i].ArmorEquip(armorID);
		Battle_Chara[i].AccessoryEquip(accessoryID);
		Battle_Chara[i].flgInsert(2);
	}
}
//どのバトルでも使う基本データをロード
void BATTLE_SCENE::LoadBattleMainData() {
	ifstream ifs1("DATA\\battle_image_source.txt");
	try
	{
		if (!ifs1) throw 1;
	}
	catch(int)
	{
		MessageBox(nullptr, TEXT("Battle Main Data Can't Be Opened."), TEXT("ERROR"), MB_OK);
	}
	string str = "";
	int i = 0;
	while (1) {
		ifs1 >> str;
		glaphics[i] = LoadGraph(str.data(), TRUE);
		if (ifs1.eof()) break;
		i++;
	}
	ifstream ifs3("DATA\\animation_source.txt");
	str = "";
	i = 0;
	while (1)
	{
		int num = 0;
		int gr_width = 0;
		int gr_height = 0;
		ifs3 >> str;
		num = stoi(str);
		ifs3 >> str;
		gr_width = stoi(str);
		ifs3 >> str;
		gr_height = stoi(str);
		ifs3 >> str;
		LoadDivGraph(str.c_str(), num, num, 1, gr_width, gr_height, animation_glaphics[i]);
		if (ifs3.eof()) break;
		i++;
	}
}
//キャラ描画位置計算
void BATTLE_SCENE::CharaDrawCalc()
{
	for (int i = 0; i < Characternum; i++)
	{
		Battle_Chara[i].drawx_yInsert(cx, cy);
	}
}
//キャラクター描画関数
void BATTLE_SCENE::CharaDraw(int characternum,int start_bright)
{
	static int normal_time =  0;
	static int attack_time =  0;
	static int move_time   =  0;
	static int damage_time =  0;
	static int magic_time  =  0;
	static int magic_flame =  0;
	static int normal_roop = 15;
	static int move_roop   =  5;
	int* array = new int[characternum];

	//キャラクターを描く順番を決める
	//表示座標drawyが小さい順に描く
	for (int i = 0; i < characternum; i++)
	{
		array[i] = -1;
	}
	for (int i = 0; i < characternum; i++)
	{
		for (int j = 0; j < characternum; j++)
		{
			if (array[j] != -1)
			{
				if (Battle_Chara[i].getdrawy() < Battle_Chara[array[j]].getdrawy())
				{
					for (int k = characternum - 1; k > j; k--)
					{
						array[k] = array[k-1];
					}
					array[j] = i;
					break;
				}
			}
			else 
			{
				array[j] = i;
				break;
			}
		}
	}
	//キャラクター表示本処理
	for (int i = 0; i < characternum; i++)
	{
		SetDrawBright(start_bright,start_bright,start_bright);
		if (ToDo == ATTACK_ANIMATION || ToDo == ATTACK_POST || ToDo == COUNTER || ToDo == COUNTER_ANIMATION || ToDo == COUNTER_POST
			|| ToDo == ENEMY_TURN_ATTACK_ANIMATION || ToDo == ENEMY_TURN_ATTACK_POST || ToDo == ENEMY_TURN_COUNTER || ToDo == ENEMY_TURN_COUNTER_ANIMATION || ToDo == ENEMY_TURN_COUNTER_POST)
		{
			if (p_choice == &Battle_Chara[array[i]] || p_attack_target == &Battle_Chara[array[i]])
			{
				SetDrawBright(255,255,255);
			}
		}
		if (Battle_Chara[array[i]].getflg())
		{
			DrawGraph(Battle_Chara[array[i]].getdrawx(), Battle_Chara[array[i]].getdrawy() + 47, glaphics[52], TRUE);	//キャラクターの影の描画	
			if		(Battle_Chara[array[i]].getDrawMode() == DRAW_MODE_NORMAL)
			{
				Battle_Chara[array[i]].BattleCharaShow((int)(normal_time / normal_roop));
			}
			else if (Battle_Chara[array[i]].getDrawMode() == DRAW_MODE_MOVE)
			{
				Battle_Chara[array[i]].BattleCharaShow((int)(move_time / move_roop));
				move_time++;
			}
			else if (Battle_Chara[array[i]].getDrawMode() == DRAW_MODE_ATTACK)
			{
				if (attack_time <= 40)
				{
					Battle_Chara[array[i]].BattleCharaShow(0);
				}
				else if (attack_time > 40 && attack_time <= 42)
				{
					Battle_Chara[array[i]].BattleCharaShow(1);
				}
				else if (attack_time > 42 && attack_time <= 44)
				{
					Battle_Chara[array[i]].BattleCharaShow(2);
				}
				else if (attack_time > 44)
				{
					Battle_Chara[array[i]].BattleCharaShow(3);
				}
				attack_time++;
			}
			else if (Battle_Chara[array[i]].getDrawMode() == DRAW_MODE_DAMAGE)
			{
				Battle_Chara[array[i]].BattleCharaShow(0);
			}
			else if (Battle_Chara[array[i]].getDrawMode() == DRAW_MODE_DOWN)
			{
				Battle_Chara[array[i]].BattleCharaShow(normal_time / normal_roop);
			}
			else if (Battle_Chara[array[i]].getDrawMode() == DRAW_MODE_MAGIC)
			{
				if (magic_time % 5 == 0)
				{
					if (magic_flame == 0)
					{
						magic_flame = 1;
					}
					else
					{
						magic_flame = 0;
					}
				}
				Battle_Chara[array[i]].BattleCharaShow(magic_flame);
			}
		}
		SetDrawBright(bright_change, bright_change, bright_change);
	}
	normal_time++;
	magic_time++;
	if (normal_time == normal_roop * 4)
	{
		normal_time = 0;
	}
	if (move_time == move_roop * 4)
	{
		move_time = 0;
	}
	if (attack_time == ATTACKANIMATIONTIME + 1)
	{
		attack_time = 0;
	}
	if (damage_time == ATTACKANIMATIONTIME + 1)
	{
		attack_time = 0;
	}
	if (magic_time == 1000)
	{
		magic_time = 0;
	}
}

bool BATTLE_SCENE::InputController(int KEY)
{
	switch (KEY)
	{
	case KEY_INPUT_SPACE:

		break;
	case KEY_INPUT_RIGHT:
		break;
	case KEY_INPUT_LEFT:
		break;
	case KEY_INPUT_UP:
		break;
	case KEY_INPUT_DOWN:
		break;
	case KEY_INPUT_C:
		break;
	case KEY_INPUT_T:
		break;
	case KEY_INPUT_X:
		break;
	case KEY_INPUT_Q:
		break;
	case KEY_INPUT_R:
		break;
	}
	return false;
}
//バトル中のカーソル移動処理関数
void BATTLE_SCENE::CarsorMove() {
	if (KeyFunc(KEY_INPUT_RIGHT, key_wait) || InputJoyPad(PAD_INPUT_RIGHT, key_wait)) {
		if (cy < MAPHEIGHT - 1)
		{
			DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
			cy++;
		}
	}
	if (KeyFunc(KEY_INPUT_DOWN, key_wait) || InputJoyPad(PAD_INPUT_DOWN, key_wait)) {
		if (cx > 0)
		{
			DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
			cx--;
		}
	}
	if (KeyFunc(KEY_INPUT_LEFT, key_wait) || InputJoyPad(PAD_INPUT_LEFT, key_wait)) {
		if (cy > 0)
		{
			DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
			cy--;
		}
	}
	if (KeyFunc(KEY_INPUT_UP, key_wait) || InputJoyPad(PAD_INPUT_UP, key_wait)) {
		if (cx < MAPWIDTH - 1)
		{
			DxLib::PlaySoundMem(sound_data[5], DX_PLAYTYPE_BACK);
			cx++;
		}
	}
}
//画面フェード（fade_indexが正ならアウト、負ならインになる）
void BATTLE_SCENE::fade(int fade_index, int upper_limit, int lower_limit)
{
	if (fade_index > 0)
	{
		if (bright_change > lower_limit)
		{
			bright_change -= fade_index;
		}
		if(bright_change <= lower_limit)
		{
			bright_change = lower_limit;
		}
	}
	else
	{
		if (bright_change < upper_limit)
		{
			bright_change -= fade_index;
		}
		if(bright_change >= upper_limit)
		{
			bright_change = upper_limit;
		}
	}
	SetDrawBright(bright_change, bright_change, bright_change);
}
//マップ表示処理関数
void BATTLE_SCENE::MapDraw()
{
	SetDrawBright(bright_change, bright_change, bright_change);
	for (int j = 0; j < MAPHEIGHT; j++)
	{
		for (int i = 0; i < MAPWIDTH; i++)
		{
			DrawGraph((i - cx)*(BLOCKWIDTH / 2) + (j - cy)*(BLOCKWIDTH / 2) + X + camerax,
					(j - cy)*(BLOCKHEIGHT / 2) - (i - cx)*(BLOCKHEIGHT / 2) + Y + cameray, mapchip_glaphics[MAP_CHIP_GRAPHIC_ARRAY[i + j * MAPWIDTH] - 1], TRUE);
		}
	}
}
//剣カーソルのアニメーション処理関数
void BATTLE_SCENE::DrawCarsolAnimation(int start_bright)
{
	static int time = 0;
	static int roop = 9;

	SetDrawBright(start_bright, start_bright, start_bright);
	DrawGraph(X + 20, Y - 115, animation_glaphics[1][(int) (time / roop)], TRUE);
	time++;
	if (time == roop * 4)
	{
		time = 0;
	}
	SetDrawBright(bright_change, bright_change, bright_change);
}
//ダメージ時の表示処理
void BATTLE_SCENE::DrawDamageAnimation()
{
	static int time = 0;
	static int roop = 1;

	if (time == ATTACKANIMATIONTIME + 1)
	{
		time = 0;
	}
	SetDrawBright(255, 255, 255);
	if (ToDo == ATTACK_ANIMATION || ToDo == ENEMY_TURN_ATTACK_ANIMATION)
	{
		if (time >= 42 && time <= 47)
		{
			DrawGraph(
				(p_attack_target->getx() - cx)*(BLOCKWIDTH / 2) + (p_attack_target->gety() - cy)*(BLOCKWIDTH / 2) + X + 5,
				(p_attack_target->gety() - cy)*(BLOCKHEIGHT / 2) - (p_attack_target->getx() - cx)*(BLOCKHEIGHT / 2) + Y - 25,
				animation_glaphics[0][(time - 42) / roop], TRUE);
		}
		if (time >= 42)
		{
			DrawFormatString(
				(p_attack_target->getx() - cx)*(BLOCKWIDTH / 2) + (p_attack_target->gety() - cy)*(BLOCKWIDTH / 2) + X + 38,
				(p_attack_target->gety() - cy)*(BLOCKHEIGHT / 2) - (p_attack_target->getx() - cx)*(BLOCKHEIGHT / 2) + Y - 60,
				GetColor(255, 255, 255), "%d", damage);
		}
	}
	else if (ToDo == COUNTER_ANIMATION || ToDo == ENEMY_TURN_COUNTER_ANIMATION)
	{
		if (time >= 42 && time <= 47)
		{
			DrawGraph(
				(p_choice->getx() - cx)*(BLOCKWIDTH / 2) + (p_choice->gety() - cy)*(BLOCKWIDTH / 2) + X + 5,
				(p_choice->gety() - cy)*(BLOCKHEIGHT / 2) - (p_choice->getx() - cx)*(BLOCKHEIGHT / 2) + Y - 25,
				animation_glaphics[0][(time - 42) / roop], TRUE);
		}
		if (time >= 42)
		{
			DrawFormatString(
				(p_choice->getx() - cx)*(BLOCKWIDTH / 2) + (p_choice->gety() - cy)*(BLOCKWIDTH / 2) + X + 38,
				(p_choice->gety() - cy)*(BLOCKHEIGHT / 2) - (p_choice->getx() - cx)*(BLOCKHEIGHT / 2) + Y - 60,
				GetColor(255, 255, 255), "%d", damage);
		}
	}
	time++;
	SetDrawBright(bright_change,bright_change,bright_change);
}
//移動可能箇所の表示処理
void BATTLE_SCENE::DrawMoveable() 
{
	static float p = 0.0;
	int bright = (int)abs(50 * (sin(p) + 1)) + 155;
	p += 0.05f;
	SetDrawBright(bright, bright, bright);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220);
	for (int i = 0; i < MAPHEIGHT; i++) {
		for (int j = 0; j < MAPWIDTH; j++) {
			if (moveable[i * MAPWIDTH + j] != -1 && !(j == p_choice->getx() && i == p_choice->gety() && !MAP[i * MAPWIDTH + j]))
			{
				DrawGraph((j - cx)*(BLOCKWIDTH / 2) + (i - cy)*(BLOCKWIDTH / 2) + X,
					(i - cy)*(BLOCKHEIGHT / 2) - (j - cx)*(BLOCKHEIGHT / 2) + Y, glaphics[6], TRUE);
			}
		}
	}
	if (p > 100.0)
	{
		p = 0.0;
	}
	SetDrawBright(bright_change,bright_change,bright_change);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
//攻撃可能箇所の表示処理
void BATTLE_SCENE::DrawAttackable()
{
	static float p = 0.0;
	int bright = (int)abs(50*(sin(p)+1)) + 155;
	p += 0.05f;
	SetDrawBright(bright, bright, bright);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 200);
	for (int i = 0; i < MAPHEIGHT; i++) {
		for (int j = 0; j < MAPWIDTH; j++) {
			if (attack_able[i * MAPWIDTH + j] != 0 && !(j == p_choice->getx() && i == p_choice->gety() && !MAP[i * MAPWIDTH + j]))
			{
				DrawGraph((j - cx)*(BLOCKWIDTH / 2) + (i - cy)*(BLOCKWIDTH / 2) + X,
					(i - cy)*(BLOCKHEIGHT / 2) - (j - cx)*(BLOCKHEIGHT / 2) + Y, glaphics[7], TRUE);
			}
		}
	}
	if (p > 100.0)
	{
		p = 0.0;
	}
	SetDrawBright(bright_change, bright_change, bright_change);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
//魔法攻撃可能箇所の表示処理
void BATTLE_SCENE::DrawMagicAble()
{
	static float p = 0.0;
	int bright = (int)abs(50 * (sin(p) + 1)) + 155;
	p += 0.05f;
	SetDrawBright(bright, bright, bright);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220);
	for (int i = 0; i < MAPHEIGHT; i++) {
		for (int j = 0; j < MAPWIDTH; j++) {
			if (magic_able[i * MAPWIDTH + j] == 1)
			{
				DrawGraph((j - cx)*(BLOCKWIDTH / 2) + (i - cy)*(BLOCKWIDTH / 2) + X,
					(i - cy)*(BLOCKHEIGHT / 2) - (j - cx)*(BLOCKHEIGHT / 2) + Y, glaphics[50], TRUE);
			}
		}
	}
	if (p > 100.0)
	{
		p = 0.0;
	}
	SetDrawBright(bright_change, bright_change, bright_change);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
//魔法攻撃判定箇所の表示処理
void BATTLE_SCENE::DrawMagicLaunch()
{
	static float p = 0.0;
	int bright = (int)abs(50 * (sin(p) + 1)) + 155;
	p += 0.05f;
	SetDrawBright(bright, bright, bright);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220);
	for (int i = 0; i < MAPHEIGHT; i++) {
		for (int j = 0; j < MAPWIDTH; j++) {
			if (magic_launch[i * MAPWIDTH + j])
			{
			DrawGraph((j - cx)*(BLOCKWIDTH / 2) + (i - cy)*(BLOCKWIDTH / 2) + X,
				(i - cy)*(BLOCKHEIGHT / 2) - (j - cx)*(BLOCKHEIGHT / 2) + Y, glaphics[7], TRUE);
			}
		}
	}
	if (p > 100.0)
	{
		p = 0.0;
	}
	SetDrawBright(bright_change, bright_change, bright_change);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
//画面左下ステータス表示
void BATTLE_SCENE::StatusShow(int drawbright)
{
	static int stx = 30;	//ステータス表示位置
	static int sty = 320;
	int White = GetColor(255, 255, 255);
	SetDrawBright(drawbright, drawbright, drawbright);

	DrawGraph(stx, sty, glaphics[12], TRUE);
	DrawFormatStringToHandle(stx + 5, sty + 5, White, meirio, "%s", p_choice->getname().c_str());
	DrawFormatStringToHandle(stx + 5, sty + 40, White, gothic, "Lv %d", p_choice->getLV());
	if (p_choice->getstandbyflg() == 0)
	{
		DrawGraph(stx + 140, sty + 20, glaphics[8], TRUE);
	}
	else
	{
		DrawGraph(stx + 140, sty + 20, glaphics[9], TRUE);
	}
	switch (p_choice->getattacktype())
	{
	case TATE:
		DrawGraph(stx + 160, sty + 30, glaphics[15], TRUE);
		break;
	case YOKO:
		DrawGraph(stx + 160, sty + 30, glaphics[16], TRUE);
		break;
	case YUMI:
		DrawGraph(stx + 160, sty + 30, glaphics[17], TRUE);
		break;
	case TUKI:
		break;
	}
	DrawStringToHandle(stx + 5, sty + 65, "HP", White, gothic);
	DrawFormatStringToHandle(stx + 170, sty + 65, White, gothic, "%3d/%3d", p_choice->getHP(), p_choice->getMAXHP());
	DrawStringToHandle(stx + 5, sty + 90, "MP", White, gothic);
	DrawFormatStringToHandle(stx + 170, sty + 90, White, gothic, "%3d/%3d", p_choice->getMP(), p_choice->getMAXMP());
	DrawGraph(stx + 40, sty + 67, glaphics[11], TRUE);
	DrawRectGraph(stx + 40, sty + 67, 0, 0, 110 * p_choice->getHP() / p_choice->getMAXHP(), 15, glaphics[10], TRUE);
	DrawGraph(stx + 40, sty + 92, glaphics[49], TRUE);
	DrawRectGraph(stx + 40, sty + 92, 0, 0, 110 * p_choice->getMP() / p_choice->getMAXMP(), 15, glaphics[48], TRUE);

	SetDrawBright(bright_change, bright_change, bright_change);
}
//画面右下ステータス表示
void BATTLE_SCENE::StatusShowSub(int drawbright)
{
	static int stx = 360;
	static int sty = 320;
	int White = GetColor(255, 255, 255);
	SetDrawBright(drawbright, drawbright, drawbright);

	DrawGraph(stx, sty, glaphics[12], TRUE);
	DrawGraph(stx, sty, glaphics[12], TRUE);
	DrawFormatStringToHandle(stx + 5, sty + 5, White, meirio, "%s", p_attack_target->getname().c_str());
	DrawFormatStringToHandle(stx + 5, sty + 40, White, gothic, "Lv %d", p_attack_target->getLV());
	if (p_attack_target->getstandbyflg() == 0)
	{
		DrawGraph(stx + 140, sty + 20, glaphics[8], TRUE);
	}
	else
	{
		DrawGraph(stx + 140, sty + 20, glaphics[9], TRUE);
	}
	switch (p_attack_target->getattacktype())
	{
	case TATE:
		DrawGraph(stx + 160, sty + 30, glaphics[15], TRUE);
		break;
	case YOKO:
		DrawGraph(stx + 160, sty + 30, glaphics[16], TRUE);
		break;
	case YUMI:
		DrawGraph(stx + 160, sty + 30, glaphics[17], TRUE);
		break;
	case TUKI:
		break;
	}
	DrawStringToHandle(stx + 5, sty + 65, "HP", White, gothic);
	DrawFormatStringToHandle(stx + 170, sty + 65, White, gothic, "%3d/%3d", p_attack_target->getHP(), p_attack_target->getMAXHP());
	DrawStringToHandle(stx + 5, sty + 90, "MP", White, gothic);
	DrawFormatStringToHandle(stx + 170, sty + 90, White, gothic, "%3d/%3d", p_attack_target->getMP(), p_attack_target->getMAXMP());
	DrawGraph(stx + 40, sty + 67, glaphics[11], TRUE);
	DrawRectGraph(stx + 40, sty + 67, 0, 0, 110 * p_attack_target->getHP() / p_attack_target->getMAXHP(), 15, glaphics[10], TRUE);
	DrawGraph(stx + 40, sty + 92, glaphics[49], TRUE);
	DrawRectGraph(stx + 40, sty + 92, 0, 0, 110 * p_attack_target->getMP() / p_attack_target->getMAXMP(), 15, glaphics[48], TRUE);
	SetDrawBright(bright_change, bright_change, bright_change);
}
//攻撃範囲計算
void BATTLE_SCENE::AttackAbleZone()
{
	//初期化
	for (int i = 0; i < MAPHEIGHT; i++)
	{
		for (int j = 0; j < MAPWIDTH; j++)
		{
			attack_able[i * MAPWIDTH + j] = 0;
		}
	}
	//パターン定義から持ってくる
	AttackZonePattern(p_choice, attack_able);
}
//反撃範囲計算
void BATTLE_SCENE::CounterAbleZone()
{
	//初期化
	for (int i = 0; i < MAPHEIGHT; i++)
	{
		for (int j = 0; j < MAPWIDTH; j++)
		{
			counter_attack_able[i * MAPWIDTH + j] = 0;
		}
	}
	//パターン定義から持ってくる
	AttackZonePattern(p_attack_target, counter_attack_able);
}
//攻撃パターン定義
void BATTLE_SCENE::AttackZonePattern(character* attack_character, int* able_array)
{
	int x = attack_character->getx();
	int y = attack_character->gety();
	switch (attack_character->getattacktype())
	{
	case TATE:
		if (x + 1 < MAPWIDTH && MAP[y * MAPWIDTH + (x + 1)] >= 0 )
		{
			able_array[y * MAPWIDTH + (x + 1)] = 1;
		}
		if (y + 1 < MAPHEIGHT && MAP[(y + 1) * MAPWIDTH + x] >= 0)
		{
			able_array[(y + 1) * MAPWIDTH + x] = 1;
		}
		if (x - 1 >= 0 && MAP[y * MAPWIDTH + (x - 1)] >= 0)
		{
			able_array[y * MAPWIDTH + (x - 1)] = 1;
		}
		if (y - 1 >= 0 && MAP[(y - 1) * MAPWIDTH + x] >= 0)
		{
			able_array[(y - 1) * MAPWIDTH + x] = 1;
		}
		break;
	case YOKO:
		if (x + 1 < MAPWIDTH && MAP[y * MAPWIDTH + (x + 1)] >= 0)
		{
			able_array[y * MAPWIDTH + (x + 1)] = 1;
		}
		if (y + 1 < MAPHEIGHT && MAP[(y + 1) * MAPWIDTH + x] >= 0)
		{
			able_array[(y + 1) * MAPWIDTH + x] = 1;
		}
		if (x - 1 >= 0 && MAP[y * MAPWIDTH + (x - 1)] >= 0)
		{
			able_array[y * MAPWIDTH + (x - 1)] = 1;
		}
		if (y - 1 >= 0 && MAP[(y - 1) * MAPWIDTH + x] >= 0)
		{
			able_array[(y - 1) * MAPWIDTH + x] = 1;
		}
		if (x + 1 < MAPWIDTH && y + 1 < MAPHEIGHT && MAP[(y + 1) * MAPWIDTH + (x + 1)] >= 0)
		{
			able_array[(y + 1) * MAPWIDTH + (x + 1)] = 1;
		}
		if (x - 1 >= 0 && y + 1 < MAPHEIGHT && MAP[(y + 1) * MAPWIDTH + (x - 1)] >= 0)
		{
			able_array[(y + 1) * MAPWIDTH + (x - 1)] = 1;
		}
		if (x - 1 >= 0 && y - 1 >= 0 && MAP[(y - 1) * MAPWIDTH + (x - 1)] >= 0)
		{
			able_array[(y - 1) * MAPWIDTH + (x - 1)] = 1;
		}
		if (x + 1 < MAPWIDTH && y - 1 >= 0 && MAP[(y - 1) * MAPWIDTH + (x + 1)] >= 0)
		{
			able_array[(y - 1) * MAPWIDTH + (x + 1)] = 1;
		}
		break;
	case YUMI:
		for (int i = -5; i < 6; i++)
		{
			if (x + i < MAPWIDTH && MAP[y * MAPWIDTH + (x + i)] >= 0)
			{
				able_array[y * MAPWIDTH + x + i] = 1;
			}
			if (y + i < MAPHEIGHT && MAP[y + i * MAPWIDTH + x] >= 0)
			{
				able_array[(y + i) * MAPWIDTH + x] = 1;
			}
		}
	case YARI:
		for (int i = 1; i < 2; i++)
		{
			if (x + i < MAPWIDTH)
			{
				able_array[y * MAPWIDTH + x + i] = 1;
			}
			if (y + i < MAPHEIGHT)
			{
				able_array[(y + i) * MAPWIDTH + x] = 1;
			}
			if (x - i >= 0)
			{
				able_array[y * MAPWIDTH + x - i] = 1;
			}
			if (y - i >= 0)
			{
				able_array[(y - i) * MAPWIDTH + x] = 1;
			}
		}
		break;
	}
}
/*攻撃タイプの範囲を定義*/
void BATTLE_SCENE::AttackZonePattern(int x, int y)
{
	switch (p_choice->getattacktype())
	{
	case TATE:
		if (x + 1 < MAPWIDTH && MAP[y * MAPWIDTH + (x + 1)] >= 0)
		{
			attack_able[y * MAPWIDTH + (x + 1)] = 1;
		}
		if (y + 1 < MAPHEIGHT && MAP[(y + 1) * MAPWIDTH + x] >= 0)
		{
			attack_able[(y + 1) * MAPWIDTH + x] = 1;
		}
		if (x - 1 >= 0 && MAP[y * MAPWIDTH + (x - 1)] >= 0)
		{
			attack_able[y * MAPWIDTH + (x - 1)] = 1;
		}
		if (y - 1 >= 0 && MAP[(y - 1) * MAPWIDTH + x] >= 0)
		{
			attack_able[(y - 1) * MAPWIDTH + x] = 1;
		}
		break;
	case YOKO:
		if (x + 1 < MAPWIDTH && MAP[y * MAPWIDTH + (x + 1)] >= 0)
		{
			attack_able[y * MAPWIDTH + (x + 1)] = 1;
		}
		if (y + 1 < MAPHEIGHT && MAP[(y + 1) * MAPWIDTH + x] >= 0)
		{
			attack_able[(y + 1) * MAPWIDTH + x] = 1;
		}
		if (x - 1 >= 0 && MAP[y * MAPWIDTH + (x - 1)] >= 0)
		{
			attack_able[y * MAPWIDTH + (x - 1)] = 1;
		}
		if (y - 1 >= 0 && MAP[(y - 1) * MAPWIDTH + x] >= 0)
		{
			attack_able[(y - 1) * MAPWIDTH + x] = 1;
		}
		if (x + 1 < MAPWIDTH && y + 1 < MAPHEIGHT && MAP[(y + 1) * MAPWIDTH + (x + 1)] >= 0)
		{
			attack_able[(y + 1) * MAPWIDTH + (x + 1)] = 1;
		}
		if (x - 1 >= 0 && y + 1 < MAPHEIGHT && MAP[(y + 1) * MAPWIDTH + (x - 1)] >= 0)
		{
			attack_able[(y + 1) * MAPWIDTH + (x - 1)] = 1;
		}
		if (x - 1 >= 0 && y - 1 >= 0 && MAP[(y - 1) * MAPWIDTH + (x - 1)] >= 0)
		{
			attack_able[(y - 1) * MAPWIDTH + (x - 1)] = 1;
		}
		if (x + 1 < MAPWIDTH && y - 1 >= 0 && MAP[(y - 1) * MAPWIDTH + (x + 1)] >= 0)
		{
			attack_able[(y - 1) * MAPWIDTH + (x + 1)] = 1;
		}
		break;
	case YUMI:
		for (int i = -5; i < 6; i++)
		{
			if (x + i < MAPWIDTH && MAP[y * MAPWIDTH + (x + i)] >= 0)
			{
				attack_able[y * MAPWIDTH + x + i] = 1;
			}
			if (y + i < MAPHEIGHT && MAP[y + i * MAPWIDTH + x] >= 0)
			{
				attack_able[(y + i) * MAPWIDTH + x] = 1;
			}
		}
	case YARI:
		for (int i = 1; i < 2; i++)
		{
			if (x + i < MAPWIDTH)
			{
				attack_able[y * MAPWIDTH + x + i] = 1;
			}
			if (y + i < MAPHEIGHT)
			{
				attack_able[(y + i) * MAPWIDTH + x] = 1;
			}
			if (x - i >= 0)
			{
				attack_able[y * MAPWIDTH + x - i] = 1;
			}
			if (y - i >= 0)
			{
				attack_able[(y - i) * MAPWIDTH + x] = 1;
			}
		}
		break;
	}
}
//魔法攻撃可能マス計算
void BATTLE_SCENE::MagicAbleZone(int x, int y, int n)
{
	int place = 0;
	magic_able[p_choice->getx() + p_choice->gety() * MAPWIDTH] = 1;
	if (n > 0)
	{
		if (x - 1 >= 0)
		{
			place = x - 1 + y * MAPWIDTH;
			if (MAP[place] == 0 || MAP[place] == 1 || MAP[place] == 2)
			{
				magic_able[place] = 1;
			}
			MagicAbleZone(x - 1, y, n - 1);
		}
		if (x + 1 < MAPWIDTH)
		{
			place = x + 1 + y * MAPWIDTH;
			if (MAP[place] == 0 || MAP[place] == 1 || MAP[place] == 2)
			{
				magic_able[place] = 1;
			}
			MagicAbleZone(x + 1, y, n - 1);
		}
		if (y - 1 >= 0)
		{
			place = x + (y - 1) * MAPWIDTH;
			if (MAP[place] == 0 || MAP[place] == 1 || MAP[place] == 2)
			{
				magic_able[place] = 1;
			}
			MagicAbleZone(x, y - 1, n - 1);
		}
		if (y + 1 < MAPHEIGHT)
		{
			place = x + (y + 1) * MAPWIDTH;
			if (MAP[place] == 0 || MAP[place] == 1 || MAP[place] == 2)
			{
				magic_able[place] = 1;
			}
			MagicAbleZone(x, y + 1, n - 1);
		}
	}
}
//魔法攻撃範囲マス計算
void BATTLE_SCENE::MagicLaunchZone(int x, int y, int n)
{
	int place = 0;
	if (n > 1)
	{
		if (x - 1 >= 0)
		{
			place = x - 1 + y * MAPWIDTH;
			if (MAP[place] == 0 || MAP[place] == 1 || MAP[place] == 2)
			{
				magic_launch[place] = 1;
			}
			MagicLaunchZone(x - 1, y, n - 1);
		}
		if (x + 1 < MAPWIDTH)
		{
			place = x + 1 + y * MAPWIDTH;
			if (MAP[place] == 0 || MAP[place] == 1 || MAP[place] == 2)
			{
				magic_launch[place] = 1;
			}
			MagicLaunchZone(x + 1, y, n - 1);
		}
		if (y - 1 >= 0)
		{
			place = x + (y - 1) * MAPWIDTH;
			if (MAP[place] == 0 || MAP[place] == 1 || MAP[place] == 2)
			{
				magic_launch[place] = 1;
			}
			MagicLaunchZone(x, y - 1, n - 1);
		}
		if (y + 1 < MAPHEIGHT)
		{
			place = x + (y + 1) * MAPWIDTH;
			if (MAP[place] == 0 || MAP[place] == 1 || MAP[place] == 2)
			{
				magic_launch[place] = 1;
			}
			MagicLaunchZone(x, y + 1, n - 1);
		}
	}
}
//勝利、敗北確認
void BATTLE_SCENE::WinCheck()
{
	int winflg[2] = {0, 0};
	for (int i = 0; i < Enemy_Characternum; i++)
	{
		if(Battle_Chara[i].getflg())
		{
			winflg[1] = 1;
		}
	}
	for (int i = Enemy_Characternum; i < Characternum; i++)
	{
		if (Battle_Chara[i].getflg())
		{
			winflg[0] = 1;
		}
	}
	if (!(ToDo == WIN || ToDo == LOSE) && winflg[0] == 1 && winflg[1] == 0)
	{
		use_flame_time = 0;
		ToDo = WIN;
	}
	else if (!(ToDo == WIN || ToDo == LOSE) && winflg[0] == 0)
	{
		use_flame_time = 0;
		ToDo = LOSE;
	}
}
void BATTLE_SCENE::MoveAbleZone(int x, int y, int MOV)
{
	int place = 0;
	moveable[p_choice->getx() + p_choice->gety() * MAPWIDTH] = p_choice->getMOV();
	if (MOV > 0)
	{
		if (x - 1 >= 0)
		{
			place = x - 1 + y * MAPWIDTH;
			if (MAP[place] == 0 || MAP[place] == p_choice->getflg())
			{
				if (moveable[place] < MOV)
				{
					moveable[place] = MOV - 1;
					
				}
				MoveAbleZone(x - 1, y, MOV - 1);
			}
		}
		if (x + 1 < MAPWIDTH)
		{
			place = x + 1 + y * MAPWIDTH;
			if (MAP[place] == 0 || MAP[place] == p_choice->getflg())
			{
				if (moveable[place] < MOV)
				{
					moveable[place] = MOV - 1;
					
				}
				MoveAbleZone(x + 1, y, MOV - 1);
			}
		}
		if (y - 1 >= 0)
		{
			place = x + (y - 1) * MAPWIDTH;
			if (MAP[place] == 0 || MAP[place] == p_choice->getflg())
			{
				if (moveable[place] < MOV)
				{
					moveable[place] = MOV - 1;
					
				}
				MoveAbleZone(x, y - 1, MOV - 1);
			}
		}
		if (y + 1 < MAPHEIGHT)
		{
			place = x + (y + 1) * MAPWIDTH;
			if (MAP[place] == 0 || MAP[place] == p_choice->getflg())
			{
				if (moveable[place] < MOV)
				{
					moveable[place] = MOV - 1;
					
				}
				MoveAbleZone(x, y + 1, MOV - 1);
			}
		}
	}
}
void BATTLE_SCENE::DrawHelp()
{
	DrawGraph(0, 0, glaphics[55], TRUE);
}