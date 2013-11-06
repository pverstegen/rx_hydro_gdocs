/*
 RF24Network library
 Copyright (C) 2012 James Coliz, Jr. <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/*
 Modifications by Paul Verstegen 2013 <verstegen.paul@gmail.com>
 Thank you to Adafruit for the awesome tutorial and DHT arduino library!
 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include "DHT.h"

#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22
// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

DHT dht(DHTPIN, DHTTYPE); //initialize DHT instance

RF24 radio(9,10);
// nRF24L01(+) radio attached using Getting Started board 

//initialize sensor values
int hydrovalue =0;
float humid=0;
float temp=0;
int light=0;

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = 1;

// Address of the other node
const uint16_t other_node = 0;

// How often to send 'hello world to the other unit
const unsigned long interval = 30000; //ms

// When did we last send?
unsigned long last_sent;

// How many have we sent already
unsigned long packets_sent;

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

void setup(void)
{
  Serial.begin(57600);
  Serial.println("NRF24L01+ Temp Humidity Soil Hydration and Light Sensor Transmitter Node");
  dht.begin();
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
}

void loop(void)
{
  // Pump the network regularly
  network.update();
  
  //read sensors
  int hydro = analogRead(4); //read soil humidity sensor
  int light = analogRead(5); //read CdS light sensor
  float humid = dht.readHumidity(); // Reading temperature or humidity takes about 250 milliseconds!
  float temp = dht.readTemperature(); // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  temp = ((temp * 1.8) + 32); //Celsius conversion
  
  // If it's time to send a message, send it!
  unsigned long now = millis();
  if ( now - last_sent >= interval  )
  {
    last_sent = now;

    Serial.print("Sending...");
    payload_t payload = { millis(), packets_sent++, hydro, light, humid, temp };
    RF24NetworkHeader header(/*to node*/ other_node);
    bool ok = network.write(header,&payload,sizeof(payload));
    if (ok)
      Serial.println("ok.");
    else
      Serial.println("failed.");
  }
}
// vim:ai:cin:sts=2 sw=2 ft=cpp
