/*	キャラクターデータの管理処理ヘッダー*/
#ifndef CHARACTER_H
#define CHARACTER_H

#include"Item.h"
/*キャラクター攻撃タイプ定義*/
enum ATTACKTYPE
{
	TATE = 1,
	YOKO,
	YUMI,
	YARI,
	TUKI
};
/*バトル中のキャラクター描画状態*/
enum drawmode
{
	DRAW_MODE_NORMAL = 1,
	DRAW_MODE_DAMAGE,
	DRAW_MODE_ATTACK,
	DRAW_MODE_MOVE,
	DRAW_MODE_DOWN,
	DRAW_MODE_MAGIC
};
class character {
public:
	/*	キャラクターデータ読み込み
		データ読み込み
	*/
	void Init(int BattleX,int BattleY,int LV,int AT,int DF,int MAT,int MDF,
			  int DIR,int MOV,int HP,int MP,ATTACKTYPE,string glaphic,string Battleglaphic,string name);
	/*戦闘時*/
	void MoveAbleInit(int*,int*,int,int);													//移動情報初期化
	void SelectTimeShow(int i, int j);														//バトル準備画面のキャラクター描画
	void BattleCharaShow(int);																//戦闘中のキャラクター描画
	void DirectionChange();																	//戦闘中プレイヤー入力で方向転換するときの処理
	int  MovAbleFlgFunc(int,int,int*,int);													//あるマスが移動可能かどうか返す
	void MoveCharacterCordinateCalc(int, int, int);											//移動中のキャラクターの描画座標を計算
	void MoveRouteCalculation(int,int,int*,int*,int,int);									//経路割り出し処理
	void MoveShow(int,int*, int, int);														//キャラクター移動中のメイン処理を担当
	void MoveCancel(int,int);																//移動キャンセル処理
	void Attack(character*,int*,int,int);													//攻撃時処理
	void Counter(character*,int*,int,int);													//カウンター時処理
	char AttackableFlgFunc(character*,int*,int,int);										//マスが攻撃可能かどうか判定
	int  AttackDamageCalculation(character*);												//攻撃ダメージ計算
	int  AttackDamageCalculation(character*, int);											//攻撃ダメージ計算 AIに使用
	void MagicAttack(character*,Magic);														//魔法攻撃
	int  MagicDamageCalculation(character*,Magic);											//魔法攻撃ダメージ計算
	void MagicUseMP(Magic);																	//魔法攻撃時、MP消費処理
	void DownCheck();																		//ダウン状態になるかをチェック	
	void StandByChange();																	//待機型変更
	void removefunc(int&);																	//戦闘不能処理
	void TurnInit();																		//ターン開始時に呼ぶ
	/*装備*/
	void WeaponEquip(int);																	//武器を装備するときに呼ぶ
	void ArmorEquip(int);																	//防具を装備するときに呼ぶ
	void AccessoryEquip(int);																//アクセサリーを装備するときに呼ぶ
	void MagicEquip(int, int);																//魔法を装備するときに呼ぶ
	void WeaponUnEquip();																	//武器を装備解除
	void ArmorUnEquip();																	//防具を装備解除
	void AccessoryUnEquip();																//アクセサリーを装備解除
	void WeaponExplanation(int, int);														//武器の説明文表示
	void ArmorExplanation(int, int);														//防具の説明文表示
	void AccessoryExplanation(int, int);													//アクセサリーの説明文表示
	void MagicExplanation(int, int, int);													//魔法の説明文表示
	void DrawEquipMagicName(int dx,int dy, int equip_magic_num);							//装備中の魔法名を表示

	void DrawStatusHead(int*);																//ステータス画面上部描画
	void DrawStatusBottom(int*);															//ステータス画面下部描画
	void DrawName(int, int, int);															//キャラクター名描画
	//ゲッター
	int  getflg()const;
	int  getx()const;
	int  gety()const;
	int  getHP()const;
	int  getMP()const;
	int  getMAXHP()const;
	int  getMAXMP()const;
	int  getLV()const;
	int  getMOV()const;
	char getmoveflg()const;
	char getattackflg()const;
	char getstandbyflg()const;
	char getdirection()const;
	char getdownflg()const;
	string getname()const;
	int getMagicID(int num)const;
	int getdrawx()const;
	int getdrawy()const;
	drawmode getDrawMode()const;
	ATTACKTYPE getattacktype()const;
	void flgInsert(int);
	void x_insert(int);
	void y_insert(int);
	void DirectionInsert(int);
	void MoveFlgInsert();
	void DrawModeInsert(drawmode);
	void drawx_yInsert(int, int);

	void EXPCalc(int);																		//経験値清算,レベルアップ処理
	void enemyEXPInsert(int);																//敵を倒したときに得る経験値をセット

	character();
	~character();
private:
	string name; //キャラクター名
	int glaphic_2D_DATA[76];							//キャラクターのバトル時2Dグラフィックデータ
	int glaphic_standing_DATA[6];						//立ち絵などのデータ
	int LV;												//レベル値
	int HP;												//現在のHP値
	int MP;												//現在のMP値
	int MAXHP;											//HP最大値
	int MAXMP;											//MP最大値
	int AT;												//物理攻撃力
	int DF;												//物理防御力
	int MAT;											//魔法攻撃力
	int MDF;											//魔法防御力
	int MOV;											//移動量
	char direction;										//方向 1右下　2左下　3左上　4右上
	int flg;											//戦闘中のフラグ 0なら戦闘不能,1,2なら戦闘中(チーム分け)
	char moveflg;										//移動完了なら0
	char attackflg;										//攻撃完了なら0
	char standbyflg;									//反撃なら0 防御なら1
	char downflg;										//ダウン状態かどうか　体力の２０%以下で発生
	ATTACKTYPE attacktype;								//攻撃タイプ
	int x;												//バトル中のx座標
	int y;												//バトル中のy座標
	int oldx, oldy;										//１マス前の座標を記憶　※移動時に利用
	int drawx, drawy;									//バトル中の2Dを描く座標
	drawmode character_draw_mode = DRAW_MODE_NORMAL;	//バトル中の2Dキャラクターの描画方法
	Weapon weapon;										//装備武器情報
	Armor armor;										//装備アーマー情報
	Accessory accessory;								//装備アクセサリー情報 
	Magic magic[3];										//装備魔法情報
	int EXP;											//経験値情報
};

#endif