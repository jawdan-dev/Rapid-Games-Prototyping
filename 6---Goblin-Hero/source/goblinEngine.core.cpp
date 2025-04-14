#include "goblinEngine.core.hpp"

void CORE_INITIALIZE() {
	static bool initialized = false;
	if (initialized)
		return;
	initialized = true;

	videoSetMode(MODE_5_3D);
	videoSetModeSub(MODE_5_2D);

	vramSetBankA(VRAM_A_TEXTURE);
	vramSetBankB(VRAM_B_TEXTURE);
	vramSetBankE(VRAM_E_TEX_PALETTE);

	{
		oamInit(&oamSub, SpriteMapping_Bmp_2D_256, false);
		int x = 0, y = 0, id = 0;
		// set up a 4x3 grid of 64x64 sprites to cover the screen
		for (y = 0; y < 3; y++)
			for (x = 0; x < 4; x++) {
				oamSub.oamMemory[id].attribute[0] = ATTR0_BMP | ATTR0_SQUARE | (64 * y);
				oamSub.oamMemory[id].attribute[1] = ATTR1_SIZE_64 | (64 * x);
				oamSub.oamMemory[id].attribute[2] = ATTR2_ALPHA(1) | (8 * 32 * y) | (8 * x);
				id++;
			}
		swiWaitForVBlank();
		oamUpdate(&oamSub);

		bgInitSub(3, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
	}

	glScreen2D();
}

void CORE_ERROR(const char* msg...) {
	consoleDemoInit();
	consoleClear();

	char buffer[256];
	va_list args;
	va_start(args, msg);
	vsprintf(buffer, msg, args);
	va_end(args);

	iprintf("\nPress any to exit.\nERROR LOG:\n\n%s", buffer);

	while (pmMainLoop()) {
		scanKeys();
		if (keysDown())
			break;
		swiWaitForVBlank();
	}

	exit(1);
}