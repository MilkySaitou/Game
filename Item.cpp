#include"DxLib.h"
#include"Item.h"

Item::Item():
	item_id(0),
	item_name("NONE"),
	item_explanation("")
{
}
int Item::getid()const
{
	return item_id;
}
void Item::DrawItemName(int x, int y)
{
	DrawFormatString(x, y, GetColor(0, 0, 0), "%s", item_name.c_str());
}
void Item::DrawItemExplanation(int x, int y)
{
	DrawFormatString(x, y, GetColor(0, 0, 0), "%s", item_explanation.c_str());
}
//*******************
//  ウェポンクラス
//*******************
Weapon::Weapon():
	category(no_weapon),
	AT_PLUS(0),
	MAT_PLUS(0)
{
}
void Weapon::WeaponInit(int id, weaponcategory catego, string name, string explanation, int AT, int MAT)
{
	item_id = id;
	category = catego;
	item_name = name;
	item_explanation = explanation;
	AT_PLUS = AT;
	MAT_PLUS = MAT;
}
weaponcategory Weapon::getWepCate()const
{
	return category;
}
int Weapon::getAT()const
{
	return AT_PLUS;
}
int Weapon::getMAT()const
{
	return MAT_PLUS;
}
//********************
//   アーマークラス
//********************
Armor::Armor():
	category(no_armor),
	DF_PLUS(0),
	MDF_PLUS(0)
{
}
void Armor::ArmorInit(int id, armorcategory catego, string name, string explanation, int DF, int MDF)
{
	item_id = id;
	category = catego;
	item_name = name;
	item_explanation = explanation;
	DF_PLUS = DF;
	MDF_PLUS = MDF;
}
armorcategory Armor::getArmCate()const
{
	return category;
}
int Armor::getDF()const
{
	return DF_PLUS;
}
int Armor::getMDF()const
{
	return MDF_PLUS;
}
//***********************
//  アクセサリークラス
//***********************
Accessory::Accessory():
	AT_PLUS(0),
	MAT_PLUS(0),
	DF_PLUS(0),
	MDF_PLUS(0),
	HP_PLUS(0),
	MP_PLUS(0)
{
}
void Accessory::AccessoryInit(int id, string name, string explanation,int AT,int MAT,int DF, int MDF,int HP, int MP)
{
	item_id = id;
	item_name = name;
	item_explanation = explanation;
	AT_PLUS = AT;
	MAT_PLUS = MAT;
	DF_PLUS = DF;
	MDF_PLUS = MDF;
	HP_PLUS = HP;
	MP_PLUS = MP;
}
int Accessory::getAT()const
{
	return AT_PLUS;
}
int Accessory::getMAT()const
{
	return MAT_PLUS;
}
int Accessory::getDF()const
{
	return DF_PLUS;
}
int Accessory::getMDF()const
{
	return MDF_PLUS;
}
int Accessory::getHP()const
{
	return HP_PLUS;
}
int Accessory::getMP()const
{
	return MP_PLUS;
}
//**********************
//     魔法クラス
//**********************
Magic::Magic():
	useMP(0),
	power(0),
	magic_type(no_magic),
	fire_distance(0),
	damage_range(0),
	sound(0)
{
	memset(glaphic, 0, sizeof(glaphic));
}
void Magic::MagicInit(int ID,string name, string explanation, int MP, int pow, magiccategory type, int dist, int rang,string glaphic_place,string sound_place)
{
	item_id = ID;
	item_name = name;
	item_explanation = explanation;
	useMP = MP;
	power = pow;
	magic_type = type;
	fire_distance = dist;
	damage_range = rang;
	LoadDivGraph(glaphic_place.c_str(), 6, 6, 1, 65, 75, glaphic, TRUE);
	sound = LoadSoundMem(sound_place.c_str());
}
void Magic::DrawMagic(int x, int y, int Flame)
{
	int t = 0;
	x = DrawGraph(x, y, glaphic[Flame], TRUE);
}
void Magic::SoundMagic()
{
	PlaySoundMem(sound, DX_PLAYTYPE_BACK);
}
magiccategory Magic::getMagicCate()const
{
	return magic_type;
}
int Magic::getFireDistance()const
{
	return fire_distance;
}
int Magic::getDamageRange()const
{
	return damage_range;
}
int Magic::getPower()const
{
	return power;
}
int Magic::getUseMP()const
{
	return useMP;
}