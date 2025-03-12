// Producto de ZACETREX TECHNOLOGIES S.A.C
// Este código se hizo con fin de compartirlo con la comunidad y tiene fines educativos
// Encuentre este y otros codigos en zacetrex.com o comuniquese con nosotros.

#include <SPI.h>
#include <RF24.h>

RF24 radio(7, 8);

int ENA = 3;
int IN1 = 4;
int IN2 = 5;
int IN3= 6;
int IN4= 9;
int ENB =10;
int joystick[6];

char instruction = 'x';
char lastInstruction = 'x';

const byte identificacion[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();

  radio.openReadingPipe(0, identificacion);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

// ENA y ENB se utilizan para controlar la velocidad del motor (PWM)
// Puede modificarlo, el rango es de 0 a 255, donde 255 es la maxima velocidad.
// No todos los motores son exactamente iguales, algunos no se moveran por lo 
// que necesitan aumentar la velocidad, tambien si coloca mucho peso, 
// no se moveran y tendra que aumentar la velocidad

  analogWrite(ENA,150);
  analogWrite(ENB,150);
}

void loop() {
  if (radio.available())
  {
    radio.read(&joystick, sizeof(joystick));
    instruction = GetInstruction();
    if (instruction != lastInstruction) {
      lastInstruction = instruction;
      switch(instruction)
      {
        case 'a':
        Serial.println("Adelante");
        Adelante();
        break;
        case 'b':
        Serial.println("Derecha");
        Derecha();
        break;
        case 'c':
        Serial.println("Atras");
        Atras();
        break;
        case 'd':
        Serial.println("Izquierda");
        Izquierda();
        break;
        case 'x':
        Serial.println("Alto");
        Alto();
        break;
      }
      Serial.println(lastInstruction);
    }

  }
}

char GetInstruction()
{
  if (joystick[2] == LOW)
  {
    return 'a';
  }
  else if (joystick[3] == LOW)
  {
    return 'b';
  }
  else if (joystick[4] == LOW)
  {
    return 'c';
  }
  else if (joystick[5] == LOW)
  {
    return 'd';
  }
  else if(joystick[1] > 550)
  {
    return 'a';
  }
  else if(joystick[0] > 550)
  {
    return 'b';
  }
  else if(joystick[1] < 300)
  {
    return 'c';
  }
  else if(joystick[0] < 300)
  {
    return 'd';
  }
  return 'x';
}


void Adelante(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void Derecha(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void Atras(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void Izquierda(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void Alto(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
