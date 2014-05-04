/* 
 * File:   tipos.h
 * Author: saran
 *
 * Created on 20 de abril de 2014, 00:20
 */

#ifndef TIPOS_H
#define	TIPOS_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct{
 unsigned int  b0 :1;
 unsigned int  b1 :1;
 unsigned int  b2 :1;
 unsigned int  b3 :1;
 unsigned int  b4 :1;
 unsigned int  b5 :1;
 unsigned int  b6 :1;
 unsigned int  b7 :1;
} Flags;

typedef union{
    uint8_t value;
    Flags bits;
} Registro;

#define corriendo myflags.b0
#define encenderR myflags.b1
#define apagarR   myflags.b2


#ifdef	__cplusplus
}
#endif

#endif	/* TIPOS_H */

