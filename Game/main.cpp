#include <DxLib.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

#define num 10
#define STAGE_NUM 4
#define DEFAULTSPEED 20

int Key[256];
int stage = 1;
int SCORE;
int speed;

int LargeFont;
int NormalFont;
int ExLargeFont;

int t;




char map[10][20];

enum MODE { TITLE, GAME };
MODE mode;

int GetHitKeyStateAll_2(int GetHitKeyStateAll_InputKey[]) {
	char GetHitKeyStateAll_Key[256];
	GetHitKeyStateAll(GetHitKeyStateAll_Key);
	for (int i = 0; i < 256; i++) {
		if (GetHitKeyStateAll_Key[i] == 1) GetHitKeyStateAll_InputKey[i]++;
		else                               GetHitKeyStateAll_InputKey[i] = 0;
	}
	return 0;
}

int title();
int game(int stg);
int chk_line();
int hint(const TCHAR *Title, const TCHAR *String);
void wait(int);
void wait_and_flip();

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	mode = TITLE;

	SetOutApplicationLogValidFlag(FALSE);


	ChangeWindowMode(TRUE);//ウィンドウモード
	if (DxLib_Init() == -1)return -1;
	if (SetDrawScreen(DX_SCREEN_BACK) != 0) return -1;//初期化と裏画面化

	NormalFont = CreateFontToHandle(NULL, 20, 1, -1);
	LargeFont = CreateFontToHandle(NULL, 30, 1);
	ExLargeFont = CreateFontToHandle(NULL, 70, 1);

	srand((unsigned)time(NULL));


	//INIT END.


	SetMainWindowText("ブロック積もうよ! β1.0.1");

	while (TRUE) {

		if (mode == TITLE) {
			if (title() == 1)break;
		}
		else if (mode == GAME) {
			game(stage);
			mode = TITLE;
		}

	}

	DxLib_End();
	return 0;
}

int title() {

	int ret = 1;
	while (ProcessMessage() == 0 &&
		ClearDrawScreen() == 0 &&
		GetHitKeyStateAll_2(Key) == 0 &&
		Key[KEY_INPUT_ESCAPE] == 0) {

		DrawStringToHandle(200, 150, "↑↓キーでステージ番号をえらんでね", GetColor(255, 255, 255), NormalFont);
		DrawStringToHandle(200, 250, "Enterでスタートするよ", GetColor(255, 255, 255), LargeFont);

		DrawStringToHandle(0, 0, "ブロック積もうよ!", GetColor(255, 255, 128), ExLargeFont);
		DrawStringToHandle(500, 70, "β1.0.1", GetColor(255, 127, 0), LargeFont);

		DrawFormatStringToHandle(200, 190, GetColor(128, 255, 128), ExLargeFont, "STAGE:%02d", stage);

		DrawStringToHandle(0, 450, "Escキーで終了します", GetColor(255, 255, 255), NormalFont);
		if (Key[KEY_INPUT_UP] == 1 && stage<(STAGE_NUM-1))stage++;
		if (Key[KEY_INPUT_DOWN] == 1 && stage>1)stage--;
		if (Key[KEY_INPUT_RETURN] == 1) {
			if (stage ==  1) {
				mode = GAME;
				ret = 2;
				break;
			}
			else {
				hint("ブロック積もうよ! ミニ", "すみません\nこれは体験版なのでステージ1しか遊べません。\nステージ2以上はお家であそんでね");
			}
		}


		wait_and_flip();
	}

	return ret;
}



