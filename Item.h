#ifndef ITEM_H
#define ITEM_H

#include<string>
using namespace std;

enum weaponcategory
{
	no_weapon = 0,
	knife,
	sword,
	ax,
	bow
};
enum armorcategory
{
	no_armor = 0,
	lightarmor,
	heabyarmor,
	kimono
};
enum magiccategory
{
	no_magic = 0,
	fire,
	water,
	wind,
	soil
};

class Item
{
protected:
	int item_id;
	string item_name;
	string item_explanation;
public:
	Item();
	int getid()const;
	void DrawItemName(int, int);
	void DrawItemExplanation(int, int);
};

class Weapon : public Item
{
private:
	weaponcategory category;
	int AT_PLUS;
	int MAT_PLUS;
public:
	Weapon();
	void WeaponInit(int, weaponcategory, string, string, int, int);
	weaponcategory getWepCate()const;
	int getAT()const;
	int getMAT()const;
};
class Armor : public Item
{
private:
	armorcategory category;
	int DF_PLUS;
	int MDF_PLUS;
public:
	Armor();
	void ArmorInit(int, armorcategory, string, string, int, int);
	armorcategory getArmCate()const;
	int getDF()const;
	int getMDF()const;
};
class Accessory : public Item
{
private:
	int AT_PLUS;
	int MAT_PLUS;
	int DF_PLUS;
	int MDF_PLUS;
	int HP_PLUS;
	int MP_PLUS;
public:
	Accessory();
	void AccessoryInit(int, string, string, int, int, int, int, int, int);
	int getAT()const;
	int getMAT()const;
	int getDF()const;
	int getMDF()const;
	int getHP()const;
	int getMP()const;
};

class Magic : public Item
{
private:
	int useMP;
	int power;
	magiccategory magic_type;
	int fire_distance;
	int damage_range;
	int glaphic[6];
	int sound;
public:
	Magic();
	void MagicInit(int, string, string, int, int, magiccategory, int, int, string,string);
	void DrawMagic(int x,int y,int flame);
	void SoundMagic();
	magiccategory getMagicCate()const;
	int getFireDistance()const;
	int getDamageRange()const;
	int getPower()const;
	int getUseMP()const;
};

#endif