 

#include "test_encoder.h" 


#include "esp_log.h"
#include "esp_err.h"

#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
 
#include "esp_timer.h"
 
#include "freertos/semphr.h"

static const char * TAG = "main";

//-----------------------------------------------------------------------------------------------//



#define CLK_PIN 	 7
#define DTA_PIN 	 6

#define CW      1       //clockwise
#define CCW     0       //couter clockwise
#define ND      255      //no rotation happened

#define POSITION_INCREMENT 	 1
         	        



volatile char sense_of_rotation = ND;  

volatile char state = 0b1111; // state = 0b XX YY => XX = int1_clk_lvl, int1_dta_lvl ; YY = int2_clk_lvl, int2_dta_lvl
                              // state == 0b0100 => CW ; state == 0b1000 => CCW
                              // state has 4 bits to avoid debounce (take only values that have form: 0100 or 1000 aka: CW or CCW)

volatile int8_t position = 0;


void app_main(void)
{
    
    
    encoder_init();



    
    
                

}





/**
 * @brief interrupt on pin clk NEGEDGE + interrupt on pin dta NEGEDGE
 *        1st interrupt happens on pin clk OR dta (if clk first => CW , if dta first => CCW)
 *        1st interrupt isr: state[0] = clk lvl
 *        2nd intterput isr: state[1] = dta_lvl
 *        if 1st interrupt happened on pin clk => state = 01 => CW
 *        if 1st interrupt happened on pin dta => state = 10 => CCW
 */
void encoder_init()
{
    //config gpio for clk pin and dta pin
    gpio_reset_pin(CLK_PIN);
    gpio_reset_pin(DTA_PIN);

   gpio_install_isr_service(0);
    
    gpio_config_t clk_pin_cfg = 
    {
        .pin_bit_mask = 1ULL << CLK_PIN,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .intr_type = GPIO_INTR_NEGEDGE,
    };
    gpio_config(&clk_pin_cfg);

    gpio_config_t dta_pin_cfg = 
    {
        .pin_bit_mask = 1ULL << DTA_PIN,
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .intr_type = GPIO_INTR_NEGEDGE,
    };
    gpio_config(&dta_pin_cfg);

    //attch same intr isr to pin_clk_intr and pin_dta_intr
    gpio_isr_handler_add(CLK_PIN, &clk_dta_isr, NULL);
    gpio_isr_handler_add(DTA_PIN, &clk_dta_isr, NULL);

    //start log_task 
    xTaskCreate(&encoder_log_task, "encoder_log_task", 4060, NULL, 5, NULL);

}



void clk_dta_isr(void * args)
{

    char clk_lvl = gpio_get_level(CLK_PIN);
    char dta_lvl = gpio_get_level(DTA_PIN);

    if (clk_lvl != dta_lvl) //we could be at intr 1
    {
        state = (clk_lvl << 3) +(dta_lvl << 2)  + 0b11; //state = 0b0111 or 0b1011
    }
    else
    if (clk_lvl == 0 && dta_lvl == 0) //we coulde be at intr 2
    {
        state = state & 0b1100; //if state == 1111 (int1 didnt happen) => new_state = 1100
                                //if state == 0111 or 0b1011 (int1 did happen) => new_state = 0100 or 1000
    }

    //in all cases:  if state == 0100 or 1000 => 2 interrupts happened correctly => CW or CCW
    if (state == 0b0100)
    {
        sense_of_rotation = CW;
        state = 0b1111;
        position+=POSITION_INCREMENT;
    }
    else
    if (state == 0b1000)
    {
        sense_of_rotation = CCW;
        state = 0b1111;
        position-=POSITION_INCREMENT;
    }
    

}



void encoder_log_task(void * pvParam)
{
    while (1)
    {
        ESP_LOGI(TAG, "sense of rotation = %d", sense_of_rotation);
        ESP_LOGI(TAG, "position = %d", position);
        
        vTaskDelay(50/portTICK_PERIOD_MS);
        
    }
    
}