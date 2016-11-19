//#include <Servo.h>
#include <SoftwareSerial.h>
#include <QueueList.h>

char getstr;
QueueList <char> pathQueue; // 路徑佇列

int Echo = A1;  // Echo回声脚(P2.0)
int Trig = A0;  //  Trig 触发脚(P2.1)

int distance = 0;
int Left_motor_go = 8;
int Left_motor_back = 9;
int Right_motor_go = 10;
int Right_motor_back = 11;

int key = A2;
int beep = A3;

const int SensorRight = 3;
const int SensorLeft = 4;

char popQueue;

int SL;
int SR;

int speed_high = 120;
int speed_normal = 80;
int speed_zero = 0;
int speed_begin = 100;

int isFork = 0; // 遇到叉路

void setup(){
    pinMode(Left_motor_go, OUTPUT); // PIN 8 (PWM)
    pinMode(Left_motor_back, OUTPUT); // PIN 9 (PWM)
    pinMode(Right_motor_go, OUTPUT); // PIN 10 (PWM)
    pinMode(Right_motor_back, OUTPUT); // PIN 11 (PWM)
    pinMode(key, INPUT);
    pinMode(beep, OUTPUT);
    pinMode(Echo, INPUT);    // 定义超声波输入脚
    pinMode(Trig, OUTPUT);   // 定义超声波输出脚
    pinMode(SensorRight, INPUT);
    pinMode(SensorLeft, INPUT);
    Serial.begin(9600);
}

void run(){
    digitalWrite(Right_motor_go, HIGH);
    digitalWrite(Right_motor_back, LOW);
    analogWrite(Right_motor_go, speed_normal);
    analogWrite(Right_motor_back, speed_zero);
    digitalWrite(Left_motor_go, LOW);
    digitalWrite(Left_motor_back, HIGH);
    analogWrite(Left_motor_go, speed_zero);
    analogWrite(Left_motor_back, speed_normal);
}

void fast_run(){
    digitalWrite(Right_motor_go, HIGH);
    digitalWrite(Right_motor_back, LOW);
    analogWrite(Right_motor_go, speed_high);
    analogWrite(Right_motor_back, speed_zero);
    digitalWrite(Left_motor_go, LOW);
    digitalWrite(Left_motor_back, HIGH);
    analogWrite(Left_motor_go, speed_zero);
    analogWrite(Left_motor_back, speed_high);
}

void stop(){
    digitalWrite(Right_motor_go, LOW);
    digitalWrite(Right_motor_back, LOW);
    digitalWrite(Left_motor_go, LOW);
    digitalWrite(Left_motor_back, LOW);
} 

void left(){
    digitalWrite(Right_motor_go, HIGH);
    digitalWrite(Right_motor_back, LOW);
    analogWrite(Right_motor_go, speed_normal);
    analogWrite(Right_motor_back, speed_zero);
    digitalWrite(Left_motor_go, LOW);
    digitalWrite(Left_motor_back, LOW);
    analogWrite(Left_motor_go, speed_zero);
    analogWrite(Left_motor_back, speed_zero);
}

void spin_left(int time){
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

void spin_right(int time){
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

void back(int time){
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

void new_right(){
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

void keyscan(){
    int val;
    val = digitalRead(key);
    while(!digitalRead(key)){
        val = digitalRead(key);
    }
    while (digitalRead(key)){
        delay(10);
        val = digitalRead(key);
        if (val == HIGH){
            digitalWrite(beep, HIGH);
            while (!digitalRead(key))
                digitalWrite(beep, LOW);
        }
        else{
            digitalWrite(beep, LOW);
        }
    }
}

bool isStop()   // 量出前方距离 
{
  digitalWrite(Trig, LOW);   // 给触发脚低电平2μs
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  // 给触发脚高电平10μs，这里至少是10μs
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);    // 持续给触发脚低电
  distance = pulseIn(Echo, HIGH);  // 读取高电平时间(单位：微秒)
  distance = distance/58;       //为什么除以58等于厘米，  Y米=（X秒*344）/2
  if(distance <= 10){
    stop();
    return 1;
  }
  return 0;
}

void choice() {
    getstr = Serial.read();
    if (getstr == 'L' || getstr == 'l' || 
        getstr == 'R' || getstr == 'r' ||
        getstr == 'F' || getstr == 'f' ||
        getstr == 'B' || getstr == 'b') {
        pathQueue.push(getstr);
  
        //Serial.println(pathQueue.peek());
    }
    delay(300);
}

void loop() {
    keyscan();
  
    while (1) {
        if (!isFork){
           
              SR = digitalRead(SensorRight);
              SL = digitalRead(SensorLeft);
              if (SL == LOW && SR == LOW) {
                  if(!isStop()){
                    run();
                  }
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
                  isFork = 1;
              }
              
        }
        else if (isFork) { // 如果遇到叉路
            //bool isFirst = 1;
            while(!pathQueue.isEmpty()){
//                if(isFirst){
//                    digitalWrite(beep, HIGH);
//                    isFirst = 0;
//                    delay(200);
//                    digitalWrite(beep, LOW);
//                }
                popQueue = pathQueue.peek();
                pathQueue.pop();
                if (popQueue == 'L' || popQueue == 'l'){
                    spin_left(5.5);
                }
                else if (popQueue == 'R' || popQueue == 'r'){
                    new_right();
                    delay(450);
                }
                else if (popQueue == 'F' || popQueue == 'f') {
                    fast_run();
                    delay(200);
                }
                else if (popQueue == 'B' || popQueue == 'b') {
                    spin_right(5.5);
                }
            }
            isFork = 0;
        }
    }
}
