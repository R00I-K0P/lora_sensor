#include <Arduino.h>
#include <LoRa.h>
#define PACKET_SIZE 256
#define TOPIC_SIZE 32
#define MESSAGE_SIZE PACKET_SIZE-TOPIC_SIZE-2

#define frequency 868E6  

#define csPin 18         
#define resetPin 23      
#define irqPin 26 
#define TOPIC_ID "sensors/1"
#define NETWORK_ID 0xF3

//packet_type:0 - message
//packet_type:1 - sub
//packet_type:2 - unsub



typedef struct __attribute__((__packed__)) Packet{
    uint8_t network_id;
    uint8_t packet_type;
    uint8_t topic[TOPIC_SIZE];
    uint8_t data[MESSAGE_SIZE];
}Packet;

typedef struct __attribute__((__packed__))  Message_packet{
    uint8_t message_len;
    uint8_t message[MESSAGE_SIZE];
}Message_packet;

void send_packet(uint8_t data[PACKET_SIZE],bool gateway = false){
    if(gateway){
        LoRa.enableInvertIQ();
    } else{
        LoRa.disableInvertIQ();
    }
    LoRa.beginPacket();

    for(int i=0;i<PACKET_SIZE;i++)
        LoRa.write(data[i]);

    LoRa.endPacket();
    if(gateway){
        LoRa.disableInvertIQ();
    } else{
        LoRa.enableInvertIQ();
    }
    LoRa.receive();
}

bool send_message(String message,bool gateway = false){
    Packet packet;
    Message_packet message_packet;

    bzero((void*)&packet,PACKET_SIZE);
    bzero((void*)&message_packet,MESSAGE_SIZE);

    packet.network_id = NETWORK_ID;
    packet.packet_type = 0;
    //+1 because .length() returns length without null terminator
    if((message.length()+1 > MESSAGE_SIZE))return false;
    
    memcpy((void*)packet.topic,TOPIC_ID,sizeof(TOPIC_ID));
    memcpy((void*)message_packet.message,(void*)message.c_str(),message.length()+1);
    message_packet.message_len = message.length()+1;

    memcpy((void*)packet.data,(void*)&message_packet,MESSAGE_SIZE);
    uint8_t data[PACKET_SIZE];
    memcpy((void*)data,(void*)&packet,PACKET_SIZE);
    send_packet(data,gateway);
    return true;
}

Packet parse_packet(uint8_t data[PACKET_SIZE]){
    Packet packet;
    bzero((void*)&packet,PACKET_SIZE);
    memcpy((void*)&packet,data,PACKET_SIZE);
    return packet;
}
Message_packet parse_message_packet(uint8_t data[MESSAGE_SIZE]){
    Message_packet message_packet;
    bzero((void*)&message_packet,MESSAGE_SIZE);
    memcpy((void*)&message_packet,data,MESSAGE_SIZE);
    return message_packet;
}

void setup_lora(uint8_t network_id,bool gateway = false){
    LoRa.setPins(csPin, resetPin, irqPin);
    if (!LoRa.begin(frequency)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }
    LoRa.setSyncWord(NETWORK_ID);
    LoRa.enableCrc();

    if(gateway){LoRa.disableInvertIQ();}else{LoRa.enableInvertIQ();}

    LoRa.receive(); 
}

void path_extractor(){

}