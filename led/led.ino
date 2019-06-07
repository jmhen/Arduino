#define IR1 2   //定义IR1为接2号IO口的红外传感器
#define LED 13  //定义LED为接13号IO口的LED，13号IO口一般是arduino板子自带的LED，在板子上标为L或者L13
 
void setup()
{
  pinMode(IR1, INPUT);    //将IR1配置为输入(输入状态一般是将要读取这个引脚的状态，即读取传感器的反馈值）
  pinMode(LED, OUTPUT);   //将LED配置为输出（输出和输入相反，一般是要给这个引脚一个电平从而控制和它相接的传感器）
  Serial.begin(9600); // open the serial port at 9600 bps:
}

void loop()
{
  if(digitalRead(IR1) == LOW)   //读取IR1口的状态，当读取为低电平时，表示红外传感器有检测到物体
  {
    Serial.print("no body\n");
    digitalWrite(LED, HIGH);    //给LED口一个高电平，即点亮led
  }
  else
  {
    Serial.print("got somebody\n");
    digitalWrite(LED, LOW);   //给LED口一个高电平，即熄灭led
  }
}
