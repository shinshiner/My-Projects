#define RIGHT_BACKWARD 10
#define RIGHT_FORWARD 9
#define LEFT_BACKWARD 13
#define LEFT_FORWARD 12
#define STEER 14
int duty = 1580;
int CYCLE = 8;
int run = 130;
int fast = 70;
char command;
void Forward();
void TurnLeft();
void TurnRight();
void Backward();
void BackLeft();
void BackRight();
void Stop();
void Steer(float ratio);
void setup()
{
  Serial.begin(9600);
  pinMode(LEFT_BACKWARD,OUTPUT);
  pinMode(RIGHT_BACKWARD,OUTPUT); 
  pinMode(LEFT_FORWARD,OUTPUT);
  pinMode(RIGHT_FORWARD,OUTPUT);
  pinMode(STEER,OUTPUT);
  digitalWrite(LEFT_FORWARD,HIGH);
  digitalWrite(RIGHT_FORWARD,HIGH);
  digitalWrite(LEFT_BACKWARD,HIGH);
  digitalWrite(RIGHT_BACKWARD,HIGH);
  digitalWrite(STEER,HIGH);
}

void loop()
{
  if (Serial.available()>0)
  {
    command=Serial.read();
    if (command=='F')
    {
      Serial.println("Forward");
      Forward();
      Serial.read();
    }
    else if (command=='B')
    {
      Serial.println("Backward");
      Backward();
      Serial.read();
    }
    else if (command=='L')
    {
      Serial.println("TurnLeft");
      TurnLeft();
      Serial.read();
    }
    else if (command=='R')
    {
      Serial.println("TurnRight");
      TurnRight();
      Serial.read();
    }
    else if (command=='l')
    {
      Serial.println("BackLeft");
      BackLeft();
      Serial.read();
    }
    else if (command=='r')
    {
      Serial.println("BackRight");
      BackRight();
      Serial.read();
    }
    else if (command=='S')
    {
      Serial.println("Stop");
      Stop();
      Serial.read();
    }
  }
}

void Forward()
{
  digitalWrite(LEFT_BACKWARD,HIGH);
  digitalWrite(RIGHT_BACKWARD,HIGH);  
  analogWrite(LEFT_FORWARD,run);
  analogWrite(RIGHT_FORWARD,run);
  Steer(1.0);
}

void Backward()
{
  digitalWrite(LEFT_FORWARD,HIGH);
  digitalWrite(RIGHT_FORWARD,HIGH);  
  analogWrite(LEFT_BACKWARD,run);
  analogWrite(RIGHT_BACKWARD,run);
  Steer(1.0);
}

void TurnLeft()
{
  analogWrite(RIGHT_FORWARD,run);
  analogWrite(LEFT_BACKWARD,fast);  
  digitalWrite(LEFT_FORWARD,HIGH);
  digitalWrite(RIGHT_BACKWARD,HIGH);
  Steer(0.9);
}

void TurnRight()
{
  
  analogWrite(LEFT_FORWARD,run);
  analogWrite(RIGHT_BACKWARD,fast);
  digitalWrite(RIGHT_FORWARD,HIGH);
  digitalWrite(LEFT_BACKWARD,HIGH);
  Steer(1.1);
}

void BackLeft()
{
  analogWrite(LEFT_BACKWARD,run);
  analogWrite(RIGHT_BACKWARD,fast);  
  digitalWrite(LEFT_FORWARD,HIGH);
  digitalWrite(RIGHT_FORWARD,HIGH);
  Steer(0.9);
}

void BackRight()
{
  analogWrite(RIGHT_BACKWARD,run);
  analogWrite(LEFT_BACKWARD,fast);  
  digitalWrite(LEFT_FORWARD,HIGH);
  digitalWrite(RIGHT_FORWARD,HIGH);
  Steer(1.1);
}

void Stop()
{
  digitalWrite(LEFT_BACKWARD,HIGH);
  digitalWrite(RIGHT_BACKWARD,HIGH);  
  digitalWrite(LEFT_FORWARD,HIGH);
  digitalWrite(RIGHT_FORWARD,HIGH);
  Steer(1.0);
}

void Steer(float ratio)
{
  int temp = ratio*duty;
  for (int i = 0;i < CYCLE;i++)
  {
    digitalWrite(STEER,HIGH);
    delayMicroseconds(temp);
    digitalWrite(STEER,LOW);
    delayMicroseconds(10000-temp);
    delayMicroseconds(6500);
  }
}
