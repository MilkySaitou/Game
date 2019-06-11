void Novel()
{
	if (GameMode == GAME_MODE_NOVEL) {
		/* 読み込み処理 */

		ifstream ifs1("TEXT\\tex001.txt");
		ifstream ifs2("TEXT\\chara001.txt");
		if (!ifs1 && !ifs2) {
			MessageBox(NULL, "テキストファイルが読み込めません", "エラー", MB_OK);
			return -1;
		}
		string ch;
		string name;
		int strroop = 0;
		char str[200][100] = { " " };
		char Name[200][20];

		while (getline(ifs1, ch)) {   //一行ずつ読み込み、strに保管
			int p = ch.length();
			ch.copy(str[strroop], p);
			str[strroop][p] = '\0';
			strroop++;
		}
		strroop = 0;
		while (getline(ifs2, name)) {
			int q = name.length();
			name.copy(Name[strroop], q);
			Name[strroop][q] = '\0';
			strroop++;
		}
		for (int j = 0; j < 200; j++) {
			for (int k = 0; k < 100; k++) {
				if (str[j][k] == 'x') {
					str[j][k] = '\n';
					k++;
				}
			}
		}
		if (TRUE) {
			int g_title1 = LoadGraph("IMG\\title\\title01.png");
			for (int j = 0; j < 256; j = j + 2) {
				if (ProcessMessage() == -1)break;
				if (CheckHitKey(KEY_INPUT_SPACE)) {
					ClearDrawScreen();
					ScreenFlip();
					break;
				}
				SetDrawBright(j, j, j);
				DrawGraph(256, 179, g_title1, TRUE);
				ScreenFlip();
			}
		}
		SetDrawBright(255, 255, 255);
		for (int j = 0; j < 100; j++) {
			if (ProcessMessage() == -1)break;
			ScreenFlip();
		}

		/* その他宣言 */
		int g_Under = LoadGraph("IMG\\nobelUnder.png");
		int g_NobelFlame = LoadGraph("IMG\\nobelFlame.png");
		int ch_p = 0;
		unsigned int ch_i = 0;
		int roop = 0;
		int ButtonFlg = ButtonWait;
		char disp[100] = { " " };

		/* 本処理 */
		while (1) {
			if (ProcessMessage() == -1)break;

			for (unsigned int j = 0; j < ch_i; j++) {
				disp[j] = str[ch_p][j];
			}

			DrawGraph(10, 305, g_NobelFlame, TRUE);
			SetFontSize(22);
			DrawFormatString(25, 315, GetColor(0, 0, 0), Name[ch_p]);
			SetFontSize(30);
			DrawFormatString(25, 354, GetColor(0, 0, 0), disp);
			if (ch_i == strlen(str[ch_p])) {
				DrawGraph(600, 430, g_Under, TRUE);
			}

			if (ButtonFlg == 0 && CheckHitKey(KEY_INPUT_SPACE)) {
				ButtonFlg = ButtonWait;
				if (ch_p == strroop - 1) {
					GameMode = GAME_MODE_BATTLE;
					break;
				}
				if (ch_i < strlen(str[ch_p])) {
					ch_i = strlen(str[ch_p]);
				}
				else {
					ch_i = 0;
					ch_p++;
					for (int j = 0; j < 100; j++) {
						disp[j] = ' ';  //disp初期化
					}
				}
			}

			if (roop > 8 && ch_i < strlen(str[ch_p])) {   //文字送り処理
				roop = 0;
				ch_i = ch_i + 2;
			}
			if (roop < 2000) {
				roop++;
			}
			if (ButtonFlg > 0 && CheckHitKeyAll() == 0) ButtonFlg -= 1;
		}
	}
}