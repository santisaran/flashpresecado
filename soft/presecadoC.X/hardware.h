/* 
 * File:   hardware.h
 * Author: saran
 *
 * Created on 19 de abril de 2014, 19:19
 */

#ifndef HARDWARE_H
#define	HARDWARE_H

#ifdef	__cplusplus
extern "C" {
#endif


#define SEL_tempd0  RA5
#define SEL_tempd1  RA3
#define SEL_tempd2  RA2
#define SEL_timed0  RA1
#define SEL_timed1  RA0
#define SEGLT       RB0
#define PUNTO       RB7

#define LEDR        RA4
#define LEDV        RB5

//defines entradas
#define SWTIMEmas   RA3
#define SWTIMEmenos RA2
#define SWTEMPmas   RA1
#define SWTEMPmenos RA0
#define SWSTARTSTOP RA4

#define SWTIMEmastris   TRISRA3
#define SWTIMEmenostris TRISRA2
#define SWTEMPmastris   TRISRA1
#define SWTEMPmenostris TRISRA0
#define SWSTARTSTOPtris TRISRA4

#define T_ESPERA_APAGADO 15  // una vez apagada la R deben pasar 15 seg para volver a encender.

#ifdef	__cplusplus
}
#endif

#endif	/* HARDWARE_H */

//extern void delay_100us(void);