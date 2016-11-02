/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#include <xc.h>         /* XC8 General Include File */

#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */
#include "interrupts.h"
#include "hardware.h"



void toggleLatch(void){
    SEGLT = 0;
    NOP();
    NOP();
    SEGLT = 1;
}

void timer1 (void){
    static uint8_t numDisplay;
    TMR1 = 65535 - 2000 + TMR1;
    if(t_inputs) t_inputs--;
    if(++numDisplay == MAXDISPLAYS) numDisplay = 0;
    if(t_resistencias) t_resistencias--;
    if(t_milisegundos) t_milisegundos--;
    else {
        t_milisegundos = 1000;
        if(t_apagado) t_apagado--;
        if(corriendo){
            if(tiempo) tiempo--;
        }
    }
    if(t_btnpress) t_btnpress--;
    switch (numDisplay){
        case 0:
            PORTA = (PORTA&0xF0) | digTe0;            
            SEL_timed1 = 0;
            toggleLatch();
            SEL_tempd0 = 1;
            break;
        case 1:
            PORTA = (PORTA&0xF0) | digTe1;
            SEL_tempd0 = 0;
            toggleLatch();
            SEL_tempd1 = 1;
            break;
        case 2:
            PORTA = (PORTA&0xF0) | digTe2;           
            SEL_tempd1 = 0;
            toggleLatch();
            SEL_tempd2 = 1;
            break;
        case 3:
            PORTA = (PORTA&0xF0) | digTi0;
            SEL_tempd2 = 0;       
            toggleLatch();
            SEL_timed0 = 1;
            break;
        case 4:
            PORTA = (PORTA&0xF0) | digTi1;
            SEL_tempd0 = 0;
            toggleLatch();
            SEL_timed1 = 1;
            break;
        default:
            numDisplay = 0;
            break;
    }

}

void interrupt isr(void)
{
    if (TMR1IF){
        timer1();
        TMR1IF = 0;
    }
}
