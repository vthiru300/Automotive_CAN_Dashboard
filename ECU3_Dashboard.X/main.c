#include <xc.h>
#include <stdint.h>
#include "can.h"
#include "clcd.h"
#include "msg_id.h"
#include "message_handler.h"
//#include "isr.h"
#include "timer0.h"

static void init_leds() {
    TRISB = 0x08; // Set RB2 as output, RB3 as input, remaining as output
    PORTB = 0x00;
}

static void init_config(void) {
    // Initialize CLCD and CANBUS
    init_timer0();
    init_clcd();
    
    init_leds();
   init_can();
    // Enable Interrupts
    PEIE = 1;
    GIE = 1;
  
}
volatile unsigned char indicator_status = '2';
void main(void) {
    // Initialize peripherals
    init_config();

    /* ECU1 main loop */
    while (1) {
        // Read CAN Bus data and handle it
        //process_canbus_data();
        char data[8];
        int len;
        uint16_t msgid;
        can_receive(&msgid,data,&len);
        data[len]='\0';
        clcd_print("SP GR  RPM  IND ",LINE1(0));
        if(msgid==SPEED_MSG_ID)
        {
            //clcd_print("SP",LINE1(0));
            clcd_print(data,LINE2(0));
        }
        if(msgid==GEAR_MSG_ID)
        {
            //clcd_print("GR",LINE1(3));
            clcd_print(data,LINE2(3));
        }
        if(msgid==RPM_MSG_ID)
        {
            //clcd_print("RPM",LINE1(6));
            clcd_print(data,LINE2(6));
            
        }
        if(msgid==INDICATOR_MSG_ID)
        {
            indicator_status = data[0];
            //clcd_print("IND",LINE1(10));
            if(data[0]=='1')
            clcd_print("-> ",LINE2(11));
            if(data[0]=='2')
            clcd_print("OFF",LINE2(11));
            if(data[0]=='3')
            clcd_print("<- ",LINE2(11));
        }
    }

    return;
}
