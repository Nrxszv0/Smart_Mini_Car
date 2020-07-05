//www.elegoo.com and I
#include <Servo.h>
Servo myservo;
#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11
#define LED 13

unsigned char carSpeed = 220;
bool state = LOW;
int Echo = A4;
int Trig = A5;
int rightDistance = 0, leftDistance = 0, middleDistance = 0, minDistance = 40;
int stopDly = 250, servoDly = 500, moveDly = 180;
int midServo = 90, leftServo = 10, rightServo = 180;
void forward() {
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Forward");
}

void back() {
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Back");
}

void left() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  Serial.println("Left");
}

void right() {
  analogWrite(ENA, carSpeed);
  analogWrite(ENB, carSpeed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  Serial.println("Right");
}
int Distance_test() {
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(20);
  digitalWrite(Trig, LOW);
  float Fdistance = pulseIn(Echo , HIGH);
  Fdistance = Fdistance / 58;
  return (int)Fdistance;
}

void stop() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  Serial.println("Stop!");
}

void stateChange() {
  state = !state;
  digitalWrite(LED, state);
  Serial.println("Light");
}

void setup() {
  myservo.attach(3, 700, 240);
  Serial.begin(9600);
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  stop();
}

void loop() {
  bluetooth();
  myservo.write(midServo);
  delay(stopDly);
  middleDistance = Distance_test();

  if (middleDistance <= minDistance) {
    stop();
    delay(stopDly);
    myservo.write(leftServo);
    delay(servoDly);
    rightDistance = Distance_test();

    delay(stopDly);
    //    myservo.write(midServo);
    //    delay(servoDly);
    myservo.write(rightServo);
    delay(servoDly);
    leftDistance = Distance_test();

    delay(stopDly);
    myservo.write(midServo);
    delay(servoDly);
    if (rightDistance > leftDistance) {
      right();
      delay(moveDly);
    }
    else if (leftDistance > rightDistance) {
      left();
      delay(moveDly);
    }
    else if ((rightDistance <= minDistance) && (leftDistance <= minDistance)) {
      back();
      delay(moveDly);
    }
    else {
      forward();
    }
  }
  else {
    forward();
  }
}

void bluetooth() {
  if (Serial.available())
  {
    char getstr = Serial.read();
    switch (getstr) {
      case 'f': forward(); break;
      case 'b': back();   break;
      case 'l': left();   break;
      case 'r': right();  break;
      case 's': stop();   break;
      case 'a': stateChange(); break;
//      case 'o': obstacleAvoidance();
      default:  break;
    }
  }
}
