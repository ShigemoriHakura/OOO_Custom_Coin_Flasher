#pragma once

#define LED_PIN 25

#define ABLEN 2
#define DATA  3
#define CLOCK 4

//#define DEBUG

#define CI522_RST  14
#define CI522_NSS  13
#define CI522_MISO 12
#define CI522_MOSI 11
#define CI522_SCK  10


static const char* StartBit = "000000001";
static const char* EndBit   = "011";