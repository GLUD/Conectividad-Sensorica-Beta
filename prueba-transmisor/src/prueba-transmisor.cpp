/*
* Arduino Wireless Communication Tutorial
*     Example 2 - Transmitter Code
*
* by Dejan Nedelkovski, www.HowToMechatronics.com
*
* Library: TMRh20/RF24, https://github.com/tmrh20/RF24/
*/
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define led 4

RF24 radio(9, 10); // CNS, CE
const byte addresses[][6] = {"00001", "00002"};
boolean buttonState = 0;
void setup() {
  Serial.begin(115200);
  pinMode(4, OUTPUT);
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00001
  radio.openReadingPipe(1, addresses[0]); // 00002
  //radio.setPALevel(RF24_PA_HIGH);
  Serial.println("Termine.");
}
void loop() {
  delay(5);
  radio.stopListening();
  int valor = 234;
  bool ok = radio.write(&valor, sizeof(valor));
  if (ok) {
    Serial.println("ok...");
  } else {
    Serial.println("failed");
  }

  delay(5);
  Serial.print("Escribi: ");
  Serial.println(valor);
  radio.startListening();
  while (!radio.available())
  radio.read(&buttonState, sizeof(buttonState));
  if (buttonState == HIGH) {
    digitalWrite(led, HIGH);
    Serial.println("Activo");
  }
  else {
    digitalWrite(led, LOW);
    Serial.println("Inactivo");
  }
}
