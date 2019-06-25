/* @file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
*/
#include <Keypad.h>
#include <Servo.h>


#define TRIG    10
#define ECHO    11

#define SERVO   12
#define MAX_DISTANCE 400
#define BUZZER  13

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6,7,8,9}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
Servo servo;
int angle = 25;
String PIN = "320";


void setup(){
  Serial.begin(9600);
  servo.attach(SERVO);
  servo.write(angle);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(BUZZER,OUTPUT);
  digitalWrite(TRIG, LOW);

}

float distance;
void getDistance() {
  static uint32_t timer;
  if (timer < millis())
  {
    digitalWrite(TRIG, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(TRIG, LOW); 
    distance = distance * 0.5 + ((((float)pulseIn(ECHO, HIGH, MAX_DISTANCE * 58.8)) / 58.8) * 0.5);

    timer = millis() + 100;
  }
}

void playUpTone(){
     tone(BUZZER, 261 );
     delay(500); //等待1000毫秒
     tone(BUZZER, 329 );
     delay(500); //等待1000毫秒
     tone(BUZZER, 392 );
     delay(500); //等待1000毫秒
     noTone(BUZZER);//停止发声
}

void playSuccessTone(){
     tone(BUZZER, 261 );
     delay(500); //等待1000毫秒
     tone(BUZZER, 329 );
     delay(500); //等待1000毫秒
     tone(BUZZER, 392 );
     delay(500); //等待1000毫秒
     tone(BUZZER, 523 );
     delay(500); //等待1000毫秒
     noTone(BUZZER);//停止发声
}

void playFailTone(){
     tone(BUZZER, 220 );
     delay(500); //等待1000毫秒
     noTone(BUZZER);//停止发声
}

void loop(){
  static uint32_t timer;
  getDistance();
 

  // this checkes if 4 is pressed, then do something. Here  we print the text but you can control something.
  if (distance > 1 && distance <15){
     Serial.print("object approaching: "); Serial.println(distance);
     playUpTone();
     timer = millis() + 10000;
     Serial.println(timer);
     String password = String("");
     uint8_t count = 0;
     char key;
     while(millis() < timer){
        key = keypad.getKey();
        if(key == 35) break;
        if(key){
          Serial.println(key);
          password += key; 
          count += 1;
        } 
     }
     Serial.println("Detected #: end of password");
     if(password.equals(PIN)){
        Serial.print("Password Correct!");
         //play sound
         playSuccessTone();

        delay(500);
        for(angle = 25; angle <= 120; angle++){                                  
          servo.write(angle);               
          delay(15);                   
        } 

        while(distance<30){//there is something infront
        getDistance();
        }

        Serial.print("Object leaving. Distance: "); Serial.println(distance);
        delay(1000);
        for(angle = 120; angle >= 25; angle--){                                  
           servo.write(angle);               
           delay(15);                   
        } 
     }else{
        Serial.print("Password Wrong!");
        playFailTone();
     }
     
     
  }
}
