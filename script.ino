#include "TFT_eSPI.h"
#include "Free_Fonts.h"
#include "RTC_SAMD51.h"
#include "DateTime.h"
#define BUZZER_PIN WIO_BUZZER /* sig pin of the buzzer */

TFT_eSPI tft;
RTC_SAMD51 rtc;
#define TFT_BLACK       0x0000      /*   0,   0,   0 */
#define TFT_NAVY        0x000F      /*   0,   0, 128 */
#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define TFT_MAROON      0x7800      /* 128,   0,   0 */
#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
#define TFT_LIGHTGREY   0xC618      /* 192, 192, 192 */
#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define TFT_BLUE        0x001F      /*   0,   0, 255 */
#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
#define TFT_RED         0xF800      /* 255,   0,   0 */
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
#define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */
#define TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 */

String incomingByte;
int nbVisiteurTotal=0;
int nbVisiteur=0;
int colorGlobal;
bool changeG = false;
bool amGreen = false;
bool changeR = false;
bool pageWindow = true;
bool amRed = false;
bool butA = false;
bool butB = false;
bool butC = false;
bool message = false;
DateTime nowGenral;



void setup() {
  tft.begin();
  tft.setRotation(3);
  Serial.begin(115200);
  
  colorGlobal = TFT_GREEN;
  tft.fillScreen(colorGlobal);
  tft.setTextColor(TFT_BLACK, colorGlobal);
  amGreen=true;
  tft.setFreeFont(&FreeSansBoldOblique12pt7b); // Le 12 correspond a la taille de la font utiliser
  tft.drawString("Need serial com",10,10); //Affiche Need serial com temps que la communication avec le port serie n'est pas établie
  while(!Serial);//Attend que la communication avec le port serie s'établie
  rtc.begin();
  pinMode(BUZZER_PIN, OUTPUT);
  nowGenral = DateTime(F(__DATE__), F(__TIME__));
  DateTime now;
  now = rtc.now();
  DateTime alarm;

  alarm = DateTime(now.year(), now.month(), now.day(), now.hour(), now.minute()+1,0 );
 

  rtc.setAlarm(0,alarm);
  rtc.enableAlarm(0, rtc.MATCH_HHMMSS);
 
  rtc.attachInterrupt(displayTime);
  rtc.adjust(nowGenral);

  pinMode(WIO_KEY_A, INPUT_PULLUP);//
  pinMode(WIO_KEY_B, INPUT_PULLUP);
  pinMode(WIO_KEY_C, INPUT_PULLUP);

  pinMode(WIO_5S_UP, INPUT_PULLUP);
  pinMode(WIO_5S_DOWN, INPUT_PULLUP);
  pinMode(WIO_5S_LEFT, INPUT_PULLUP);
  pinMode(WIO_5S_RIGHT, INPUT_PULLUP);
  pinMode(WIO_5S_PRESS, INPUT_PULLUP);

  initial();// initialise the mainWindow
}

/*
Nom de fonction : displayTime
Les variables : aucun
Les parametre :  uint32_t flag : pour l'intruption 
Description : il affiche le nombre total de visiteur et le nombre total d'affluance 
              tout les 15 minutes sur le terminale
*/
void displayTime(uint32_t flag){
    Serial.println("-----------------");
    Serial.println("Heure : "+String(nowGenral.hour())+":"+String(nowGenral.minute()));
    Serial.println("Nombre Total de visiteur : "+String(nbVisiteurTotal));
    Serial.println("Affluance : "+String(nbVisiteur));
    DateTime now;
    now = rtc.now();
    DateTime alarm ;

    alarm = DateTime(now.year(), now.month(), now.day(), now.hour(), now.minute()+1,0 );
    
    
    rtc.setAlarm(0,alarm); // match after 15 seconds
    rtc.enableAlarm(0, rtc.MATCH_HHMMSS); // match Every Day
    rtc.attachInterrupt(displayTime);
}

int totalFive=0;

/*
Nom de fonction : clockRaz
Les variables : - now : type : DateTime 
                      description : un variable qui aura le temps actuelle  
Les parametre :  aucun
Description : il change l'affichage du temps si
              le minute a changer ou l'heure 
*/

