//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 866E6

#define durationSleep  30   // secondes
//packet counter
int counter = 0;

void setup() {
  
  //initialize Serial Monitor
  Serial.begin(115200);
  delay(2000);
  //Serial.println("Prueba de envio LoRa");

  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);
  
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  //Serial.println("LoRa Inicializacion OK!");
  delay(2000);
}

void loop() 
{
  // LoRa esta habilitat
  //LoRa.idle();
  Serial.print("Enviando paquete: ");
  Serial.println(counter);

  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print("BIO");
  LoRa.print(counter);
  LoRa.endPacket();

  counter++;
  // LoRa se'n va a dormir
  //LoRa.sleep();
  // Wait
  delay(durationSleep * 1000);
}
