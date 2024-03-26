/**
 * @author Batto1
*/

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

#include "zephyr/kernel.h"
#include <zephyr/logging/log.h>

#include "time_and_clock_utils.h"


LOG_MODULE_REGISTER(time_and_clock_utils, LOG_LEVEL_DBG);



/* ------------------  CONVERSION UTILS ------------------ */
uint64_t Micro_Sec_To_Milli_Sec(uint64_t a_micro_sec)
{
	return (a_micro_sec / (uint64_t)1000);
}

uint64_t Micro_Sec_To_Sec(uint64_t a_micro_sec)
{
	return (a_micro_sec / (uint64_t)1000000);
}




/* ------------------  CLOCK ------------------ */
/**
 * @brief Takes two positive time values in clock format and subtracts them. Final time value needs to be bigger than the initial time. 
 * @param [out] a_result_buf 	A buffer that will hold the subtraction result.
 * @param [in]  a_final_time 	Minuend (the value that some other value will be subtracted from it)
 * @param [in]  a_initial_time Subtrahend (the value that will be subtracted)
 * @note 	Caution must be taken to make sure that the time value the a_final_time holds is larger than the a_initial_time holds. 
 * @retval one of the error values from enum TimeAndClockErrors
*/
TimeAndClockErrors Clock_Subtract_Two_Time_Points(TimeElapsedClock * a_result_buf, TimeElapsedClock a_final_time, TimeElapsedClock a_initial_time)
{	
	TimeAndClockErrors error = TIME_UTIL_ERROR_NONE;

	// define variables for the result
	int u_second   = 0;
	int m_second 	= 0;
	int second 	= 0;
	int minute 	= 0;
	int hour 		= 0;
	int day 		= 0;
	
	// define and initialize inital time variables
	int init_u_second   = (int)a_initial_time.u_sec;
	int init_m_second 	= (int)a_initial_time.m_sec;
	int init_second	= (int)a_initial_time.sec;
	int init_minute	= (int)a_initial_time.min;
	int init_hour		= (int)a_initial_time.hour;
	int init_day		= (int)a_initial_time.day;

	// define and initialize final time variable
	int final_u_second  = (int)a_final_time.u_sec;
	int final_m_second 	= (int)a_final_time.m_sec;
	int final_second 	= (int)a_final_time.sec;
	int final_minute 	= (int)a_final_time.min;
	int final_hour      = (int)a_final_time.hour;
	int final_day       = (int)a_final_time.day;
	

	u_second = final_u_second - init_u_second;
	if(u_second < 0){
		u_second += 1000;
		final_m_second--;
	}

	m_second = final_m_second - init_m_second;
	if(m_second < 0){
		m_second += 1000;
		final_second--;
	}

	second = final_second - init_second;
	if(second < 0){
		second += 60;
		final_minute--;
	}

	minute = final_minute - init_minute;
	if(minute < 0){
		minute += 60;
		final_hour--;
	}
		
	hour = final_hour - init_hour;
	if(hour < 0){
		hour += 24;
		final_day--;
	}

	day = final_day - init_day;

	if(u_second < 0 || m_second < 0 || second < 0 || minute < 0 || hour < 0 || day < 0){
		error = TIME_UTIL_ERROR_NEGATIVE;
	}

	a_result_buf->u_sec = (uint16_t) u_second;
	a_result_buf->m_sec = (uint16_t) m_second;
	a_result_buf->sec   = (uint8_t)  second;
	a_result_buf->min   = (uint8_t)  minute;
	a_result_buf->hour  = (uint8_t)  hour;
	a_result_buf->day   = (uint32_t) day;

	return error;
}

