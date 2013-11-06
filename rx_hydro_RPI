/*
 RF24Network library 
 Copyright (C) 2012 James Coliz, Jr. <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/*
 Modifications by Paul Verstegen 2013 <verstegen.paul@gmail.com>
 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = 0;

// Address of the other node
const uint16_t other_node = 1;

// Structure of our payload
struct payload_t
{
  unsigned long ms;
  unsigned long counter;
  int hydro;
  int light;
  float humid;
  float temp;
};

//define global variables
unsigned long serialdata;
int inbyte;
int hy;
int li;
float hu;
float te;

void setup(void)
{
  Serial.begin(57600);
  Serial.println("NRF24L01+ Temp Humidity Soil Hydration and Light Sensor Reciever Node");
 
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
}

void loop(void)
{
  // Pump the network regularly
  network.update();

  // Is there anything ready for us?
  if ( network.available() )
  {
    // If so, grab it and print it out
    RF24NetworkHeader header;
    payload_t payload;
    network.read(header,&payload,sizeof(payload));
    //uncomment this for debugging, but will break communications with Python Logger
    /*
    Serial.print("Received packet #");
    Serial.print(payload.counter);
    Serial.print(" at ");
    Serial.print(payload.ms);
    Serial.print(" -- ");
    Serial.print("Hydro value: ");
    Serial.print(payload.hydro);
    Serial.print(" -- ");
    Serial.print("Light value: ");
    Serial.print(payload.light);
    Serial.print(" -- ");
    Serial.print("Humidity value: ");
    Serial.print(payload.humid);
    Serial.print("%");
    Serial.print(" -- ");
    Serial.print("Temp Value: ");
    Serial.print(payload.temp);
    Serial.println(" F");
    */
    hy = payload.hydro;
    li = payload.light;
    hu = payload.humid;
    te = payload.temp;
  }
    
  while(Serial.available())
  {
    getSerial();
     
    if(serialdata == 66) //command to request Hydro Value
    {
     Serial.print(hy);
    }
     
    if(serialdata == 67) //command to request Light Value
    {
     Serial.print(li);
    }
     
     if(serialdata == 68) //command to request Humidity
     {
      Serial.print(hu);
     }
     
     if(serialdata == 69) //command to request Temp
     {
      Serial.print(te);
     }
  }  
}

long getSerial()
{
  serialdata = 0;
  while (inbyte != '/')
  {
    inbyte = Serial.read(); 
    if (inbyte > 0 && inbyte != '/')
    {
     
      serialdata = (serialdata * 10) + (inbyte - '0');
    }
  }
  inbyte = 0;
  return serialdata;
}
