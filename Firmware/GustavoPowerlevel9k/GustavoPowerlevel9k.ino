/******************************************************************************/
/* school_shield.ino                                        Data: 18/01/2018  */
/* Descrizione: programma di esempio e di sviluppo per il progetto            */
/*              school_shield sviluppato al FabLab Valsamoggia                */
/* Versione:    0.00                                                          */
/* Modifiche:                                                                 */
/*              Christian Corsetti (18/01/2018)                               */
/*              Creazione programma                                           */
/*              Christian Corsetti (18/01/2018)                               */
/*              Test dei tasti e aggiunta funzioni separate per la gestione   */
/*              Christian Corsetti (19/01/2018)                               */
/*              Test delle fotoresistenze e aggiunte funzioni separate        */
/*              Christian Corsetti (29/01/2018)                               */
/*              Trasporto del progetto da Platformio a IDE Arduino            */
/*              Pulizia del codice e commenti per rendere più chiare le       */
/*              varie parti del programma                                     */
/*              Christian COrsetti (03/02/2018)                               */
/*              Aggiunto il controllo di luminosità tramite fotoresistenze    */
/*              Aggiunte le funzioni phts_init, phts_check_state e relativo   */
/*              codice per il funzionamento in setup e loop                   */
/*              Christian Corsetti (04/02/2018)                               */
/*              Aggiunta rainbowCycle                                         */
/*              Inizio spostamento funzioni principali per creazione libreria */
/*              Pulizia codice e commenti delle parti principali              */
/*              Bug fix su scarsa sensibilità per il cambiamento di           */
/*              Luminosità                                                    */
/******************************************************************************/
/* 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
/*----------------------------------------------------------------------------*/
/* Includes                                                                   */
/*----------------------------------------------------------------------------*/
#include "Adafruit_NeoPixel.h"          // Libreria per la gestione led
#include "Timer.h"                      // Libreria gestione timer
#include "struttura_hardware.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
/*----------------------------------------------------------------------------*/
/* Defines                                                                    */
/*----------------------------------------------------------------------------*/
#define TIMEOUT             100
#define PERIOD              60000       
#define THRESHOLD           50
/*----------------------------------------------------------------------------*/
/* Variables                                                                  */
/*----------------------------------------------------------------------------*/
/*== Variabili Locali ========================================================*/

/*== Variabili Globali =======================================================*/
uint32_t matrix_color    ;  // Contiene il colore con cui si accendono i led
int luminosity = 75      ;  // Contiene la luminosità che avranno i led
uint8_t btn              ;  // Contiene quale tasto è stato premuto
uint8_t pht              ;
/*== Variabili Definite ======================================================*/
Adafruit_NeoPixel matrix ;  // Classe per la gestione dei Led
matrix_rgb_t matrix_rgb  ;  // contiene il colore in RGB (RED, GREEN, BLUE)
Timer t;                    // variabile per il funzionamento del timer
phts_t photoR1;
phts_t photoR2;
phts_t photoR3;
phts_t photoR4;

