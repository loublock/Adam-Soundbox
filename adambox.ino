#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <Keypad.h> //Bibliothek zum auslesen der Matrix Tasten... (4x4)
#include <Adafruit_NeoPixel.h>
#define PIXEL1_PIN    5
#define PIXEL2_PIN    6
#define PIXEL1_COUNT 5  // Number of NeoPixels
#define PIXEL2_COUNT 5  // Number of NeoPixels

Adafruit_NeoPixel strip1(PIXEL1_COUNT, PIXEL1_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2(PIXEL2_COUNT, PIXEL2_PIN, NEO_GRB + NEO_KHZ800);

SoftwareSerial mySoftwareSerial(10, 11); // RX, TX
DFRobotDFPlayerMini myDFPlayer;
void printDetail(uint8_t type, int value);
int increaseVol(int baseVol, int inc);

int a; //SerialVariable
int i = 1; //LayerMode
int rand_track = 1;
bool shuffleMP3 = false;

const byte ROWS = 4; // Reihen
const byte COLS = 4; // Spalten
char keys[ROWS][COLS] = {
  {'1','2','3','4'},
  {'5','6','7','8'},
  {'9','A','B','C'},
  {'D','E','F','G'}
};
// ReihenPins.
byte rowPins[ROWS] = { 9, 4, 3, 2 };
// SpaltenPINS.
byte colPins[COLS] = { 13, 12, 8, 7 }; 

// Create the Keypad
Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


void setup()
{
  randomSeed(analogRead(0));
  mySoftwareSerial.begin(9600);
  Serial.begin(9600);
  
  Serial.println();
  Serial.println(F("Adams Magic Shoutbox"));
  Serial.println(F("Wart halt!..."));
    
  strip1.begin(); // Initialize NeoPixel strip object (REQUIRED)
  strip1.show();  // Initialize all pixels to 'off'
  strip2.begin(); // Initialize NeoPixel strip object (REQUIRED)
  strip2.show();  // Initialize all pixels to 'off'
  

  if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
    Serial.println(F("Unable to begin:"));
    Serial.println(F("1.Please recheck the connection!"));
    Serial.println(F("2.Please insert the SD card!"));
    while(true);
  }
  Serial.println(F("DFPlayer Mini online."));
  
  myDFPlayer.setTimeOut(500); //Set serial communictaion time out 500ms
  
  //----Set volume----
  myDFPlayer.volume(20);  //Set volume value (0~30).
  //myDFPlayer.volumeUp(); //Volume Up
  //myDFPlayer.volumeDown(); //Volume Down
  
  //----Set different EQ----
  myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);
  myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

  
  Serial.print("Aktuelle Lautstärke: ");
  Serial.println(myDFPlayer.readVolume()); //read current volume
  Serial.print("Anzahl an Titel auf SD-Karte: ");
  Serial.println(myDFPlayer.readFileCounts()); //read all file counts in SD card
  
  colorWipe(strip1.Color(0,   50,   0), 120);    // green
  colorWipe(strip2.Color(0,   50,   0), 120);    // green

  myDFPlayer.playFolder(1, 1);  //danke fürs einschaltenm
  delay(1500);
  myDFPlayer.loopFolder(2); //loop 5s silence
}

