#define LED 13  //定义LED为接13号IO口的LED，13号IO口一般是arduino板子自带的LED，在板子上标为L或者L13
 
void setup()
{
  pinMode(LED, OUTPUT);   //将LED配置为输出（输出和输入相反，一般是要给这个引脚一个电平从而控制和它相接的传感器）
}

void loop()
{
    digitalWrite(LED, HIGH);    //给LED口一个高电平，即点亮led
    delay(1000);
    digitalWrite(LED, LOW);   //给LED口一个高电平，即熄灭led
    delay(1000);
    /*
     digitalToggle(LED);
     delay(500);
     */
}
