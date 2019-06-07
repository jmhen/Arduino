#include <SoftWire.h>
#include "Adafruit_APDS9960.h"
#include <U8g2lib.h>

Adafruit_APDS9960 apds;
U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define RED   1
#define GREEN 2
#define BLUE  3
#define YELLOW 4

//int R_F = 13000;
//int G_F = 19000;
//int B_F = 25000;
//int r_f = 768;
//int g_f = 1024;
//int b_f = 1280;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

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

//  r = map(r, r_f, R_F, 0, 255);  
//  g = map(g, g_f, G_F, 0, 255);
//  b = map(b, b_f, B_F, 0, 255);
  
  //Find the largest value in R, G, B. For example, the maximum is R means that the object is Red
  if (r > g){
    mx = r;   
    t = RED; 
    if( r-g < 20 ) t = YELLOW;
    } 
  else{
    mx = g;
    t = GREEN;
    if( g-r < 20 ) t = YELLOW;
    }

  if (b > mx){
    mx = b;
    t = BLUE;
  }

  
  Serial.print("R:"); Serial.print(r);
  Serial.print("G:"); Serial.print(g);
  Serial.print("B:"); Serial.println(b);

  //Returns the color only if the RGB value is greater than 30, otherwise returns 0
  if(t == BLUE && b > 50)
    return t;
  else if(t == GREEN && g > 50)
    return t;
  else if(t == RED && r > 50)
    return t;
  else if(t == YELLOW && r >50 )
    return t;
  else 
    return 0;
  return 0;
}

//Draw screen image
void draw()
{
  int t;
  t = colorDetect();
  u8g2.firstPage();
  do
  {
    u8g2.setFont(u8g2_font_courB24_tf);
    u8g2.setCursor(0, 38);
    //Display color
    if (t == RED)
      u8g2.print("plastic");
    if (t == BLUE)
      u8g2.print("paper");
    if (t == GREEN)
      u8g2.print("glass");
    if (t == YELLOW)
      u8g2.print("metal");
  } while ( u8g2.nextPage());
}

void loop() {
  draw();
  delay(200);
}
