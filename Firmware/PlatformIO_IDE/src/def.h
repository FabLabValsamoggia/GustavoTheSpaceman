/******************************************************************************/
/* def.h                                                    Data: 18/01/2018  */
/* Descrizione: definizioni condivise per il firmware del progetto            */
/*              school_shield sviluppato al FabLab Valsamoggia.               */
/*              Sono definiti tutti i vari pin utilizzati in tutto il         */
/*              il programma, le strutture dedicate e alcuni prototipi        */
/*              di funzione                                                   */
/* Versione:    0.00                                                          */
/* Modifiche:                                                                 */
/*              Christian Corsetti (18/01/2018)                               */
/*              Creazione programma                                           */
/*              Christian Corsetti (18/01/2018)                               */
/*              Aggiunta funzione per il check dello stato dei tasti          */
/******************************************************************************/
/*----------------------------------------------------------------------------*/
/* Defines                                                                    */
/*----------------------------------------------------------------------------*/
#ifndef _DEF
#define _DEF
#define BTN1                2
#define BTN2                3
#define BTN3                4
#define BTN4                5

#define PHOTO1              A7
#define PHOTO2              A6
#define PHOTO3              A5
#define PHOTO4              A4

#define VREF                5.0
/*----------------------------------------------------------------------------*/
/* Variables                                                                  */
/*----------------------------------------------------------------------------*/
/*----------------------------*/
/* struttura gestione tasti   */
/*----------------------------*/
typedef struct
{
    bool val     ; /* valore attuale in lettura */
    bool old_val ; /* vecchio valore letto */
}btn_state_t;

/*----------------------------*/
/* struttura gestione photoR  */
/*----------------------------*/
typedef struct
{
    int val     ; /* valore attuale in lettura */
    float old_val ; /* vecchio valore letto */
}phts_t;

/*----------------------------------------------------------------------------*/
/* Prototypes                                                                 */
/*----------------------------------------------------------------------------*/
void btns_init(void);
uint8_t btns_check_state(uint16_t debounce);
uint8_t phts_check_state(void);
void phts_init(void);
#endif