void clockRaz(){
  tft.setTextColor(TFT_BLACK, TFT_BLUE);
  DateTime now;
  now = rtc.now();
  
  
  // Serial.println("second : "+String(now.second()));
  if(now.hour()!=nowGenral.hour()){
    nowGenral=rtc.now();
    tft.drawString(String(nowGenral.hour()),230,10); 
  }
  if(now.minute()!=nowGenral.minute()){
    nowGenral=rtc.now();
    tft.drawString(String(nowGenral.minute()),270,10);

  }
  //  if(now.second()!=nowGenral.second()){
  //   nowGenral=rtc.now();
  //   tft.drawString(String(nowGenral.second()),300,10);

  // 
 
  tft.setTextColor(TFT_BLACK, colorGlobal);
}


/*
Nom de fonction : clockInitial
Les variables : aucun
Les parametre :  aucun
Description : il affiche l'heure au debut 
*/

void clockInitial(){

  tft.fillRect(0,0,320,50,TFT_BLUE);
  tft.drawString(String(nowGenral.hour()),230,10);
  tft.drawString(":",260,10);
  tft.drawString(String(nowGenral.minute()),270,10);
}


/*
Nom de fonction : initial
Les variables : aucun
Les parametre : aucun 
Description : il mettre tout les affichage visual pour 
              le page de compteur de visiteur(mainWindow) 
*/

void initial()
{
  tft.fillScreen(colorGlobal);
  tft.setTextColor(TFT_BLACK, TFT_BLUE);
  clockInitial();
  tft.drawString("+",10,0);
  tft.drawString("-",85,0);
  tft.drawString("RAZ",170,10);
  tft.setTextColor(TFT_BLACK, colorGlobal);
  tft.drawString("Visiteur    : ",10,70);
  tft.drawString("Affluence : ",10,120);
  tft.drawString(String(nbVisiteurTotal),150,70);
  tft.drawString(String(nbVisiteur),150,120);
  if(butA)
    tft.drawString("Confirmez ? ",80,200);
}

bool fiveSec=true;


/*
Nom de fonction : visite
Les variables : aucun
Les parametre : aucun
Description : il fait tout les calcul et il gere les button 
*/

int tempsActuel;
void visite()
{
  
if (butB) {
    if (nbVisiteur>0){
      nbVisiteur--;
    }
      butB=false;
  }
  if (butC) {
    nbVisiteurTotal++;
    nbVisiteur++;
    butC=false;
  }
  if (butA)
  {
    if (fiveSec)
    {
      if (digitalRead(WIO_5S_PRESS) == LOW)
      {
        nbVisiteur=0;
        nbVisiteurTotal=0;
        butA=false;
        Serial.println("-----------------------------");
        Serial.println("Remise des compteurs à zéro !");
        tft.fillRect(60,200,320,50,colorGlobal);
        
      }
    }
    
    
  }
 
 
  tft.fillRect(140,65,90,80,colorGlobal);
  tft.drawString(String(nbVisiteurTotal),150,70);
  tft.drawString(String(nbVisiteur),150,120);
  clockRaz();
  delay(170);
}


/*
Nom de fonction : mainWindow
Les variables : aucun
Les parametre : aucun 
Description : il fait le fenetre de principal 
*/
void mainWindow()
{
  while(true){
    DateTime now = DateTime(F(__DATE__), F(__TIME__));
    
    now = rtc.now();
    rtc.adjust(now);
    int tempsActuel2=now.minute()*60+now.second();
    if(abs(tempsActuel2-tempsActuel)<5){
      fiveSec=true;
      // Serial.println("tempsActuel2: "+String(tempsActuel2)+"tempsActuel: "+String(tempsActuel));
    }
    else{
        fiveSec=false;
        tft.fillRect(60,200,320,50,colorGlobal);
    }
    clockRaz();
    if (nbVisiteur>10)
      changeR=true;
    else
      changeG=true;
    if (changeR and amGreen)
    {
      colorGlobal=TFT_RED;
      changeG=false;
      changeR=false;
      amGreen=false;
      amRed=true;
      initial();
      if (message) {serialMessag();}
      music1();
    }
    if (changeG and amRed)
    {
      colorGlobal=TFT_GREEN;
      changeG=false;
      changeR=false;
      amRed=false;
      amGreen=true;
      initial();
      if (message) {serialMessag();}
    }
    if (digitalRead(WIO_KEY_A) == LOW){
      DateTime now = DateTime(F(__DATE__), F(__TIME__));

      now = rtc.now();
      rtc.adjust(now);
      tempsActuel=now.minute()*60+now.second();
      tft.drawString("Confirmez ? ",80,200);
      butA=true;
    }
      
    if (digitalRead(WIO_KEY_B) == LOW)
      butB=true;
    if (digitalRead(WIO_KEY_C) == LOW)
      butC=true;
    if (butA || butB || butC)
    {
      visite();
    }
    if(Serial.available() > 2 && !message && Serial.available() != NULL)
    {
      serialMessag();
      music2();
      message=true;
    }
    if (digitalRead(WIO_5S_UP) == LOW && message)
    {
      pageWindow = false;
      break;
    }
  }
}

