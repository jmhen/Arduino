#define Photosensitive A2   //光敏传感器接在控制板上的A2
#define LED 13

uint8_t brightness;

void setup()
{
  Serial.begin(9600);   //主要用于调试时打印数据，这个程序没有使用
  pinMode(Photosensitive, INPUT);
  pinMode(LED, OUTPUT);
}

uint8_t AdcChange(uint16_t x)
{
  brightness = map(x, 0, 1023, 100, 0);   //最亮为100，最暗为0，中间一一对应(x待转换的值，0,1023x的范围，100,0需要转换到的范围）
  return brightness;
}

void loop()
{
  Serial.print(AdcChange(analogRead(Photosensitive)));
  Serial.print("\n");
  if(AdcChange(analogRead(Photosensitive)) >= 15)   //亮度大于10的我们认为是没有物体遮住光敏传感器
  {
    digitalWrite(LED, HIGH);    //点亮13口led灯
  }
  else
  {  
    digitalWrite(LED, LOW);   //熄灭
  }
}
