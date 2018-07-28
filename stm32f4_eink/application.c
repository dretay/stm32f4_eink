#include "application.h"

void run(void)
{
	gfxInit();
	char* input = "drew, drew!";
	font_t font;
	coord_t height, width, rx, ry, rcx, rcy;
	width = gdispGetWidth();
	font = gdispOpenFont("DEJAVUSANS32");
	gdispClear(White);
	gdispDrawStringBox(0, 0, width, 100, input, font, Black, justifyCenter);
	gdispGFlush(gdispGetDisplay(0));
	
	gdispDrawStringBox(0, 100, width, 100, input, font, Black, justifyCenter);
	gdispGFlush(gdispGetDisplay(0));

	gdispDrawStringBox(0, 200, width, 100, input, font, Black, justifyCenter);
	gdispGFlush(gdispGetDisplay(0));

}