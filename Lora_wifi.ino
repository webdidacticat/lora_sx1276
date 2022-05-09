/*
 *  This sketch demonstrates how to scan WiFi networks.
 *  The API is almost the same as with the WiFi Shield library,
 *  the most obvious difference being the different file you need to include:
 */
 
#include <WiFi.h>
#include <LoRa.h>
#include <SPI.h>

#ifndef STASSID
#define STASSID "ssid"
#define STAPSK  "password"
#endif

#define durationSleepWifi  60   // secondes
#define NB_TRYWIFI     1000   // nbr d'essai connexion WiFi, number of try to connect WiFi

const char* ssid     = STASSID;
const char* password = STAPSK;

const char* host = "192.168.xx.xx";
const uint16_t port = 80;

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

String msg_lora,msg_lora2;
int i = 0;

void setup()
{
    Serial.begin(115200);

    init_WIFI();
    init_LORA();

    Serial.println("Setup done");
}

void loop()
{
    // LoRa esta habilitat
    //LoRa.idle();
    // Recibimos dato por LORA
    LORAreceive();
    // Enviamos el dato a la web
    if(i==1){
      WIFIsend();
    }
    // LoRa se'n va a dormir
    //LoRa.sleep();
    // Wait
    //ESP.deepSleep(durationSleepWifi * 1000000);
}

void init_WIFI()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  // Start Fixed IP
  IPAddress ip(192,168,xx,xx);   
  IPAddress gateway(192,168,xx,xx);   
  IPAddress subnet(255,255,255,0);   
  WiFi.config(ip, gateway, subnet);
  Serial.println("Start Connection");
  int _try = 0;
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("..");
    delay(500);
    _try++;
    if ( _try >= NB_TRYWIFI ) {
        Serial.println("Impossible to connect WiFi network, go to deep sleep");
        ESP.deepSleep(durationSleepWifi * 1000000);
    }
  }
  Serial.println("Connected to the WiFi network");
}

void init_LORA()
{
  Serial.println("LoRa Receiver Test");
  
  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initializing OK!");
}

void LORAreceive()
{
    //try to parse packet
  i=0;
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    //received a packet
    Serial.print("Received packet ");

    //read packet
    while (LoRa.available()) {
      msg_lora = LoRa.readString();
      Serial.print(msg_lora);
      i=1;
    }

    //print RSSI of packet
    int rssi = LoRa.packetRssi();
    Serial.print(" with RSSI ");    
    Serial.println(rssi);
    msg_lora2 = msg_lora + "&rssi=" + rssi;
    Serial.println(msg_lora2);
  }
}

void WIFIsend()
{
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    delay(5000);
    return;
  }
  // This will send the request to the server
  client.print("GET /lora.php?lora=");
  client.print(msg_lora2);
  client.println(" HTTP/1.1");
  client.println("Host: 192.168.xx.xx");
  client.println("Connection: close");
  client.println();
  while(client.connected() || client.available())
  {
    String line = client.readStringUntil('\r');
    //Serial.print(line);
  }
  //Serial.println();

  //Serial.println("closing connection");
  client.stop();

}