void loop()
{
    char key = kpd.getKey();
    if(key)  // Check for a valid key.
    {
      switch (key)
      {
        case '1':
          Serial.println("1: LayerMode");
          if(i<3){
            i++;
          }
          else{
            i=1;
          }
          Serial.println(i);
          if(i==1){
          LightMode0();
          }
          if(i==2){
          LightMode1();        
          }
          if(i==3){
          LightMode2();        
          }
          break;
        case '2':
          Serial.println("2: Shuffle");
          //myDFPlayer.randomAll(); //Random play all the mp3.
          randomMP3();
          break;
        case '3':
          Serial.println("3: Lauter");
          myDFPlayer.volumeDown(); //Volume Up
          break;  
        case '4':
          Serial.println("4: Leiser");
          myDFPlayer.volumeUp(); //Volume Down
          break;   
        case '5':
          Serial.println("5");
          myDFPlayer.advertise(1+(i-1)*12);  //play specific mp3 in SD:/1/001.mp3; Folder Name(1~99); File Name(1~255)
          break;
        case '6':
          Serial.println("6");
          myDFPlayer.advertise(2+(i-1)*12);  //play specific mp3 in SD:/1/001.mp3; Folder Name(1~99); File Name(1~255)
          break;
        case '7':
          Serial.println("7");
          myDFPlayer.advertise(3+(i-1)*12);  //play specific mp3 in SD:/1/001.mp3; Folder Name(1~99); File Name(1~255)
          break;  
        case '8':
          Serial.println("8");
          myDFPlayer.advertise(4+(i-1)*12);  //play specific mp3 in SD:/1/001.mp3; Folder Name(1~99); File Name(1~255)
          break;   
        case '9':
          Serial.println("9");
          myDFPlayer.advertise(5+(i-1)*12);  //play specific mp3 in SD:/1/001.mp3; Folder Name(1~99); File Name(1~255)
          break;
        case 'A':
          Serial.println("A");
          myDFPlayer.advertise(6+(i-1)*12);  //play specific mp3 in SD:/1/001.mp3; Folder Name(1~99); File Name(1~255)
          break;
        case 'B':
          Serial.println("B");
          myDFPlayer.advertise(7+(i-1)*12);  //play specific mp3 in SD:/1/001.mp3; Folder Name(1~99); File Name(1~255)
          break;  
        case 'C':
          Serial.println("C");
          myDFPlayer.advertise(8+(i-1)*12);  //play specific mp3 in SD:/1/001.mp3; Folder Name(1~99); File Name(1~255)
          break;  
        case 'D':
          Serial.println("D");
          myDFPlayer.advertise(9+(i-1)*12);  //play specific mp3 in SD:/1/001.mp3; Folder Name(1~99); File Name(1~255)
          break;
        case 'E':
          Serial.println("E");
          myDFPlayer.advertise(10+(i-1)*12);  //play specific mp3 in SD:/1/001.mp3; Folder Name(1~99); File Name(1~255)
          break;
        case 'F':
          Serial.println("F");
          myDFPlayer.advertise(11+(i-1)*12);  //play specific mp3 in SD:/1/001.mp3; Folder Name(1~99); File Name(1~255)
          break;  
        case 'G':
          Serial.println("G");
          myDFPlayer.advertise(12+(i-1)*12);  //play specific mp3 in SD:/1/001.mp3; Folder Name(1~99); File Name(1~255)
          break;                                
        default:
          Serial.println("ERROR");
      }
    }
  
  //Serielle Kommunikation mit BLE modul:
  if (Serial.available())
  {
    a = Serial.parseInt();
    int track_count = 36;
      
    if (a == 99)
    {
      //myDFPlayer.randomAll(); //Random play all the mp3.
      randomMP3();
    }
    else if (a == 98) 
    {
      myDFPlayer.volume(adjustVolume(myDFPlayer.readVolume(), 5));
    }
    else if (a == 97) 
    {
      myDFPlayer.volume(adjustVolume(myDFPlayer.readVolume(), -5));
    }
    else if ((a >= 0) && (a < track_count))
    {
      //iphone plist starts at 0
      myDFPlayer.advertise(a+1); 
    }
    else
    {
      return;
    }
  }
  
  //Informationen Seriell ausgeben:
  if (myDFPlayer.available())
  {
    printDetail(myDFPlayer.readType(), myDFPlayer.read()); //Print the detail message from DFPlayer to handle different errors and states.
  }
}

int adjustVolume(int baseVol, int inc)
{
  int vol = 20;
  vol = baseVol;
  vol = baseVol + inc;
  if (vol > 30)
  {
    vol = 30;
  }
  else if (vol < 0)
  {
    vol = 0;
  }
  return vol;
}

void randomMP3()
{
    //only working when hit the random generator twice
    //otherwise track 1 is always played
    rand_track = random(1,myDFPlayer.readFileCounts()+1-38);
    rand_track = random(1,myDFPlayer.readFileCounts()+1-38);
    Serial.println(rand_track);
    char buffer[30];
    sprintf(buffer, "Number %d", rand_track);
    myDFPlayer.playMp3Folder(rand_track);
    shuffleMP3 = true;
}

