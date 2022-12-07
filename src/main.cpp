
#include <SPI.h>              // include libraries
#include <LoRa.h>
#include "protocol.h"

       // change for your board; must be a hardware interrupt pin
int counter = 0;


void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Sender");
  setup_lora(1);
  delay(5000);
  Serial.println("sending sub packet");
}

void onReceive(int packetSize){
  if (packetSize == 0) return; 
  
  uint8_t buffer[PACKET_SIZE];
  bzero(buffer,PACKET_SIZE);

  for(int i = 0; i < packetSize; i++){
    buffer[i] = (uint8_t)LoRa.read();
  }
  Packet packet = parse_packet(buffer);
  Serial.println("Got packet...");
  if(packet.network_id != NETWORK_ID){
    Serial.println("network ids dont match");
    Serial.print("packet networkid: ");
    Serial.println(packet.network_id);
    Serial.print("my networkid: ");
    Serial.println(NETWORK_ID);
    return;
  }
  Message_packet message_packet = parse_message_packet(packet.data);
  Serial.println((char*)message_packet.message);
}

long lastSendTime = 0;        // last send time
int interval = 2000;  
void loop() {
  if (millis() - lastSendTime > interval) {
    counter++;
    Serial.print("Sending packet: ");
    String message = "HeLoRa World!";   // send a message

    send_message("ek sit hier en werk");
    Serial.println("Sending " + message);
    lastSendTime = millis();            // timestamp the message
    interval = 5000;    // 2-3 seconds
  }
  onReceive(LoRa.parsePacket());
}
