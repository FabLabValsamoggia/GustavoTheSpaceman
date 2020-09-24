/******************************************************************************/
/* button.cpp                                               Data: 18/01/2018  */
/* Descrizione: funzioni per la gestione dei tasti per la scheda              */
/*              school_shield sviluppata al FabLab Valsamoggia.               */
/* Versione:    0.00                                                          */
/* Modifiche:                                                                 */
/*              Christian Corsetti (18/01/2018)                               */
/*              Creazione programma                                           */
/*              Christian Corsetti (18/01/2018)                               */
/*              creata funzione btns_check_state                              */
/*              Christian Corsetti (18/01/2018)                               */
/*              creata funzione btns_init                                     */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* Includes                                                                   */
/*----------------------------------------------------------------------------*/
#include <Arduino.h>
#include "def.h"

/*----------------------------------------------------------------------------*/
/* Defines                                                                    */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Variables                                                                  */
/*----------------------------------------------------------------------------*/

/*----------------------------*/
/* Variabili per i tasti      */
/*----------------------------*/
btn_state_t btn1;
btn_state_t btn2;
btn_state_t btn3;
btn_state_t btn4;
/*----------------------------------------------------------------------------*/
/* Prototypes                                                                 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* btns_init                                                                  */
/* Descrizione:     inizializza i tasti come input                            */
/* Parametri input: void                                                      */
/* Parametri out:   void                                                      */
/*----------------------------------------------------------------------------*/
void btns_init(void)
{
    /* Init dei tasti come parametri di input */
    pinMode(BTN1, INPUT);
    pinMode(BTN2, INPUT);
    pinMode(BTN3, INPUT);
    pinMode(BTN4, INPUT);
}
uint8_t btns_check_state(uint16_t debounce)
{
    /*-------------------------------------------------*/
    /*                    TASTO 1                      */
    /*-------------------------------------------------*/
    /* leggo il valore del tasto */
    btn1.val = digitalRead(BTN1);
    /* controllo sia diverso dal valore letto prima */
    if((btn1.val == LOW) && (btn1.old_val == HIGH))
    {
        /* debounce del tasto */
        delay(debounce);
        /* rilettura del tasto */
        btn1.val = digitalRead(BTN1);
        if(btn1.val == LOW)
        {
            return(1);
        }
    }
    /*-------------------------------------------------*/
    /*                    TASTO 2                      */
    /*-------------------------------------------------*/
    /* leggo il valore del tasto */
    btn2.val = digitalRead(BTN2);
    /* controllo sia diverso dal valore letto prima */
    if((btn2.val == LOW) && (btn2.old_val == HIGH))
    {
        /*debounce del tasto */
        delay(debounce);
        /* rilettura del tasto */
        btn2.val = digitalRead(BTN2);
        if(btn2.val == LOW)
        {
            return(2);
        }
    }
    /*-------------------------------------------------*/
    /*                    TASTO 3                      */
    /*-------------------------------------------------*/
    /* leggo il valore del tasto */
    btn3.val = digitalRead(BTN3);
    /* controllo sia diverso dal valore letto prima */
    if((btn3.val == LOW) && (btn3.old_val == HIGH))
    {
        /*debounce del tasto */
        delay(debounce);
        /* rilettura del tasto */
        btn3.val = digitalRead(BTN3);
        if(btn3.val == LOW)
        {
            return(3);
        }
    }
    /*-------------------------------------------------*/
    /*                    TASTO 4                      */
    /*-------------------------------------------------*/
    /* leggo il valore del tasto */
    btn4.val = digitalRead(BTN4);
    /* controllo sia diverso dal valore letto prima */
    if((btn4.val == LOW) && (btn4.old_val == HIGH))
    {
        /*debounce del tasto */
        delay(debounce);
        /* rilettura del tasto */
        btn4.val = digitalRead(BTN4);
        if(btn4.val == LOW)
        {
            return(4);
        }
    }
    /* setto il valore letto come vecchio valore */
    btn1.old_val = btn1.val;
    btn2.old_val = btn2.val;
    btn3.old_val = btn3.val;
    btn4.old_val = btn4.val;
    return(0);
}
