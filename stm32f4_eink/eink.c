#include "eink.h"

static font_t DejaVuSans10, DejaVuSans12, DejaVuSans12Bold, DejaVuSans16, DejaVuSans20, DejaVuSans24, DejaVuSans32, Fixed_5x8, Fixed_7x14;
static coord_t DisplayWidth, DisplayHeight, DisplayWidthMidpoint, DisplayHeightMidpoint, DateHeaderHeight, WeatherHeaderHeight, MeetingsHeight;

static void render_date_header()
{
	static const char day_of_week[9] = "Friday";
	font_t day_of_week_font = DejaVuSans32;
	coord_t day_of_week_width = gdispGetStringWidth(day_of_week, day_of_week_font) + 1;
	coord_t day_of_week_height = gdispGetFontMetric(day_of_week_font, fontHeight) + 1;
	gdispDrawStringBox(
		(DisplayWidthMidpoint - (day_of_week_width / 2)), 
		0, 
		day_of_week_width, 
		day_of_week_height, 
		day_of_week, 
		day_of_week_font, 
		Black, 
		justifyCenter);
	
	static const char time_of_day[9] = "9:03PM";
	font_t time_of_day_font = DejaVuSans20;
	coord_t time_of_day_width = gdispGetStringWidth(time_of_day, time_of_day_font) + 1;
	coord_t time_of_day_height = gdispGetFontMetric(time_of_day_font, fontHeight) + 1;
	gdispDrawStringBox(
		(DisplayWidth - (time_of_day_width / 2))-50,
		//todo this 50 needs to be better defiend 
		day_of_week_height/2-10, 
		time_of_day_width, 
		time_of_day_height, 
		time_of_day, 
		time_of_day_font, 
		Black, 
		justifyCenter);
	
	static const char month[11] = "Aug. 13th";
	font_t month_font = DejaVuSans20;
	coord_t month_width = gdispGetStringWidth(month, month_font) + 1;
	coord_t month_height = gdispGetFontMetric(month_font, fontHeight) + 2;
	gdispDrawStringBox(
		10, 
		day_of_week_height/2-10, 
		month_width, 
		month_height, 
		month, 
		month_font, 
		Black, 
		justifyCenter);
	
	
	DateHeaderHeight = day_of_week_height;


}
static void render_container(coord_t x, coord_t y, coord_t height, char* header)
{
	coord_t arc_coord_x = 25;		
	font_t header_font = DejaVuSans12Bold;
	coord_t header_width = gdispGetStringWidth(header, header_font) + 1;
	coord_t header_height_offset = 10;
	coord_t header_height = gdispGetFontMetric(header_font, fontHeight) + 1;


	gdispDrawBox(x, y + header_height_offset, DisplayWidth, height - y, Black);
	gdispFillArc(arc_coord_x, y + header_height_offset, header_height / 2 + 4, 90, 270, Black);
	gdispFillArea(
		arc_coord_x,
		y+header_height_offset-(header_height / 2)-4,
		header_width,
		header_height+9,
		Black);
	gdispFillArc(arc_coord_x + header_width, y + header_height_offset, header_height / 2 + 4, 270, 90, Black);
	gdispDrawStringBox(
		arc_coord_x, 
		y+header_height_offset-(header_height / 2)+1, 
		header_width, 
		header_height, 
		header, 
		header_font, 
		White, 
		justifyCenter);
}
static void render_next_meetings(Meeting *meetings, int meeting_count)
{
	coord_t cell_spacing = 0;
	coord_t meetings_offset = 20;
	coord_t meetings_starting_offset = 123;
	
	for (int i = 0; i < meeting_count; i++)
	{
		font_t next_meeting_start_font = DejaVuSans16;
		coord_t next_meeting_start_width = gdispGetStringWidth(meetings[i].start, next_meeting_start_font) + 1;
		coord_t next_meeting_start_height = gdispGetFontMetric(next_meeting_start_font, fontHeight) + 1;
		gdispDrawStringBox(
			10, 
			(meetings_offset * 2)+(i*cell_spacing)+meetings_starting_offset+15, 
			next_meeting_start_width, 
			next_meeting_start_height, 
			meetings[i].start, 
			next_meeting_start_font, 
			Black, 
			justifyCenter);

		font_t next_meeting_end_font = DejaVuSans16;
		coord_t next_meeting_end_width = gdispGetStringWidth(meetings[i].end, next_meeting_end_font) + 1;
		coord_t next_meeting_end_height = gdispGetFontMetric(next_meeting_end_font, fontHeight) + 1;
		gdispDrawStringBox(
			10, 
			(meetings_offset * 2)+(i * cell_spacing)+meetings_starting_offset+next_meeting_start_height+15, 
			next_meeting_end_width, 
			next_meeting_end_height, 
			meetings[i].end, 
			next_meeting_end_font, 
			Black, 
			justifyCenter);

		font_t next_meeting_summary_font = DejaVuSans20;
		coord_t next_meeting_summary_width = gdispGetStringWidth(meetings[i].title, next_meeting_summary_font) + 1;
		coord_t next_meeting_summary_height = gdispGetFontMetric(next_meeting_summary_font, fontHeight) + 1;
		gdispDrawStringBox(
			next_meeting_start_width+20, 
			(meetings_offset * 2)+(i * cell_spacing)+meetings_starting_offset+10, 
			next_meeting_summary_width, 
			next_meeting_summary_height, 
			meetings[i].title, 
			next_meeting_summary_font, 
			Black, 
			justifyCenter);

		font_t next_meeting_time_font = DejaVuSans16;
		coord_t next_meeting_time_width = gdispGetStringWidth(meetings[i].room, next_meeting_time_font) + 1;
		coord_t next_meeting_time_height = gdispGetFontMetric(next_meeting_time_font, fontHeight) + 1;
		gdispDrawStringBox(
			next_meeting_start_width+20, 
			(meetings_offset * 2)+(i * cell_spacing)+meetings_starting_offset+next_meeting_summary_height+10, 
			next_meeting_time_width, 
			next_meeting_time_height, 
			meetings[i].room, 
			next_meeting_time_font, 
			Black, 
			justifyCenter);
		cell_spacing = next_meeting_summary_height + next_meeting_time_height + 10;
		MeetingsHeight += cell_spacing;
	}	
	static const char header[12] = "Meetings";
	render_container(0, meetings_starting_offset + meetings_offset, 439, &header);
}
static void render_todo()
{
	coord_t todo_offset = 459;
	coord_t TodoHeight = 0;
	coord_t offset = 10;
	for (int i = 0; i < 6; i++)
	{
		gdispFillCircle(30, todo_offset + (i*TodoHeight) + 30 + offset, 7, Black);
		static const char todo_item[70] = "Be the change you wish to see in the world";
		font_t todo_item_font = DejaVuSans12;
		coord_t todo_item_width = gdispGetStringWidth(todo_item, todo_item_font) + 1;
		coord_t todo_item_height = gdispGetFontMetric(todo_item_font, fontHeight) + 1;
		gdispDrawStringBox(
			45,
			(i * TodoHeight)+todo_offset +23+offset, 
			todo_item_width, 
			todo_item_height+2, 
			todo_item, 
			todo_item_font, 
			Black, 
			justifyCenter);	
		TodoHeight = todo_item_height + 10;
	}
	static const char header[12] = "Todo";
	render_container(0, todo_offset, 628, &header);

}

