#include "StatusView.h"

static View view;
static font_t DejaVuSans10, DejaVuSans12, DejaVuSans12Bold, DejaVuSans16, DejaVuSans20, DejaVuSans24, DejaVuSans32, Fixed_5x8, Fixed_7x14;
static coord_t DisplayWidth, DisplayHeight, DisplayWidthMidpoint, DisplayHeightMidpoint;


static void render()
{
	gdispGClear(gdispGetDisplay(0), White);
	
	char* msg;
	if (SystemState.status->has_message)
	{
		msg = SystemState.status->message;
	}
	font_t status_font = DejaVuSans32;
	coord_t width = gdispGetStringWidth(msg, status_font) + 1;
	coord_t height = gdispGetFontMetric(status_font, fontHeight) + 1;
	gdispDrawStringBox(
		(DisplayWidthMidpoint - (width / 2)), 
		260, 
		width, 
		height, 
		msg, 
		status_font, 
		Black, 
		justifyCenter);

//	RetrivalStatus_StatusType_START = 0,
//	RetrivalStatus_StatusType_UPDATE = 1,
//	RetrivalStatus_StatusType_FLUSH = 2,
//	RetrivalStatus_StatusType_FAIL = 3
	char status_icon_map[][22] = { 
		"info_160.bmp",
		"info_160.bmp",
		"info_160.bmp",
		"error_160.bmp",		
	};


	gdispImage info_img;
	gdispImageOpenFile(&info_img, status_icon_map[SystemState.status->status]);
	gdispImageDraw(&info_img, DisplayWidthMidpoint - (160 / 2), 100, DisplayWidthMidpoint + (160 / 2), 260, 0, 0);
	gdispImageClose(&info_img);

	gdispGFlush(gdispGetDisplay(0));	
}

static View* init()
{
	view.render = render;

	DejaVuSans10 =  gdispOpenFont("DejaVuSans10");
	DejaVuSans12 =  gdispOpenFont("DejaVuSans12");
	DejaVuSans12Bold =  gdispOpenFont("DejaVuSansBold12");
	DejaVuSans16 =  gdispOpenFont("DejaVuSans16");

	DejaVuSans20 = gdispOpenFont("DejaVuSans20");
	DejaVuSans24 = gdispOpenFont("DejaVuSans24");
	
	DejaVuSans32 = gdispOpenFont("DejaVuSans32");
	Fixed_5x8 = gdispOpenFont("fixed_5x8");
	Fixed_7x14 = gdispOpenFont("fixed_7x14");

	DisplayWidth = gdispGetWidth();
	DisplayWidthMidpoint = DisplayWidth / 2;
	DisplayHeight = gdispGetHeight();
	DisplayHeightMidpoint = DisplayHeight / 2;
	
	return &view;
}
const struct statusview StatusView = { 
	.init = init,
};