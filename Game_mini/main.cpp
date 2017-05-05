#include <DxLib.h>


int Key[256];

int GetHitKeyStateAll_2(int[]);


int main() {

	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1)
		return -1;


	SetDrawScreen(DX_SCREEN_BACK); //•`‰ææ‚ğ— ‰æ–Ê‚Éİ’è


	int Handle;     // ‰æ‘œŠi”[—pƒnƒ“ƒhƒ‹

	while (ProcessMessage() == 0 &&
		ClearDrawScreen() == 0 &&
		GetHitKeyStateAll_2(Key) == 0 &&
		Key[KEY_INPUT_ESCAPE] == 0) {


		ScreenFlip();

	}



	DxLib_End();
	return 0;

}

int GetHitKeyStateAll_2(int GetHitKeyStateAll_InputKey[]) {
	char GetHitKeyStateAll_Key[256];
	GetHitKeyStateAll(GetHitKeyStateAll_Key);
	for (int i = 0; i < 256; i++) {
		if (GetHitKeyStateAll_Key[i] == 1) GetHitKeyStateAll_InputKey[i]++;
		else                               GetHitKeyStateAll_InputKey[i] = 0;
	}
	return 0;
}