void rainbow(int wait) {
  for(long firstPixelHue = 0; firstPixelHue < 3*65536; firstPixelHue += 256) {
    for(int i=0; i<strip1.numPixels(); i++) { // For each pixel in strip...
      int pixelHue1 = firstPixelHue + (i * 65536L / strip1.numPixels());
      int pixelHue2 = firstPixelHue + (i * 65536L / strip2.numPixels());
      strip1.setPixelColor(i, strip1.gamma32(strip1.ColorHSV(pixelHue1)));
      strip2.setPixelColor(i, strip2.gamma32(strip2.ColorHSV(pixelHue2)));
    }
    strip1.show(); // Update strip with new contents
    strip2.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}
void LightModeWelcome(){
  strip1.setPixelColor(0, strip1.Color(50, 50, 50));
  strip1.setPixelColor(1, strip1.Color(50, 50, 50));
  strip1.setPixelColor(2, strip1.Color(50, 50, 50));
  strip1.setPixelColor(3, strip1.Color(50, 50, 50));
  strip1.setPixelColor(4, strip1.Color(50, 50, 50));
  strip1.setPixelColor(5, strip1.Color(50, 50, 50));

  strip2.setPixelColor(0, strip2.Color(50, 50, 50));
  strip2.setPixelColor(1, strip2.Color(50, 50, 50));
  strip2.setPixelColor(2, strip2.Color(50, 50, 50));
  strip2.setPixelColor(3, strip2.Color(50, 50, 50));
  strip2.setPixelColor(4, strip2.Color(50, 50, 50));
  strip2.setPixelColor(5, strip2.Color(50, 50, 50));

  strip1.show();
  strip2.show();
}

void LightMode0(){
  strip1.setPixelColor(0, strip1.Color(0, 50, 0));
  strip1.setPixelColor(1, strip1.Color(0, 50, 0));
  strip1.setPixelColor(2, strip1.Color(0, 50, 0));
  strip1.setPixelColor(3, strip1.Color(0, 50, 0));
  strip1.setPixelColor(4, strip1.Color(0, 50, 0));
  strip1.setPixelColor(5, strip1.Color(0, 50, 0));

  strip2.setPixelColor(0, strip2.Color(0, 50, 0));
  strip2.setPixelColor(1, strip2.Color(0, 50, 0));
  strip2.setPixelColor(2, strip2.Color(0, 50, 0));
  strip2.setPixelColor(3, strip2.Color(0, 50, 0));
  strip2.setPixelColor(4, strip2.Color(0, 50, 0));
  strip2.setPixelColor(5, strip2.Color(0, 50, 0));

  strip1.show();
  strip2.show();
}
void LightMode1(){
  strip1.setPixelColor(0, strip1.Color(50, 0, 0));
  strip1.setPixelColor(1, strip1.Color(50, 0, 0));
  strip1.setPixelColor(2, strip1.Color(50, 0, 0));
  strip1.setPixelColor(3, strip1.Color(50, 0, 0));
  strip1.setPixelColor(4, strip1.Color(50, 0, 0));
  strip1.setPixelColor(5, strip1.Color(50, 0, 0));

  strip2.setPixelColor(0, strip2.Color(50, 0, 0));
  strip2.setPixelColor(1, strip2.Color(50, 0, 0));
  strip2.setPixelColor(2, strip2.Color(50, 0, 0));
  strip2.setPixelColor(3, strip2.Color(50, 0, 0));
  strip2.setPixelColor(4, strip2.Color(50, 0, 0));
  strip2.setPixelColor(5, strip2.Color(50, 0, 0));

  strip1.show();
  strip2.show();
}
void LightMode2(){
  strip1.setPixelColor(0, strip1.Color(0, 0, 50));
  strip1.setPixelColor(1, strip1.Color(0, 0, 50));
  strip1.setPixelColor(2, strip1.Color(0, 0, 50));
  strip1.setPixelColor(3, strip1.Color(0, 0, 50));
  strip1.setPixelColor(4, strip1.Color(0, 0, 50));
  strip1.setPixelColor(5, strip1.Color(0, 0, 50));

  strip2.setPixelColor(0, strip2.Color(0, 0, 50));
  strip2.setPixelColor(1, strip2.Color(0, 0, 50));
  strip2.setPixelColor(2, strip2.Color(0, 0, 50));
  strip2.setPixelColor(3, strip2.Color(0, 0, 50));
  strip2.setPixelColor(4, strip2.Color(0, 0, 50));
  strip2.setPixelColor(5, strip2.Color(0, 0, 50));

  strip1.show();
  strip2.show();
}

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip1.numPixels(); i++) { // For each pixel in strip...
    strip1.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip2.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip1.show();                          //  Update strip to match
    strip2.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip1.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<strip1.numPixels(); c += 3) {
        strip1.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip1.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

void printDetail(uint8_t type, int value){
  switch (type) {
    case TimeOut:
      Serial.println(F("Time Out!"));
      if (shuffleMP3)
      {
        randomMP3();
      }
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
    case DFPlayerUSBInserted:
      Serial.println("USB Inserted!");
      break;
    case DFPlayerUSBRemoved:
      Serial.println("USB Removed!");
      break;
    case DFPlayerPlayFinished:
      Serial.print(F("Number:"));
      Serial.print(value);
      Serial.println(F(" Play Finished!"));
      if (shuffleMP3)
      {
        randomMP3();
      }
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
          if (shuffleMP3)
          {
            randomMP3();
          }
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
