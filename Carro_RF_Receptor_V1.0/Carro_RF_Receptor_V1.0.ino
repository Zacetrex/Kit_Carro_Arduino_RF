// Producto de ZACETREX TECHNOLOGIES S.A.C
// Este código se hizo con fin de compartirlo con la comunidad y tiene fines educativos
// Más códigos en zacetrex.com

#include <SPI.h>   // Librería necesaria para comunicación SPI (usada por el NRF24L01)
#include <RF24.h>  // Librería para manejar el módulo NRF24L01

// Creamos el objeto radio indicando los pines CE y CSN
RF24 radio(7, 8);  

// Pines del driver L298N
int ENA = 3;   // Habilita y controla la velocidad del motor A (PWM)
int IN1 = 4;   // Control del sentido motor A
int IN2 = 5;   // Control del sentido motor A
int IN3 = 6;   // Control del sentido motor B
int IN4 = 9;   // Control del sentido motor B
int ENB = 10;  // Habilita y controla la velocidad del motor B (PWM)

// Arreglo donde se guardan los datos que llegan del control remoto
int joystick[6];

char instruction = 'x';      // Instrucción actual
char lastInstruction = 'x';  // Última instrucción ejecutada (para no repetir)

// Dirección para la comunicación NRF24L01
const byte identificacion[6] = "00001";

void setup() {
  Serial.begin(9600);   // Inicia el monitor serial
  radio.begin();        // Inicializa el módulo de radio

  // Abre un canal de lectura (pipe)
  radio.openReadingPipe(0, identificacion);

  // Ajusta la potencia del NRF24L01 (mínima para evitar interferencias)
  radio.setPALevel(RF24_PA_MIN);

  // Comienza a escuchar datos
  radio.startListening();

  // ENA y ENB se utilizan para controlar la velocidad del motor (PWM)
  // Puede modificarlo, el rango es de 0 a 255, donde 255 es la maxima velocidad.
  // No todos los motores son exactamente iguales, algunos no se moveran por lo 
  // que necesitan aumentar la velocidad, tambien si coloca mucho peso, 
  // no se moveran y tendra que aumentar la velocidad
  analogWrite(ENA, 150);  
  analogWrite(ENB, 150);
}

void loop() {

  // Verifica si llegó algún dato por el NRF24L01
  if (radio.available())
  {
    // Guarda los datos recibidos en el arreglo joystick
    radio.read(&joystick, sizeof(joystick));

    // Obtiene la instrucción según el joystick enviado
    instruction = GetInstruction();

    // Solo ejecuta el movimiento cuando la instrucción cambia
    if (instruction != lastInstruction) {

      lastInstruction = instruction;

      // Según la instrucción recibida, mueve el carro
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

      // Muestra la última instrucción en el monitor
      Serial.println(lastInstruction);
    }
  }
}

// Función que interpreta los valores del joystick y decide la instrucción
char GetInstruction()
{
  // Botones digitales (joystick[2], joystick[3], etc.)
  if (joystick[2] == LOW) return 'a';  // Adelante
  else if (joystick[3] == LOW) return 'b'; // Derecha
  else if (joystick[4] == LOW) return 'c'; // Atrás
  else if (joystick[5] == LOW) return 'd'; // Izquierda

  // Movimientos analógicos del joystick
  if(joystick[1] > 550) return 'a';  // Adelante
  else if(joystick[0] > 550) return 'b'; // Derecha
  else if(joystick[1] < 300) return 'c'; // Atrás
  else if(joystick[0] < 300) return 'd'; // Izquierda

  // Si nada coincide, se detiene
  return 'x';
}

// Funciones de movimiento del carro
// Cambian el estado lógico de las entradas del L298N para controlar el sentido de giro

void Adelante(){
  // Motor A hacia adelante
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  // Motor B hacia adelante
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void Derecha(){
  // Motor A hacia atrás
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  // Motor B hacia adelante
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  // Esto hace que el carro gire hacia la derecha
}

void Atras(){
  // Motor A hacia atrás
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  // Motor B hacia atrás
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void Izquierda(){
  // Motor A hacia adelante
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  // Motor B hacia atrás
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void Alto(){
  // Apaga ambos motores
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
