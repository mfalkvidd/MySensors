/*
 * The MySensors Arduino library handles the wireless radio link and protocol
 * between your home built sensors/actuators and HA controller of choice.
 * The sensors forms a self healing radio network with optional repeaters. Each
 * repeater and gateway builds a routing tables in EEPROM which keeps track of the
 * network topology allowing messages to be routed to nodes.
 *
 * Created by Henrik Ekblad <henrik.ekblad@mysensors.org>
 * Copyright (C) 2013-2020 Sensnology AB
 * Full contributor list: https://github.com/mysensors/MySensors/graphs/contributors
 *
 * Documentation: http://www.mysensors.org
 * Support Forum: http://forum.mysensors.org
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 */

#include <iostream>
#include <cstdio>
#include <unistd.h>

// For more options run ./configure --help

// Config file
//#define MY_LINUX_CONFIG_FILE "/etc/mysensors.conf"

// How many clients should be able to connect to this gateway (default 1)
#define MY_GATEWAY_MAX_CLIENTS 10

// Serial config
// Enable this if you are using an Arduino connected to the USB
//#define MY_LINUX_SERIAL_PORT "/dev/ttyUSB0"
// Enable this if you need to connect to a controller running on the same device
// You also need to define MY_LINUX_SERIAL_PORT above with the symlink name for the PTY device
//#define MY_LINUX_SERIAL_IS_PTY
// Grant access to the specified system group for the serial device
//#define MY_LINUX_SERIAL_GROUPNAME "tty"

// MQTT options
//#define MY_CONTROLLER_IP_ADDRESS 192, 168, 178, 68
//#define MY_PORT 1883
//#define MY_MQTT_CLIENT_ID "mysensors-1"
//#define MY_MQTT_PUBLISH_TOPIC_PREFIX "mygateway1-out"
//#define MY_MQTT_SUBSCRIBE_TOPIC_PREFIX "mygateway1-in"

// Enable these if your MQTT broker requires username/password
//#define MY_MQTT_USER "username"
//#define MY_MQTT_PASSWORD "password"

// Flash leds on rx/tx/err
//#define MY_DEFAULT_ERR_LED_PIN 12  // Error LED pin
//#define MY_DEFAULT_RX_LED_PIN  16  // Receive LED pin
//#define MY_DEFAULT_TX_LED_PIN  18  // Transmit LED pin
// Inverse the blinking feature
//#define MY_WITH_LEDS_BLINKING_INVERSE

// Enable software signing
//#define MY_SIGNING_SOFT
// Enable signing related debug
//#define MY_DEBUG_VERBOSE_SIGNING
// Enable this to request signatures
//#define MY_SIGNING_REQUEST_SIGNATURES
// Enable this to to weaken security for gradual deployment purpose
// (see signing documentation for details)
//#define MY_SIGNING_WEAK_SECURITY

// Enables RF24 encryption (all nodes and gateway must have this enabled, and all must be
// personalized with the same AES key)
//#define MY_RF24_ENABLE_ENCRYPTION

// Enable inclusion mode if your HA Controller supports it (e.g. Vera Controller)
//#define MY_INCLUSION_MODE_FEATURE
// Enable Inclusion mode button on gateway
//#define MY_INCLUSION_BUTTON_FEATURE
// Set inclusion mode duration (in seconds)
//#define MY_INCLUSION_MODE_DURATION 60
// Digital pin used for inclusion mode button
//#define MY_INCLUSION_MODE_BUTTON_PIN  3
#define MY_INDICATION_HANDLER
static uint32_t txOK = 0;
static uint32_t txERR = 0;
#define REPORT_INTERVAL 300000 // Report every 5 minutes
#define CHILD_ID_TX_OK 1
#define CHILD_ID_TX_ERR 2

#include <MySensors.h>

#define ARDUINO 100
// This space is intended to be used to include arduino libraries

#undef ARDUINO
MyMessage txOKmsg(CHILD_ID_TX_OK, V_KWH);
MyMessage txERRmsg(CHILD_ID_TX_ERR, V_KWH);

void indication(indication_t ind)
{
  switch (ind)
  {
    case INDICATION_TX:
      txOK++;
      break;
    case INDICATION_ERR_TX:
      txERR++;
      break;
  }
}

void setup()
{
	// Setup locally attached sensors
}

void presentation()
{
	// Present locally attached sensors here
  sendSketchInfo(F("RPi NRF24 GW"), F("1.0"));
  present(CHILD_ID_TX_OK, S_POWER);
  present(CHILD_ID_TX_ERR, S_POWER);
}

void loop()
{
	// Send locally attached sensors data here
  static unsigned long last_send = 0;
  if (millis() - last_send > REPORT_INTERVAL) {
    send(txOKmsg.set(txOK));
    send(txERRmsg.set(txERR));
    last_send = millis();
  }
}
