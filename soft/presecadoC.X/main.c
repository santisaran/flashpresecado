/* 
 * File:   main.c
 * Author: saran
 *
 * Created on 5 de abril de 2014, 15:36
 */
//#define NO_BIT_DEFINES

#include <xc.h>
#include <pic16f818.h>
#include <stdint.h>
#include <stdbool.h>
#include "hardware.h"
#include "tipos.h"

#define REC_T_INPUTS 10

#define setbit(var, bit)  ((var) |= (1 << (bit)))
#define clrbit(var, bit)  ((var) &= ~(1 << (bit)))
#define testbit(var, bit) ((var) & (1 <<(bit)))

#define TEMPMAX     210
#define TEMPMIN     50
#define TIEMMAX     9*60-1

#define T_RECARGAINI 500    //tiempo de primera repetición ms
#define T_RECARGAREP 200    //tiempo de repetición ms

/*
 * 
 */
//Definiciones de variables displays.


/* i.e. uint8_t <variable_name>; */

Flags myflags;


enum bits{
    swTempMenos  =   0,
    swTempMas,
    swTiempoMenos,
    swTiempoMas,
    swStartStop,
};

uint8_t entradas, bloq_entradas;
uint8_t aux;
uint16_t tiempo; //590 segundos maximo
uint16_t termocupla;
uint16_t temperatura;    //210 grados máximo
uint8_t tempd0, tempd1, tempd2;
uint8_t timed0, timed1;

uint8_t t_inputs = 0;
uint8_t t_apagado;
uint16_t t_resistencias = 0;
uint16_t t_milisegundos=1000;
uint16_t t_btnpress = 0;     //timer para detectar botón presionado por mas de 0.5s

uint8_t digTe0, digTe1, digTe2, digTi0, digTi1;

void delay_100us(void);

void encender(void){
    corriendo = 1;
}

void apagar(void){
    corriendo = 0;
}

void controltemp(void){
    if(termocupla < temperatura){
        encenderR = 1;
        apagarR = 0;
    }
    else{
        t_apagado = T_ESPERA_APAGADO;
        apagarR = 1;
        encenderR = 0;
    }
}

void convertirTemp(void){
    //Carga los valores para los digitos
    //como el pic no tiene la instrucción dividir, conviene hacerlo así
    uint16_t tempbk;
    TMR1IE = 0;
    digTe2 = 0;
    tempbk = temperatura;
    while (tempbk>=100){
        tempbk=tempbk-100;
        digTe2++;
    }
    digTe1 = 0;
    while (tempbk>=10){
        tempbk=tempbk-10;
        digTe1++;
    }
    digTe0 = (uint8_t)tempbk;
    TMR1IE = 1;
}

void convertirTiempo(void){
    //Carga los valores para los digitos, en segundos o en minutos,decenas de segundos.
    uint16_t tiempobk;
    tiempobk = tiempo;
    TMR1IE = 0;
    digTi1 = 0;
    if (tiempo < 60){
        PUNTO = 0;
        while(tiempobk>=10){
            digTi1++;
            tiempobk = tiempobk - 10;
        }
        digTi0 = (uint8_t)tiempobk;
    }
    else{
        PUNTO = 1;
        while(tiempobk>=60){
            digTi1++;
            tiempobk = tiempobk -60;
        }
        digTi0 = 0;
        while (tiempobk>=10){
            digTi0++;
            tiempobk = tiempobk -10;
        }
    }
    
    TMR1IE = 1;
}

void verteclas(void){

    static Registro old_teclas = 0x01F;
    uint8_t trisabk,puertoabk;

    trisabk = TRISA;
    puertoabk = PORTA;

    TRISA = TRISA | 0x1F;

    delay_100us();
    if(old_teclas.value  == (PORTA & 0x1F) ){
        entradas = old_teclas.value;
    }
    else{
        old_teclas.value = (PORTA & 0x1F);
        //Hubo algún cambio, recargo el contador
        t_btnpress = T_RECARGAINI;

    }

    if (testbit(entradas,swStartStop)){
        //se soltó el botón
        // -> activar lectura del boton.
        clrbit(bloq_entradas,swStartStop);
    }
    if (t_btnpress==0){
        bloq_entradas = bloq_entradas & \
            !(swTiempoMas|swTiempoMenos|swTempMas|swTempMenos);
    }
    else{
        if (testbit(entradas,swTiempoMas)){
            //se soltó el botón, activar lectura del boton.
            clrbit(bloq_entradas,swTiempoMas);
        }
        if (testbit(entradas,swTiempoMenos)){
            //se soltó el botón, activar lectura del boton.
            clrbit(bloq_entradas,swTiempoMenos);
        }
        if (testbit(entradas,swTempMas)){
            //se soltó el botón, activar lectura del boton.
            clrbit(bloq_entradas,swTempMas);
        }
        if (testbit(entradas,swTempMenos)){
            //se soltó el botón, activar lectura del boton.
            clrbit(bloq_entradas,swTempMenos);
        }
    }


    TRISA = trisabk;
    PORTA = puertoabk;
}