/**
 * @brief Takes two positive time values in clock format and sums them
 * @param [out] a_result_buf 	A buffer that will hold the summation result
 * @param [in] a_first_time 	First time value in clock format to be summed
 * @param [in] a_second_time 	Second time value in clock format to be summed
 * @retval one of the error values from enum TimeAndClockErrors
*/
TimeAndClockErrors Clock_Sum_Two_Time_Points(TimeElapsedClock * a_result_buf, TimeElapsedClock a_first_time, TimeElapsedClock a_second_time)
{
	TimeAndClockErrors error = TIME_UTIL_ERROR_NONE;

	// define variables for the result
	int u_second   = 0;
	int m_second 	= 0;
	int second 	= 0;
	int minute 	= 0;
	int hour 		= 0;
	int day 		= 0;
	
	// define and initialize inital time variables
	int first_u_second  = (int)a_first_time.u_sec;
	int first_m_second 	= (int)a_first_time.m_sec;
	int first_second	= (int)a_first_time.sec;
	int first_minute	= (int)a_first_time.min;
	int first_hour		= (int)a_first_time.hour;
	int first_day		= (int)a_first_time.day;

	// define and initialize final time variable
	int second_u_second  = (int)a_second_time.u_sec;
	int second_m_second  = (int)a_second_time.m_sec;
	int second_second 	 = (int)a_second_time.sec;
	int second_minute 	 = (int)a_second_time.min;
	int second_hour      = (int)a_second_time.hour;
	int second_day       = (int)a_second_time.day;

	u_second = first_u_second + second_u_second;
	if(u_second >= 1000){
		u_second -= 1000;
		first_m_second ++;
	}

	m_second = first_m_second + second_m_second;
	if(m_second >= 1000){
		m_second -= 1000;
		first_second ++;
	}

	second = first_second + second_second;
	if(second >= 60){
		second -= 60;
		first_minute ++;
	}

	minute = first_minute + second_minute;
	if(minute >= 60){
		minute -= 60;
		first_hour ++;
	}

	hour = first_hour + second_hour;
	if(hour >= 24){
		hour -= 24;
		first_day ++;
	}

	day = first_day + second_day;

	if(u_second < 0 || m_second < 0 || second < 0 || minute < 0 || hour < 0 || day < 0){
		error = TIME_UTIL_ERROR_NEGATIVE;
	}
	
	a_result_buf->u_sec = (uint16_t) u_second;
	a_result_buf->m_sec = (uint16_t) m_second;
	a_result_buf->sec   = (uint8_t)  second;
	a_result_buf->min   = (uint8_t)  minute;
	a_result_buf->hour  = (uint8_t)  hour;
	a_result_buf->day   = (uint32_t) day;

	return error;
}

void Increment_a_Millisecond_And_Update_Clock(TimeElapsedClock * a_clock)
{	
	a_clock->m_sec ++;
	if(a_clock->m_sec == 1000){
		a_clock->m_sec = 0;
		a_clock->sec ++;
	}
	if(a_clock->sec == 60){
		a_clock->sec = 0;
		a_clock->min ++;
	}
	if(a_clock->min == 60){
		a_clock->min = 0;
		a_clock->hour ++;
	}
	if(a_clock->hour == 24){
		a_clock->hour = 0;
		a_clock->day ++;
	}
}

void Increment_a_Second_And_Update_Clock(TimeElapsedClock * a_clock)
{
	a_clock->sec ++;
	if(a_clock->sec == 60){
		a_clock->sec = 0;
		a_clock->min ++;
	}
	if(a_clock->min == 60){
		a_clock->min = 0;
		a_clock->hour ++;
	}
	if(a_clock->hour == 24){
		a_clock->hour = 0;
		a_clock->day ++;
	}
}

