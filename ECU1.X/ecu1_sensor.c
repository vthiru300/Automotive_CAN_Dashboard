#include "ecu1_sensor.h"
#include "adc.h"
#include "can.h"
#include "msg_id.h"
#include "uart.h"
#include "clcd.h"
#include "matrix_keypad.h"
#define _XTAL_FREQ 20000000UL

char EV[9][3]={"ON","GN","G1","G2","G3","G4","G5","GR","GC"};
uint16_t get_speed()
{
    // Implement the speed function
    int val=read_adc(CHANNEL4)/10.23;
    char speed[3];
    speed[0]='0'+val/10;
    speed[1]='0'+val%10;
    speed[2]='\0';
    //clcd_print("SPEED", LINE1(0));
    //clcd_print(speed,LINE2(0));
    can_transmit(SPEED_MSG_ID,speed,2);
    __delay_ms(80);
//    char data[3];
//    int len;
//    uint16_t msgid;
//    can_receive(&msgid,data,&len);
//    
//    data[2]='\0';
//
//    clcd_print("SPEED", LINE1(0));
//    clcd_print(data,LINE2(0));
    return val;
}

unsigned char get_gear_pos()
{
    //implement gear function
  static int ind = 0;
    char key = read_switches(STATE_CHANGE);

    if (key == MK_SW1 && ind < 7 && ind!=8)
        ind++;

    else if (key == MK_SW2 && ind > 0 && ind!=8)
        ind--;

    else if (key == MK_SW3)
        ind = 8;
    
    can_transmit(GEAR_MSG_ID,EV[ind],2);
    __delay_ms(80);
//    char data[3];
//    int len;
//    uint16_t msgid;
//    can_receive(&msgid,data,&len);
//    
//    data[2]='\0';
//    clcd_print("GEAR", LINE1(6));
//    clcd_print(data, LINE2(6));
    return ind;
}