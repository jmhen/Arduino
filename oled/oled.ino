#include <U8g2lib.h>

#define IR1 2   //红外传感器接的io口
//#define IR2 4
//#define IR3 10

U8G2_SSD1306_128X64_NONAME_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   //实例化

int count = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print("Setting up u8g2");
  u8g2.begin();
  u8g2.clearBuffer();
  
  Serial.print("Set Display Rotation");
  u8g2.setDisplayRotation(U8G2_R2);   //屏幕旋转180度

  Serial.print("Set Font");
  u8g2.setFont(u8g2_font_fub20_tr);   //设置字体   More fonts can be found here https://github.com/olikraus/u8g2/wiki/fntlistall
  
  u8g2.firstPage();

  Serial.print("Display something");
  
  do {//通电之后绘制LOBOT图样
    u8g2.setCursor(22, 38);   //位置
    u8g2.println("LOBOT");    //要绘制的字符
  } while ( u8g2.nextPage() );
  
  pinMode(IR1, INPUT);
//  pinMode(IR2, INPUT);
//  pinMode(IR3, INPUT);
  delay(1500);
}

void draw() {
  static uint32_t timer;
  if (timer < millis())   //用来延时
  {
    timer = millis() + 50;  //延时50ms，即下次运行到这里需要等50ms
    u8g2.firstPage();
    do {
      if(count == 0){
          u8g2.drawEllipse(15, 20, 15, 20, U8G2_DRAW_ALL); //画一个空心椭圆。(起始X，起始Y，椭圆的两个参数，画全圆)
          u8g2.drawEllipse(63, 20, 15, 20, U8G2_DRAW_ALL);
          u8g2.drawEllipse(112, 20, 15, 20, U8G2_DRAW_ALL);
          count++;
          Serial.print(count);
      }
      

      if (digitalRead(IR1) == LOW)    //红外检测到物体
      {
        Serial.print("Human detected. Start drawing a circle");
        u8g2.drawFilledEllipse(15, 20, 10, 15, U8G2_DRAW_ALL); //画一个实心椭圆。
        
      }
//      if (digitalRead(IR2) == LOW)
//      {
//        u8g2.drawFilledEllipse(63, 20, 10, 15, U8G2_DRAW_ALL);
//      }
//      if (digitalRead(IR3) == LOW)
//      {
//        u8g2.drawFilledEllipse(112, 20, 10, 15, U8G2_DRAW_ALL);
//      }
    } while ( u8g2.nextPage());
  }
}

void loop()
{
  draw();
}
