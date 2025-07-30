#include <Servo.h>

#define PIN_LEFT_MOTOR_ENABLE   8  // ENB
#define PIN_LEFT_MOTOR_BACK     10 // IN1
#define PIN_LEFT_MOTOR_FORWARD  11 // IN2

#define PIN_RIGHT_MOTOR_ENABLE  7  // ENA
#define PIN_RIGHT_MOTOR_BACK    6  // IN2
#define PIN_RIGHT_MOTOR_FORWARD 9  // IN2

#define PIN_SONAR_ECHO          A0
#define PIN_SONAR_TRIGGER       A1

#define SERVO_FORWARD           130
#define SERVO_LEFT              190
#define SERVO_RIGHT             70

int Fspeedd = 0;      // forward distance
int Rspeedd = 0;      // right distance
int Lspeedd = 0;      // left distance
int directionn = 0;   //
Servo myservo;        // new myservo
int delay_time = 250; // set stable time
int Fgo = 8;
int Rgo = 6;
int Lgo = 4;
int Bgo = 2;

void setup()
{
    Serial.begin(9600);
    pinMode(PIN_LEFT_MOTOR_ENABLE, OUTPUT);
    pinMode(PIN_LEFT_MOTOR_BACK, OUTPUT);
    pinMode(PIN_LEFT_MOTOR_FORWARD, OUTPUT);

    pinMode(PIN_RIGHT_MOTOR_ENABLE, OUTPUT);
    pinMode(PIN_RIGHT_MOTOR_BACK, OUTPUT);
    pinMode(PIN_RIGHT_MOTOR_FORWARD, OUTPUT);
    
    pinMode(PIN_SONAR_ECHO, INPUT);
    pinMode(PIN_SONAR_TRIGGER, OUTPUT);
    
    myservo.attach(5);

    digitalWrite(PIN_LEFT_MOTOR_ENABLE, HIGH);
    digitalWrite(PIN_RIGHT_MOTOR_ENABLE, HIGH);
}
void advance(int a) // forward
{
    digitalWrite(PIN_RIGHT_MOTOR_BACK, LOW);
    digitalWrite(PIN_RIGHT_MOTOR_FORWARD, HIGH);
    digitalWrite(PIN_LEFT_MOTOR_BACK, LOW);
    digitalWrite(PIN_LEFT_MOTOR_FORWARD, HIGH);
    delay(a * 15);
}
void turnR(int d) //turn right
{
    digitalWrite(PIN_RIGHT_MOTOR_BACK, LOW);
    digitalWrite(PIN_RIGHT_MOTOR_FORWARD, HIGH);
    digitalWrite(PIN_LEFT_MOTOR_BACK, HIGH);
    digitalWrite(PIN_LEFT_MOTOR_FORWARD, LOW);
    delay(d * 50);
}
void turnL(int e) //turn left
{
    digitalWrite(PIN_RIGHT_MOTOR_BACK, HIGH);
    digitalWrite(PIN_RIGHT_MOTOR_FORWARD, LOW);
    digitalWrite(PIN_LEFT_MOTOR_BACK, LOW);
    digitalWrite(PIN_LEFT_MOTOR_FORWARD, HIGH);
    delay(e * 50);
}
void stopp(int f) //stop
{
    digitalWrite(PIN_RIGHT_MOTOR_BACK, HIGH);
    digitalWrite(PIN_RIGHT_MOTOR_FORWARD, HIGH);
    digitalWrite(PIN_LEFT_MOTOR_BACK, HIGH);
    digitalWrite(PIN_LEFT_MOTOR_FORWARD, HIGH);
    delay(f * 100);
}
void back(int g) //back
{
    digitalWrite(PIN_RIGHT_MOTOR_BACK, HIGH);
    digitalWrite(PIN_RIGHT_MOTOR_FORWARD, LOW);
    digitalWrite(PIN_LEFT_MOTOR_BACK, HIGH);
    digitalWrite(PIN_LEFT_MOTOR_FORWARD, LOW);
    delay(g * 300);
}
void detection() //test the distance of different direction
{
    int delay_time = 250; //
    delay(200);
    ask_pin_F();      // read forward distance
    if (Fspeedd < 10) // if distance less then 10
    {
        stopp(1);
        back(2);
    }
    if (Fspeedd < 25) // if distance less then 10
    {
        stopp(1);
        ask_pin_L();
        delay(delay_time);
        ask_pin_R();
        delay(delay_time);
        if (Lspeedd > Rspeedd) //if left distance more than right distance
        {
            directionn = Rgo;
        }
        if (Lspeedd <= Rspeedd) //if left distance not more than right distance
        {
            directionn = Lgo;
        }
        //if left distance and right distance both less than 10
        if (Lspeedd < 10 && Rspeedd < 10)
        {
            directionn = Bgo;
        }
    }
    else
    {
        directionn = Fgo; // forward go
    }
}
void ask_pin_F() // test forward distance
{
    myservo.write(SERVO_FORWARD);
    digitalWrite(PIN_SONAR_TRIGGER, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_SONAR_TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_SONAR_TRIGGER, LOW);
    float Fdistance = pulseIn(PIN_SONAR_ECHO, HIGH);
    Fdistance = Fdistance / 5.8 / 10;
    Serial.print("F distance:");
    Serial.println(Fdistance);
    Fspeedd = Fdistance;
}
void ask_pin_L() // test left distance
{
    myservo.write(SERVO_LEFT);
    delay(delay_time);
    digitalWrite(PIN_SONAR_TRIGGER, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_SONAR_TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_SONAR_TRIGGER, LOW);
    float Ldistance = pulseIn(PIN_SONAR_ECHO, HIGH);
    Ldistance = Ldistance / 5.8 / 10;
    Serial.print("L distance:");
    Serial.println(Ldistance);
    Lspeedd = Ldistance;
}
void ask_pin_R() // test right distance
{
    myservo.write(SERVO_RIGHT);
    delay(delay_time);
    digitalWrite(PIN_SONAR_TRIGGER, LOW);
    delayMicroseconds(2);
    digitalWrite(PIN_SONAR_TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(PIN_SONAR_TRIGGER, LOW);
    float Rdistance = pulseIn(PIN_SONAR_ECHO, HIGH);
    Rdistance = Rdistance / 5.8 / 10;
    Serial.print("R distance:");
    Serial.println(Rdistance);
    Rspeedd = Rdistance;
}
void loop()
{
    myservo.write(SERVO_FORWARD);

    analogWrite(PIN_RIGHT_MOTOR_BACK, 0);
    analogWrite(PIN_RIGHT_MOTOR_FORWARD, 0);
    
    analogWrite(PIN_LEFT_MOTOR_BACK, 0);
    analogWrite(PIN_LEFT_MOTOR_FORWARD, 0);

    
    //detection();
    if (directionn == 2)
    {
        back(3);
        turnL(2);
        Serial.print(" Reverse ");
    }
    if (directionn == 6)
    {
        back(1);
        turnR(6);
        Serial.print(" Right ");
    }
    if (directionn == 4)
    {
        back(1);
        turnL(6);
        Serial.print(" Left ");
    }
    if (directionn == 8)
    {
        advance(1);
        Serial.print(" Advance ");
        Serial.print(" ");
    }
}
