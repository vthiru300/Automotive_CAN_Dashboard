#include "adc.h"
#include "can.h"
#include "ecu1_sensor.h"
#include "msg_id.h"
#include "uart.h"
//#include "clcd.h"

int main()
{
    //Call the functions
    init_matrix_keypad();
    init_adc();
   // init_clcd();
    init_can();
    while(1)
    {    
    get_speed();
    get_gear_pos();
    }
}