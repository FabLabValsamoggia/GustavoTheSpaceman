/******************************************************************************/
/* photo.cpp                                                 Data: 18/01/2018 */
/* Descrizione: funzione gestione photoresistenze per la scheda               */
/*              school_shield sviluppata al FabLab Valsamoggia                */
/* Versione:    0.00                                                          */
/* Modifiche:                                                                 */
/*              Christian Corsetti (18/01/2018)                               */
/*              Creazione programma                                           */
/*              Christian Corsetti (18/01/2018)                               */
/*              Creata funzione phts_check_state                              */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* Includes                                                                   */
/*----------------------------------------------------------------------------*/
#include <Arduino.h>
#include "def.h"

/*----------------------------------------------------------------------------*/
/* Defines                                                                    */
/*----------------------------------------------------------------------------*/
#define SENS 10
#define SOGLIA 300
/*----------------------------------------------------------------------------*/
/* Variables                                                                  */
/*----------------------------------------------------------------------------*/
int p1, p2, p3, p4;
float fp1, fp2, fp3, fp4;

phts_t photoR1;
phts_t photoR2;
phts_t photoR3;
phts_t photoR4;

float media_soglia;

void phts_init(void)
{
  photoR1.val = analogRead(PHOTO1);
  photoR2.val = analogRead(PHOTO2);
  photoR3.val = analogRead(PHOTO3);
  photoR4.val = analogRead(PHOTO4);

  photoR1.old_val = photoR1.val * (5.00 / 1023);
  photoR2.old_val = photoR2.val * (5.00 / 1023);
  photoR3.old_val = photoR3.val * (5.00 / 1023);
  photoR4.old_val = photoR4.val * (5.00 / 1023);

  media_soglia = (photoR1.old_val + photoR2.old_val + photoR3.old_val + photoR4.old_val) / 4;
  Serial.println(media_soglia);
}

uint8_t phts_check_state(void)
{
    p1 = analogRead(PHOTO1);
    p2 = analogRead(PHOTO2);
    p3 = analogRead(PHOTO3);
    p4 = analogRead(PHOTO4);
/*
    Serial.print("P1: ");
    Serial.println(p1);
    Serial.print("P2: ");
    Serial.println(p2);
    Serial.print("P3: ");
    Serial.println(p3);
    Serial.print("P4: ");
    Serial.println(p4);
*/
    fp1 = p1 * (5.0 /1023.0);
    fp2 = p2 * (5.0 /1023.0);
    fp4 = p3 * (5.0 /1023.0);
    fp4 = p4 * (5.0 /1023.0);

    Serial.println(media_soglia);
    Serial.print("P1: ");
    Serial.println(fp1);
    Serial.print("P2: ");
    Serial.println(fp2);
    Serial.print("P3: ");
    Serial.println(fp3);
    Serial.print("P4: ");
    Serial.println(fp4);
    if((fp1 < (media_soglia - 10)) || (fp2 < (media_soglia - 10)))
        return(1);
    if((fp3 < media_soglia) || (fp4 < media_soglia))
        return(3);

    return(0);
}