/**
 * @brief Converts given clock value to a string.
 * @param [out] a_clk_buf 	User provided buffer for writing the clock value in. Buffer size is recommended to be at least 25 bytes.
 * @param [out] a_legend_buf  User provided buffer for writing the clock legend values as [d:h:m:s.ms,us]. Buffer size is recommended to be at least 16 bytes.
 * @param [in] a_clock 		Pointer to the clock variable that user provides the clock values in it.
 * @param [in] a_print_usec	Whether to include micro seconds in the clock string. True includes, false doesn't.
 * @param [in] a_print_msec	Whether to include milli seconds in the clock string. True includes, false doesn't.
 * @param [in] a_print_sec	Whether to include seconds in the clock string. True includes, false doesn't.
 * @param [in] a_print_min	Whether to include minutes in the clock string.True includes, false doesn't.
 * @param [in] a_print_hour	Whether to include hours in the clock string. True includes, false doesn't.
 * @param [in] a_print_day	Whether to include days in the clock string. True includes, false doesn't. 
 * @todo return values of functions should be checked and error coded
*/
void Clock_To_Str(char * a_clk_buf, char * a_legend_buf, TimeElapsedClock * a_clock, bool a_print_usec, bool a_print_msec, bool a_print_sec, bool a_print_min, bool a_print_hour, bool a_print_day)
{

	// if clk legend printing is enabled, print it according to what user chose to wrote.
	if(NULL != a_legend_buf){
		// make ready the clock legend buffer
		char clk_legend_buf[16]; 
		snprintk(clk_legend_buf, 16, "");
		strcat(clk_legend_buf,"[");
		if(a_print_day){
			strcat(clk_legend_buf,"d");
		}
		if(a_print_hour){
			strcat(clk_legend_buf, ":h");
		}
		if(a_print_min){
			strcat(clk_legend_buf, ":m");
		}
		if(a_print_sec){
			strcat(clk_legend_buf, ":s");
		}
		if(a_print_msec){
			strcat(clk_legend_buf, ".ms");
		}
		if(a_print_usec){
			strcat(clk_legend_buf, ",us");
		}
		strcat(clk_legend_buf,"]");
		// copy the string to user provided buffer.
		strcpy(a_legend_buf, clk_legend_buf); 
	}

	// make ready the clock values buffer
	char clk_values_buf[30]; 
	snprintk(clk_values_buf, 30, "");
	char temp[10]; // temp char that will hold individual clock value elements.

	strcat(clk_values_buf,"[");
	if(a_print_day){
		snprintk(temp, 10, "%"PRIu32, a_clock->day);
		strcat(clk_values_buf, temp);
	}
	if(a_print_hour){
		snprintk(temp, 10, ":%"PRIu8, a_clock->hour);
		strcat(clk_values_buf, temp);
	}
	if(a_print_min){
		snprintk(temp, 10, ":%"PRIu8, a_clock->min);
		strcat(clk_values_buf, temp);
	}
	if(a_print_sec){
		snprintk(temp, 10, ":%"PRIu8, a_clock->sec);
		strcat(clk_values_buf, temp);
	}
	if(a_print_msec){
		snprintk(temp, 10, ".%"PRIu16, a_clock->m_sec);
		strcat(clk_values_buf, temp);
	}
	if(a_print_usec){
		snprintk(temp, 10, ",%"PRIu16, a_clock->u_sec);
		strcat(clk_values_buf, temp);
	}
	strcat(clk_values_buf,"]");

	// copy the string to user provided buffer.
	strcpy(a_clk_buf, clk_values_buf); 
}





/* ------------------  TICKS ------------------ */

/**
 * @brief Get system uptime, in system ticks. This routine returns the elapsed time since the system booted, in ticks
 * @return Current uptime in ticks.
*/
int64_t Get_Uptime_Ticks()
{
	return k_uptime_ticks();
}

/**
 * @brief Convert ticks to milliseconds.
 * @note funcun düzgün çalışıp çalışmadığı daha test edilmedi
*/
uint64_t Ticks_To_Milliseconds(uint64_t a_ticks)
{
	return (k_ticks_to_ms_floor64(a_ticks));
}

/**
 * @brief Convert ticks to seconds. (zephyr\include\zephyr\sys\time_units.h has most of the conversions but not for seconds.)
 * @warning Throws away the fraction part after doing an integer division for conversion.
 * @note funcun düzgün çalışıp çalışmadığı daha test edilmedi
 * 
*/
uint64_t Ticks_To_Seconds(uint64_t a_ticks)
{
	return (k_ticks_to_ms_floor64(a_ticks) / (uint64_t)1000);
}


/**
 * @brief Get uptime ticks as milliseconds.
 * @note funcun düzgün çalışıp çalışmadığı daha test edilmedi
 * @note k_uptime_ticks() int64_t döndürüyor ancak içine yazıldığı k_ticks_to_us_floor64() hem uint64_t'yi hem argüman olarak alıyor hem de döndürüyor.
*/
uint64_t Get_Uptime_Ticks_As_Milliseconds(void)
{
	return k_ticks_to_ms_floor64( ((uint64_t)k_uptime_ticks()) );
}

