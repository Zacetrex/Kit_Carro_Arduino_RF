// Producto de ZACETREX TECHNOLOGIES S.A.C
// Este código se hizo con fin de compartirlo con la comunidad y tiene fines educativos
// Encuentre otros codigos en zacetrex.com o comuniquese con nosotros.

#include <SPI.h>
#include <RF24.h>
#define xAxis 14
#define yAxis 15

RF24 radio(9, 10);

int butonA = 2;
int butonB = 3;
int butonC = 4;
int butonD = 5;
int joystick[6];
const byte identificacion[6] = "00001";

void setup()
{
  radio.begin();
  radio.openWritingPipe(identificacion);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  
  pinMode(butonA, INPUT_PULLUP);
  pinMode(butonB, INPUT_PULLUP);
  pinMode(butonC, INPUT_PULLUP);
  pinMode(butonD, INPUT_PULLUP);

  digitalWrite(butonA, LOW);
  digitalWrite(butonB, LOW);
  digitalWrite(butonC, LOW);
  digitalWrite(butonD, LOW);
  Serial.begin(9600);
}

void loop()
{
  GetLectura();
  radio.write(&joystick, sizeof(joystick));

  delay(100); 
}

void GetLectura()
{
  joystick[0] = analogRead(xAxis);
  Serial.println(joystick[0]);
  joystick[1] = analogRead(yAxis);
  Serial.println(joystick[1]);
  joystick[2] = digitalRead(butonA);
  Serial.println(joystick[2]);
  joystick[3] = digitalRead(butonB);
  Serial.println(joystick[3]);
  joystick[4] = digitalRead(butonC);
  Serial.println(joystick[4]);
  joystick[5] = digitalRead(butonD);
  Serial.println(joystick[5]);
}
