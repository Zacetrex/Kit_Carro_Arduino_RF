// Producto de ZACETREX TECHNOLOGIES S.A.C
// Este código se hizo con fin de compartirlo con la comunidad y tiene fines educativos
// Más códigos en zacetrex.com

// -------------------------------------------------------------
// TRANSMISOR – CONTROL REMOTO CON JOYSTICK
// Arduino UNO + Shield Joystick + NRF24L01
// -------------------------------------------------------------

#include <SPI.h>     // Librería necesaria para comunicación SPI (usada por el NRF24L01)
#include <RF24.h>    // Librería específica para manejar el NRF24L01

// Los pines A0 y A1 del Arduino también se pueden llamar 14 y 15.
// En este caso se usan para leer los ejes del joystick.
#define xAxis 14     // Eje X del joystick (izquierda ↔ derecha)
#define yAxis 15     // Eje Y del joystick (arriba ↔ abajo)

// Se crea el objeto "radio". Parámetros:
// CE en pin 9  -> Activa el módulo
// CSN en pin 10 -> Selección del módulo SPI
RF24 radio(9, 10);

// Botones del shield joystick (vienen en las líneas 2, 3, 4, 5)
int butonA = 2;
int butonB = 3;
int butonC = 4;
int butonD = 5;

// Arreglo que enviará: X, Y, y 4 botones
int joystick[6];

// Dirección (canal) para comunicarse con el receptor
// Debe ser IGUAL en transmisor y receptor
const byte identificacion[6] = "00001";

void setup()
{
  // Encender el módulo NRF24L01
  radio.begin();

  // Abrimos un "canal de escritura" (transmisión)
  radio.openWritingPipe(identificacion);

  // Nivel de potencia bajo (ahorra energía)
  radio.setPALevel(RF24_PA_MIN);

  // Le indicamos que solo va a transmitir, no recibir
  radio.stopListening();
  
  // Configuramos los botones como entradas con resistencia interna pull-up
  // Esto significa: sin presionar = HIGH, presionado = LOW
  pinMode(butonA, INPUT_PULLUP);
  pinMode(butonB, INPUT_PULLUP);
  pinMode(butonC, INPUT_PULLUP);
  pinMode(butonD, INPUT_PULLUP);

  // Aseguramos que los pines tengan estado inicial
  digitalWrite(butonA, LOW);
  digitalWrite(butonB, LOW);
  digitalWrite(butonC, LOW);
  digitalWrite(butonD, LOW);

  // Activamos el monitor serial (solo para ver valores)
  Serial.begin(9600);
}

void loop()
{
  // Leer joystick y botones
  GetLectura();

  // Enviar el arreglo completo por radio
  // "sizeof(joystick)" indica cuántos bytes enviar
  radio.write(&joystick, sizeof(joystick));

  delay(100); // Pausa pequeña para evitar saturar el canal
}

void GetLectura()
{
  // Leer el eje X del joystick (0–1023)
  joystick[0] = analogRead(xAxis);
  Serial.println(joystick[0]);

  // Leer eje Y
  joystick[1] = analogRead(yAxis);
  Serial.println(joystick[1]);

  // Leer botones (LOW = presionado)
  joystick[2] = digitalRead(butonA);
  Serial.println(joystick[2]);

  joystick[3] = digitalRead(butonB);
  Serial.println(joystick[3]);

  joystick[4] = digitalRead(butonC);
  Serial.println(joystick[4]);

  joystick[5] = digitalRead(butonD);
  Serial.println(joystick[5]);
}
