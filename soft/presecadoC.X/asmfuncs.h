/* 
 * File:   asmfuncs.h
 * Author: saran
 *
 * Created on 7 de abril de 2014, 23:01
 */

#ifndef ASMFUNCS_H
#define	ASMFUNCS_H

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* ASMFUNCS_H */

#define setBit(reg,bitio)    bsf reg,bitio 

#define clrBit(reg,bitio)   bcf reg,bitio

#define comBit(_reg,bitio)   \
    movlw  1<<bitio         \
    xorwf reg,1
