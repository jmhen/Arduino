#include <SoftwareSerial.h>
#include <LobotServoController.h>
#include <U8g2lib.h>

#define RxPin 9
#define TxPin 8
#define LED   13

#define Photosensitive A2
#define SOUND A0

#define ACTIONGROUP_CLAMP   11

uint8_t volume;
uint8_t brightness;
uint8_t result;

SoftwareSerial mySerial(RxPin, TxPin);  //Set an example for SoftwareSerial  
LobotServoController myController(mySerial); //Set an example forLobotSerialController 
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

void setup() {
  // put your setup code here, to run once:
  mySerial.begin(9600);  //Set mySerial baud rate to 9600
  Serial.begin(9600);
  u8g2.begin();
  u8g2.setDisplayRotation(U8G2_R2);
  u8g2.setFont(u8g2_font_fub20_tr);
  u8g2.firstPage();
  do {
    u8g2.setCursor(22, 38);
    u8g2.println("LOBOT");
  } while ( u8g2.nextPage() );
  
  pinMode(LED, OUTPUT);  
  pinMode(Photosensitive, INPUT);  //Configure the LS pin as input
  pinMode(SOUND, INPUT);  // Configure the  SOUND pin as 
  myController.runActionGroup(0, 1);
  delay(1500);
}

uint8_t AdcChange(uint16_t x)
{
  brightness = map(x, 0, 1023, 100, 0);
  return brightness;
}

void run()
{
  static uint32_t timer;
  if (myController.isRunning() == false)
  {
    //Serial.println(AdcChange(analogRead(Photosensitive)));    
    if (AdcChange(analogRead(Photosensitive)) > 20)  //当光敏被遮住时
    {
      if (AdcChange(analogRead(SOUND)) <= 90) //如果检测到声音
      {
        myController.runActionGroup(ACTIONGROUP_CLAMP, 1); //运行1号动作组
        timer = millis() + 200;
      }
    } 
  }
}

void loop()
{
  myController.receiveHandler();
  run();
}

