
#pragma once
#ifndef __TEST_ENCODER_H__
#define __TEST_ENCODER_H__





/*-----------------------------------------------------------------------------------------------*/
/* headers                                                                                       */
/*-----------------------------------------------------------------------------------------------*/

void encoder_init();

void clk_dta_isr(void * args);

void encoder_log_task(void * pvParam);







#endif // __TEST_ENCODER_H__