/*	�L�����N�^�[�f�[�^�̊Ǘ������w�b�_�[*/
#ifndef CHARACTER_H
#define CHARACTER_H

#include"Item.h"
/*�L�����N�^�[�U���^�C�v��`*/
enum ATTACKTYPE
{
	TATE = 1,
	YOKO,
	YUMI,
	YARI,
	TUKI
};
/*�o�g�����̃L�����N�^�[�`����*/
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
	/*	�L�����N�^�[�f�[�^�ǂݍ���
		�f�[�^�ǂݍ���
	*/
	void Init(int BattleX,int BattleY,int LV,int AT,int DF,int MAT,int MDF,
			  int DIR,int MOV,int HP,int MP,ATTACKTYPE,string glaphic,string Battleglaphic,string name);
	/*�퓬��*/
	void MoveAbleInit(int*,int*,int,int);													//�ړ���񏉊���
	void SelectTimeShow(int i, int j);														//�o�g��������ʂ̃L�����N�^�[�`��
	void BattleCharaShow(int);																//�퓬���̃L�����N�^�[�`��
	void DirectionChange();																	//�퓬���v���C���[���͂ŕ����]������Ƃ��̏���
	int  MovAbleFlgFunc(int,int,int*,int);													//����}�X���ړ��\���ǂ����Ԃ�
	void MoveCharacterCordinateCalc(int, int, int);											//�ړ����̃L�����N�^�[�̕`����W���v�Z
	void MoveRouteCalculation(int,int,int*,int*,int,int);									//�o�H����o������
	void MoveShow(int,int*, int, int);														//�L�����N�^�[�ړ����̃��C��������S��
	void MoveCancel(int,int);																//�ړ��L�����Z������
	void Attack(character*,int*,int,int);													//�U��������
	void Counter(character*,int*,int,int);													//�J�E���^�[������
	char AttackableFlgFunc(character*,int*,int,int);										//�}�X���U���\���ǂ�������
	int  AttackDamageCalculation(character*);												//�U���_���[�W�v�Z
	int  AttackDamageCalculation(character*, int);											//�U���_���[�W�v�Z AI�Ɏg�p
	void MagicAttack(character*,Magic);														//���@�U��
	int  MagicDamageCalculation(character*,Magic);											//���@�U���_���[�W�v�Z
	void MagicUseMP(Magic);																	//���@�U�����AMP�����
	void DownCheck();																		//�_�E����ԂɂȂ邩���`�F�b�N	
	void StandByChange();																	//�ҋ@�^�ύX
	void removefunc(int&);																	//�퓬�s�\����
	void TurnInit();																		//�^�[���J�n���ɌĂ�
	/*����*/
	void WeaponEquip(int);																	//����𑕔�����Ƃ��ɌĂ�
	void ArmorEquip(int);																	//�h��𑕔�����Ƃ��ɌĂ�
	void AccessoryEquip(int);																//�A�N�Z�T���[�𑕔�����Ƃ��ɌĂ�
	void MagicEquip(int, int);																//���@�𑕔�����Ƃ��ɌĂ�
	void WeaponUnEquip();																	//����𑕔�����
	void ArmorUnEquip();																	//�h��𑕔�����
	void AccessoryUnEquip();																//�A�N�Z�T���[�𑕔�����
	void WeaponExplanation(int, int);														//����̐������\��
	void ArmorExplanation(int, int);														//�h��̐������\��
	void AccessoryExplanation(int, int);													//�A�N�Z�T���[�̐������\��
	void MagicExplanation(int, int, int);													//���@�̐������\��
	void DrawEquipMagicName(int dx,int dy, int equip_magic_num);							//�������̖��@����\��

	void DrawStatusHead(int*);																//�X�e�[�^�X��ʏ㕔�`��
	void DrawStatusBottom(int*);															//�X�e�[�^�X��ʉ����`��
	void DrawName(int, int, int);															//�L�����N�^�[���`��
	//�Q�b�^�[
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

	void EXPCalc(int);																		//�o���l���Z,���x���A�b�v����
	void enemyEXPInsert(int);																//�G��|�����Ƃ��ɓ���o���l���Z�b�g

	character();
	~character();
private:
	string name; //�L�����N�^�[��
	int glaphic_2D_DATA[76];							//�L�����N�^�[�̃o�g����2D�O���t�B�b�N�f�[�^
	int glaphic_standing_DATA[6];						//�����G�Ȃǂ̃f�[�^
	int LV;												//���x���l
	int HP;												//���݂�HP�l
	int MP;												//���݂�MP�l
	int MAXHP;											//HP�ő�l
	int MAXMP;											//MP�ő�l
	int AT;												//�����U����
	int DF;												//�����h���
	int MAT;											//���@�U����
	int MDF;											//���@�h���
	int MOV;											//�ړ���
	char direction;										//���� 1�E���@2�����@3����@4�E��
	int flg;											//�퓬���̃t���O 0�Ȃ�퓬�s�\,1,2�Ȃ�퓬��(�`�[������)
	char moveflg;										//�ړ������Ȃ�0
	char attackflg;										//�U�������Ȃ�0
	char standbyflg;									//�����Ȃ�0 �h��Ȃ�1
	char downflg;										//�_�E����Ԃ��ǂ����@�̗͂̂Q�O%�ȉ��Ŕ���
	ATTACKTYPE attacktype;								//�U���^�C�v
	int x;												//�o�g������x���W
	int y;												//�o�g������y���W
	int oldx, oldy;										//�P�}�X�O�̍��W���L���@���ړ����ɗ��p
	int drawx, drawy;									//�o�g������2D��`�����W
	drawmode character_draw_mode = DRAW_MODE_NORMAL;	//�o�g������2D�L�����N�^�[�̕`����@
	Weapon weapon;										//����������
	Armor armor;										//�����A�[�}�[���
	Accessory accessory;								//�����A�N�Z�T���[��� 
	Magic magic[3];										//�������@���
	int EXP;											//�o���l���
};

#endif