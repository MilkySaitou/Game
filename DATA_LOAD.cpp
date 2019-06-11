#include"DATA_LOAD.h"
#include"DxLib.h"

//データ読み込み時,例外が発生した場合は全ての値が捨てられます.

//プレイヤーデータ読み込み
void LoadPlayerData()
{
	std::ifstream ifs("DATA\\chara_first.txt");
	try
	{
		if (!ifs) throw 1;
	}
	catch (int)
	{
		MessageBox(nullptr, TEXT("PLAYERDATA CAN'T BE OPENED."), TEXT("ERROR"), MB_OK);
		return;
	}
	int AT;
	int DF;
	int MAT;
	int MDF;
	int DIR;
	int MOVENUM;
	int HP;
	int MP;
	ATTACKTYPE ATTACK = TATE;
	string glaphic_data_place; //画像データの保管場所を指す
	string glaphic_face_place;
	string name; //キャラクターの名前
	int wepID;
	int armID;
	int acceID;
	int magicID1;
	int magicID2;
	int magicID3;
	string buf;
	for (int i = 0; i < player_data.getPlayableNum(); i++)
	{
		try
		{
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
			ifs >> glaphic_face_place;
			ifs >> name;
			ifs >> buf;
			wepID = stoi(buf);
			ifs >> buf;
			armID = stoi(buf);
			ifs >> buf;
			acceID = stoi(buf);
			ifs >> buf;
			magicID1 = stoi(buf);
			ifs >> buf;
			magicID2 = stoi(buf);
			ifs >> buf;
			magicID3 = stoi(buf);
			player_data.character_Init(AT, DF, MAT, MDF, DIR, MOVENUM, HP, MP, ATTACK, glaphic_data_place, glaphic_face_place,
				name, wepID, armID, acceID, magicID1, magicID2, magicID3, i);
		}
		catch (std::invalid_argument e)
		{
			MessageBox(nullptr, TEXT("Incorrect Data.(PLAYER DATA)"), TEXT("ERROR"), MB_OK);
		}

	}
}
//武器データ読み込み
void LoadWeaponData()
{
	ifstream ifs("DATA\\weapon_list.txt");
	try
	{
		if (!ifs) throw 1;
	}
	catch (int)
	{
		MessageBox(nullptr, TEXT("WEAPON_DATA CAN'T BE OPENED."), TEXT("ERROR"), MB_OK);
		return;
	}
	//空アイテムID 0を定義
	weapon_data[0].WeaponInit(0, no_weapon, "NONE", "", 0, 0);
	//↓データ読み込み
	string buf;
	int category;
	int AT;
	int MAT;
	string name;
	string explanation;
	int i = 1;
	weaponcategory catego = no_weapon;
	while (1)
	{
		try
		{
			ifs >> buf;
			category = stoi(buf);
			if (category == 1)
			{
				catego = knife;
			}
			else if (category == 2)
			{
				catego = sword;
			}
			else if (category == 3)
			{
				catego = ax;
			}
			else if (category == 4)
			{
				catego = bow;
			}
			ifs >> buf;
			AT = stoi(buf);
			ifs >> buf;
			MAT = stoi(buf);
			ifs >> buf;
			name = buf;
			ifs >> buf;
			explanation = buf;
			weapon_data[i].WeaponInit(i, catego, name, explanation, AT, MAT);
		}
		catch(std::invalid_argument e)
		{
			MessageBox(nullptr, TEXT("Incorrect DATA.(WEAPON DATA)"), TEXT("ERROR"), MB_OK);
		}
		i++;
		if (ifs.eof()) break;
	}
}
//アーマーデータ読み込み
void LoadArmorData()
{
	ifstream ifs("DATA\\armor_list.txt");
	try
	{
		if (!ifs) throw 1;
	}
	catch (int)
	{
		MessageBox(NULL, TEXT("ARMOR_DATA CAN'T BE OPENED."), TEXT("ERROR"), MB_OK);
		return;
	}
	//空アイテムID　0
	armor_data[0].ArmorInit(0, no_armor, "NONE", "", 0, 0);
	//以下読み込み
	string buf;
	int category;
	int DF;
	int MDF;
	string name;
	string explanation;
	int i = 1;
	armorcategory catego = no_armor;
	while (1)
	{
		try
		{
			ifs >> buf;
			category = stoi(buf);
			if (category == 1)
			{
				catego = kimono;
			}
			else if (category == 2)
			{
				catego = lightarmor;
			}
			else if (category == 3)
			{
				catego = heabyarmor;
			}
			ifs >> buf;
			DF = stoi(buf);
			ifs >> buf;
			MDF = stoi(buf);
			ifs >> buf;
			name = buf;
			ifs >> buf;
			explanation = buf;
			armor_data[i].ArmorInit(i, catego, name, explanation, DF, MDF);
			i++;
			if (ifs.eof()) break;
		}
		catch (std::invalid_argument e)
		{
			MessageBox(NULL, TEXT("Incorrect DATA.(ARMOR DATA)"), TEXT("ERROR"), MB_OK);
		}
	}
}
//アクセサリーデータ読み込み
void LoadAccessoryData()
{
	ifstream ifs("DATA\\accessory_list.txt");
	try
	{
		if (!ifs) throw 1;
	}
	catch (int)
	{
		MessageBox(NULL, TEXT("ACCESSORY_DATA CAN'T BE OPENED."), TEXT("ERROR"), MB_OK);
		return;
	}
	//空アイテムID　0
	accessory_data[0].AccessoryInit(0, "NONE", "", 0, 0, 0, 0, 0, 0);
	//以下読み込み
	string buf;
	int AT;
	int DF;
	int MAT;
	int MDF;
	int HP;
	int MP;
	string name;
	string explanation;
	int i = 1;
	while (1)
	{
		try
		{
			ifs >> buf;
			AT = stoi(buf);
			ifs >> buf;
			MAT = stoi(buf);
			ifs >> buf;
			DF = stoi(buf);
			ifs >> buf;
			MDF = stoi(buf);
			ifs >> buf;
			HP = stoi(buf);
			ifs >> buf;
			MP = stoi(buf);
			ifs >> buf;
			name = buf;
			ifs >> buf;
			explanation = buf;
			accessory_data[i].AccessoryInit(i, name, explanation, AT, MAT, DF, MDF, HP, MP);
			i++;
			if (ifs.eof()) break;
		}
		catch (std::invalid_argument e)
		{
			MessageBox(NULL, TEXT("Incorrect DATA.(ACCESSORY DATA)"), TEXT("ERROR"), MB_OK);
		}
	}
}
//魔法データ読み込み
void LoadMagicData()
{
	ifstream ifs("DATA\\magic_list.txt");
	try
	{
		if (!ifs) throw 1;
	}
	catch (int)
	{
		MessageBox(NULL, TEXT("MAGIC_DATA CAN'T BE OPENED."), TEXT("ERROR"), MB_OK);
		return;
	}
	//以下読み込み
	string buf;
	string name;
	string explanation;
	string glaphic_place;
	string sound_place;
	int MP;
	int power;
	magiccategory type = no_magic;
	int fire_dist;
	int range;
	int i = 1;

	while (1)
	{
		try
		{
			ifs >> buf;
			name = buf;
			ifs >> buf;
			explanation = buf;
			ifs >> buf;
			MP = stoi(buf);
			ifs >> buf;
			power = stoi(buf);
			ifs >> buf;
			switch (stoi(buf))
			{
			case 1:
				type = fire;
				break;
			case 2:
				type = water;
				break;
			case 3:
				type = wind;
				break;
			case 4:
				type = soil;
				break;
			}
			ifs >> buf;
			fire_dist = stoi(buf);
			ifs >> buf;
			range = stoi(buf);
			ifs >> buf;
			glaphic_place = buf;
			ifs >> buf;
			sound_place = buf;
			magic_data[i].MagicInit(i, name, explanation, MP, power, type, fire_dist, range, glaphic_place, sound_place);
			i++;
			if (ifs.eof()) break;
		}
		catch (std::invalid_argument e)
		{
			MessageBox(NULL, TEXT("Incorrect DATA.(MAGIC DATA)"), TEXT("ERROR"), MB_OK);
		}
	}
}
//音データ読み込み
void LoadSoundData()
{
	ifstream ifs("DATA\\SE.txt");
	try
	{
		if (!ifs) throw 1;
	}
	catch (int)
	{
		MessageBox(nullptr, TEXT("SOUND_DATA CAN'T BE OPENED."), TEXT("ERROR"), MB_OK);
		return;
	}
	string buf;
	int i = 0;
	while (1)
	{
		try
		{
			ifs >> buf;
			sound_data[i] = LoadSoundMem(buf.c_str());
			i++;
			if (ifs.eof()) break;
		}
		catch (std::invalid_argument e)
		{
			MessageBox(nullptr, TEXT("Incorrect DATA.(SOUND DATA)"), TEXT("ERROR"), MB_OK);
		}
	}
	ifs >> buf;
}
void LoadEXPtable()
{
	ifstream ifs("DATA\\EXP_table.txt");
	try
	{
		if (!ifs) throw 1;
	}
	catch (int)
	{
		MessageBox(nullptr, TEXT("EXPTABLE_DATA CAN'T BE OPENED."), TEXT("ERROR"), MB_OK);
		return;
	}
	string buf;
	int i = 0;
	while (!ifs.eof())
	{
		try
		{
			ifs >> buf;
			EXP_table[i] = stoi(buf);
			i++;
		}
		catch (std::invalid_argument e)
		{
			MessageBox(nullptr, TEXT("Incorrect DATA.(SOUND DATA)"), TEXT("ERROR"), MB_OK);
		}
	}
}
//経験値テーブル生成(コメントアウト)
/*void EXP()
{
	std::ofstream ofs("DATA\\EXP_table.txt");
	for (int i = 2; i < 101; i++)
	{
		int s = int(100.0 * pow(1.8 ,double(i) / 8.0));
		ofs << s;
		ofs << std::endl;
	}
}
*/