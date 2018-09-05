/* Automatically generated nanopb header */
/* Generated by nanopb-0.3.9 at Wed Sep 05 00:02:39 2018. */

#ifndef PB_UNION_PB_H_INCLUDED
#define PB_UNION_PB_H_INCLUDED
#include <pb.h>

/* @@protoc_insertion_point(includes) */
#if PB_PROTO_HEADER_VERSION != 30
#error Regenerate this file with the current version of nanopb generator.
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* Enum definitions */
typedef enum _Weather_WeatherType {
    Weather_WeatherType_CHANCEOFSTORM = 0,
    Weather_WeatherType_CLOUDS = 1,
    Weather_WeatherType_FOGDAY = 2,
    Weather_WeatherType_FOGNIGHT = 3,
    Weather_WeatherType_LITTLERAIN = 4,
    Weather_WeatherType_LITTLESNOW = 5,
    Weather_WeatherType_MOON = 6,
    Weather_WeatherType_PARTLYCLOUDYDAY = 7,
    Weather_WeatherType_PARTLYCLOUDYNIGHT = 8,
    Weather_WeatherType_RAIN = 9,
    Weather_WeatherType_SLEET = 10,
    Weather_WeatherType_SNOW = 11,
    Weather_WeatherType_STORM = 12
} Weather_WeatherType;
#define _Weather_WeatherType_MIN Weather_WeatherType_CHANCEOFSTORM
#define _Weather_WeatherType_MAX Weather_WeatherType_STORM
#define _Weather_WeatherType_ARRAYSIZE ((Weather_WeatherType)(Weather_WeatherType_STORM+1))

typedef enum _RetrivalStatus_StatusType {
    RetrivalStatus_StatusType_START = 0,
    RetrivalStatus_StatusType_UPDATE = 1,
    RetrivalStatus_StatusType_FLUSH = 2,
    RetrivalStatus_StatusType_FAIL = 3
} RetrivalStatus_StatusType;
#define _RetrivalStatus_StatusType_MIN RetrivalStatus_StatusType_START
#define _RetrivalStatus_StatusType_MAX RetrivalStatus_StatusType_FAIL
#define _RetrivalStatus_StatusType_ARRAYSIZE ((RetrivalStatus_StatusType)(RetrivalStatus_StatusType_FAIL+1))

/* Struct definitions */
typedef struct _Header {
    uint32_t length;
/* @@protoc_insertion_point(struct:Header) */
} Header;

typedef struct _Meeting {
    uint32_t start;
    char human_start[9];
    char human_end[9];
    char title[64];
    char room[64];
    uint32_t idx;
/* @@protoc_insertion_point(struct:Meeting) */
} Meeting;

typedef struct _RetrivalStatus {
    RetrivalStatus_StatusType status;
    bool has_message;
    char message[32];
/* @@protoc_insertion_point(struct:RetrivalStatus) */
} RetrivalStatus;

typedef struct _Time {
    uint32_t year;
    uint32_t month;
    uint32_t date;
    uint32_t weekday;
    uint32_t hours;
    uint32_t minutes;
    uint32_t seconds;
    bool am;
/* @@protoc_insertion_point(struct:Time) */
} Time;

typedef struct _Todo {
    char title[64];
    bool has_status;
    bool status;
    uint32_t idx;
/* @@protoc_insertion_point(struct:Todo) */
} Todo;

typedef struct _Weather {
    Weather_WeatherType type;
    uint32_t start;
    char human_start[6];
    uint32_t idx;
    int32_t temperature;
/* @@protoc_insertion_point(struct:Weather) */
} Weather;

typedef struct _UnionMessage {
    bool has_state;
    RetrivalStatus state;
    bool has_meeting;
    Meeting meeting;
    bool has_todo;
    Todo todo;
    bool has_weather;
    Weather weather;
    bool has_time;
    Time time;
    bool has_header;
    Header header;
/* @@protoc_insertion_point(struct:UnionMessage) */
} UnionMessage;

/* Default values for struct fields */

/* Initializer values for message structs */
#define Weather_init_default                     {(Weather_WeatherType)0, 0, "", 0, 0}
#define Meeting_init_default                     {0, "", "", "", "", 0}
#define Todo_init_default                        {"", false, 0, 0}
#define RetrivalStatus_init_default              {(RetrivalStatus_StatusType)0, false, ""}
#define Time_init_default                        {0, 0, 0, 0, 0, 0, 0, 0}
#define Header_init_default                      {0}
#define UnionMessage_init_default                {false, RetrivalStatus_init_default, false, Meeting_init_default, false, Todo_init_default, false, Weather_init_default, false, Time_init_default, false, Header_init_default}
#define Weather_init_zero                        {(Weather_WeatherType)0, 0, "", 0, 0}
#define Meeting_init_zero                        {0, "", "", "", "", 0}
#define Todo_init_zero                           {"", false, 0, 0}
#define RetrivalStatus_init_zero                 {(RetrivalStatus_StatusType)0, false, ""}
#define Time_init_zero                           {0, 0, 0, 0, 0, 0, 0, 0}
#define Header_init_zero                         {0}
#define UnionMessage_init_zero                   {false, RetrivalStatus_init_zero, false, Meeting_init_zero, false, Todo_init_zero, false, Weather_init_zero, false, Time_init_zero, false, Header_init_zero}

/* Field tags (for use in manual encoding/decoding) */
#define Header_length_tag                        1
#define Meeting_start_tag                        1
#define Meeting_human_start_tag                  2
#define Meeting_human_end_tag                    3
#define Meeting_title_tag                        4
#define Meeting_room_tag                         5
#define Meeting_idx_tag                          6
#define RetrivalStatus_status_tag                1
#define RetrivalStatus_message_tag               2
#define Time_year_tag                            1
#define Time_month_tag                           2
#define Time_date_tag                            3
#define Time_weekday_tag                         4
#define Time_hours_tag                           5
#define Time_minutes_tag                         6
#define Time_seconds_tag                         7
#define Time_am_tag                              8
#define Todo_title_tag                           1
#define Todo_status_tag                          2
#define Todo_idx_tag                             3
#define Weather_type_tag                         1
#define Weather_start_tag                        2
#define Weather_human_start_tag                  3
#define Weather_idx_tag                          4
#define Weather_temperature_tag                  5
#define UnionMessage_state_tag                   1
#define UnionMessage_meeting_tag                 2
#define UnionMessage_todo_tag                    3
#define UnionMessage_weather_tag                 4
#define UnionMessage_time_tag                    5
#define UnionMessage_header_tag                  6

/* Struct field encoding specification for nanopb */
extern const pb_field_t Weather_fields[6];
extern const pb_field_t Meeting_fields[7];
extern const pb_field_t Todo_fields[4];
extern const pb_field_t RetrivalStatus_fields[3];
extern const pb_field_t Time_fields[9];
extern const pb_field_t Header_fields[2];
extern const pb_field_t UnionMessage_fields[7];

/* Maximum encoded size of messages (where known) */
#define Weather_size                             33
#define Meeting_size                             166
#define Todo_size                                74
#define RetrivalStatus_size                      36
#define Time_size                                44
#define Header_size                              6
#define UnionMessage_size                        372

/* Message IDs (where set with "msgid" option) */
#ifdef PB_MSGID

#define UNION_MESSAGES \


#endif

#ifdef __cplusplus
} /* extern "C" */
#endif
/* @@protoc_insertion_point(eof) */

#endif
