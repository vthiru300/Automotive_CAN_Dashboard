#include "ecu2_sensor.h"
#include "adc.h"
#include "can.h"
#include "msg_id.h"
#include "uart.h"
#include "clcd.h"
#include "ssd_display.h"
#include "digital_keypad.h"
#define _XTAL_FREQ 20000000UL
static unsigned char ssd[MAX_SSD_CNT];
static unsigned char digit[] = {ZERO, ONE,TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};
int main()
{
    //init_matrix_keypad();
    init_digital_keypad();
    init_LED();
    init_adc();
    init_can();
    
    //init_ssd_control();
    //init_clcd();
    //call the functions
    while(1)
    {
      int val=get_rpm();
//    
      
//    ssd[3] = digit[val%10];
//    ssd[2] = digit[val/10%10];
//    ssd[1] = digit[val/100%10];
//    ssd[0] = digit[val/1000];
//    display(ssd);
//  get_engine_temp();
     process_indicator();
     
    }
}