void serialMessag()
{
  tft.drawString("Nouveau message !",50,160);
}

/*
Nom de fonction : mainBreak
Les variables : aucun
Les parametre : aucun 
Description : c'est le page de recevoir les message depuis le terminal
*/
void mainBreak()
{  
  if (Serial.available() > 2 && Serial.read() > 101) {
    tft.setTextColor(TFT_BLACK, TFT_WHITE);
    incomingByte = Serial.readString();
    Serial.println("-----------------");
    Serial.println("Message reçu !");
    Serial.println();
    tft.fillScreen(TFT_WHITE);
    tft.drawString(incomingByte,10,10);
    tft.drawString("DOWN pour sortir ",20,200);
    message=false;
  } else{

   while ( Serial.available() > 0 ) Serial.read();
    
  }
}



/*
Nom de fonction : music
Les variables : - i : type : int 
                      description : un compteur dans un for
Les parametre : aucun  
Description : il fait le music d'alert
*/
int length1=2;
char notes1[] = "ec";
int beats1[] = { 1, 1};
int tempo1 = 120;
void music1() {

    for(int i = 0; i < length1; i++) {
        if(notes1[i] == ' ') {
            delay(beats1[i] * tempo1);
        } else {
            playNote(notes1[i], beats1[i] * tempo1);
        }
        delay(tempo1 / 2);    /* delay between notes */
    }
 
}

/*
Nom de fonction : music
Les variables : - i : type : int 
                      description : un compteur dans un for
Les parametre : aucun  
Description : il fait le music de notification 
*/

int length2 = 5;         /* the number of notes */
char notes2[] = "ecfcg";
int beats2[] = { 1, 1, 1, 1, 1 };
int tempo2 = 100;
void music2() {

    for(int i = 0; i < length2; i++) {
        if(notes2[i] == ' ') {
            delay(beats2[i] * tempo2);
        } else {
            playNote(notes2[i], beats2[i] * tempo2);
        }
        delay(tempo2 / 2);    /* delay between notes */
    }
 
}
 
 
/*
Nom de fonction : playTone
Les variables : - i :  type : long 
                          description : c'est le tone de note
Les parametre :  - tone :  type : int 
                          description : un compteur dans un for
                - duration :  type : int 
                              description : c'est le durée souhaite
Description : il joue le tone d'un note
*/
void playTone(int tone, int duration) {
    for (long i = 0; i < duration * 1000L; i += tone * 2) {
        digitalWrite(BUZZER_PIN, HIGH);
        delayMicroseconds(tone);
        digitalWrite(BUZZER_PIN, LOW);
        delayMicroseconds(tone);
    }
}

/*
Nom de fonction : playNote
Les variables : - i :  type : long 
                          description : c'est le tone de note
Les parametre :  - note :  type : char 
                          description : le note jouer
                - duration :  type : int 
                              description : c'est le durée souhaite
Description : il joue un note 
*/ 
void playNote(char note, int duration) {
    char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
    int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };
 
    // play the tone corresponding to the note name
    for (int i = 0; i < 8; i++) {
        if (names[i] == note) {
            playTone(tones[i], duration);
        }
    }
}

void loop() 
{
  if (digitalRead(WIO_5S_UP) == LOW && message)
  {
    pageWindow = false;
  }
  if (digitalRead(WIO_5S_DOWN) == LOW)
  {
    initial();
    pageWindow = true;
  }

  if (pageWindow)
    mainWindow();
  if (!pageWindow)
    mainBreak();
}
