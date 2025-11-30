// Producto de ZACETREX TECHNOLOGIES S.A.C
// Este código se hizo con fin de compartirlo con la comunidad y tiene fines educativos
// Más códigos en zacetrex.com

// -------------------------------------------------------------
// RECEPTOR – CARRO ROBOT
// Arduino UNO + L298N + NRF24L01
// -------------------------------------------------------------

#include <SPI.h>     // Comunicación SPI para NRF24L01
#include <RF24.h>    // Librería del NRF24L01

// Pines del NRF24L01 en el receptor
// CE -> pin 7
// CSN -> pin 8
RF24 radio(7, 8);

// Pines del driver L298N
int ENA = 3;   // Controla velocidad del motor A (PWM)
int IN1 = 4;   // Sentido motor A
int IN2 = 5;

int IN3 = 6;   // Sentido motor B
int IN4 = 9;
int ENB = 10;  // Velocidad motor B (PWM)

// Arreglo para recibir los datos del transmisor
int joystick[6];

// Variable para decidir hacia dónde moverse
char instruction = 'x';      // 'x' = detener
char lastInstruction = 'x';  // Guarda la última instrucción para evitar repetir

// Dirección del canal (igual al transmisor)
const byte identificacion[6] = "00001";

void setup() {

  Serial.begin(9600);

  // Inicializamos el radio
  radio.begin();

  // Abrimos canal de lectura 0 con la identificación indicada
  radio.openReadingPipe(0, identificacion);

  // Potencia baja
  radio.setPALevel(RF24_PA_MIN);

  // Modo recepción
  radio.startListening();

  // Configurar velocidad de los motores
  // PWM (0–255), aquí velocidad media (150)
  analogWrite(ENA,150);
  analogWrite(ENB,150);
}

void loop() {

  // Verificamos si hay datos disponibles en el radio
  if (radio.available())
  {
    // Guardamos los datos recibidos dentro del array joystick[]
    radio.read(&joystick, sizeof(joystick));

    // Interpretar el movimiento dependiendo del joystick
    instruction = GetInstruction();

    // Solo actuamos si la instrucción cambió
    if (instruction != lastInstruction) {

      lastInstruction = instruction;

      // Elegir acción según la letra recibida
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
  // Botones primero (tienen prioridad)
  if (joystick[2] == LOW)  return 'a'; // Botón A → Adelante
  if (joystick[3] == LOW)  return 'b'; // Botón B → Derecha
  if (joystick[4] == LOW)  return 'c'; // Botón C → Atrás
  if (joystick[5] == LOW)  return 'd'; // Botón D → Izquierda

  // Joystick analógico
  // Valores arriba de 550 = empujado hacia un lado
  // Valores menores a 300 = empujado hacia el lado opuesto

  if (joystick[1] > 550) return 'a'; // Adelante
  if (joystick[0] > 550) return 'b'; // Derecha
  if (joystick[1] < 300) return 'c'; // Atrás
  if (joystick[0] < 300) return 'd'; // Izquierda

  // Si no se detecta nada, se detiene
  return 'x';
}

// -------------------------------------------------------------
// Funciones que indican cómo mover los motores del L298N
// -------------------------------------------------------------

void Adelante(){
  digitalWrite(IN1, LOW);   // Motor A adelante
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);   // Motor B adelante
  digitalWrite(IN4, HIGH);
}

void Derecha(){
  digitalWrite(IN1, HIGH);  // Motor A atrás
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);   // Motor B adelante
  digitalWrite(IN4, HIGH);
}

void Atras(){
  digitalWrite(IN1, HIGH);  // Motor A atrás
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);  // Motor B atrás
  digitalWrite(IN4, LOW);
}

void Izquierda(){
  digitalWrite(IN1, LOW);   // Motor A adelante
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);  // Motor B atrás
  digitalWrite(IN4, LOW);
}

void Alto(){
  // Apaga ambos motores
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
