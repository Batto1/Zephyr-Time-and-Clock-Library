/**
 * @author Batto1
 * @brief  Example application for showcasing some routines of time and clock utilities library.	 
 * @date   26.03.2024
 *	 
 */

#include <zephyr/kernel.h>

#include "time_and_clock_utils.h"

void main(void)
{
	TimeAndClockErrors err = TIME_UTIL_ERROR_NONE;
	TimeElapsedClock prev_time = Get_Uptime_Ticks_As_Clock_Time(); // get the current time in clock format when this line runs.

	k_busy_wait(2000000); // busy wait for 2 seconds.
	TimeElapsedClock final_time = Get_Uptime_Ticks_As_Clock_Time();

	TimeElapsedClock difference_time = {0};
	err = Clock_Subtract_Two_Time_Points(&difference_time, final_time, prev_time);
	if(TIME_UTIL_ERROR_NONE == err){
		char legend_buf[CLOCK_LEGEND_MAX_STRING_SIZE] = ""; // buffer for printing time legend in.
		char clock_buf[25];
		Clock_To_Str(clock_buf, legend_buf, &difference_time, true, true, true, true, true, true);
		printk("%s %s\n", legend_buf, clock_buf);
	}else{
		printk("ERROR while subtracting: %d\n", err);
	}

	

}