DFRobotDFPlayerMini myDFPlayer;
SoftwareSerial mySoftwareSerial(11, 10);
unsigned int volume = 15;
/*----------------------------------------------------------------------------*/
/* Prototypes                                                                 */
/*----------------------------------------------------------------------------*/
void whiteOverRainbow(uint8_t wait, uint8_t whiteSpeed, uint8_t whiteLength );
void printDetail(uint8_t type, int value);
/*----------------------------------------------------------------------------*/
/* phts_init                                                                  */
/* Descrizione: Inizializza la parte hardware relativa alle fotoresistenze    */
/* Return     : void                                                          */
/*----------------------------------------------------------------------------*/
void phts_init(void)
{
  /*==========================================================================*/
  /* Leggo i valori delle fotoresistenze all'accensione. Questi saranno i     */
  /* valori di riferimento per il cambiamento di luminosità.                  */
  /* N.B: questa implementazione è la maniera più semplicema presenta un bug. */
  /* Passando in un luogo con fonte luminosa differente, i led potrebbero     */
  /* cambiare automaticamente il valore di luminosità. Questo è dovuto al     */
  /* fatto che i valori letti durante il run time del programma potrebbero    */
  /* essere fuori soglia (vedi phts_check_state per dettagli) senza coprire   */
  /* le fotoresistenze. Il metodo migliore per eliminare questo bug è         */
  /* cambiare la soglia fino a trovare il migliore compromesso possibile.     */
  /* Per eliminare del tutto il problema, implementare un PID                 */
  /*==========================================================================*/
  photoR1.old_val = analogRead(PHOTO1);
  photoR2.old_val = analogRead(PHOTO2);
  photoR3.old_val = analogRead(PHOTO3);
  photoR4.old_val = analogRead(PHOTO4);
#ifdef DEBUG
  Serial.println("valori iniziali");
  Serial.println(photoR1.old_val);
  Serial.println(photoR2.old_val);
  Serial.println(photoR3.old_val);
  Serial.println(photoR4.old_val);
#endif
}
/*----------------------------------------------------------------------------*/
/* phts_check_state                                                           */
/* Descrizione: legge il valore attuale delle fotoresistenze                  */
/* Return     : 1 se sono "coperte" photoR1 e/o photoR2                       */
/*              2 se sono "coperte" photoR3 e/o photoR4                       */
/*              0 se non è stato "coperto" niente                             */
/*----------------------------------------------------------------------------*/
uint8_t phts_check_state(void)
{
  photoR1.val = analogRead(PHOTO1);
  photoR2.val = analogRead(PHOTO2);
  photoR3.val = analogRead(PHOTO3);
  photoR4.val = analogRead(PHOTO4);
#ifdef DEBUG
  Serial.println(photoR1.val);
  Serial.println(photoR2.val);
  Serial.println(photoR3.val);
  Serial.println(photoR4.val);
  Serial.println("\r\n");
  Serial.println("\r\n");
#endif
  if ((photoR1.val > photoR1.old_val + THRESHOLD) || (photoR2.val > photoR2.old_val + THRESHOLD))
    return (1);
  if ((photoR3.val > photoR3.old_val + THRESHOLD) || (photoR4.val > photoR4.old_val + THRESHOLD))
    return (2);

  return (0);
}
/*----------------------------------------------------------------------------*/
/* SpegniLed                                                                  */
/* Descrizione: funzione interrupt attivata ogni x secondi dal timer.         */
/*              Verifica se ci sono tasti premuti e in caso negativo, spegne  */
/*              i led                                                         */
/* Return     : void                                                          */
/*----------------------------------------------------------------------------*/
void SpegniLed()
{
  if (!btns_check_state(20))
  {
    matrix_rgb.r = 0;
    matrix_rgb.g = 0;
    matrix_rgb.b = 0;
    matrix_color = matrix.Color(luminosity * matrix_rgb.r,
                                luminosity * matrix_rgb.g,
                                luminosity * matrix_rgb.b);

    /* Accendo i led con il colore selezionato tramite il tasto */
    for (int i = 0; i < MATRIX_NLED; i++)
    {
      matrix.setPixelColor(i, matrix_color);
      matrix.show();
      delay(TIMEOUT);
    }
  }
}
/*----------------------------------------------------------------------------*/
/* setup()                                                                    */
/* Descrizione: Funzione per inizializzare tutto l'hardware necessario per    */
/*              il funzionamento corretto del programma                       */
/* Return     : void                                                          */
/*----------------------------------------------------------------------------*/
void setup()
{
  mySoftwareSerial.begin(9600);
  /* Inzializzo la comunicazione seriale           */
  Serial.begin(115200);

  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

   if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true){
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.volume(volume);  //Set volume value. From 0 to 30
  myDFPlayer.play(1);  //Play the first mp3
  
  /* Inizializzo la striscia di led */
  matrix = Adafruit_NeoPixel(MATRIX_NLED, MATRIX_PLED,
                             NEO_GRB + NEO_KHZ800);
  /*===============================================*/
  /* Una volta inizializzata, posso far partire    */
  /* tutti i processi che servono per poter        */
  /* utilizzare i led nel modo giusto              */
  /*===============================================*/
  matrix.begin();
  /* parto con i led spenti */
  matrix.show();
  /*===============================================*/
  /* Inizializzo i tasti. Siccome devo leggere     */
  /* lo stato in cui si trovano li inizializzo     */
  /* come input.                                   */
  /* N.B. : per questioni hardware, lo stato 0     */
  /* indica tasto premuto, lo stato 1 indica       */
  /* non premuto                                   */
  /*===============================================*/
  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);
  pinMode(BTN3, INPUT);
  pinMode(BTN4, INPUT);
  /* Inizializzo lo stato delle fotoresistenze     */
  phts_init();

  /* Inizializzo il timer per l'interrupt          */
  int tickEvent = t.every(PERIOD, SpegniLed);
}
/*----------------------------------------------------------------------------*/
/* setup()                                                                    */
/* Descrizione: Parte di programma che si ripete all'infinito. Contiene tutto */
/*              il codice che serve per far funzionare l'algoritmo del        */
/*              programma                                                     */
/* Return     : void                                                          */
/*----------------------------------------------------------------------------*/
void loop()
{
  /* Refresh del timer */
  t.update();
  if (myDFPlayer.available()) 
  {
    printDetail(myDFPlayer.readType(), myDFPlayer.read());
  }
  /* leggo lo stato attuale delle fotoresistenze */
  pht = phts_check_state();
  
  switch (pht)
  {
    case 1:
      if (luminosity < 255)
      {
        luminosity += 10;
      }
      break;
    case 2:
      if (luminosity < 255)
      {
        luminosity += 50;
      }
      break;
    case 3:
      if (luminosity > 1)
      {
        luminosity -= 10;
      }
      break;
    case 4:
      if (luminosity > 1)
      {
        luminosity -= 50;
      }
      break;
    default:
      break;
  }
  if (luminosity > 255)
    luminosity = 255;
  if (luminosity < 1)
    luminosity = 1;
  Serial.println(luminosity);

  btn = btns_check_state(20);
  switch (btn)
  {
    case 1:
      /*===================================================*/
      /* Se il tasto premuto è il primo, setto il colore   */
      /* dei led blu                                       */
      /*===================================================*/
      matrix_rgb.r = 0;
      matrix_rgb.g = 0;
      matrix_rgb.b = 1;
      matrix_color = matrix.Color(luminosity * matrix_rgb.r,
                                  luminosity * matrix_rgb.g,
                                  luminosity * matrix_rgb.b);
      break;
    case 2:
      /*===================================================*/
      /* Se il tasto premuto è il secondo, setto il colore */
      /* dei led verde                                     */
      /*===================================================*/
      //matrix_rgb.r = 0;
      //matrix_rgb.g = 1;
      //matrix_rgb.b = 0;
      //matrix_color = matrix.Color(luminosity * matrix_rgb.r,
        //                          luminosity * matrix_rgb.g,
          //                        luminosity * matrix_rgb.b);
      if(volume < 30)
      {
        volume++;
        myDFPlayer.volume(volume);
      }
      break;
    case 3:
      /*===================================================*/
      /* Se il tasto premuto è il terzo, setto il colore   */
      /* dei led rosso                                     */
      /*===================================================*/
      //matrix_rgb.r = 1;
      //matrix_rgb.g = 0;
      //matrix_rgb.b = 0;
      //matrix_color = matrix.Color(luminosity * matrix_rgb.r,
        //                          luminosity * matrix_rgb.g,
        //                          luminosity * matrix_rgb.b);
      if(volume > 0)
      {
        volume--;
        myDFPlayer.volume(volume);
      }
      break;
    case 4:
      /*===================================================*/
      /* Se il tasto premuto è il quarto, chiamo la        */
      /* funzione per creare la sequenza arcobaleno nei    */
      /* nei led                                           */
      /*===================================================*/
      //whiteOverRainbow(20, 75, 5);
      rainbowCycle(20);
      break;
    default:
      matrix_color = matrix.Color(luminosity * matrix_rgb.r,
                                  luminosity * matrix_rgb.g,
                                  luminosity * matrix_rgb.b);
      break;
  }

  /* Accendo i led con il colore selezionato tramite il tasto */
  for (int i = 0; i < MATRIX_NLED; i++)
  {
    matrix.setPixelColor(i, matrix_color);
    matrix.show();
    delay(TIMEOUT);
  }
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      break;
    case WrongStack:
      Serial.println(F("Stack Wrong!"));
      break;
    case DFPlayerCardInserted:
      Serial.println(F("Card Inserted!"));
      break;
    case DFPlayerCardRemoved:
      Serial.println(F("Card Removed!"));
      break;
    case DFPlayerCardOnline:
      Serial.println(F("Card Online!"));
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      myDFPlayer.next();
      break;
    case DFPlayerError:
      Serial.print(F("DFPlayerError:"));
      switch (value) {
        case Busy:
          Serial.println(F("Card not found"));
          break;
        case Sleeping:
          Serial.println(F("Sleeping"));
          break;
        case SerialWrongStack:
          Serial.println(F("Get Wrong Stack"));
          break;
        case CheckSumNotMatch:
          Serial.println(F("Check Sum Not Match"));
          break;
        case FileIndexOut:
          Serial.println(F("File Index Out of Bound"));
          break;
        case FileMismatch:
          Serial.println(F("Cannot Find File"));
          break;
        case Advertise:
          Serial.println(F("In Advertise"));
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }

}
// dato in input un valore tra 0 e 255 restituisce un valore rgb
uint32_t Wheel(byte WheelPos)
{
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85)
  {
    return matrix.Color(255 - WheelPos * 3, 0, WheelPos * 3, 0);
  }
  if (WheelPos < 170)
  {
    WheelPos -= 85;
    return matrix.Color(0, WheelPos * 3, 255 - WheelPos * 3, 0);
  }
  WheelPos -= 170;
  return matrix.Color(WheelPos * 3, 255 - WheelPos * 3, 0, 0);
}

