/*
Esta utiliza las bibliotecas:
http://www.prometec.net/nrf2401/
https://www.arduino.cc/en/Tutorial/WebClientRepeating
https://github.com/DavyLandman/AESLib
 */
/**
 * Bibilioteca arduino
 */
#include <Arduino.h>
/*Fin*/

/**
 * Dependencias módulo de Ethernet
 */
#include <Ethernet.h>
#include <SPI.h>
/*Fin*/

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
RF24 radio(9, 53);
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL }; // LongLong = 64 bits.
/*Fin*/

void configSerial() {
  // Open serial communications and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
}

void configRF() {
  printf_begin();
  printf("\n\rnRF24L01 MEGA 2560\n\r");
  Serial.println("Config RF 2.4GHz");
  pinMode(53, OUTPUT); //Only for mega

  radio.begin();
  radio.setChannel(124);
  radio.setPALevel(RF24_PA_MIN);
  radio.setDataRate(RF24_2MBPS);
  radio.setAutoAck(1);                     // Ensure autoACK is enabled
  //radio.setCRCLength(RF24_CRC_8);          // Use 8-bit CRC for performance
  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15, 15);
  // optionally, reduce the payload size.  seems to
  // improve reliability
  //radio.setPayloadSize(8);

  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);

  radio.startListening();
  delay(200);
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging
  Serial.println("End Config RF 2.4GHz");
}

void setup() {
  configSerial();
  configRF();
}


void interactRF() {
  // Importante!!! Borrar true
  if (radio.available()) { // Si hay datos disponibles
      Serial.print("Recibi");
  //if (true || radio.available()) { // Solo para pruebas
    char got_isla[2];
    bool done = false;
    while (!done) {
      // Espera aqui hasta recibir algo

      done = radio.read(&got_isla, 2);
      Serial.print("Dato Recibido = ");
      Serial.print((int)got_isla[0]);
      Serial.print(",");
      Serial.println((int)got_isla[1]);
      delay(20); // Para dar tiempo al emisor
    }

    // radio.stopListening(); // Dejamos d escuchar para poder hablar
    //
    // radio.write(&got_isla, 2);
    // Serial.println("Enviando Respuesta");
    // radio.startListening(); // Volvemos a la escucha para recibir mas
  }
}

void loop() {
  interactRF();
  // delay(1000); // Quitar, solo sirve para pruebas
}
