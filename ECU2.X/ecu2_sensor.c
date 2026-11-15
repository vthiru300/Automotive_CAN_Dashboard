#include "ecu2_sensor.h"
#include "adc.h"
#include "can.h"
#include "msg_id.h"
#include "uart.h"
#include "clcd.h"
#include "digital_keypad.h"
#include "ssd_display.h"
#define _XTAL_FREQ 20000000UL
void init_LED()
{
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB6 = 0;
    TRISBbits.TRISB7 = 0;

    LATBbits.LATB0 = 0;
    LATBbits.LATB1 = 0;
    LATBbits.LATB6 = 0;
    LATBbits.LATB7 = 0;
    
}
uint16_t get_rpm()
{
    //Implement the rpm function
    // Implement the speed function
    int val=read_adc(CHANNEL4)/0.1705;
    char rpm[5];
    
    rpm[0]='0'+val/1000;
    rpm[1]='0'+(val/100)%10;
    rpm[2]='0'+(val/10)%10;
    rpm[3]='0'+val%10;        
    rpm[4]='\0';
    can_transmit(RPM_MSG_ID,rpm,4);
    __delay_ms(80);
//    char data[5];
//    int len;
//    uint16_t msgid;
//    can_receive(&msgid,data,&len);
//    
//    data[4]='\0';
//    
//   clcd_print("RPM", LINE1(0));
//   clcd_print(data,LINE2(0));
    return val;
    
}

uint16_t get_engine_temp()
{
    //Implement the engine temperature function
    int val=read_adc(CHANNEL6)*5;
    char temp[3];
     temp[0]='0'+val/10;
     temp[1]='0'+val%10;
     temp[2]='\0';
    //clcd_print("temp", LINE1(6));
    //clcd_print(temp,LINE2(6));
    //__delay_ms(50);
    return val;
    
}


IndicatorStatus process_indicator(void)
{
    static unsigned int blink_count = 0;
    static unsigned char indicator = 1;
//    char c[1]="1";
//     can_transmit(INDICATOR_MSG_ID,c,1);
//       __delay_ms(80);
//       char data[1];
//       int len;
//       uint16_t msgid;
//       can_receive(&msgid,data,&len);
//       clcd_print("IND", LINE1(6));
//       clcd_print(data,LINE2(6));
    char key = read_digital_keypad(STATE_CHANGE);
    if (key == SWITCH1)
    {
        indicator = 1;  // Left indicator
        blink_count = 0;
        RB0 = 0;
        RB1 = 0;
        uint8_t c[1]="1";
       can_transmit(INDICATOR_MSG_ID,c,1);
       __delay_ms(80);
//       char data[1];
//       int len;
//       uint16_t msgid;
//       can_receive(&msgid,data,&len);
//       clcd_print("IND", LINE1(6));
//       clcd_print(data,LINE2(6));
       
    }
    else if (key == SWITCH2)
    {
        indicator = 0;  // Turn indicators off
        blink_count = 0;

        RB0 = 0;
        RB1 = 0;
        RB6 = 0;
        RB7 = 0;
        uint8_t c[1]="2";
        can_transmit(INDICATOR_MSG_ID,c,1);
       __delay_ms(80);
//       char data[1];
//       int len;
//       uint16_t msgid;
//       can_receive(&msgid,data,&len);
//       clcd_print("IND", LINE1(6));
//       clcd_print(data,LINE2(6));
        //clcd_print("OFF", LINE2(10));
        
        return e_ind_off;
        
    }
    else if (key == SWITCH3)
    {
        indicator = 2;  // Right indicator
        blink_count = 0;
        RB6 = 0;
        RB7 = 0;
        uint8_t c[1]="3";
        can_transmit(INDICATOR_MSG_ID,c,1);
       __delay_ms(80);
       //char data[1];
       //int len;
       //uint16_t msgid;
       //can_receive(&msgid,data,&len);
       //clcd_print("IND", LINE1(6));
       //clcd_print(data,LINE2(6));
         
    }
    

    if (indicator == 1)
    {
        if (++blink_count >= 5000)
        {
            RB0 = !RB0;
            RB1 = !RB1;
            blink_count = 0;
        }

        //clcd_print("ON", LINE2(10));
        return e_ind_left;
    }
    else if (indicator == 2)
    {
        if (++blink_count >= 5000)
        {
            RB6 = !RB6;
            RB7 = !RB7;
            blink_count = 0;
        }

        //clcd_print("ON", LINE2(10));
        return e_ind_right;
    }

    return e_ind_off;
}

     
 