#include <SoftwareSerial.h>
#include <LobotServoController.h>
#include <U8g2lib.h>

#define RxPin 9
#define TxPin 8
#define LED   13

#define TOUCH 2   //触摸传感器

int result;
bool refreshNow = true;

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
SoftwareSerial mySerial(RxPin, TxPin);
LobotServoController myController(mySerial);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(LED, OUTPUT);
  pinMode(TOUCH, INPUT);
  digitalWrite(LED, LOW);
  u8g2.begin();
  u8g2.setDisplayRotation(U8G2_R2);  //Rotate the picture 180 degrees
  u8g2.setFont(u8g2_font_fub20_tr);
  u8g2.firstPage();
  do {
    u8g2.setCursor(22, 38);
    u8g2.println("LOBOT");
  } while ( u8g2.nextPage() );
  myController.runActionGroup(9, 1);
  delay(1500);
}

void draw() {
  if (refreshNow ==  true)
  {
    refreshNow = false;
    u8g2.firstPage();
    do {
      u8g2.setFont(u8g2_font_courB24_tf);
      u8g2.setCursor(52, 38);
      if (result > 9) 
        u8g2.setCursor(41, 38);
      u8g2.println(result);  
    } while ( u8g2.nextPage());
  }
}

void run()
{
  static uint8_t step;
  static uint32_t timer;
  static uint16_t count;
  if (myController.isRunning())
    return;
  if (timer > millis())
    return;
  switch(step)
  {
    case 0:
      if (!digitalRead(TOUCH))
      {//检测触摸传感器
        delay(80);
        if (digitalRead(TOUCH))
        {//如果是短按一次
          digitalWrite(LED, !digitalRead(LED));
          result ++ ;
          refreshNow = true;
          count = 0;
          step = 1;
          timer = millis() + 200;
        }
      }
      break;
    case 1:
      if (!digitalRead(TOUCH))
      {//检测触摸传感器
        delay(80);
        if (digitalRead(TOUCH))
        {//如果是短按一次
          result ++ ;
          digitalWrite(LED, !digitalRead(LED));
          if (result > 3)
            result = 3;
          count = 0;
          refreshNow = true;
          timer = millis() + 200;
        }
      }
      else
      {
        count++;
        if (count > 400)
        {
          step = 2;
          count = 0;
        }
        else
        {
          timer = millis() + 1;
        }
      }
      break;
    case 2:
      myController.runActionGroup(result,1);    
      count++;
      if (count > 150)  
      {
        refreshNow = true;
        count = 0;
        result = 0;
        step = 0;
      }
      break;
    default:
      result = 0;
      count = 0;
      step = 0;
      break; 
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  myController.receiveHandler();
  run();    
  draw();  
}
