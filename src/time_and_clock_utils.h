/**
 * @author Batto1
 * @brief  This library presents some time utility routines as well as time representations and calculations in 'clock' format.
 * @note   Use zephyr\include\zephyr\sys\time_units.h for conversion between units i.e. "ticks to ms", "hw cycles to ns".
 * @note   If HW cycle is used, choose 32 bit or 64 bit appropriate to your system. Trying the basic functionality of the functions before using it in your application is recommended.
*/

#ifndef TIME_AND_CLOCK_UTILS_H
#define TIME_AND_CLOCK_UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <zephyr/kernel.h>
#include <stdint.h>

#define CLOCK_LEGEND_MAX_STRING_SIZE 16

/** 
 * @brief enum for encoding errors
*/
typedef enum TimeAndClockErrors
{
	TIME_UTIL_ERROR_NONE = 0,
	TIME_UTIL_ERROR_NEGATIVE = 1, /* if one of the values related to time or clock is negative while it mustn't be while making calculations */
}TimeAndClockErrors;

/**
 * @brief struct type that will hold different units of time variables.
*/
typedef struct timeElapsedClock{
	uint32_t 	day;
	uint16_t  u_sec;
	uint16_t  m_sec;
	uint8_t 	sec;
	uint8_t 	min;
	uint8_t	hour;
}TimeElapsedClock;

/**
 * @brief struct type that will hold all the units of time variables.
*/
typedef struct timeCategories{
	int64_t  u_secs;
	int64_t  m_secs;
	int64_t 	secs;
	int64_t 	mins;
	int64_t	hours;
	int64_t 	days;
	int64_t 	weeks;
	int64_t 	months;
	int64_t 	years;
	int64_t 	milleniums;
	int64_t 	centuries;
}TimeCategories;


/* ------------------  CONVERSION UTILS ------------------ */
uint64_t Micro_Sec_To_Milli_Sec(uint64_t a_micro_sec);
uint64_t Micro_Sec_To_Sec(uint64_t a_micro_sec);


/* ------------------  CLOCK ------------------ */
TimeAndClockErrors Clock_Subtract_Two_Time_Points(TimeElapsedClock * a_result_buf, TimeElapsedClock a_final_time, TimeElapsedClock a_initial_time);
TimeAndClockErrors Clock_Sum_Two_Time_Points(TimeElapsedClock * a_result_buf, TimeElapsedClock a_first_time, TimeElapsedClock a_second_time);
void Increment_a_Millisecond_And_Update_Clock(TimeElapsedClock * a_clock);
void Increment_a_Second_And_Update_Clock(TimeElapsedClock * a_clock);
void Clock_To_Str(char * a_clk_buf, char * a_legend_buf, TimeElapsedClock * a_clock, bool a_print_usec, bool a_print_msec, bool a_print_sec, bool a_print_min, bool a_print_hour, bool a_print_day);



/* ------------------  TICKS ------------------ */
int Get_Ticks_Per_Sec(void);
int Print_Ticks_Per_Sec(void);


uint64_t Ticks_To_Milliseconds(uint64_t a_ticks);
uint64_t Ticks_To_Seconds(uint64_t a_ticks);
int64_t Get_Uptime_Ticks();
uint64_t Get_Uptime_Ticks_As_Milliseconds(void);
uint64_t Get_Uptime_Ticks_As_Seconds(void);
TimeElapsedClock Ticks_To_Clock_Time(int64_t ticks);
TimeElapsedClock Get_Uptime_Ticks_As_Clock_Time(void);


/* ------------------  HW_CYCLES ------------------ */
int Get_HW_Cycles_Per_Sec(void);
void Print_HW_Cycles_Per_Sec(void);

/* ---  32 BIT HW_CYCLES --- */
uint32_t HW_Cycles_To_Milliseconds_32(uint32_t a_cycles);
uint32_t HW_Cycles_To_Seconds_32(uint32_t a_cycles);
uint32_t Get_Uptime_HW_Cycles_32();
uint32_t Get_Uptime_HW_Cycles_As_Milliseconds_32(void);
uint32_t Get_Uptime_HW_Cycles_As_Seconds_32(void);
TimeElapsedClock HW_Cycles_To_Clock_Time_32(uint32_t a_cycles);
TimeElapsedClock Get_Uptime_HW_Cycles_As_Clock_Time_32(void);

/* ---  64 BIT HW_CYCLES --- */
uint64_t HW_Cycles_To_Milliseconds_64(uint64_t a_cycles);
uint64_t HW_Cycles_To_Seconds_64(uint64_t a_cycles);
uint64_t Get_Uptime_HW_Cycles_64();
uint64_t Get_Uptime_HW_Cycles_As_Milliseconds_64(void);
uint64_t Get_Uptime_HW_Cycles_As_Seconds_64(void);
TimeElapsedClock HW_Cycles_To_Clock_Time_64(uint64_t a_cycles);
TimeElapsedClock Get_Uptime_HW_Cycles_As_Clock_Time_64(void);

/* ------------------  POWEREDUP TIME ------------------ */
void Accumulate_Time_Secs(uint64_t * a_secs_buff, uint64_t * a_prev_secs);
void Create_Poweredup_Time_Secs(uint64_t * a_secs_buff);
TimeAndClockErrors Accumulate_Time_Clk(TimeElapsedClock * a_clk, TimeElapsedClock * a_previous_clk);
TimeAndClockErrors Create_Poweredup_Time_Clk(TimeElapsedClock * a_clk);



#ifdef __cplusplus
}
#endif


#endif