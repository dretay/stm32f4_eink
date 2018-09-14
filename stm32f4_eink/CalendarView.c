#include "CalendarView.h"

static font_t DejaVuSans10, DejaVuSans12, DejaVuSans12Bold, DejaVuSans16, DejaVuSans20, DejaVuSans24, DejaVuSans32, Fixed_5x8, Fixed_7x14;
static coord_t DisplayWidth, DisplayHeight, DisplayWidthMidpoint, DisplayHeightMidpoint, DateHeaderHeight, WeatherHeaderHeight, MeetingsHeight;

static void render_date_header()
{
	char *day_of_week = DateHelper.get_day_of_week();
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
	
	int year_right_offset = 50;
	int year_top_offset = 10;
	char year[5];
	sprintf(year, "%d", DateHelper.get_year());
	font_t year_font = DejaVuSans20;
	coord_t year_width = gdispGetStringWidth(year, year_font) + 1;
	coord_t year_height = gdispGetFontMetric(year_font, fontHeight) + 1;
	gdispDrawStringBox(
		(DisplayWidth - (year_width / 2))-year_right_offset,
		day_of_week_height/2-year_top_offset, 
		year_width, 
		year_height, 
		year, 
		year_font, 
		Black, 
		justifyCenter);	
	
	int month_left_offset = 10;
	int month_top_offset = 10;
	char* month = DateHelper.get_month();
	font_t month_font = DejaVuSans20;
	coord_t month_width = gdispGetStringWidth(month, month_font) + 1;
	coord_t month_height = gdispGetFontMetric(month_font, fontHeight) + 2;
	gdispDrawStringBox(
		month_left_offset, 
		day_of_week_height/2-month_top_offset, 
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
	coord_t header_height = gdispGetFontMetric(header_font, fontHeight) + 2;


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


static void render_weather(Weather *weathers, int weather_cnt)
{	
	int weather_img_width = 48;
	int weather_img_height = 48;
	int weather_spacing = 73;
	int weather_height_offset = 26;
	int weather_offset = 20;
	
	int weathers_to_display = 5;
	int start = 0;
	char weather_icon_map[][22] = { 
		"chanceofstorm.bmp",
		"clouds.bmp",
		"fogday.bmp",
		"fognight.bmp",
		"littlerain.bmp",
		"littlesnow.bmp",
		"moon.bmp",
		"partlycloudyday.bmp",
		"partlycloudynight.bmp",
		"rain.bmp",
		"sleet.bmp",
		"snow.bmp",
		"storm.bmp"
	};	
	time_t now_epoch =  DateHelper.get_epoch();

	for (start = 0; start < weather_cnt; start++)
	{
		if (!(now_epoch > weathers[start].start))
		{
			break;
		}
	}


	for (int i = start; i < MIN(start + weathers_to_display, weather_cnt); i++)
	{
		font_t weather_time_font = DejaVuSans12Bold;
		int weather_time_width = gdispGetStringWidth(weathers[i].human_start, weather_time_font) + 1;
		int weather_time_height = gdispGetFontMetric(weather_time_font, fontHeight) + 1;
		
		gdispDrawStringBox(
			(i * weather_spacing) + (strlen(weathers[i].human_start) == 4 ? 11 : 5) +(weather_time_width / 2), 
			DateHeaderHeight+weather_height_offset, 
			weather_time_width, 
			weather_time_height, 
			weathers[i].human_start, 
			weather_time_font, 
			Black, 
			justifyCenter);

		gdispImage weather_img;
		gdispImageOpenFile(&weather_img, weather_icon_map[weathers[i].type]);
		gdispImageDraw(&weather_img, (i * weather_spacing) + weather_offset, DateHeaderHeight + weather_time_height + weather_height_offset, weather_img_width, weather_img_height, 0, 0);
		gdispImageClose(&weather_img);
		
		char temperature_str[4];
		sprintf(temperature_str, "%ld", weathers[i].temperature);
		font_t weather_temp_font = DejaVuSans12;
		int weather_temp_width = gdispGetStringWidth(temperature_str, weather_temp_font) + 1;
		int weather_temp_height = gdispGetFontMetric(weather_temp_font, fontHeight) + 1;		
		int weather_temp_right_padding = 5;
		int weather_temp_top_offset = 25;
		gdispDrawStringBox(
			((i * weather_spacing) + weather_offset)  +(weather_temp_width / 2)+weather_temp_right_padding, 
			DateHeaderHeight+weather_temp_height+weather_img_height+weather_temp_top_offset, 
			weather_temp_width, 
			weather_temp_height, 
			temperature_str, 
			weather_temp_font, 
			Black, 
			justifyCenter);
		int weather_degree_top_offset = 28;
		int weather_degree_left_padding = 7;
		gdispDrawCircle(
			((i * weather_spacing) + weather_offset)  + (weather_temp_width / 2) + weather_temp_width + weather_degree_left_padding, 
			DateHeaderHeight + weather_temp_height + weather_img_height + weather_degree_top_offset, 
			2, 
			Black);
		
		WeatherHeaderHeight = DateHeaderHeight + weather_img_height + weather_time_height;
	}
	WeatherHeaderHeight += 10;
	static const char header[12] = "Weather";
	render_container(0, DateHeaderHeight, 133, &header);
}
static void render_next_meetings(Meeting *meetings, int meeting_count)
{
	int cell_spacing = 0;
	int meetings_offset = 20;
	int meetings_starting_offset = 133;

	int meetings_to_display = 6;
	int start = 0;
	time_t now_epoch =  DateHelper.get_epoch();

	for (start = 0; start < meeting_count; start++)
	{
		if (!(now_epoch > meetings[start].start))
		{
			break;
		}
	}
	
	for (int i = start; i < MIN(start + meetings_to_display, meeting_count); i++)
	{
		font_t next_meeting_start_font = DejaVuSans16;
		int next_meeting_start_width = gdispGetStringWidth(meetings[i].human_start, next_meeting_start_font) + 1;
		int next_meeting_start_height = gdispGetFontMetric(next_meeting_start_font, fontHeight) + 1;
		gdispDrawStringBox(
			10, 
			(meetings_offset * 2)+((i-start)*cell_spacing)+meetings_starting_offset+15, 
			next_meeting_start_width, 
			next_meeting_start_height, 
			meetings[i].human_start, 
			next_meeting_start_font, 
			Black, 
			justifyCenter);

		font_t next_meeting_end_font = DejaVuSans16;
		int next_meeting_end_width = gdispGetStringWidth(meetings[i].human_end, next_meeting_end_font) + 1;
		int next_meeting_end_height = gdispGetFontMetric(next_meeting_end_font, fontHeight) + 1;
		gdispDrawStringBox(
			10, 
			(meetings_offset * 2)+((i - start) * cell_spacing)+meetings_starting_offset+next_meeting_start_height+15, 
			next_meeting_end_width, 
			next_meeting_end_height, 
			meetings[i].human_end, 
			next_meeting_end_font, 
			Black, 
			justifyCenter);

		font_t next_meeting_summary_font = DejaVuSans20;
		int next_meeting_summary_width = gdispGetStringWidth(meetings[i].title, next_meeting_summary_font) + 1;
		int next_meeting_summary_height = gdispGetFontMetric(next_meeting_summary_font, fontHeight) + 1;
		gdispDrawStringBox(
			75, 
			(meetings_offset * 2)+((i - start) * cell_spacing)+meetings_starting_offset+10, 
			next_meeting_summary_width, 
			next_meeting_summary_height, 
			meetings[i].title, 
			next_meeting_summary_font, 
			Black, 
			justifyCenter);

		font_t next_meeting_time_font = DejaVuSans16;
		int next_meeting_time_width = gdispGetStringWidth(meetings[i].room, next_meeting_time_font) + 1;
		int next_meeting_time_height = gdispGetFontMetric(next_meeting_time_font, fontHeight) + 1;
		gdispDrawStringBox(
			70, 
			(meetings_offset * 2)+((i - start) * cell_spacing)+meetings_starting_offset+next_meeting_summary_height+10, 
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
	render_container(0, meetings_starting_offset + meetings_offset, 449, &header);
}
static void render_todo(Todo *todos, int todo_count)
{
	int todo_offset = 469;
	int TodoHeight = 0;
	int offset = 10;
	int todos_to_display = 5;
	
	for (int i = 0; i < MIN(todo_count, todos_to_display); i++)	
	{
		gdispFillCircle(30, todo_offset + (i*TodoHeight) + 30 + offset, 7, Black);		
		font_t todo_item_font = DejaVuSans12;
		int todo_item_width = gdispGetStringWidth(todos[i].title, todo_item_font) + 1;
		int todo_item_height = gdispGetFontMetric(todo_item_font, fontHeight) + 1;
		gdispDrawStringBox(
			45,
			(i * TodoHeight)+todo_offset +23+offset, 
			todo_item_width, 
			todo_item_height+2, 
			todos[i].title, 
			todo_item_font, 
			Black, 
			justifyCenter);	
		TodoHeight = todo_item_height + 10;
	}
	static const char header[12] = "Todo";
	render_container(0, todo_offset, 628, &header);

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
static void draw(Meeting *meetings, int meeting_count, Todo *todos, int todo_cnt, Weather *weathers, int weather_cnt)
{
	gdispClear(White);
	render_date_header();
	render_weather(weathers, weather_cnt);
	render_next_meetings(meetings,meeting_count);
	render_todo(todos,todo_cnt);

	gdispGFlush(gdispGetDisplay(0));	
}
const struct calendarview CalendarView= { 
	.init = init,
	.draw = draw,
};
	
	