#define TRIG    10
#define ECHO    11
#define MAX_DISTANCE 400
#define LED  13

float distance;
void getDistance() {
  static uint32_t timer;
  if (timer < millis())
  {
    digitalWrite(TRIG, HIGH); 
    delayMicroseconds(10); 
    digitalWrite(TRIG, LOW); 
    distance = distance * 0.5 + ((((float)pulseIn(ECHO, HIGH, MAX_DISTANCE * 58.8)) / 58.8) * 0.5);

    timer = millis() + 50;
  }
}

void setup() {
  Serial.begin(9600);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(TRIG, LOW);
  digitalWrite(LED, LOW);
}

int value;
void flash()
{
  static uint32_t timer;
  if (timer < millis())
  {
    timer = millis() + value;
    digitalWrite(LED, !digitalRead(LED));
  }
}

void loop() {
  getDistance();
  Serial.print(distance);
  Serial.print("\n");
  if (distance > 5 && distance < 25)
  {//当距离在5-25时led闪烁的快慢跟距离有关系
    value = map(distance*100, 500, 2500, 50, 500); //Converts the distance to the corresponding time，about map https://www.arduino.cc/en/Reference/Map
    //将距离映射到50-500来当做led闪烁的间距
    flash();
  } 
  else if (distance > 0 && distance < 5)
  {
    digitalWrite(LED, LOW);
  } 
  else 
  {
    digitalWrite(LED, HIGH);
  }
}
