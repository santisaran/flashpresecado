/* 
 * File:   interrupts.h
 * Author: saran
 *
 * Created on 5 de abril de 2014, 22:42
 */
#include "tipos.h"

extern int variable;
extern uint8_t t_inputs;
extern uint8_t t_apagado;
extern uint16_t t_btnpress;
extern uint16_t t_resistencias;
extern uint16_t t_milisegundos;
extern uint16_t tiempo;
#define MAXDISPLAYS 5

extern Flags myflags;

extern uint8_t digTe0, digTe1, digTe2, digTi0, digTi1;