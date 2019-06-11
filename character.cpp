#include "character.h"
#include "player.h"
#include "DxLib.h"
#include <cstdlib>

#define BLOCKWIDTH 60
#define BLOCKHEIGHT 30
#define STATUS_X 23
#define STATUS_Y 60

extern int X;
extern int Y;
extern int CHARACENTERX;
extern Player player_data;
extern Weapon weapon_data[40];
extern Armor armor_data[40];
extern Accessory accessory_data[40];
extern Magic magic_data[40];
extern int EXP_table[99];

//コンストラクタ
//初期化リストで初期化する
character::character():
	name(""),								//キャラクター名
	LV(1),									//レベル値
	HP(0),									//現在のHP値
	MP(0),									//現在のMP値
	MAXHP(0),								//HP最大値
	MAXMP(0),								//MP最大値
	AT(0),									//物理攻撃力
	DF(0),									//物理防御力
	MAT(0),									//魔法攻撃力
	MDF(0),									//魔法防御力
	MOV(0),									//移動量
	direction(1),							//方向 1右下　2左下　3左上　4右上
	flg(0),									//戦闘中のフラグ 0なら戦闘不能,１なら戦闘中
	moveflg(0),								//移動完了なら0
	attackflg(0),							//攻撃完了なら0
	standbyflg(0),							//反撃なら0 防御なら1
	downflg(0),								//ダウン状態かどうか　体力の２０%以下で発生
	attacktype(TATE),						//攻撃タイプ
	x(0),									//バトル中のx座標
	y(0),									//バトル中のy座標
	oldx(0),								//１マス前の座標を記憶　※移動時に利用
	oldy(0),
	drawx(0),								//バトル中2Dキャラクタを描く座標
	drawy(0),
	character_draw_mode(DRAW_MODE_NORMAL),	//バトル中の2Dキャラクターの描画方法
	EXP(0)
{
}
//デストラクタ
character::~character()
{
}
void character::Init(int in_X,int in_y,int in_LV,int in_AT,int in_DF,int in_MAT,int in_MDF,
							int in_DIR,int in_MOVENUM,int in_MAXHP,int in_MAXMP, ATTACKTYPE A,
							string glaphic,string glaphic_face,string p)
{
	moveflg = 1;
	attackflg = 1;
	downflg = 0;
	standbyflg = 0;
	attacktype = A;
	x = in_X;
	y = in_y;
	LV = in_LV;
	HP = in_MAXHP;
	MP = in_MAXMP;
	MAXHP = in_MAXHP;
	MAXMP = in_MAXMP;
	AT = in_AT;
	DF = in_DF;
	MAT = in_MAT;
	MDF = in_MDF;
	MOV = in_MOVENUM;
	direction = in_DIR;
	name = p;
	LoadDivGraph(glaphic.c_str(), 76, 8, 10, 65, 75, glaphic_2D_DATA);
	glaphic_standing_DATA[0] = LoadGraph(glaphic_face.c_str(), TRUE);
}
//ターン変更時の動作
void character::TurnInit() 
{
	moveflg = 1;
	attackflg = 1;
}
//バトル準備画面でのキャラクター描画
void character::SelectTimeShow(int i,int j)
{
	DrawGraph(40 + i * 80, 300 + j * 90, glaphic_2D_DATA[0], TRUE);
}
//バトル中のキャラクター回転処理
void character::DirectionChange()
{
	if (direction != 4)
	{
		direction++;
	}
	else
	{
		direction = 1;
	}
}
//キャラクター盤面表示処理
void character::BattleCharaShow(int i)
{
	/*direction
	|	1.右上向き
	|	2.右下向き
	|	3.左下向き
	|	4.左上向き
	*/
	if (direction == 1)
	{
		if (character_draw_mode == DRAW_MODE_NORMAL)
		{
			DrawGraph(drawx, drawy, glaphic_2D_DATA[i], TRUE);
		}
		else if (character_draw_mode == DRAW_MODE_MOVE)
		{
			DrawGraph(drawx, drawy, glaphic_2D_DATA[i + 4], TRUE);
		}
		else if (character_draw_mode == DRAW_MODE_ATTACK)
		{
			DrawGraph(drawx, drawy, glaphic_2D_DATA[i + 32], TRUE);
		}
		else if (character_draw_mode == DRAW_MODE_DAMAGE)
		{
			DrawGraph(drawx, drawy, glaphic_2D_DATA[56], TRUE);
		}
		else if (character_draw_mode == DRAW_MODE_DOWN)
		{
			DrawGraph(drawx, drawy, glaphic_2D_DATA[i + 60],TRUE);
		}
		else if (character_draw_mode == DRAW_MODE_MAGIC)
		{
			DrawGraph(drawx, drawy, glaphic_2D_DATA[i + 48], TRUE);
		}
	}
	else if (direction == 2) {
		if (character_draw_mode == DRAW_MODE_NORMAL)
		{
			DrawGraph(drawx, drawy, glaphic_2D_DATA[i + 24], TRUE);
		}
		else if (character_draw_mode == DRAW_MODE_MOVE)
		{
			DrawGraph(drawx, drawy, glaphic_2D_DATA[i + 28], TRUE);
		}
		else if (character_draw_mode == DRAW_MODE_ATTACK)
		{
			DrawGraph(drawx, drawy, glaphic_2D_DATA[i + 44], TRUE);
		}
		else if (character_draw_mode == DRAW_MODE_DAMAGE)
		{
			DrawGraph(drawx, drawy, glaphic_2D_DATA[59], TRUE);
		}
		else if (character_draw_mode == DRAW_MODE_DOWN)
		{
			DrawGraph(drawx, drawy, glaphic_2D_DATA[i + 72], TRUE);
		}
		else if (character_draw_mode == DRAW_MODE_MAGIC)
		{
			DrawGraph(drawx, drawy, glaphic_2D_DATA[i + 54], TRUE);
		}
	}
	else if (direction == 3) {
		if (character_draw_mode == DRAW_MODE_NORMAL)
		{
			DrawGraph(drawx, drawy, glaphic_2D_DATA[i + 16], TRUE);
		}
		else if (character_draw_mode == DRAW_MODE_MOVE)
		{
			DrawGraph(drawx, drawy, glaphic_2D_DATA[i + 20], TRUE);
		}
		else if (character_draw_mode == DRAW_MODE_ATTACK)
		{
			DrawGraph(drawx, drawy, glaphic_2D_DATA[i + 40], TRUE);
		}
		else if (character_draw_mode == DRAW_MODE_DAMAGE)
		{
			DrawGraph(drawx, drawy, glaphic_2D_DATA[58], TRUE);
		}
		else if (character_draw_mode == DRAW_MODE_DOWN)
		{
			DrawGraph(drawx, drawy, glaphic_2D_DATA[i + 68], TRUE);
		}
		else if (character_draw_mode == DRAW_MODE_MAGIC)
		{
			DrawGraph(drawx, drawy, glaphic_2D_DATA[i + 52], TRUE);
		}
	}
	else
	{
		if (character_draw_mode == DRAW_MODE_NORMAL)
		{
			DrawGraph(drawx, drawy, glaphic_2D_DATA[i + 8], TRUE);
		}
		else if (character_draw_mode == DRAW_MODE_MOVE)
		{
			DrawGraph(drawx, drawy, glaphic_2D_DATA[i + 12], TRUE);
		}
		else if (character_draw_mode == DRAW_MODE_ATTACK)
		{
			DrawGraph(drawx, drawy, glaphic_2D_DATA[i + 36], TRUE);
		}
		else if (character_draw_mode == DRAW_MODE_DAMAGE)
		{
			DrawGraph(drawx, drawy, glaphic_2D_DATA[57], TRUE);
		}
		else if (character_draw_mode == DRAW_MODE_DOWN)
		{
			DrawGraph(drawx, drawy, glaphic_2D_DATA[i + 64], TRUE);
		}
		else if (character_draw_mode == DRAW_MODE_MAGIC)
		{
			DrawGraph(drawx, drawy, glaphic_2D_DATA[i + 50], TRUE);
		}
	}
}

