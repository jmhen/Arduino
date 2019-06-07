#include <LobotServoController.h>
//直到一个动作组运行后运行下一个动作组
#define rxPin 10
#define txPin 11

SoftwareSerial mySerial(rxPin, txPin);
LobotServoController myse(mySerial);

void setup() {
  pinMode(13,OUTPUT);
  mySerial.begin(9600);
  Serial.begin(9600);
}
void loop() {
  static int i = 0;
  myse.receiveHandle();  //接收处理，用到 isRunning就不可缺少，并且应该确保它能过被定时或及时地执行，处理接收数据，获得动作组运行状态
  if(!myse.isRunning())  //isRunning() 会返回是否有动作组在运行，若是有的话返回true，若没有（就是动作组运行完成，停止或是从没有运行过）的话就返回false
  {
    Serial.print("Run Action:");
    Serial.println(i);
    myse.runActionGroup(i,1); //没有动作组在运行的话就运行动作组
    if(++i > 2)  //0，1，2 三个动作组循环运行
      i = 0;
  }
}
