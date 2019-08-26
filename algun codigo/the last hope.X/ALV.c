/*
 * File:   ALV.c
 * Author: Usuario
 *
 * Created on 19 de agosto de 2019, 03:01 PM
 */


// Seccion de liberias 
#include <xc.h> 
#include "la.h"
#include "pic18f4585.h"
//Seccion de directivos
//ISR de alta prioridad
__interrupt ( high_priority ) void high_isr ( void ){
    Nop ( ); //Funcion para consumir un ciclo de instruccion
}
//ISR de baja prioridad 
__interrupt ( low_priority ) void low_isr ( void ){
    Nop ( ); //Funcion para consumir un ciclo de instruccion
}
// -------------------Declaracion de funciones---------------------------
void delay ( void );        // Se declara la funcion delay
void portInit ( void );     // Se declara al funcion portInit 
// -------------------Inicio del programa---------------------------
void main ( void ) {
    unsigned char viene ; //Informacion por recibir
    unsigned char va ;  //Informacion por enviar 
    portInit ( );   //Se declaran los puertos a usar 
    //Inicio del ciclo infinito 
    while ( 1 ) {
        if (PIR1bits.RCIF == 1){ // Si RCREG esta lleno (llego informacion)
            viene = RCREG1; // se guarda lo que llego de RCREG1 en viene
            LATB = viene; //Se imprime en binario lo que llego de los leds
            RCREG1 = 0; //Se resetea el registro recibidor
        }
        if(viene != va){ //Cuando lo que se va a enviar no es igual a lo que se ha recibido (para no ciclar) 
            if (TXSTA1bits.TRMT == 1 ){ // Y el registro para transmitir info este vacio 
                va =  viene +1; //Se aumenta el caracter 
                if(va == 58){
                    va = 48; 
                }
                if(va == 123){
                    va = 97; 
                }
                  if(va == 91){
                    va = 65; 
                }
                if( va >47 && va <58 || va >96 && va <123 || va >64 && va <91 ){
                    TXREG1 = va;  // Se envia el nuevo caracter al registro de transmision USART
                va = viene; // 
                }
            } 
        }
    }
       
}
//--------------Contenido de las funciones -------------
void delay ( void ){ //Se desarolla el contenido de la funcion delay 
    for (int i=0; i<100; i++ ) // Ciclo para incrementar el tiempo de delay
        __delay_ms ( 10 ) ;    // Se llama la libreria delay para generar un retraso de 10 milisegundos
}
void portInit ( void ) { //Se desarolla el contenido de la funcion portInit
    ADCON1bits.PCFG = 0xFF;
    TRISCbits.RC6 = 0; //Se declara el TX como output
    TRISCbits.RC7 = 1; //Se declara el RX como input
    
    SPBRGH1:SPBRG1 = 42; //Baudrate de 34
    TXSTAbits.BRGH = 1 ; // Highrate value 
    BAUDCONbits.BRG16 = 1; //Se usa baudrate de 16 bits
    
    TXSTAbits.TX9 = 0; // Transmision de 8 bits
    TXSTAbits.TXEN = 1; //Se permite la transmision 
    TXSTAbits.SYNC = 0; //Modo asincrono 
    RCSTAbits.SPEN = 1; // Enciedne TX y RX
    RCSTAbits.RX9 = 0; //Recepcion a 8 bits 
    RCSTAbits.CREN = 1; //Habilita recibir
}
