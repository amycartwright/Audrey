int PM1=9; //define Pulse pin
int PM2=7; //define Pulse pin
int DM1=8; //define Direction pin
int DM2=6; //define Direction pin
int down, up, mx, mn;
//Pin
void setup() {
  mx = down = 4000;
  mn = up = 500;
  pinMode (PM1, OUTPUT);
  pinMode (DM1, OUTPUT);
  
  pinMode (PM2, OUTPUT);
  pinMode (DM2, OUTPUT);
//  pinMode (ENA, OUTPUT);
Serial.begin(9600);

}

void loop() {
  Serial.println(down);
  for (int i=0; i<400; i++)    //Forward 5000 steps
  {
    digitalWrite(DM1,LOW);
    digitalWrite(DM2,LOW);

    digitalWrite(PM1,HIGH);
    digitalWrite(PM2,HIGH);
    delayMicroseconds(20);
    digitalWrite(PM1,LOW);
    digitalWrite(PM2,LOW);
    delayMicroseconds(down);
  }
    Serial.println(up);

  for (int i=0; i<400; i++)   //Backward 5000 steps
  {
    digitalWrite(DM1,HIGH);
    digitalWrite(DM2,HIGH);

    digitalWrite(PM1,HIGH);
    digitalWrite(PM2,HIGH);
    delayMicroseconds(20);
    digitalWrite(PM1,LOW);
    digitalWrite(PM2,LOW);
    delayMicroseconds(up);
  }
  up+=500;
  down-=500;
  if(up > mx) up = mn;
  if(down < mn) down = mx;
}
