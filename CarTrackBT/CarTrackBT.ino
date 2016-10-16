//#include <Servo.h>
#include <SoftwareSerial.h>
#include <QueueList.h>

char getstr;
//char str_guiding[100];
QueueList <char> pathQueue;

int Left_motor_go = 8;
int Left_motor_back = 9;
int Right_motor_go = 10;
int Right_motor_back = 11;

int key = A2;
int beep = A3;

const int SensorRight = 3;
const int SensorLeft = 4;

int SL;
int SR;

int speed_high = 120;
int speed_normal = 30;
int speed_zero = 0;
int speed_begin = 100;

int count = 0;
int flag = 0;
int a = 0;
int i = 0;
int j = 0;
int xflag = 0;

void setup()
{
  pinMode(Left_motor_go, OUTPUT); // PIN 8 (PWM)
  pinMode(Left_motor_back, OUTPUT); // PIN 9 (PWM)
  pinMode(Right_motor_go, OUTPUT); // PIN 10 (PWM)
  pinMode(Right_motor_back, OUTPUT); // PIN 11 (PWM)
  pinMode(key, INPUT);
  pinMode(beep, OUTPUT);
  pinMode(SensorRight, INPUT);
  pinMode(SensorLeft, INPUT);
  Serial.begin(9600);
}

void run()
{
  digitalWrite(Right_motor_go, HIGH);
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, speed_normal);
  analogWrite(Right_motor_back, speed_zero);
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, HIGH);
  analogWrite(Left_motor_go, speed_zero);
  analogWrite(Left_motor_back, speed_normal);
}

void fast_run()
{
  digitalWrite(Right_motor_go, HIGH);
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, speed_high);
  analogWrite(Right_motor_back, speed_zero);
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, HIGH);
  analogWrite(Left_motor_go, speed_zero);
  analogWrite(Left_motor_back, speed_high);
}

void stop()
{
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, LOW);
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, LOW);
}

void left()
{
  digitalWrite(Right_motor_go, HIGH);
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, speed_normal);
  analogWrite(Right_motor_back, speed_zero);
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_go, speed_zero);
  analogWrite(Left_motor_back, speed_zero);
}

void spin_left(int time)
{
  digitalWrite(Right_motor_go, HIGH);
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, speed_high);
  analogWrite(Right_motor_back, speed_zero);
  digitalWrite(Left_motor_go, HIGH);
  digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_go, speed_high);
  analogWrite(Left_motor_back, speed_zero);
  delay(time * 100);
}

void right()
{
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, speed_zero);
  analogWrite(Right_motor_back, speed_zero);
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, HIGH);
  analogWrite(Left_motor_go, speed_zero);
  analogWrite(Left_motor_back, speed_normal);
}

void spin_right(int time)
{
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, HIGH);
  analogWrite(Right_motor_go, speed_zero);
  analogWrite(Right_motor_back, speed_high);
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, HIGH);
  analogWrite(Left_motor_go, speed_zero);
  analogWrite(Left_motor_back, speed_high);
  delay(time * 100);
}

void back(int time)
{
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, HIGH);
  analogWrite(Right_motor_go, speed_zero);
  analogWrite(Right_motor_back, speed_normal);
  digitalWrite(Left_motor_go, HIGH);
  digitalWrite(Left_motor_back, LOW);
  analogWrite(Left_motor_go, speed_normal);
  analogWrite(Left_motor_back, speed_zero);
  delay(time * 100);
}

void new_right()
{
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, LOW);
  analogWrite(Right_motor_go, speed_zero);
  analogWrite(Right_motor_back, speed_zero);
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, HIGH);
  analogWrite(Left_motor_go, speed_zero);
  analogWrite(Left_motor_back, speed_high);
}
//==========================================================

void keyscan()
{
  int val;
  val = digitalRead(key);
  while (!digitalRead(key))
  {
    val = digitalRead(key);
  }
  while (digitalRead(key))
  {
    delay(10);
    val = digitalRead(key);
    if (val == HIGH)
    {
      digitalWrite(beep, HIGH);
      while (!digitalRead(key))
        digitalWrite(beep, LOW);
    }
    else
      digitalWrite(beep, LOW);
  }
}



void choice() {
  getstr = Serial.read();
  if (getstr == 'L' || getstr == 'l' || 
      getstr == 'R' || getstr == 'r' ||
      getstr == 'U' || getstr == 'u' ||
      getstr == 'D' || getstr == 'd') {
        pathQueue.push(getstr);

        Serial.println(pathQueue.peek());
  }
  delay(300);
}

void loop() {
  keyscan();
  
  while (1) {
    if (flag == 0) {
      SR = digitalRead(SensorRight);
      SL = digitalRead(SensorLeft);
      if (SL == LOW && SR == LOW) {
        run();
      }
      else if (SL == HIGH & SR == LOW) {
        left();
      }
      else if (SR == HIGH & SL == LOW) {
        right();
      }
      else {
        stop();
        choice();
        flag = 1;
      }
    }
    if (flag == 1 ) {
      while(!pathQueue.isEmpty()){
          char popQueue = pathQueue.peek();
          pathQueue.pop();
          if (popQueue == 'L' || popQueue == 'l'){
            spin_left(5);
          }
          else if (popQueue == 'R' || popQueue == 'r'){
            new_right();
            delay(400);
          }
          else if (popQueue == 'U' || popQueue == 'u') {
            fast_run();
            delay(200);
          }
          else if (popQueue == 'D' || popQueue == 'd') {
            spin_right(6.5);
          }
      }
      flag = 0;
    }
  }
}
