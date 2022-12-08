
#include <SPI.h>              // include libraries
#include <LoRa.h>
#include "protocol.h"

int counter = 0;

void callback(uint8_t topic[TOPIC_SIZE],uint8_t message[MESSAGE_SIZE]){
  // Serial.print("Topic: ");
  // Serial.println((char*)topic);
  Serial.print("Message: ");
  Serial.println((char*)message);
  send_message("pool/sensor1",String((char*)message));
}

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("LoRa Sender");
  delay(5000);
  
  subscribe("pool/sensor");
  set_callback(callback);
  setup_lora(0xF3);
}

long timer = 0;        // last send time
void loop() {

  lora_loop();
}