/**
 * @brief Get uptime ticks as seconds. (zephyr\include\zephyr\sys\time_units.h has most of the conversions but not for seconds.)
 * @note funcun düzgün çalışıp çalışmadığı daha test edilmedi
 * @note k_uptime_ticks() int64_t döndürüyor ancak içine yazıldığı k_ticks_to_us_floor64() hem uint64_t'yi hem argüman olarak alıyor hem de döndürüyor.
*/
uint64_t Get_Uptime_Ticks_As_Seconds(void)
{
	return Ticks_To_Seconds( ((uint64_t)k_uptime_ticks()) );
}

/**
 * @brief Convert ticks to clock time.
 * @warning in some test code, getting clock data from these ticks caused around +100ms increase on clock time when everytime device was reset, comparing to a clock working with timer interrupts. Other than that it's pretty accurate. I haven't observed a single milliseconds changing inaccurately over some time.  
 * @return clock time of type TimeElapsedClock. 
*/
TimeElapsedClock Ticks_To_Clock_Time(int64_t ticks)
{
	TimeElapsedClock clk; // not inited because all of its elements will be set later anyways.
	
	uint64_t total_micro_sec = k_ticks_to_us_floor64((uint64_t)ticks); // func tanımına baktığıma göre k_ticks_to_us_floor64() ve k_ticks_to_ms_floor64() arasında fark yok.
	uint64_t total_milli_sec = total_micro_sec / (uint64_t)1000;
	uint64_t total_sec 		= total_milli_sec / (uint64_t)1000;
	uint64_t total_minutes	= total_sec 	   / (uint64_t)60;
	uint64_t total_hours 	= total_minutes   / (uint64_t)60;
	uint64_t total_days 	= total_hours 	   / (uint64_t)24;

	clk.u_sec = (uint16_t)(total_micro_sec % 1000);
	clk.m_sec = (uint16_t)(total_milli_sec % 1000);
	clk.sec 	= (uint8_t) (total_sec 	    % 60);
	clk.min 	= (uint8_t) (total_minutes   % 60);
	clk.hour  = (uint8_t) (total_hours     % 24);
	clk.day 	= (uint32_t)(total_days);

	return clk;
}

/**
 * @brief Get system uptime tracked by ticks in clock format.
 * @note funcun düzgün çalışıp çalışmadığı daha test edilmedi
 * 
*/
TimeElapsedClock Get_Uptime_Ticks_As_Clock_Time(void)
{
	return Ticks_To_Clock_Time(k_uptime_ticks());
}

/**
 * @brief Get CONFIG_SYS_CLOCK_TICKS_PER_SEC value.
 * @return CONFIG_SYS_CLOCK_TICKS_PER_SEC if no error.
 * @return -1 if error
*/
int Get_Ticks_Per_Sec(void)
{
#ifdef CONFIG_SYS_CLOCK_TICKS_PER_SEC
	return CONFIG_SYS_CLOCK_TICKS_PER_SEC;
#else
	return -1;
#endif
}

/**
 * @brief Print CONFIG_SYS_CLOCK_TICKS_PER_SEC value.
 * @return CONFIG_SYS_CLOCK_TICKS_PER_SEC if no error.
 * @return -1 if error
*/
int Print_Ticks_Per_Sec(void)
{
#ifdef CONFIG_SYS_CLOCK_TICKS_PER_SEC
	printk("* I: CONFIG_SYS_CLOCK_TICKS_PER_SEC = %d\n", CONFIG_SYS_CLOCK_TICKS_PER_SEC);
	return CONFIG_SYS_CLOCK_TICKS_PER_SEC;
#else
	return -1;
#endif
}



/* ------------------  HW_CYCLES ------------------ */

/**
 * @brief Get HW cycles per sec.
 * @note function uses sys_clock_hw_cycles_per_sec(); if CONFIG_TIMER_READS_ITS_FREQUENCY_AT_RUNTIME is defined this function returns what sys_clock_hw_cycles_per_sec_runtime_get() returns; if not defined, returns CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC macro value.
 * @return what returns from sys_clock_hw_cycles_per_sec();
*/
int Get_HW_Cycles_Per_Sec(void)
{
	return sys_clock_hw_cycles_per_sec();
}

/**
 * @brief Print HW cycles per sec.
 * @note function uses sys_clock_hw_cycles_per_sec(); if CONFIG_TIMER_READS_ITS_FREQUENCY_AT_RUNTIME is defined this function returns what sys_clock_hw_cycles_per_sec_runtime_get() returns; if not defined, returns CONFIG_SYS_CLOCK_HW_CYCLES_PER_SEC macro value.
*/
void Print_HW_Cycles_Per_Sec(void)
{
	printk("* I: HW cycles per second = %d\n", sys_clock_hw_cycles_per_sec());
}


