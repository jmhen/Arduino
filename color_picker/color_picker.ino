#include <SoftWire.h>
#include "Adafruit_APDS9960.h"
#include <U8g2lib.h>

//add libraries for controlling learm
#include <SoftwareSerial.h>
#include <LobotServoController.h>

#define PhotosensitiveA A2 
#define PhotosensitiveB A0
#define LED 13

#define NIL 0
#define RED   1
#define GREEN 2
#define BLUE  3
#define YELLOW 4

//#define IR2 2

#define RxPin 9
#define TxPin 8


#define ACTIONGROUP_CHECK  0
#define ACTIONGROUP_RED    1
#define ACTIONGROUP_GREEN  2
#define ACTIONGROUP_BLUE   3
#define ACTIONGROUP_YELLOW 4
#define ACTIONGROUP_OTHER  5




Adafruit_APDS9960 apds;
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

//for controlling learm
SoftwareSerial mySerial(RxPin, TxPin);
LobotServoController myController(mySerial);

uint8_t brightness;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);

  pinMode(PhotosensitiveA, INPUT);
  pinMode(PhotosensitiveB, INPUT);
  pinMode(LED, OUTPUT);
  
  if(!apds.begin()){
    Serial.println("failed to initialize device! Please check your wiring.");
  }
  else Serial.println("Device initialized!");

  //enable color sensign mode
  apds.enableColor(true);

  //Display logo
  u8g2.begin();
  u8g2.setDisplayRotation(U8G2_R2);
  u8g2.setFont(u8g2_font_fub20_tr);
  u8g2.firstPage();
  do {
    u8g2.setCursor(22, 38);
    u8g2.println("LOBOT");
  } while ( u8g2.nextPage() );

  //Test Run Action Group
//  pinMode(IR2, INPUT);
//  myController.runActionGroup(0, 1);

  delay(500);
}

int colorDetect()
{
  uint16_t r, g, b, c;
  int t;
  int mx;
  //wait for color data to be ready
  while(!apds.colorDataReady()){
    delay(5);
  }
  apds.getColorData(&r, &g, &b, &c);
  
  //Find the largest value in R, G, B. For example, the maximum is R means that the object is Red
  if (r > g){
    mx = r;   
    t = RED; 
    if( r-g < 40 ) t = YELLOW;
    } 
  else{
    mx = g;
    t = GREEN;
    if( g-r < 40 ) t = YELLOW;
    }

  if (b > mx){
    mx = b;
    t = BLUE;
  }

  
  Serial.print("R:"); Serial.print(r);
  Serial.print("G:"); Serial.print(g);
  Serial.print("B:"); Serial.println(b);

  //Returns the color only if the RGB value is greater than 30, otherwise returns 0
  if(t == BLUE && b > 80)
    return t;
  else if(t == GREEN && g > 80)
    return t;
  else if(t == RED && r > 80)
    return t;
  else if(t == YELLOW && r >80 )
    return t;
  else 
    return 0;
  return 0;
}


int color = 5;
bool refreshNow = true;

//Draw screen image
void draw()
{
  
  if (refreshNow == true)
  {
    refreshNow = false;
    u8g2.firstPage();
    do
    {
      u8g2.setFont(u8g2_font_courB24_tf);
      u8g2.setCursor(0, 38);
      //Display color
      if (color == NIL)
        u8g2.print(" FAIL");
      if (color == RED)
        u8g2.print("plastic");
      if (color == BLUE)
        u8g2.print("paper");
      if (color == GREEN)
        u8g2.print("glass");
      if (color == YELLOW)
        u8g2.print("metal");
    } while ( u8g2.nextPage());
  }
}

uint8_t AdcChange(uint16_t x)
{
  brightness = map(x, 0, 1023, 100, 0);   //最亮为100，最暗为0，中间一一对应(x待转换的值，0,1023x的范围，100,0需要转换到的范围）
  return brightness;
}

void run()
{
  static uint32_t timer;
  static uint8_t step;
  if (timer < millis())
  {
    switch (step)
    {
      case 0:
        
      // Execute NO.0 action group to move the object over the color sensor, if an object is detected by obscuring light.

          //Only start processing when there is object on station 1 but no object on station 2
          if(AdcChange(analogRead(PhotosensitiveA)) <= 15)  //亮度大于15的我们认为是没有物体遮住光敏传感器
          {
            Serial.print("light intensity: ");
            Serial.println(AdcChange(analogRead(PhotosensitiveA)));
            digitalWrite(LED, HIGH);    //点亮13口led灯
            delay(500);
            myController.runActionGroup(ACTIONGROUP_CHECK, 1);
            Serial.println("ActtionGroup: check");
            timer = millis() + 7000; //rough duration of action
            step = 1;
          }
          else
          {  
            digitalWrite(LED, LOW);   //熄灭
            timer = millis() + 50;
          }
          
        color = 5;
        refreshNow = true; //Refresh the screen.
        break;



        
      case 1:
        step = 0;
        //The action group is finished, which means that the object has been moved above the color sensor.
        if (myController.isRunning() == false)
        {
          step = 2;
          timer = millis() + 300;

        }
        break;


        
      case 2:
      //Detect the color of objects.
        color = colorDetect();
        Serial.print("color detected: "); Serial.println(color);
        draw();
        if (color == RED)
        {
//          Serial.print("station 1: "); Serial.println(AdcChange(analogRead(PhotosensitiveA)));
//          Serial.print("station 2: "); Serial.println(AdcChange(analogRead(PhotosensitiveB)));
          while(AdcChange(analogRead(PhotosensitiveB))<=15){}
          delay(1000);
          Serial.println("Station 2 Freed!");
          myController.runActionGroup(ACTIONGROUP_RED, 1);  
          Serial.println("ActtionGroup: RED");
          
        }
        else if (color == GREEN)
        {
          myController.runActionGroup(ACTIONGROUP_GREEN, 1); 
          Serial.println("ActtionGroup: GREEN");
        }
        else if (color == BLUE)
        {
          myController.runActionGroup(ACTIONGROUP_BLUE, 1); 
          Serial.println("ActtionGroup: BLUE");
        }

        else if (color == YELLOW)
        {
          myController.runActionGroup(ACTIONGROUP_YELLOW, 1); 
          Serial.println("ActtionGroup: BLUE");
        }
        else
        {
          myController.runActionGroup(ACTIONGROUP_OTHER , 1);  
          Serial.println("ActtionGroup: UNKNOWN");
        }
        refreshNow = true; //Refresh the screen to display the color.
        step = 3;
        timer = millis() + 200;
        break;


        
      case 3:
      //Returen case 0 until the action group finish
        if (myController.isRunning() == false)
        {
          step = 0;
          timer = millis() + 1000;
        }
        break;
     default:
      step = 0;
      break;
    }
  }
}


void loop() {
  myController.receiveHandler(); 
  run();
  
}