//###########################################
//　		移動処理関係
//###########################################

//移動の前処理
void character::MoveAbleInit(int* moveable,int* moveable_inverse,int MapWidth,int MapHeight)
{
	for (int i = 0; i < MapHeight; i++) 
	{
		for(int j = 0; j < MapWidth; j++)
		{
			moveable[i * MapWidth + j] = -1;
			moveable_inverse[i * MapWidth + j] = -1;
		}
	}
}
//カーソルで指定されたマスが移動可能かを判定
int  character::MovAbleFlgFunc(int cx, int cy,int* moveable,int MapWidth)
{
	if (moveable[cy * MapWidth + cx] != -1) return 1;
	return 0;
}
//経路割り出し関数
void character::MoveRouteCalculation(int cx, int cy,int* moveable,int* moveable_inverse,int MapWidth,int MapHeight) 
{
	moveable_inverse[cy * MapWidth + cx] = moveable[cy * MapWidth + cx];
	if (cy + 1 < MapHeight)
	{
		if (moveable[(cy + 1) * MapWidth + cx] == moveable[cy * MapWidth + cx] + 1)
		{
			MoveRouteCalculation(cx, cy + 1,moveable,moveable_inverse,MapWidth,MapHeight);
		}
	}
	if (cx + 1 < MapWidth)
	{
		if (moveable[cy * MapWidth + cx + 1] == moveable[cy * MapWidth + cx] + 1)
		{
			MoveRouteCalculation(cx + 1, cy,moveable,moveable_inverse,MapWidth,MapHeight);
		}
	}
	if (cy - 1 >= 0)
	{
		if (moveable[(cy - 1) * MapWidth + cx] == moveable[cy * MapWidth + cx] + 1)
		{
			MoveRouteCalculation(cx, cy - 1,moveable,moveable_inverse,MapWidth,MapHeight);
		}
	}
	if (cx - 1 >= 0)
	{
		if (moveable[cy * MapWidth + cx - 1] == moveable[cy * MapWidth + cx] + 1)
		{
			MoveRouteCalculation(cx - 1, cy,moveable,moveable_inverse,MapWidth,MapHeight);
		}
	}
}
void character::MoveCancel(int last_x, int last_y)
{
	x = last_x;
	y = last_y;
	moveflg = 1;
}
void character::MoveCharacterCordinateCalc(int cx, int cy, int time)
{
	int cordinateX = (x - cx)*(BLOCKWIDTH / 2) + (y - cy)*(BLOCKWIDTH / 2) + X + (BLOCKWIDTH / 2 - CHARACENTERX) - 2;
	int cordinateY = (y - cy)*(BLOCKHEIGHT / 2) - (x - cx)*(BLOCKHEIGHT / 2) + Y - 55;
	int oldcordinateX = (oldx - cx)*(BLOCKWIDTH / 2) + (oldy - cy)*(BLOCKWIDTH / 2) + X + (BLOCKWIDTH / 2 - CHARACENTERX) - 2;
	int oldcordinateY = (oldy - cy)*(BLOCKHEIGHT / 2) - (oldx - cx)*(BLOCKHEIGHT / 2) + Y - 55;
	double T = double(time);
	drawx = oldcordinateX - (int)((oldcordinateX - cordinateX) * (T / 20));
	drawy = oldcordinateY - (int)((oldcordinateY - cordinateY) * (T / 20));
}
void character::MoveShow(int movenum,int* moveable_inverse,int MAPHEIGHT,int MAPWIDTH)
{
	oldx = x;
	oldy = y;
	for (int i = 0; i < MAPHEIGHT; i++)
	{
		for (int j = 0; j < MAPWIDTH; j++)
		{
			if (moveable_inverse[i * MAPWIDTH + j] == MOV - movenum)
			{
				x = j;
				y = i;
				if (oldx - x > 0)
				{
					direction = 1;
				}
				if (oldy - y > 0)
				{
					direction = 2;
				}
				if (oldx - x < 0)
				{
					direction = 3;
				}
				if (oldy - y < 0)
				{
					direction = 4;
				}
			}
		}
	}
	moveflg = 0;
}
//###########################################
//　		攻撃処理関係
//###########################################
char character::AttackableFlgFunc(character* p,int* attackable,int MapWidth,int MapHeight)
{
	if (p != NULL) {
		if (attackable[(p->y) * MapWidth + (p->x)])return 1;
	}
	return 0;
}
int	 character::AttackDamageCalculation(character* target_chara)
{
	int dmg = int((AT - (target_chara->DF)) - (AT - (target_chara->DF)) * (((abs((target_chara->y) - y) - 1) + (abs((target_chara->x) - x) - 1))*0.1));
	if (target_chara->flg == flg)
	{
		dmg = 1;	//同じチームへの攻撃は１ダメージ
	}
	else
	{
		//方向ダメージ補正定義
		if (direction == 1)
		{
			switch (target_chara->getdirection())
			{
			case 1:
				dmg = int(dmg * 1.4);
				break;
			case 2:
				dmg = int(dmg * 1.2);
				break;
			case 4:
				dmg = int(dmg * 1.2);
				break;
			}
		}
		else if (direction == 2)
		{
			switch (target_chara->getdirection())
			{
			case 2:
				dmg = int(dmg * 1.4);
				break;
			case 1:
				dmg = int(dmg * 1.2);
				break;
			case 3:
				dmg = int(dmg * 1.2);
				break;
			}
		}
		else if (direction == 3)
		{
			switch (target_chara->getdirection())
			{
			case 3:
				dmg = int(dmg * 1.4);
				break;
			case 2:
				dmg = int(dmg * 1.2);
				break;
			case 4:
				dmg = int(dmg * 1.2);
				break;
			}
		}
		else if (direction == 4)
		{
			switch (target_chara->getdirection())
			{
			case 4:
				dmg = int(dmg * 1.4);
				break;
			case 1:
				dmg = int(dmg * 1.2);
				break;
			case 3:
				dmg = int(dmg * 1.2);
				break;
			}
		}

		if (target_chara->getstandbyflg() == 1)
		{
			//防御態勢なら25%ダメージカット
			dmg = int(dmg * 0.75);       
		}
		if (target_chara->getdownflg() == 1)
		{
			//ダウン状態ならダメージ20%アップ
			dmg = int(dmg * 1.20);
		}
		if (downflg == 1)
		{
			//自身がダウン状態ならダメージ20%ダウン
			dmg = int(dmg * 0.80);
		}

		if (dmg <= 1)
		{
			dmg = 1;     //１ダメージ以下は１ダメージに修正
		}
	}
	return dmg;
}
//AI処理用(仮想的に方向を指定する必要があるため)
//それ以外の処理はAttackDamageCalculation(character*)と同じ
int	 character::AttackDamageCalculation(character* target_chara, int direct)
{
	int dmg = int((AT - (target_chara->DF)) - (AT - (target_chara->DF)) * (((abs((target_chara->y) - y) - 1) + (abs((target_chara->x) - x) - 1))*0.1));
	if (target_chara->flg == flg)
	{
		dmg = 1;	//同じチームへの攻撃は１ダメージ
	}
	else
	{
		//方向ダメージ補正
		if (direct == 1)
		{
			switch (target_chara->getdirection())
			{
			case 1:
				dmg = int(dmg * 1.4);
				break;
			case 2:
				dmg = int(dmg * 1.2);
				break;
			case 4:
				dmg = int(dmg * 1.2);
				break;
			}
		}
		else if (direct == 2)
		{
			switch (target_chara->getdirection())
			{
			case 2:
				dmg = int(dmg * 1.4);
				break;
			case 1:
				dmg = int(dmg * 1.2);
				break;
			case 3:
				dmg = int(dmg * 1.2);
				break;
			}
		}
		else if (direct == 3)
		{
			switch (target_chara->getdirection())
			{
			case 3:
				dmg = int(dmg * 1.4);
				break;
			case 2:
				dmg = int(dmg * 1.2);
				break;
			case 4:
				dmg = int(dmg * 1.2);
				break;
			}
		}
		else if (direct == 4)
		{
			switch (target_chara->getdirection())
			{
			case 4:
				dmg = int(dmg * 1.4);
				break;
			case 1:
				dmg = int(dmg * 1.2);
				break;
			case 3:
				dmg = int(dmg * 1.2);
				break;
			}
		}

		if (target_chara->getstandbyflg() == 1)
		{
			dmg = int(dmg * 0.75);
		}
		if (target_chara->getdownflg() == 1)
		{
			dmg = int(dmg * 1.20);
		}
		if (downflg == 1)
		{
			dmg = int(dmg * 0.80);
		}

		if (dmg <= 1)
		{
			dmg = 1;     //１ダメージ以下は１ダメージに修正
		}
	}
	return dmg;
}
void character::DownCheck()
{
	//HPが20%以下かを判定
	if ((HP * 100 / MAXHP) <= 20.0)
	{
		downflg = 1;
	}
	if ((HP * 100 / MAXHP) > 20.0)
	{
		downflg = 0;
	}
}
void character::Attack(character* p,int* attackable,int MapWidth,int MapHeight)
{
	if (standbyflg) StandByChange();
	if (x - p->getx() > 0)
	{
		direction = 1;
	}
	else if (y - p->gety() > 0)
	{
		direction = 2;
	}
	else if (x - p->getx() < 0)
	{
		direction = 3;
	}
	else if (y - p->gety() < 0)
	{
		direction = 4;
	}
	if (attackable[(p->y) * MapWidth + (p->x)] == 1)
	{
		p->HP -= AttackDamageCalculation(p);
		attackflg = 0;
	}
}
void character::Counter(character* p,int* counterable,int MapWidth,int MapHeight)
{
	if (x - p->getx() > 0)
	{
		direction = 1;
	}
	else if (y - p->gety() > 0)
	{
		direction = 2;
	}
	else if (x - p->getx() < 0)
	{
		direction = 3;
	}
	else if (y - p->gety() < 0)
	{
		direction = 4;
	}
	if (counterable[(p->y) * MapWidth + (p->x)] == 1)
	{
		p->HP -= AttackDamageCalculation(p);
	}
}
void character::StandByChange()
{
	if (!standbyflg) {
		standbyflg = 1;
	} else {
		standbyflg = 0;
	}
}
void character::MagicAttack(character* choice, Magic magic)
{
	choice ->HP -= MagicDamageCalculation(choice, magic);
	
}
int  character::MagicDamageCalculation(character* choice, Magic magic)
{
	int damage = 0;
	damage = MAT + magic.getPower() - (choice -> MDF);
	//ダメージ修正 最低威力の2分の1
	if (damage <= magic.getPower() / 2)
	{
		damage = magic.getPower() / 2;
	}
	return damage;
}
void character::MagicUseMP(Magic magic)
{
	attackflg = 0;
	MP -= magic.getUseMP();
}
//バトル中戦闘不能キャラクター削除処理
void character::removefunc(int& battle_get_exp) 
{
	if (flg == 2)
	{
		battle_get_exp += EXP;
	}
	flg = 0;
}
/*#######################################*/
//   ステータス画面 ToDoがSTATUSのとき
/*#######################################*/
void character::DrawStatusHead(int* glaphic_back)
{
	/*   ステータス上段    */
	const unsigned int Black = GetColor(0, 0, 0);
	DxLib::DrawGraph		(STATUS_X, STATUS_Y, glaphic_back[13], TRUE);
	DxLib::DrawGraph		(STATUS_X + 59, STATUS_Y + 30, glaphic_standing_DATA[0], TRUE);
	DxLib::DrawFormatString(STATUS_X + 500, STATUS_Y + 16, Black, "%d", LV);
	DxLib:: DrawFormatString(STATUS_X + 461, STATUS_Y + 38, Black, "%03d / %03d", HP, MAXHP);
	DxLib::DrawFormatString(STATUS_X + 461, STATUS_Y + 60, Black, "%03d / %03d", MP, MAXMP);
	//-------------------------------------------------
	DxLib:: DrawFormatString(STATUS_X + 513, STATUS_Y + 90, Black, "%03d", AT);
	DxLib::DrawFormatString(STATUS_X + 513, STATUS_Y + 112, Black, "%03d", DF);
	DxLib::DrawFormatString(STATUS_X + 513, STATUS_Y + 134, Black, "%03d", MAT);
	DxLib::DrawFormatString(STATUS_X + 513, STATUS_Y + 156, Black, "%03d", MDF);
	//-------------------------------------------------
	DxLib::DrawFormatString(STATUS_X + 240, STATUS_Y + 40, Black, "%s", name.c_str());
	//-------------------------------------------------
}
void character::DrawStatusBottom(int* glaphic_back)
{
	if (!glaphic_back) return;
	/*   ステータス下段  */
	const unsigned int Black = GetColor(0, 0, 0);
	DxLib::DrawGraph		(STATUS_X , STATUS_Y + 203, glaphic_back[14], TRUE);
	weapon.DrawItemName		(STATUS_X + 100, STATUS_Y + 240);
	DxLib::DrawGraph		(STATUS_X + 40, STATUS_Y + 230,glaphic_back[22],TRUE);
	switch (weapon.getWepCate())
	{
	case knife:
		DxLib::DrawGraph(STATUS_X + 45, STATUS_Y + 235, glaphic_back[23],TRUE);
		break;
	case sword:
		DxLib::DrawGraph(STATUS_X + 45, STATUS_Y + 235, glaphic_back[24], TRUE);
		break;
	case bow:
		DxLib::DrawGraph(STATUS_X + 45, STATUS_Y + 235, glaphic_back[25], TRUE);
		break;
	case ax:
		DxLib::DrawGraph(STATUS_X + 45, STATUS_Y + 235, glaphic_back[26], TRUE);
		break;
	}
	//---------------------------------------------------------------------
	armor.DrawItemName			(STATUS_X + 100, STATUS_Y + 290);
	DxLib::DrawGraph			(STATUS_X + 40, STATUS_Y + 280, glaphic_back[22], TRUE);
	switch (armor.getArmCate())
	{
	case kimono:
		DxLib::DrawGraph(STATUS_X + 45, STATUS_Y + 285, glaphic_back[27], TRUE);
		break;
	case lightarmor:
		DxLib::DrawGraph(STATUS_X + 45, STATUS_Y + 285, glaphic_back[28], TRUE);
		break;
	case heabyarmor:
		DxLib::DrawGraph(STATUS_X + 45, STATUS_Y + 285, glaphic_back[29], TRUE);
		break;
	}
	//----------------------------------------------------------------------
	accessory.DrawItemName		(STATUS_X + 100, STATUS_Y + 340);
	DxLib::DrawGraph			(STATUS_X + 40, STATUS_Y + 330, glaphic_back[22], TRUE);
	if (accessory.getid() != 0)
	{
		DxLib::DrawGraph(STATUS_X + 45, STATUS_Y + 335, glaphic_back[30], TRUE);
	}
	//----------------------------------------------------------------------
	DxLib::DrawGraph		(STATUS_X + 360, STATUS_Y + 230, glaphic_back[22], TRUE);
	magic[0].DrawItemName	(STATUS_X + 420, STATUS_Y + 240);
	switch (magic[0].getMagicCate())
	{
	case 1:
		DrawGraph(STATUS_X + 365, STATUS_Y + 235, glaphic_back[33], TRUE);
		break;
	case 2:
		DrawGraph(STATUS_X + 365, STATUS_Y + 235, glaphic_back[34], TRUE);
		break;
	case 3:
		DrawGraph(STATUS_X + 365, STATUS_Y + 235, glaphic_back[35], TRUE);
		break;
	case 4:
		DrawGraph(STATUS_X + 365, STATUS_Y + 235, glaphic_back[36], TRUE);
		break;
	}
	DrawGraph				(STATUS_X + 360, STATUS_Y + 280, glaphic_back[22], TRUE);
	magic[1].DrawItemName	(STATUS_X + 420, STATUS_Y + 290);
	switch (magic[1].getMagicCate())
	{
	case 1:
		DrawGraph(STATUS_X + 365, STATUS_Y + 235, glaphic_back[33], TRUE);
		break;
	case 2:
		DrawGraph(STATUS_X + 365, STATUS_Y + 235, glaphic_back[34], TRUE);
		break;
	case 3:
		DrawGraph(STATUS_X + 365, STATUS_Y + 235, glaphic_back[35], TRUE);
		break;
	case 4:
		DrawGraph(STATUS_X + 365, STATUS_Y + 235, glaphic_back[36], TRUE);
		break;
	}
	DrawGraph				(STATUS_X + 360, STATUS_Y + 330, glaphic_back[22], TRUE);
	switch (magic[2].getMagicCate())
	{
	case 1:
		DrawGraph(STATUS_X + 365, STATUS_Y + 235, glaphic_back[33], TRUE);
		break;
	case 2:
		DrawGraph(STATUS_X + 365, STATUS_Y + 235, glaphic_back[34], TRUE);
		break;
	case 3:
		DrawGraph(STATUS_X + 365, STATUS_Y + 235, glaphic_back[35], TRUE);
		break;
	case 4:
		DrawGraph(STATUS_X + 365, STATUS_Y + 235, glaphic_back[36], TRUE);
		break;
	}
	magic[2].DrawItemName	(STATUS_X + 420, STATUS_Y + 340);
}
void character::DrawName(int draw_x, int draw_y,int color)
{
	DrawFormatString(draw_x, draw_y, color, "%s", name.c_str());
}
/*######################################*/
//##            装備関連              ##
/*######################################*/
void character::WeaponExplanation(int s, int t)
{
	weapon.DrawItemExplanation(s, t);
}
void character::ArmorExplanation(int s, int t)
{
	armor.DrawItemExplanation(s, t);
}
void character::AccessoryExplanation(int s, int t)
{
	accessory.DrawItemExplanation(s, t);
}
void character::MagicExplanation(int num, int s, int t)
{
	magic[num].DrawItemExplanation(s, t);
}
void character::WeaponEquip(int ID)
{
	weapon = weapon_data[ID];
	AT  += weapon.getAT();
	MAT += weapon.getMAT();
}
void character::ArmorEquip(int ID)
{
	armor = armor_data[ID];
	DF  += armor.getDF();
	MDF += armor.getMDF();
}
void character::AccessoryEquip(int ID)
{
	accessory = accessory_data[ID];
	AT    += accessory.getAT();
	MAT   += accessory.getMAT();
	DF    += accessory.getDF();
	MDF   += accessory.getMDF();
	HP	  += accessory.getHP();
	MAXHP += accessory.getHP();
	MP    += accessory.getMP();
	MAXMP += accessory.getMP();
}
void character::MagicEquip(int ID,int num)
{
	magic[num] = magic_data[ID];
}
int	 character::getMagicID(int num)const
{
	return magic[num].getid();
}
void character::WeaponUnEquip()
{
	AT  -= weapon.getAT();
	MAT -= weapon.getMAT();
	weapon = weapon_data[0];
}
void character::ArmorUnEquip()
{
	DF  -= armor.getDF();
	MDF -= armor.getMDF();
	armor = armor_data[0];
}
void character::AccessoryUnEquip()
{
	AT  -= accessory.getAT();
	MAT -= accessory.getMAT();
	DF  -= accessory.getDF();
	MDF -= accessory.getMDF();
	accessory = accessory_data[0];
}
void character::DrawEquipMagicName(int dx, int dy, int equip_magic_num)
{
	magic[equip_magic_num].DrawItemName(dx, dy);
}
/*#########################################*/
//      private変数をリターンするだけ
/*#########################################*/
int			character::getflg()const 
{
	return flg;
}
int			character::getx()const 
{
	return x;
}
int			character::gety()const 
{
	return y;
}
char		character::getmoveflg()const 
{
	return moveflg;
}
char		character::getattackflg()const 
{
	return attackflg;
}
int			character::getHP()const
{
	return HP;
}
int			character::getMOV()const
{
	return MOV;
}
int			character::getdrawx()const
{
	return drawx;
}
int			character::getdrawy()const
{
	return drawy;
}
string		character::getname()const
{
	return name;
}
ATTACKTYPE  character::getattacktype()const
{
	return attacktype;
}
char		character::getstandbyflg()const
{
	return standbyflg;
}
char		character::getdirection()const
{
	return direction;
}
char		character::getdownflg()const
{
	return downflg;
}
drawmode	character::getDrawMode()const
{
	return character_draw_mode;
}
int			character::getMP()const
{
	return MP;
}
int			character::getMAXHP()const
{
	return MAXHP;
}
int			character::getMAXMP()const
{
	return MAXMP;
}
int			character::getLV()const
{
	return LV;
}
/*##########################################*/
//	private変数に間接的に値を入れる処理群
/*##########################################*/
//キャラクターのフラグ入力(味方１、敵２)
void character::flgInsert(int FLG)
{
	flg = FLG;
}
void character::MoveFlgInsert()
{
	moveflg = 0;
}
void character::x_insert(int t)
{
	x = t;
}
void character::y_insert(int t)
{
	y = t;
}
void character::DirectionInsert(int t)
{
	direction = t;
}
void character::DrawModeInsert(drawmode mode)
{
	character_draw_mode = mode;
}
void character::drawx_yInsert(int cx, int cy)
{
	drawx = (x - cx)*(BLOCKWIDTH / 2) + (y - cy)*(BLOCKWIDTH / 2) + X + (BLOCKWIDTH / 2 - CHARACENTERX) - 2;
	drawy = (y - cy)*(BLOCKHEIGHT / 2) - (x - cx)*(BLOCKHEIGHT / 2) + Y - 55;
}
void character::EXPCalc(int exp_value)
{
	EXP += exp_value;
	//レベルアップ処理
	while (EXP >= EXP_table[LV - 1])
	{
		EXP -= EXP_table[LV - 1];
		LV++;
	}
}
void character::enemyEXPInsert(int inEXP)
{
	EXP = inEXP;
}