// gioco di colori
void whiteOverRainbow(uint8_t wait, uint8_t whiteSpeed, uint8_t whiteLength )
{
  if (whiteLength >= matrix.numPixels())
  {
    whiteLength = matrix.numPixels() - 1;
  }

  int head = whiteLength - 1;
  int tail = 0;

  int loops = 3;
  int loopNum = 0;

  static unsigned long lastTime = 0;


  while (true) {
    for (int j = 0; j < 256; j++) {
      for (uint16_t i = 0; i < matrix.numPixels(); i++) {
        if ((i >= tail && i <= head)   ||
            (tail > head && i >= tail) ||
            (tail > head && i <= head) )
        {
          matrix.setPixelColor(i, matrix.Color(0, 0, 0, 255 ) );
        }
        else {
          matrix.setPixelColor(i, Wheel(((i * 256 / matrix.numPixels()) + j) & 255));
        }

      }

      if (millis() - lastTime > whiteSpeed) {
        head++;
        tail++;
        if (head == matrix.numPixels()) {
          loopNum++;
        }
        lastTime = millis();
      }

      if (loopNum == loops) return;

      head %= matrix.numPixels();
      tail %= matrix.numPixels();
      matrix.show();
      delay(wait);
    }
  }

}

void rainbowCycle(uint8_t wait)
{
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) {
    for (i = 0; i < matrix.numPixels(); i++) {
      matrix.setPixelColor(i, Wheel(((i * 256 / matrix.numPixels()) + j) & 255));
    }
    matrix.show();
    delay(wait);
  }
}