static void render_weather()
{	
	coord_t weather_img_width = 48;
	coord_t weather_img_height = 48;
	coord_t weather_spacing = 73;
	coord_t weather_height_offset = 26;
	coord_t weather_offset = 20;
	
	for (int i = 0; i < 5; i++)
	{
		gdispImage weather_img;
		gdispImageOpenFile(&weather_img, "partlycloudyday.bmp");
		gdispImageDraw(&weather_img, (i * weather_spacing) + weather_offset, DateHeaderHeight + weather_height_offset, weather_img_width, weather_img_height, 0, 0);
		gdispImageClose(&weather_img);
	
		char weather_time[5] = "9am";
		sprintf(weather_time, "%dam", (i + 7));
		font_t weather_time_font = DejaVuSans12Bold;
		coord_t weather_time_width = gdispGetStringWidth(weather_time, weather_time_font) + 1;
		coord_t weather_time_height = gdispGetFontMetric(weather_time_font, fontHeight) + 1;
		coord_t weather_time_offset = weather_img_width / 2;
		gdispDrawStringBox(
			((i * weather_spacing) + weather_offset) - (strlen(weather_time) == 3 ? 7 : 15) +(weather_time_width / 2), 
			DateHeaderHeight+weather_img_height+25, 
			weather_time_width, 
			weather_time_height, 
			weather_time, 
			weather_time_font, 
			Black, 
			justifyCenter);
		
		WeatherHeaderHeight = DateHeaderHeight + weather_img_height + weather_time_height;
	}
	WeatherHeaderHeight += 10;
	static const char header[12] = "Weather";
	render_container(0, DateHeaderHeight, 123, &header);
}
static void init()
{
	gfxInit();
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
}
static void draw(Meeting *meetings, int meeting_count)
{
	gdispClear(White);
	render_date_header();
	render_weather();
	render_next_meetings(meetings,meeting_count);
	render_todo();

	gdispGFlush(gdispGetDisplay(0));	
}
const struct eink EInk = { 
	.init = init,
	.draw = draw,
};
	
	