#include "Adafruit_NeoPixel.h"          // Libreria per la gestione led

#define MATRIX_PLED         9     // pin per i led (D9)    
#define MATRIX_NLED         16    // numero di led nella matrice
#define BTN1                2     // pin tasto 1
#define BTN2                3     // pin tasto 2
#define BTN3                4     // pin tasto 3
#define BTN4                5     // pin tasto 4 

#define PHOTO1              A7    // pin fotoresistenza 1
#define PHOTO2              A6    // pin fotoresistenza 2
#define PHOTO3              A5    // pin fotoresistenza 3
#define PHOTO4              A4    // pin fotoresistenza 4

//= Variabili per i bottoni =================================================//
int bottone1             ;
int bottone2             ;
int bottone3             ;
int bottone4             ;
//= Variabili per i LED ====================================================//
uint32_t matrix_color    ;  // Contiene il colore con cui si accendono i led
Adafruit_NeoPixel matrix ;  // Classe per la gestione dei Led
int red                  ;  // RGB
int green                ;  // RGB
int blue                 ;  // RGB

void setup() 
{
  // Setto i bottoni come INPUT
  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);
  pinMode(BTN3, INPUT);
  pinMode(BTN4, INPUT);

  Serial.begin(115200);

  /* Inizializzo la striscia di led */
  matrix = Adafruit_NeoPixel(MATRIX_NLED, MATRIX_PLED,
                             NEO_GRB + NEO_KHZ800);
   matrix.begin();
  /* parto con i led spenti */
  matrix.show();
}

void loop() 
{
  /* leggo il valore del tasto */
  bottone1 = digitalRead(BTN1);
  if(bottone1 == LOW)
  {
    // Setto il colore blu per esempio
    red = 0;
    green = 0;
    blue = 75;
    matrix_color = matrix.Color(red, green, blue); 
  }
  /* leggo il valore del tasto */
  bottone2 = digitalRead(BTN2);
  if(bottone2 == LOW)
  {
    // Setto il colore rosso per esempio
    red = 75;
    green = 0;
    blue = 0;
    matrix_color = matrix.Color(red, green, blue); 
  }
  /* leggo il valore del tasto */
  bottone3 = digitalRead(BTN3);
  if(bottone3 == LOW)
  {
    // Setto il colore verde per esempio
    red = 0;
    green = 75;
    blue = 0;
    matrix_color = matrix.Color(red, green, blue); 
  }
  /* leggo il valore del tasto */
  bottone4 = digitalRead(BTN4);
  if(bottone4 == LOW)
  {
    // Setto un mix di colori per esempio
    red = 25;
    green = 125;
    blue = 90;
    matrix_color = matrix.Color(red, green, blue); 
  }
  /* Accendo i led con il colore selezionato tramite il tasto */
  for (int i = 0; i < MATRIX_NLED; i++)
  {
    matrix.setPixelColor(i, matrix_color);
    matrix.show();
  }
}
