#include "Keyboard.h"

int key = 15;
int check = 0;
int count = 0;
int character[5] = {0,0,0,0,0};


char alphabet[64] =
{'E','T','I','A','N','M','S','U','R','W','D','K','G',
'O','H','V','F','\0','L','Ä','P','J','B','X','C','Y','Z',
'Q','Ö','\0','5','4','\0','3','\0','\0','\0','2','&','\0','+',
'\0','\0','Å','\0','1','6','=','/','\0','\0','\0','(','\0','7',
'\0','\0','\0','8','\0','9','0','\0'};


long msh; //ms held
long space; //ms not held

byte prev = HIGH;

unsigned long fpress;
unsigned long fspace;

void setup() {
Serial.begin(57600); // begin serial communication at 115200 baud rate
pinMode(key, INPUT_PULLUP);
}

void translate(){
  int n = 0;
  
  if (character[4] != 0){
    n = (((character[0]+character[0]+character[1]) *2 +character[2])*2 +character[3])*2 +character[4];
  }
  else if (character[3] != 0){
    n = ((character[0]+character[0]+character[1]) *2 +character[2])*2 +character[3];
  }
  else if (character[2] != 0){
    n = (character[0]+character[0]+character[1]) *2 +character[2];
  }
  else if (character[1] != 0){
    n = character[0]+character[0]+character[1];
  }
  else{
    n = character[0];
  }
  
  if(alphabet[n-1] != '\0'){
    //Serial.print(alphabet[n-1]);
    Keyboard.print(alphabet[n-1]);
  }
  
  int i;
  for(i = 0; i < 5; i++){
    //Serial.print(character[i]);
    character[i] = 0;
  }
  count = 0;
}

void loop() {
  int cur = digitalRead(key);

  if (cur == LOW && prev == HIGH && (millis() - fpress)) {
    fpress = millis();
  }

  msh = (millis() - fpress);

    if (cur == HIGH && prev == LOW) {
      if (msh > 30 && msh < 150) {
        character[count] = 1;
        count++;
        //Serial.print(msh);
        //Serial.print(" dot ");
      }
      else if (msh >= 150) {
         character[count] = 2;
         count++;
         //Serial.print(msh);
         //Serial.print(" dash ");
      }
      fspace = millis();
      check = 1;
    }
    else{
      space = (millis() - fspace);
      
      if(space > 550 && space < 3000 && check == 1){ //new character
        check = 2;
        count = 0;
        translate();
        //Serial.print(" ");
      }
      else if(space > 3000 && space < 7000 &&check == 2){ // new word
        //Serial.print(" ");
        Keyboard.print(" ");
        check = 3;
      }
      else if(space > 7000 &&check == 3){ //new sentence
        //Serial.print("\b.\n");
        //Keyboard.print("\b.\n");
        check = 0;
      }
    }
    
    prev = cur;
}

/*

A .-
B -...
C -.-.
D -..
E .
F ..-.
G --.
H ....
I ..
J .---
K -.-
L .-..
M --
N -.
O ---
P .--.
Q --.-
R .-.
S ...
T -
U ..-
V ...-
W .--
X -..-
Y -.--
Z --..
Å .--.-
Ä .-.-
Ö ---.

1 .----
2 ..---
3 ...--
4 ....-
5 .....
6 -....
7 --...
8 ---..
9 ----.
0 -----

& .-...
+ .-.-.
/ -..-.
= -...-
( -.--.

*/
