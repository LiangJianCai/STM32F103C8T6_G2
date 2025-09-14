#include "boardconfig.h"

void BOARD_Init()
{
	OLED_Init();
	OLED_Clear();
	menu_init();

}