/* ---  32 BIT HW_CYCLES --- */
/**
 * @brief Get system uptime, in system clock HW cycles. Read the hardware clock. This routine returns the current time, as measured by the system's hardware clock.
 * @note 32 bit.
 * @return Current hardware clock up-counter (in cycles).
*/
uint32_t Get_Uptime_HW_Cycles_32()
{
	return k_cycle_get_32();
}

/**
 * @brief Convert HW cycles to milliseconds.
 * @note 32 bit.
 * @note funcun düzgün çalışıp çalışmadığı daha test edilmedi.
*/
uint32_t HW_Cycles_To_Milliseconds_32(uint32_t a_cycles)
{
	return (k_cyc_to_ms_floor32(a_cycles));
}

/**
 * @brief Convert HW cycles to seconds. (zephyr\include\zephyr\sys\time_units.h has most of the conversions but not for seconds.)
 * @note 32 bit.
 * @warning Throws away the fraction part after doing an integer division for conversion.
 * @note funcun düzgün çalışıp çalışmadığı daha test edilmedi.
*/
uint32_t HW_Cycles_To_Seconds_32(uint32_t a_cycles)
{
	return (k_cyc_to_ms_floor32(a_cycles) / (uint32_t)1000);
}

/**
 * @brief Get uptime HW cycles as milliseconds.
 * @note 32 bit.
 * @note funcun düzgün çalışıp çalışmadığı daha test edilmedi.
*/
uint32_t Get_Uptime_HW_Cycles_As_Milliseconds_32(void)
{
	return k_cyc_to_ms_floor32(k_cycle_get_32());
}

/**
 * @brief Get uptime HW cycles as seconds. (zephyr\include\zephyr\sys\time_units.h has most of the conversions but not for seconds.)
 * @note 32 bit.
 * @note funcun düzgün çalışıp çalışmadığı daha test edilmedi
*/
uint32_t Get_Uptime_HW_Cycles_As_Seconds_32(void)
{
	return HW_Cycles_To_Seconds_32(k_cycle_get_32());
}

/**
 * @brief Convert HW cycles to clock time.
 * @return clock time of type TimeElapsedClock. 
*/
TimeElapsedClock HW_Cycles_To_Clock_Time_32(uint32_t a_cycles)
{
	TimeElapsedClock clk; // not inited because all of its elements will be set later anyways.
	
	uint32_t total_micro_sec = k_cyc_to_us_floor32(a_cycles); 
	uint32_t total_milli_sec = total_micro_sec / (uint32_t)1000;
	uint32_t total_sec 		= total_milli_sec / (uint32_t)1000;
	uint32_t total_minutes   = total_sec 	   / (uint32_t)60;
	uint32_t total_hours 	= total_minutes   / (uint32_t)60;
	uint32_t total_days 	= total_hours 	   / (uint32_t)24;

	clk.u_sec = (uint16_t)(total_micro_sec % 1000);
	clk.m_sec = (uint16_t)(total_milli_sec % 1000);
	clk.sec 	= (uint8_t) (total_sec 	    % 60);
	clk.min 	= (uint8_t) (total_minutes   % 60);
	clk.hour  = (uint8_t) (total_hours     % 24);
	clk.day 	= total_days;
	return clk;
}

/**
 * @brief Get system uptime tracked by HW cycles in clock format.
 * @note funcun düzgün çalışıp çalışmadığı daha test edilmedi
*/
TimeElapsedClock Get_Uptime_HW_Cycles_As_Clock_Time_32(void)
{
	return HW_Cycles_To_Clock_Time_32(k_cycle_get_32());
}


/* ---  64 BIT HW_CYCLES --- */


/**
 * @brief Get system uptime, in system clock HW cycles. Read the hardware clock. This routine returns the current time, as measured by the system's hardware clock.
 * @note 64 bit.
 * @return Current hardware clock up-counter (in cycles).
*/
uint64_t Get_Uptime_HW_Cycles_64()
{
	return k_cycle_get_64();
}