int game(int stg) {

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 20; j++) {
			map[i][j] = 0;
		}
	}

	switch (stg) {
	case 0:
		hint("Debug", "このステージはデバッグ用だお\n  (`・ω・´)");
		break;
	case 1:
		hint("STAGE 1", "・ルール\n2種類のブロックが上から落ちて来ます\n同じ種類のブロックを\n横一列に並べてね\n・種類の混ざった列をつくると\n  その列は消せなくなるよ\n・だんだん速くなるから気をつけてね\n\n何かキーで続ける");
		hint("STAGE 1", "・操作\n1～0:ブロックをその場所に移動\n←→ : ブロックを左右に移動\nShift : 高速落下\n何かキーで始める");
		break;
	case 2:
		hint("STAGE 2", "STAGE 2からは新しいブロック\n▲が登場します。\n\n\n\n何かキーを押してね");
		break;
	case 3:
		hint("STAGE 3", "STAGE 3 からは◆が増えます。\n\n\n\n\n何かキーを押してね");
	}


	speed = DEFAULTSPEED;
	SCORE = 0;
	int fallingx = rand() % 10, fallingy = 0;
	int fallwait = speed;
	char fallingtype = 1, nextfalltype = 1;
	while (ProcessMessage() == 0 &&
		ClearDrawScreen() == 0 &&
		GetHitKeyStateAll_2(Key) == 0 ) {

		

		if (fallwait <= 0) {
			fallwait = speed;

			if (map[fallingx][fallingy + 1] == 0 && fallingy < 19)
				fallingy++;
			else {
				map[fallingx][fallingy] = fallingtype;
				fallingtype = nextfalltype;
				nextfalltype = rand()%(stg+1)+1;///////////////////////////////////////
				fallingx = rand() % 10;
				fallingy = 0;
			}


		}
		else {
			fallwait--;
			if (Key[KEY_INPUT_LSHIFT]|| Key[KEY_INPUT_RSHIFT])fallwait-=4;
		}


		for (int i = 0; i < 10; i++) if (Key[i + 2] == 1 && map[i][fallingy] == 0) fallingx = i;//キー入力はまとめちゃおうね(`・ω・´)
		if ((Key[KEY_INPUT_RIGHT] == 1 || Key[KEY_INPUT_D] == 1) && fallingx < 9 && map[fallingx + 1][fallingy] == 0) fallingx++;
		if ((Key[KEY_INPUT_LEFT ] == 1 || Key[KEY_INPUT_A] == 1) && fallingx > 0 && map[fallingx - 1][fallingy] == 0) fallingx--;




		//DRAW KoKoKaRa!===============================================================
		DrawBox(20, 20, 220, 420, -1, 0);
		for (int i = 0; i < 10; i++) {
			DrawFormatStringToHandle(25 + i * 20, 430, -1, NormalFont, "%c", "1234567890"[i]);
		}

		DrawFormatStringToHandle(260, 150, GetColor(255, 255, 255), NormalFont, "SCORE: %d", SCORE);
		DrawFormatStringToHandle(260, 200, GetColor(255, 255, 255), NormalFont, "NEXT : %s", 
			(nextfalltype == 1) ? "■" : 
			(nextfalltype == 2) ? "●" : 
			(nextfalltype == 3) ? "▲" : "◆");

		DrawFormatStringToHandle(270, 10, GetColor(255, 255, 255), NormalFont, "STAGE %d", stg);

		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 20; j++) {
				switch (map[i][j]) {
				case 1:
					DrawBox(20 + i * 20, 20 + j * 20, 40 + i * 20, 40 + j * 20, GetColor(255, 0, 255), 1);
					break;
				case 2:
					DrawCircle(30 + i * 20, 30 + j * 20, 10, GetColor(0, 205, 255), 1);
					break;
				case 3:
					DrawTriangle(30 + i * 20, 20 + j * 20, 20 + i * 20, 40 + j * 20, 40 + i * 20, 40 + j * 20,GetColor(205,205,0),1);
					break;
				case 4:
					DrawTriangle(30 + i * 20, 20 + j * 20, 20 + i * 20, 30 + j * 20, 40 + i * 20, 30 + j * 20, GetColor(0, 155, 0), 1);
					DrawTriangle(30 + i * 20, 40 + j * 20, 20 + i * 20, 30 + j * 20, 40 + i * 20, 30 + j * 20, GetColor(0, 155, 0), 1);
					break;

				}
			}
		}

		switch (fallingtype) {
		case 1:
			DrawBox(20 + fallingx * 20, 20 + fallingy * 20, 40 + fallingx * 20, 40 + fallingy * 20, GetColor(255, 100, 255), 1);
			break;
		case 2:
			DrawCircle(30 + fallingx * 20, 30 + fallingy * 20, 10, GetColor(100, 255, 255), 1);
			break;
		case 3:
			DrawTriangle(30 + fallingx * 20, 20 + fallingy * 20, 20 + fallingx * 20, 40 + fallingy * 20, 40 + fallingx * 20, 40 + fallingy * 20, GetColor(255, 255, 100), 1);
			break;
		case 4:
			DrawTriangle(30 + fallingx * 20, 20 + fallingy * 20, 20 + fallingx * 20, 30 + fallingy * 20, 40 + fallingx * 20, 30 + fallingy * 20, GetColor(100, 255, 100), 1);
			DrawTriangle(30 + fallingx * 20, 40 + fallingy * 20, 20 + fallingx * 20, 30 + fallingy * 20, 40 + fallingx * 20, 30 + fallingy * 20, GetColor(100, 255, 100), 1);
			break;
		}
		//Draw KoKoMaDe!=================================================================

		if (chk_line()) {
			DrawBox(100, 100, 540, 380, GetColor(100, 100, 100), 1);
			DrawStringToHandle(150, 200, "GAME OVER", GetColor(255, 0, 0), ExLargeFont);
			wait(6);
			wait_and_flip();
			WaitKey();
			wait(10);
			break;
		}



		wait_and_flip();
	}
	return 0;
}

int chk_line() {
	bool flag1,flag2,flag3,flag4;

	for (int i = 0; i < 10; i++) {
		if (map[i][0] != 0)return 1;
	}

	for (int j = 1; j < 20; j++) {
		flag1 = flag2 = flag3 = flag4 = true;
		for (int i = 0; i < 10; i++) {
			if (map[i][j] != 1)flag1 = false;
			if (map[i][j] != 2)flag2 = false;
			if (map[i][j] != 3)flag3 = false;
			if (map[i][j] != 4)flag4 = false;


		}

		if (flag1 || flag2 || flag3 || flag4 ) {
			if (!(rand() % 100) && speed>2)speed--;//speed up!]
			SCORE += 100;
			for (int jj = j; jj > 0; jj--) {
				for (int i = 0; i < 10; i++) {
					map[i][jj] = map[i][jj - 1];
				}
			}
			for (int i = 0; i < 10; i++) map[i][0] = 0;
		}

	}
	return 0;
}

int hint(const TCHAR *Title, const TCHAR *String) {
	DrawBox(140, 10, 500, 250, GetColor(50, 50, 50), 1);
	DrawStringToHandle(150, 15, Title, -1, LargeFont);
	DrawStringToHandle(150, 40, String, -1, NormalFont);
	wait_and_flip();
	WaitKey();
	return 0;
}



void wait_and_flip() {
	int time;
	static int t = 0;
	time = GetNowCount() - t;
	if (16 - time>0)
		Sleep(16 - time);
	t = GetNowCount();

	ScreenFlip();
	return;
}
void wait(int a) { for (; a; a--)wait_and_flip(); }

