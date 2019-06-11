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
	BATTLE_SCENE();																					//�R���X�g���N�^
	void BattleMain(int, int, int, int, string);													//�o�g�������C���֐�
	void Play();																					//�o�g�����W�b�N����
	void Draw();																					//�o�g���`�揈��
	void MapDraw();																					//�}�b�v�`�揈��
	void CharaDrawCalc();																			//�L�����N�^�[�`��ʒu�v�Z����
	void CharaDraw(int, int);																		//�L�����N�^�[�`�揈��
	bool InputController(int);
	void CarsorMove();																				//�J�[�\���ړ�����
	void DrawCarsolAnimation(int);																	//�J�[�\���i���j�̕\���A�j���[�V��������
	void DrawDamageAnimation();																		//�_���[�W�\������
	void fade(int, int upper_limit = 255, int lower_limit = 0);										//��ʈÓ]�A���]����(��P���������Ȃ�A�E�g�A���Ȃ�C��)
	void DrawMoveable();																			//�ړ��\�}�X�\������
	void DrawAttackable();																			//�U���\�}�X�\������
	void DrawMagicAble();																			//���@�U���\�}�X�\������
	void DrawMagicLaunch();																			//���@�U���͈̓}�X�\������
	void StatusShow(int drawbright = 255);															//�����X�e�[�^�X�\������
	void StatusShowSub(int drawbright = 255);														//�E���X�e�[�^�X�\������
	void AttackAbleZone();																			//�U���\�}�X�v�Z����
	void CounterAbleZone();																			//�J�E���^�[�\�}�X�v�Z����
	void AttackZonePattern(character*,int*);														//�U���p�^�[���̒�`
	void AttackZonePattern(int x, int y);															//AI���p���@�I�[�o�[���[�h
	void MagicAbleZone(int x, int y, int range);													//���@�U���\�}�X�v�Z
	void MagicLaunchZone(int x, int y, int range);													//���@�U���͈̓}�X�v�Z
	void MoveAbleZone(int x, int y, int mov);														//�ړ��\�͈̓}�X�v�Z
	void WinCheck();																				//�������Ă��邩�A�s�k���Ă��邩����
	void LoadDataExtract(string data, string& mapdata, string& enemydata, string& mapchipdata);		//�o�g���ɕK�v�ȃf�[�^�ۑ��ꏊ�̓W�J
	void LoadMapChipData(string mapchipdata);														//�}�b�v�`�b�v�f�[�^�ǂݍ���
	void LoadMapData(string mapdata);																//�}�b�v�f�[�^�ǂݍ���
	void LoadEnemyData(string enemy_data);															//�G�f�[�^�ǂݍ���
	void LoadBattleMainData();																		//�o�g�����C���f�[�^�ǂݍ���
	void DrawHelp();

private:
	//����֘A
	int Enemy_Characternum;
	int Player_Characternum;
	int Characternum;
	int key_wait;					//�L�[���͏d��
	int choice_carsol;					//�Q��L�����N�^�[�I�����̃J�[�\���ʒu
	int choice;							//�Q��L�����N�^�[�l��
	int go_character[20];				//�Q��L�����N�^�[����ێ��z��
	char player_num;					//�v���C���[�L�����Q���ő�l��
	int cx, cy;							//�J�[�\�����W
	int MAPWIDTH, MAPHEIGHT;			//�}�b�v��
	int ai_move_x, ai_move_y;			//AI�ړ�����}�X
	int ai_attack_x, ai_attack_y;		//AI�U������}�X
	int last_move_x, last_move_y;		//�Ō�Ɉړ������L�����N�^�[�̌��̈ʒu
	char standbytemp;					//�U�����X�^���o�C��Ԃ�ύX�A���̏�Ԃ�ێ�����
	int status_current_character_team;	//�X�e�[�^�X��ʂɈڂ�Ƃ��ɂǂ����̃`�[��������������ێ�
	int damage;							//�_���[�W���ێ�
	int magic_target_num;				//���@�U���͈͂̓G��
	int oldx, oldy;						//�ړ��O�̃}�X�̍��W�ێ�
	int status_carsor;					//�X�e�[�^�X�\�����̃J�[�\���ʒu
	int getEXP;							//�o�g�����ł̊l��EXP
	BATTLE_PROCESS ToDo;				//�v���Z�X�ړ�
	character* p_choice;				//�I���L����
	character* p_attack_target;			//��U���L����
	character* p_magic_target[20];		//�햂�@�U���L����
	character* p_lastmovechara;			//�Ō�Ɉړ������L����
	int menu_X;							//���j���[�\���ړ�
	int menu_X_animation;				//���j���[�\������
	int moven;							//���[�u�A�j���[�V�������A�������v��
	int glaphics[100];					//�O���t�B�b�N�z��
	int animation_glaphics[30][15];		//�o�g�����̊�{�A�j���[�V�����ێ��@15�����܂őΉ��\
	int* mapchip_glaphics;				//�}�b�v�`�b�v�摜�ۊ�
	int BGM;							//�퓬����BGM
	int bright_change;					//��ʕ`��̖��邳�f�[�^�ێ�
	int use_flame_time;					//�A�j���[�V�����\�����Ԑ���A�t���[���҂��ɂ����p
	int camerax;						//�J�������W�@��������
	int cameray;						//�J�������W�@��������
	Magic current_magic;				//���ݑI������Ă��閂�@��ێ�
	int flg[30];						//�o�g�����̉�ʑJ�ڂɎg��
	const int BLOCKWIDTH = 60;			//�P�}�X�̉摜�̉��̒���
	const int BLOCKHEIGHT = 30;			//�P�}�X�̉摜�̏c�̒���
	const int ATTACKANIMATIONTIME = 90;	//�U�����̃t���[����
	int* MAP;							//�}�b�v�̏󋵕ێ�
	int* MAP_CHIP_GRAPHIC_ARRAY;		//�ǂ̃}�b�v�`�b�v��`�悷�邩�ێ�
	int* moveable;						//�ړ��\�}�X�ێ�
	int* moveable_inverse;				//�ړ��}�X�����̂ڂ�
	int* attack_able;					//�U���͈͂̕ێ�
	int* counter_attack_able;			//�J�E���^�[�U���͈͂̕ێ�
	int* magic_able;					//���@�U���͈͂̕ێ�
	int* magic_launch;					//���@�_���[�W�͈͂̕ێ�	
	character* Battle_Chara;			//�o�g�����̃L�����N�^�[�I�u�W�F�N�g
};

class START_MENU : public Game
{
public:
	int START_SCREEN();
};

#endif