/**
 * @brief Convert HW cycles to milliseconds.
 * @note 64 bit.
 * @note funcun düzgün çalışıp çalışmadığı daha test edilmedi.
*/
uint64_t HW_Cycles_To_Milliseconds_64(uint64_t a_cycles)
{
	return (k_cyc_to_ms_floor64(a_cycles));
}

/**
 * @brief Convert HW cycles to seconds. (zephyr\include\zephyr\sys\time_units.h has most of the conversions but not for seconds.)
 * @note 64 bit.
 * @warning Throws away the fraction part after doing an integer division for conversion.
 * @note funcun düzgün çalışıp çalışmadığı daha test edilmedi.
*/
uint64_t HW_Cycles_To_Seconds_64(uint64_t a_cycles)
{
	return (k_cyc_to_ms_floor64(a_cycles) / (uint64_t)1000);
}

/**
 * @brief Get uptime HW cycles as milliseconds.
 * @note 64 bit.
 * @note funcun düzgün çalışıp çalışmadığı daha test edilmedi.
*/
uint64_t Get_Uptime_HW_Cycles_As_Milliseconds_64(void)
{
	return k_cyc_to_ms_floor64(k_cycle_get_64());
}

/**
 * @brief Get uptime HW cycles as seconds. (zephyr\include\zephyr\sys\time_units.h has most of the conversions but not for seconds.)
 * @note 64 bit.
 * @note funcun düzgün çalışıp çalışmadığı daha test edilmedi
*/
uint64_t Get_Uptime_HW_Cycles_As_Seconds_64(void)
{
	return HW_Cycles_To_Seconds_64(k_cycle_get_64());
}

/**
 * @brief Convert HW cycles to clock time.
 * @return clock time of type TimeElapsedClock. 
*/
TimeElapsedClock HW_Cycles_To_Clock_Time_64(uint64_t a_cycles)
{
	TimeElapsedClock clk; // not inited because all of its elements will be set later anyways.
	
	uint64_t total_micro_sec = k_cyc_to_us_floor64(a_cycles); 
	uint64_t total_milli_sec = total_micro_sec / (uint64_t)1000;
	uint64_t total_sec 		= total_milli_sec / (uint64_t)1000;
	uint64_t total_minutes	= total_sec 	   / (uint64_t)60;
	uint64_t total_hours 	= total_minutes   / (uint64_t)60;
	uint64_t total_days 	= total_hours 	   / (uint64_t)24;

	clk.u_sec = (uint16_t)(total_micro_sec % 1000);
	clk.m_sec = (uint16_t)(total_milli_sec % 1000);
	clk.sec 	= (uint8_t) (total_sec 	    % 60);
	clk.min 	= (uint8_t) (total_minutes   % 60);
	clk.hour  = (uint8_t) (total_hours     % 24);
	clk.day 	= (uint32_t)(total_days);

	return clk;
}

/**
 * @brief Get system uptime tracked by HW cycles in clock format.
 * @note funcun düzgün çalışıp çalışmadığı daha test edilmedi
*/
TimeElapsedClock Get_Uptime_HW_Cycles_As_Clock_Time_64(void)
{
	return HW_Cycles_To_Clock_Time_64(k_cycle_get_64());
}




/**
 * @brief Helps to accumulate time everytime that this function is called. Data is in seconds. 
 * @param [out] a_secs_buff  pointer to the user provided buffer where the accumulated time data will be written.
 * @param [in, out]  a_prev_secs Pointer to a variable where the previous values needed for accumulated time calculations. For usual accumulated time calculations this variable shouldn't be modified after it's proper initialization and should be left only for use of this function. But, user can indicate a custom previous time i.e. with some offset. This can be provided modified beforehand if some different type of accumulated time regarding the use case is wanted to be calculated.
 * @warning pointer to the a_secs_buff variable provided in this function should be an appropriate type of variable where it can hold accumulated time values i.e. RAM retained variable if data is wanted to be RAM retained. It's up to user to provide synchronisation and protection as well as maintain this variable as needed, before and after it's manipulated in this function.
*/
void Accumulate_Time_Secs(uint64_t * a_secs_buff, uint64_t * a_prev_secs)
{
     uint64_t current_secs = Get_Uptime_Ticks_As_Seconds();

     uint64_t time_diff_to_be_added = {0};

     time_diff_to_be_added = current_secs - * a_prev_secs;

     * a_prev_secs = current_secs; // update previous value to current for later iterations

	* a_secs_buff += time_diff_to_be_added;
}


