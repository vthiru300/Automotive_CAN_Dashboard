#include <xc.h>
#include "isr.h"
#include "timer0.h"

extern volatile unsigned char indicator_status;

unsigned int count = 0;

void __interrupt() isr(void)
{
    if (TMR0IF == 1)
    {
        TMR0IF = 0;
        TMR0 = TMR0 + 8;

        count++;

        if (count >= 10000)
        {
            count = 0;

            if (indicator_status == '3')
            {
                RB0 = !RB0;
                RB1 = !RB1;

                RB6 = 0;
                RB7 = 0;
            }
            else if (indicator_status == '1')
            {
                RB6 = !RB6;
                RB7 = !RB7;

                RB0 = 0;
                RB1 = 0;
            }
            else
            {
                RB0 = 0;
                RB1 = 0;
                RB6 = 0;
                RB7 = 0;
            }
        }
    }
}