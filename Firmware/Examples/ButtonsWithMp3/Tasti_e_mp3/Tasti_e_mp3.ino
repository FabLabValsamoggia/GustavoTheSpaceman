#include "DFRobotDFPlayerMini.h"
#include "SoftwareSerial.h"

#define BTN1                2     // pin tasto 1
#define BTN2                3     // pin tasto 2
#define BTN3                4     // pin tasto 3
#define BTN4                5     // pin tasto 4 

//= Variabili per i bottoni =================================================//
int bottone1             ;
int bottone2             ;
int bottone3             ;
int bottone4             ;

//= Variabili mp3 ==========================================/
DFRobotDFPlayerMini myDFPlayer;
SoftwareSerial mySoftwareSerial(11, 10);
unsigned int volume = 15;
void setup()
{
  mySoftwareSerial.begin(9600);
  Serial.begin(115200);
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini Demo"));
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while (true) {
      delay(0); // Code to compatible with ESP8266 watch dog.
    }
  }

  Serial.println(F("DFPlayer Mini online."));
  // Setto il volume (valori da 0 a 30)
  myDFPlayer.volume(volume);
  // Riproduzione dei brani casuale
  myDFPlayer.randomAll();
}

void loop()
{
  if (myDFPlayer.available())
  {
    printDetail(myDFPlayer.readType(), myDFPlayer.read());
  }
  /* leggo il valore del tasto */
  bottone1 = digitalRead(BTN1);
  if (bottone1 == LOW)
  {
    // Alzo il volume
    myDFPlayer.volumeUp();
  }
  /* leggo il valore del tasto */
  bottone2 = digitalRead(BTN2);
  if (bottone2 == LOW)
  {
    // Abbasso il volume
    myDFPlayer.volumeDown();
  }
  /* leggo il valore del tasto */
  bottone3 = digitalRead(BTN3);
  if (bottone3 == LOW)
  {
    // Metto in pausa
     myDFPlayer.pause();
  }
  /* leggo il valore del tasto */
  bottone4 = digitalRead(BTN4);
  if (bottone4 == LOW)
  {
    // Riprendo
    myDFPlayer.start();
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
      //myDFPlayer.next();
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
