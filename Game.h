#ifndef GAME_H
#define GAME_H

#include"character.h"
#include"player.h"
#include"KeyConfig.h"

extern Player		player_data;
extern Weapon		weapon_data[40];
extern Armor		armor_data[40];
extern Accessory	accessory_data[40];
extern Magic		magic_data[40];
extern int			sound_data[100];
extern int			EXP_table[99];
extern Keymap		key;

class Game{
public:
	void Init()
	{
	}
	void End();
};

enum BATTLE_PROCESS
{
	BATTLE_START,
	BATTLE_CHARACTER_SELECT_INIT,
	BATTLE_CHARACTER_SELECT,
	TURN_CHANGE,
	SELECT_TO_EXPLORE,
	EXPLORE,
	MENU,
	MENU_TO_EXPLORE,
	MOVE,
	MOVE_ANIMATION,
	MOVE_POST,
	ATTACK,
	ATTACK_ANIMATION,
	ATTACK_POST,
	COUNTER,
	COUNTER_ANIMATION,
	COUNTER_POST,
	MAGIC,
	MAGIC_LAUNCH,
	MAGIC_ANIMATION,
	MAGIC_POST,
	STATUS,
	MENU_TO_STATUS,
	STATUS_TO_MENU1,
	STATUS_TO_MENU2,
	STATUS_TO_EXPLORE1,
	STATUS_TO_EXPLORE2,
	ENEMY_MOVE_ABLE,
	ENEMY_ATTACK_ABLE,
	ENEMY_TURN_AI_THINK,
	ENEMY_TURN_MOVE,
	ENEMY_TURN_MOVE_ANIMATION,
	ENEMY_TURN_MOVE_POST,
	ENEMY_TURN_ATTACK,
	ENEMY_TURN_ATTACK_ANIMATION,
	ENEMY_TURN_ATTACK_POST,
	ENEMY_TURN_COUNTER,
	ENEMY_TURN_COUNTER_ANIMATION,
	ENEMY_TURN_COUNTER_POST,
	WIN,
	LOSE
};

class BATTLE_SCENE : public Game
{
public:
	BATTLE_SCENE();																					//コンストラクタ
	void BattleMain(int, int, int, int, string);													//バトル中メイン関数
	void Play();																					//バトルロジック処理
	void Draw();																					//バトル描画処理
	void MapDraw();																					//マップ描画処理
	void CharaDrawCalc();																			//キャラクター描画位置計算処理
	void CharaDraw(int, int);																		//キャラクター描画処理
	bool InputController(int);
	void CarsorMove();																				//カーソル移動処理
	void DrawCarsolAnimation(int);																	//カーソル（剣）の表示アニメーション処理
	void DrawDamageAnimation();																		//ダメージ表示処理
	void fade(int, int upper_limit = 255, int lower_limit = 0);										//画面暗転、明転処理(第１引数が正ならアウト、負ならイン)
	void DrawMoveable();																			//移動可能マス表示処理
	void DrawAttackable();																			//攻撃可能マス表示処理
	void DrawMagicAble();																			//魔法攻撃可能マス表示処理
	void DrawMagicLaunch();																			//魔法攻撃範囲マス表示処理
	void StatusShow(int drawbright = 255);															//左下ステータス表示処理
	void StatusShowSub(int drawbright = 255);														//右下ステータス表示処理
	void AttackAbleZone();																			//攻撃可能マス計算処理
	void CounterAbleZone();																			//カウンター可能マス計算処理
	void AttackZonePattern(character*,int*);														//攻撃パターンの定義
	void AttackZonePattern(int x, int y);															//AI利用時　オーバーロード
	void MagicAbleZone(int x, int y, int range);													//魔法攻撃可能マス計算
	void MagicLaunchZone(int x, int y, int range);													//魔法攻撃範囲マス計算
	void MoveAbleZone(int x, int y, int mov);														//移動可能範囲マス計算
	void WinCheck();																				//勝利しているか、敗北しているか判定
	void LoadDataExtract(string data, string& mapdata, string& enemydata, string& mapchipdata);		//バトルに必要なデータ保存場所の展開
	void LoadMapChipData(string mapchipdata);														//マップチップデータ読み込み
	void LoadMapData(string mapdata);																//マップデータ読み込み
	void LoadEnemyData(string enemy_data);															//敵データ読み込み
	void LoadBattleMainData();																		//バトルメインデータ読み込み
	void DrawHelp();

private:
	//操作関連
	int Enemy_Characternum;
	int Player_Characternum;
	int Characternum;
	int key_wait;					//キー入力重み
	int choice_carsol;					//参戦キャラクター選択時のカーソル位置
	int choice;							//参戦キャラクター人数
	int go_character[20];				//参戦キャラクター決定保持配列
	char player_num;					//プレイヤーキャラ参加最大人数
	int cx, cy;							//カーソル座標
	int MAPWIDTH, MAPHEIGHT;			//マップ幅
	int ai_move_x, ai_move_y;			//AI移動決定マス
	int ai_attack_x, ai_attack_y;		//AI攻撃決定マス
	int last_move_x, last_move_y;		//最後に移動したキャラクターの元の位置
	char standbytemp;					//攻撃時スタンバイ状態を変更、元の状態を保持する
	int status_current_character_team;	//ステータス画面に移るときにどっちのチームから入ったか保持
	int damage;							//ダメージ数保持
	int magic_target_num;				//魔法攻撃範囲の敵数
	int oldx, oldy;						//移動前のマスの座標保持
	int status_carsor;					//ステータス表示時のカーソル位置
	int getEXP;							//バトル内での獲得EXP
	BATTLE_PROCESS ToDo;				//プロセス移動
	character* p_choice;				//選択キャラ
	character* p_attack_target;			//被攻撃キャラ
	character* p_magic_target[20];		//被魔法攻撃キャラ
	character* p_lastmovechara;			//最後に移動したキャラ
	int menu_X;							//メニュー表示移動
	int menu_X_animation;				//メニュー表示時間
	int moven;							//ムーブアニメーション時、歩数を計る
	int glaphics[100];					//グラフィック配列
	int animation_glaphics[30][15];		//バトル中の基本アニメーション保持　15枚分まで対応可能
	int* mapchip_glaphics;				//マップチップ画像保管
	int BGM;							//戦闘中のBGM
	int bright_change;					//画面描画の明るさデータ保持
	int use_flame_time;					//アニメーション表示時間制御、フレーム待ちにも利用
	int camerax;						//カメラ座標　※未実装
	int cameray;						//カメラ座標　※未実装
	Magic current_magic;				//現在選択されている魔法を保持
	int flg[30];						//バトル中の画面遷移に使う
	const int BLOCKWIDTH = 60;			//１マスの画像の横の長さ
	const int BLOCKHEIGHT = 30;			//１マスの画像の縦の長さ
	const int ATTACKANIMATIONTIME = 90;	//攻撃時のフレーム数
	int* MAP;							//マップの状況保持
	int* MAP_CHIP_GRAPHIC_ARRAY;		//どのマップチップを描画するか保持
	int* moveable;						//移動可能マス保持
	int* moveable_inverse;				//移動マスさかのぼり
	int* attack_able;					//攻撃範囲の保持
	int* counter_attack_able;			//カウンター攻撃範囲の保持
	int* magic_able;					//魔法攻撃範囲の保持
	int* magic_launch;					//魔法ダメージ範囲の保持	
	character* Battle_Chara;			//バトル中のキャラクターオブジェクト
};

class START_MENU : public Game
{
public:
	int START_SCREEN();
};

#endif