/**
 * @brief Helps to accumulate time everytime that this function is called to create time data that is tracked since the power up of the device. Data is in seconds.
 * @param [out] a_secs_buff  pointer to the user provided buffer where the powered up time data will be written.
 * @warning pointer to the a_secs_buff variable provided in this function should be an appropriate type of variable where it can hold poweredup time values i.e. RAM retained variable. It's up to user to provide synchronisation and protection as well as maintain this variable as needed, before and after it's manipulated in this function.
*/
void Create_Poweredup_Time_Secs(uint64_t * a_secs_buff)
{

	static uint64_t prev_secs = {0};

     uint64_t current_secs = Get_Uptime_Ticks_As_Seconds();

     uint64_t time_diff_to_be_added = {0};

     time_diff_to_be_added = current_secs - prev_secs;

     prev_secs = current_secs; // update previous value to current for later iterations

	* a_secs_buff += time_diff_to_be_added;
}



/**
 * @brief Helps to accumulate time everytime that this function is called. Data is in TimeElapsedClock format. 
 * @param [out] a_clk  pointer to the user provided buffer where the accumulated time data will be written.
 * @param [in, out]  a_previous_clk Pointer to a variable where the previous values needed for accumulated time calculations. For usual accumulated time calculations this variable shouldn't be modified after it's proper initialization and should be left only for use of this function. But, user can indicate a custom previous time i.e. with some offset. This can be provided modified beforehand if some different type of accumulated time regarding the use case is wanted to be calculated.
 * @warning pointer to the a_clk variable provided in this function should be an appropriate type of variable where it can hold accumulated time values i.e. RAM retained variable if data is wanted to be retained. It's up to user to provide synchronisation and protection as well as maintain this variable as needed, before and after it's manipulated in this function.
*/
TimeAndClockErrors Accumulate_Time_Clk(TimeElapsedClock * a_clk, TimeElapsedClock * a_previous_clk)
{
     TimeElapsedClock current_clk = Get_Uptime_Ticks_As_Clock_Time();

     TimeElapsedClock time_diff_to_be_added = {0};

     TimeAndClockErrors err = 0;
     err = Clock_Subtract_Two_Time_Points(&time_diff_to_be_added, current_clk, * a_previous_clk);
     if(err != TIME_UTIL_ERROR_NONE){
          LOG_ERR("Error while subtracting time in %s", __func__);
     }

     * a_previous_clk = current_clk; // update previous value to current for later iterations

     err = Clock_Sum_Two_Time_Points(a_clk, time_diff_to_be_added, *a_clk);
	if(err != TIME_UTIL_ERROR_NONE){
          LOG_ERR("Error while summing time in %s", __func__);
     }

	return err;
}


/**
 * @brief Helps to accumulate time everytime that this function is called to create time data that is tracked since the power up of the device. Data is in TimeElapsedClock format.
 * @param [out] a_clk  pointer to the user provided buffer where the powered up time data will be written.
 * @warning pointer to the a_clk variable provided in this function should be an appropriate type of variable where it can hold poweredup time values i.e. ram retained variable. It's up to user to provide synchronisation and protection as well as maintain this variable as needed, before and after it's manipulated in this function.
*/
TimeAndClockErrors Create_Poweredup_Time_Clk(TimeElapsedClock * a_clk)
{

	static TimeElapsedClock prev_clk = {0};

     TimeElapsedClock current_clk = Get_Uptime_Ticks_As_Clock_Time();

     TimeElapsedClock time_diff_to_be_added = {0};

     TimeAndClockErrors err = 0;
     err = Clock_Subtract_Two_Time_Points(&time_diff_to_be_added, current_clk, prev_clk);
     if(err != TIME_UTIL_ERROR_NONE){
          LOG_ERR("Error while subtracting time in %s", __func__);
     }

     prev_clk = current_clk; // update previous value to current for later iterations

	err = Clock_Sum_Two_Time_Points(a_clk, time_diff_to_be_added, *a_clk);
	if(err != TIME_UTIL_ERROR_NONE){
          LOG_ERR("Error while summing time in %s", __func__);
     }

     
	return err;
}