void delay_100us(void){
    #asm
    movlw   33;
    movwf   (_aux);
_decremento:
    decfsz  _aux;
    goto    _decremento
    #endasm;
}
//TODO configurar pullups
void main(void) {

    Flags banderas;
    #define ComenzarCuenta banderas.b0
    #define botonTimeUp    banderas.b1
    #define botonTimeDown  banderas.b2
    #define botonTempUp    banderas.b3
    #define botonTempDown  banderas.b4

    TMR1 = 65535 - 2000;
    TMR1ON = 1;
    TMR1IE = 1;
    PEIE = 1;
    GIE = 1;
    ADCON1 = 0x6;
    while (1){
        CLRWDT();
        controltemp();
        if(!t_inputs){
            t_inputs = REC_T_INPUTS;
            verteclas();
            if(bloq_entradas != 0x1F){
                //si no están todas las entradas bloqueadas, recargo timer
                //y luego veo cual es la que no está bloqueada
                if(t_btnpress==0){
                    t_btnpress = T_RECARGAREP;
                }
                if(!testbit(bloq_entradas,swStartStop)){
                    //no está bloqueada la lectura, leo la entrada
                    // botón presionado-> entrada en 0
                    if(!testbit(entradas,swStartStop)){
     //Acción para el botón StartStop
                        if(ComenzarCuenta){
                            apagar();
                        }
                        else{
                            encender();
                        }
                        //bloqueo lectura para evitar reentrar.
                        setbit(bloq_entradas,swStartStop);
                    }
                }

                if(!testbit(bloq_entradas,swTempMas)){
                    //no está bloqueada la lectura, leo la entrada
                    // botón presionado-> entrada en 0
                    if(!testbit(entradas,swTempMas)){
    //Acción para el botón temperatura mas
                        temperatura++;
                        if(temperatura==TEMPMAX+1){
                            temperatura = 0;
                        }
                        convertirTemp();
                        //bloqueo lectura para evitar reentrar.
                        setbit(bloq_entradas,swTempMas);
                    }
                }
                if(!testbit(bloq_entradas,swTempMenos)){
                    //no está bloqueada la lectura, leo la entrada
                    // botón presionado-> entrada en 0
                    if(!testbit(entradas,swTempMenos)){
    //Acción para el botón temperatura menos
                        if(temperatura){
                            temperatura--;
                        }
                        else{
                            temperatura = TEMPMAX;
                        }
                        convertirTemp();
                        //bloqueo lectura para evitar reentrar.
                        setbit(bloq_entradas,swTempMenos);
                    }
                }
                if(!testbit(bloq_entradas,swTiempoMas)){
                    //no está bloqueada la lectura, leo la entrada
                    // botón presionado-> entrada en 0
                    if(!testbit(entradas,swTiempoMas)){
    //Acción para el botón tiempo mas
                        tiempo++;
                        if (tiempo==TIEMMAX){
                            tiempo = 0;
                        }
                        convertirTiempo();
                        //bloqueo lectura para evitar reentrar.
                        setbit(bloq_entradas,swTiempoMas);
                    }
                }
                if(!testbit(bloq_entradas,swTiempoMenos)){
                    //no está bloqueada la lectura, leo la entrada
                    // botón presionado-> entrada en 0
                    if(!testbit(entradas,swTiempoMenos)){
    //Acción para el botón tiempo menos
                        if(tiempo){
                            tiempo--;
                        }
                        else{
                            tiempo = TIEMMAX;
                        }
                        convertirTiempo();
                        //bloqueo lectura para evitar reentrar.
                        setbit(bloq_entradas,swTiempoMenos);
                    }
                }
            }
        }
    }
}

#define MAXDISPLAYS 5

