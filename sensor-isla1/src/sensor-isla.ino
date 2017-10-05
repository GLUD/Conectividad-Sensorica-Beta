/*  ----------------------------------------------------------------
    http://www.prometec.net/duplex-nrf2401
    Prog_79B_Emisor

    Usando un NRF2401 para comunicar dos Arduinos en modo Duplex
    Programa Emisor:
   --------------------------------------------------------------------
 */

/**
  * Dependencias módulo RF
  */
#include "RF24.h"
#include "nRF24L01.h"
#include <SPI.h>
#include "printf.h" // For test purpose
/*Fin*/


/**
 * Variables Módulo RF
 */
RF24 radio(9, 10);
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL }; // LongLong = 64 bits.
/*Fin*/

/**
 * Variables Módulo Ultrasonido
 */
#define ledEstado 4 // Indicador para deteccion del vehículo
#define trigPin 6
#define echoPin 5

int limiteInferior = 10; // Distancia max entre el suelo y el vehiculo (cms)
int limiteSuperior = 50; // Distancia max entre el suelo y el vehiculo (cms)
// 212 cms Toyota Hilux
long duracion, distancia;
static char idIsla = 1;
volatile byte estadoIsla = 1;
/*Fin*/

/**
 * Variables Módulo Ultrasonido
 */
const int pinBateria = A0; // Distancia max entre el suelo y el vehiculo (cms)
/*Fin*/

void setup(void) {
  configSerial();
  configRF();
  configUltrasonido();
}

void configSerial() { Serial.begin(115200); }

void configRF() {
  printf_begin();
  printf("\n\rnRF24L01+ SENSOR ISLA\n\r");
  Serial.println("Config RF 2.4GHz");
  pinMode(10, OUTPUT); // solo para MEGA

  radio.begin();                           // Setup and configure rf radio
  radio.setChannel(124);
  //radio.setPALevel(RF24_PA_MIN);
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_1MBPS);
  radio.setAutoAck(1);                     // Ensure autoACK is enabled
  //radio.setCRCLength(RF24_CRC_8);          // Use 8-bit CRC for performance
  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15, 15);
  //radio.setPayloadSize(8);

  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);

  radio.startListening();                 // Start listening
  //radio.stopListening();
  delay(500);
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging
  Serial.println("End Config RF 2.4GHz");
}

void configUltrasonido() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(ledEstado, OUTPUT);
}

void configBateria(){
  pinMode(pinBateria, INPUT);
}

void loop(void) {
  enviarDatoRF();
  medirDistancia();
  medirBateria();
  delay(500);
}

void enviarDatoRF() {
  radio.stopListening(); // Paramos la escucha para poder hablar
  char msg[2] = {idIsla, (char)estadoIsla};
  // unsigned long time = millis();
  Serial.print("Enviando: ");
  Serial.print((int)msg[0]);
  Serial.print(",");
  Serial.println((int)msg[1]);
  // bool ok = radio.write(&time, sizeof(unsigned long));
  bool ok = radio.write(msg, 2);

  if (ok) {
    Serial.println(F("ok..."));
  } else {
    Serial.println(F("failed!!!"));
  }

  radio.startListening(); // Volvemos a la escucha
  //
  // unsigned long started_waiting_at = millis();
  // bool timeout = false;
  // while (!radio.available() && !timeout) { // Esperasmos repsuesta hasta
  // 200ms
  //   if (millis() - started_waiting_at > 200) {
  //     timeout = true;
  //   }
  // }
  //
  // if (timeout) {
  //   Serial.println("FailedEstado, response timed out");
  // } else { // Leemos el mensaje recibido
  //   char got_isla[2];
  //   radio.read(&got_isla, 2);
  //
  //   Serial.print("Respuesta = ");
  //   Serial.print((int)got_isla[0]);
  //   Serial.println((int)got_isla[1]);
  // }
}

void medirDistancia() {
  digitalWrite(trigPin,
               LOW); // Nos aseguramos de que el trigger está desactivado
  // delayMicroseconds(2);              // Para asegurarnos de que el trigger
  // esta LOW
  digitalWrite(trigPin, HIGH); // Activamos el pulso de salida
  // delayMicroseconds(10);             // Esperamos 10µs. El pulso sigue active
  // este tiempo
  digitalWrite(trigPin, LOW); // Cortamos el pulso y a esperar el echo
  duracion = pulseIn(echoPin, HIGH);
  distancia = duracion / 2 / 29.1;
  if (limiteInferior < distancia && distancia < limiteSuperior) {
    Serial.println(String(distancia) + F(" cm. INSIDE"));
    digitalWrite(ledEstado, HIGH);
    estadoIsla = 1;
  } else {
    Serial.println(String(distancia) + F(" cm. OUTSIDE"));
    digitalWrite(ledEstado, LOW);
    estadoIsla = 0;
  }
  // delay (500) ;                  // Para limitar el número de mediciones
}

void medirBateria() {
  double a = analogRead(pinBateria);
  Serial.println(String(a/10.23) + F("% Bateria"));
}
