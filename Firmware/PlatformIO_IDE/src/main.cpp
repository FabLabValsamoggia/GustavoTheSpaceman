/******************************************************************************/
/* main.cpp                                                 Data: 18/01/2018  */
/* Descrizione: primo programma di esempio per il progetto                    */
/*              school_shield sviluppato al FabLab Valsamoggia                */
/* Versione:    0.00                                                          */
/* Modifiche:                                                                 */
/*              Christian Corsetti (18/01/2018)                               */
/*              Creazione programma                                           */
/*              Christian Corsetti (18/01/2018)                               */
/*              Test dei tasti e aggiunta funzioni separate per la gestione   */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/* Includes                                                                   */
/*----------------------------------------------------------------------------*/
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "def.h"
#include <Timer.h>
/*----------------------------------------------------------------------------*/
/* Defines                                                                    */
/*----------------------------------------------------------------------------*/
#define MATRIX_PLED         9           /* pin per i led (D9) */
#define MATRIX_NLED         16          /* numero di led nella matrice */

/*----------------------------------------------------------------------------*/
/* Variables                                                                  */
/*----------------------------------------------------------------------------*/
/*----------------------------*/
/* Classe per la gestione led */
/*----------------------------*/
Adafruit_NeoPixel matrix     ;
unsigned short timeout = 100 ; /* delay tra l'accesione di un led e l'altro */
uint32_t matrix_color        ; /* variabile colore matrice dei led */

const unsigned long PERIOD1 = 2000;    //one second
const unsigned long PERIOD2 = 10000;   //ten seconds
Timer t;                               //instantiate the timer object

typedef struct
{
  int32_t r;
  int32_t g;
  int32_t b;
}matrix_rgb_t;

matrix_rgb_t matrix_rgb;
int luminosity;
/*----------------------------------------------------------------------------*/
/* Prototypes                                                                 */
/*----------------------------------------------------------------------------*/
void whiteOverRainbow(uint8_t wait, uint8_t whiteSpeed, uint8_t whiteLength );
void print(void)
{
 // Serial.println(luminosity);
}
void setup()
{
    /* Init della variabile matrix */
    matrix = Adafruit_NeoPixel(MATRIX_NLED, MATRIX_PLED,
                               NEO_GRB + NEO_KHZ800);
    /* start della variabile matrix */
    matrix.begin();
    /* parto con i led spenti */
    matrix.show();
    /* colore iniziale dei led senza alcun tasto premuto */
    matrix_rgb.r = 50;
    matrix_rgb.g = 50;
    matrix_rgb.b = 50;
    luminosity = 1;
    matrix_color = matrix.Color(luminosity * matrix_rgb.r,
                                luminosity * matrix_rgb.g,
                                luminosity * matrix_rgb.b);

    /* init dei tasti */
    btns_init();
    /* init valori phts */
    phts_init();
    /* Init serial per debug */
    Serial.begin(9600);
    t.every(PERIOD1, print);
}

void loop()
{
    uint8_t btn;
    uint8_t pht;

    btn = btns_check_state(20);
    switch(btn)
    {
        case 1:
              matrix_rgb.r = 0;
              matrix_rgb.g = 0;
              matrix_rgb.b = 1;
              matrix_color = matrix.Color(luminosity * matrix_rgb.r,
                                          luminosity * matrix_rgb.g,
                                          luminosity * matrix_rgb.b);
              break;
        case 2:
              matrix_rgb.r = 0;
              matrix_rgb.g = 1;
              matrix_rgb.b = 0;
              matrix_color = matrix.Color(luminosity * matrix_rgb.r,
                                          luminosity * matrix_rgb.g,
                                          luminosity * matrix_rgb.b);
              break;
        case 3:
              matrix_rgb.r = 1;
              matrix_rgb.g = 0;
              matrix_rgb.b = 0;
              matrix_color = matrix.Color(luminosity * matrix_rgb.r,
                                          luminosity * matrix_rgb.g,
                                          luminosity * matrix_rgb.b);
              break;
        //case 4: matrix_color = matrix.Color(255, 51, 255); break;
        case 4:
              whiteOverRainbow(20, 75, 5);
              break;
    }

    for(int i = 0; i < MATRIX_NLED; i++)
    {
        matrix.setPixelColor(i, matrix_color);
        matrix.show();
        delay(timeout);
    }

    pht = phts_check_state();
    switch(pht)
    {
      case 1:
            Serial.println("case 1");
            if(luminosity < 255)
            {
              luminosity += 30;
              if(luminosity > 255)
              {
                  luminosity = 255;
              }
              matrix_color = matrix.Color(luminosity * matrix_rgb.r,
                                          luminosity * matrix_rgb.g,
                                          luminosity * matrix_rgb.b);
            }
            pht = 0;
            break;
    case 2:
          Serial.println("case 2");
          if(luminosity < 255)
          {
            luminosity += 10;
            if(luminosity > 255)
            {
                luminosity = 255;
            }
            matrix_color = matrix.Color(luminosity * matrix_rgb.r,
                                        luminosity * matrix_rgb.g,
                                        luminosity * matrix_rgb.b);
          }
          pht = 0;
          break;
      case 3:
            Serial.println("case 3");
            if(luminosity > 10)
            {
                luminosity -= 10;
                if(luminosity < 10)
                    luminosity = 10;
                matrix_color = matrix.Color(luminosity * matrix_rgb.r,
                                            luminosity * matrix_rgb.g,
                                            luminosity * matrix_rgb.b);
            }
            pht = 0;
            break;
    case 4:
    Serial.println("case 4");
          if(luminosity > 10)
          {
              luminosity -= 30;
              if(luminosity < 10)
                  luminosity = 10;
              matrix_color = matrix.Color(luminosity * matrix_rgb.r,
                                          luminosity * matrix_rgb.g,
                                          luminosity * matrix_rgb.b);
          }
          pht = 0;
          break;
    default:
            Serial.println("nessun case");
            break;
    }

    t.update();
}
// dato in input un valore tra 0 e 255 restituisce un valore rgb
uint32_t Wheel(byte WheelPos)
{
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85)
    {
        return matrix.Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
    }
    if(WheelPos < 170)
    {
        WheelPos -= 85;
        return matrix.Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
    }
    WheelPos -= 170;
    return matrix.Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
}

// gioco di colori
void whiteOverRainbow(uint8_t wait, uint8_t whiteSpeed, uint8_t whiteLength )
{
    if(whiteLength >= matrix.numPixels())
    {
        whiteLength = matrix.numPixels() - 1;
    }

      int head = whiteLength - 1;
      int tail = 0;

      int loops = 3;
      int loopNum = 0;

      static unsigned long lastTime = 0;


      while(true){
        for(int j=0; j<256; j++) {
          for(uint16_t i=0; i<matrix.numPixels(); i++) {
            if((i >= tail && i <= head)   ||
               (tail > head && i >= tail) ||
               (tail > head && i <= head) )
               {
                  matrix.setPixelColor(i, matrix.Color(0,0,0, 255 ) );
               }
            else{
              matrix.setPixelColor(i, Wheel(((i * 256 / matrix.numPixels()) + j) & 255));
            }

          }

          if(millis() - lastTime > whiteSpeed) {
            head++;
            tail++;
            if(head == matrix.numPixels()){
              loopNum++;
            }
            lastTime = millis();
          }

          if(loopNum == loops) return;

          head%=matrix.numPixels();
          tail%=matrix.numPixels();
            matrix.show();
            delay(wait);
        }
      }

}
