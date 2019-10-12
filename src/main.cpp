#include <Arduino.h>
#include <Wire.h>
#include <SparkFun_APDS9960.h>

// Pins
#define APDS9960_INT 2 // Needs to be an interrupt pin

// Constants
#define LIGHT_INT_HIGH 1000 // High light level for interrupt
#define LIGHT_INT_LOW 10    // Low light level for interrupt

// Global variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
uint16_t ambient_light = 0;
uint16_t red_light = 0;
uint16_t green_light = 0;
uint16_t blue_light = 0;
int isr_flag = 0;
uint16_t threshold = 0;

//method
void interruptRoutine()
{
  isr_flag = 1;
}

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6          // 네오픽셀 연결 디지털 핀 번호 적기
#define NUM_LEDS 24    // 네오픽셀 소자 수, 1부터 시작. (3개 연결시, 3 작성)
#define BRIGHTNESS 255 // 네오픽셀 밝기 설정 0(어두움) ~ 255(밝음)

int i;//네오픽셀 변수

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);


#include <Playtune.h>

const byte PROGMEM avengers[] = {
0x90,0x3E,0x00,0x7A,0x80,0x00,0x02,0x90,0x3E,0x00,0x7A,0x80,0x00,0x02,0x90,0x3E,
0x00,0x7A,0x80,0x01,0x79,0x90,0x3E,0x00,0x7A,0x80,0x00,0x02,0x90,0x3E,0x00,0x7A,
0x80,0x00,0x02,0x90,0x3E,0x00,0x7A,0x80,0x01,0x79,0x90,0x3E,0x00,0x7A,0x80,0x00,
0x02,0x90,0x3E,0x00,0x7A,0x80,0x00,0x02,0x90,0x3E,0x00,0x7A,0x80,0x00,0x02,0x90,
0x3E,0x00,0x7A,0x80,0x00,0x02,0x90,0x3F,0x00,0x7A,0x80,0x00,0x02,0x90,0x3F,0x00,
0x7A,0x80,0x00,0x02,0x90,0x3F,0x00,0x7A,0x80,0x01,0x79,0x90,0x3F,0x00,0x7A,0x80,
0x00,0x02,0x90,0x3F,0x00,0x7A,0x80,0x00,0x02,0x90,0x40,0x00,0x7A,0x80,0x01,0x79,
0x90,0x40,0x00,0x7A,0x80,0x00,0x02,0x90,0x40,0x00,0x7A,0x80,0x00,0x02,0x90,0x40,
0x00,0x7A,0x80,0x00,0x02,0x90,0x40,0x00,0x7A,0x80,0x00,0x02,0x90,0x41,0x00,0x7A,
0x80,0x00,0x02,0x90,0x41,0x00,0x7A,0x80,0x00,0x02,0x90,0x41,0x00,0x7A,0x80,0x01,
0x79,0x90,0x41,0x00,0x7A,0x80,0x00,0x02,0x90,0x41,0x00,0x7A,0x80,0x00,0x02,0x90,
0x40,0x00,0x7A,0x80,0x01,0x79,0x90,0x40,0x00,0x7A,0x80,0x00,0x02,0x90,0x40,0x00,
0x7A,0x80,0x00,0x02,0x90,0x40,0x00,0x7A,0x80,0x00,0x02,0x90,0x40,0x00,0x7A,0x80,
0x00,0x02,0x90,0x3F,0x00,0x7A,0x80,0x00,0x02,0x90,0x3F,0x00,0x7A,0x80,0x00,0x02,
0x90,0x3F,0x00,0x7A,0x80,0x01,0x79,0x90,0x3F,0x00,0x7A,0x80,0x00,0x02,0x90,0x3F,
0x00,0x7A,0x80,0x00,0x02,0x90,0x3A,0x00,0xF7,0x80,0x00,0xFC,0x90,0x3C,0x00,0xF7,
0x80,0x00,0xFC,0x90,0x4A,0x91,0x37,0x92,0x4F,0x00,0x7A,0x80,0x81,0x00,0x02,0x90,
0x3E,0x91,0x4A,0x00,0x7A,0x80,0x81,0x00,0x02,0x90,0x3E,0x91,0x4A,0x00,0x7A,0x80,
0x81,0x01,0x79,0x90,0x3E,0x91,0x4A,0x00,0x7A,0x80,0x81,0x00,0x02,0x90,0x3E,0x91,
0x4A,0x00,0x7A,0x80,0x81,0x00,0x02,0x90,0x3E,0x91,0x4A,0x00,0x7A,0x80,0x81,0x01,
0x79,0x90,0x3E,0x91,0x4A,0x00,0x7A,0x80,0x81,0x00,0x02,0x90,0x3E,0x91,0x4A,0x00,
0x7A,0x80,0x81,0x00,0x02,0x90,0x3E,0x91,0x4A,0x00,0x7A,0x80,0x81,0x00,0x02,0x90,
0x3E,0x91,0x4A,0x00,0x7A,0x82,0x80,0x81,0x00,0x02,0x92,0x4F,0x90,0x3F,0x91,0x4B,
0x00,0x7A,0x80,0x81,0x00,0x02,0x90,0x3F,0x91,0x4B,0x00,0x7A,0x80,0x81,0x00,0x02,
0x90,0x3F,0x91,0x4B,0x00,0x7A,0x80,0x81,0x01,0x79,0x90,0x3F,0x91,0x4B,0x00,0x7A,
0x80,0x81,0x00,0x02,0x90,0x3F,0x91,0x4B,0x00,0x7A,0x80,0x81,0x00,0x02,0x90,0x40,
0x91,0x4C,0x00,0x7A,0x80,0x81,0x01,0x79,0x90,0x40,0x91,0x4C,0x00,0x7A,0x80,0x81,
0x00,0x02,0x90,0x40,0x91,0x4C,0x00,0x7A,0x80,0x81,0x00,0x02,0x90,0x40,0x91,0x4C,
0x00,0x7A,0x80,0x81,0x00,0x02,0x90,0x40,0x91,0x4C,0x00,0x7A,0x82,0x80,0x81,0x00,
0x02,0x92,0x4F,0x90,0x41,0x91,0x4D,0x00,0x7A,0x80,0x81,0x00,0x02,0x90,0x41,0x91,
0x4D,0x00,0x7A,0x80,0x81,0x00,0x02,0x90,0x41,0x91,0x4D,0x00,0x7A,0x80,0x81,0x01,
0x79,0x90,0x41,0x91,0x4D,0x00,0x7A,0x80,0x81,0x00,0x02,0x90,0x41,0x91,0x4D,0x00,
0x7A,0x80,0x81,0x00,0x02,0x90,0x40,0x91,0x4C,0x00,0x7A,0x80,0x81,0x01,0x79,0x90,
0x40,0x91,0x4C,0x00,0x7A,0x80,0x81,0x00,0x02,0x90,0x40,0x91,0x4C,0x00,0x7A,0x80,
0x81,0x00,0x02,0x90,0x40,0x91,0x4C,0x00,0x7A,0x80,0x81,0x00,0x02,0x90,0x40,0x91,
0x4C,0x00,0x7A,0x82,0x80,0x81,0x00,0x02,0x92,0x4F,0x90,0x3F,0x91,0x4B,0x00,0x7A,
0x80,0x81,0x00,0x02,0x90,0x3F,0x91,0x4B,0x00,0x7A,0x80,0x81,0x00,0x02,0x90,0x3F,
0x91,0x4B,0x00,0x7A,0x80,0x81,0x01,0x79,0x90,0x3F,0x91,0x4B,0x00,0x7A,0x80,0x81,
0x00,0x02,0x90,0x3F,0x91,0x4B,0x00,0x7A,0x80,0x81,0x00,0x02,0x90,0x3A,0x91,0x46,
0x00,0xF7,0x80,0x81,0x00,0xFC,0x90,0x3C,0x91,0x48,0x00,0xF7,0x80,0x81,0x00,0xFA,
0x82,0x00,0x02,0x90,0x37,0x91,0x3E,0x92,0x4A,0x00,0x7A,0x81,0x82,0x00,0x02,0x91,
0x3E,0x92,0x4A,0x00,0x7A,0x81,0x82,0x00,0x02,0x91,0x3E,0x92,0x4A,0x00,0x7A,0x81,
0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,
0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x91,0x3E,0x92,0x4A,0x00,0x7A,0x81,0x82,
0x00,0x02,0x91,0x3E,0x92,0x4A,0x00,0x7A,0x81,0x82,0x00,0x02,0x91,0x3E,0x92,0x4A,
0x00,0x7A,0x81,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,
0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x91,0x3E,0x92,0x4A,0x00,
0x7A,0x81,0x82,0x00,0x02,0x91,0x3E,0x92,0x4A,0x00,0x7A,0x80,0x81,0x82,0x00,0x02,
0x91,0x3E,0x92,0x4A,0x90,0x37,0x00,0x7A,0x81,0x82,0x80,0x00,0x02,0x91,0x3E,0x92,
0x4A,0x90,0x37,0x00,0x7A,0x81,0x82,0x80,0x00,0x02,0x90,0x3E,0x91,0x3F,0x92,0x4B,
0x00,0x7A,0x81,0x82,0x00,0x02,0x91,0x3F,0x92,0x4B,0x00,0x7A,0x81,0x82,0x00,0x02,
0x91,0x3F,0x92,0x4B,0x00,0x7A,0x81,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x80,0x82,
0x00,0x02,0x90,0x3C,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x3C,0x80,
0x00,0x3E,0x82,0x00,0x02,0x91,0x3F,0x92,0x4B,0x90,0x37,0x00,0x7A,0x81,0x82,0x80,
0x00,0x02,0x90,0x3A,0x91,0x3F,0x92,0x4B,0x00,0x3C,0x80,0x00,0x3E,0x81,0x82,0x00,
0x02,0x90,0x3A,0x91,0x40,0x92,0x4C,0x00,0x7A,0x81,0x82,0x00,0x02,0x92,0x4A,0x00,
0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x80,
0x82,0x00,0x02,0x90,0x3C,0x91,0x40,0x92,0x4C,0x00,0x7A,0x81,0x82,0x00,0x02,0x91,
0x40,0x92,0x4C,0x00,0x7A,0x81,0x82,0x00,0x02,0x91,0x40,0x92,0x4C,0x00,0x7A,0x80,
0x81,0x82,0x00,0x02,0x90,0x3E,0x91,0x40,0x92,0x4C,0x00,0x7A,0x80,0x81,0x82,0x00,
0x02,0x90,0x3E,0x91,0x41,0x92,0x4D,0x00,0x7A,0x81,0x82,0x00,0x02,0x91,0x41,0x92,
0x4D,0x00,0x7A,0x81,0x82,0x00,0x02,0x91,0x41,0x92,0x4D,0x00,0x7A,0x81,0x82,0x00,
0x02,0x92,0x4F,0x00,0x7A,0x80,0x82,0x00,0x02,0x90,0x37,0x92,0x4F,0x00,0x7A,0x82,
0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x91,0x41,0x92,0x4D,0x00,0x7A,0x81,
0x82,0x00,0x02,0x91,0x41,0x92,0x4D,0x00,0x7A,0x81,0x82,0x00,0x02,0x91,0x40,0x92,
0x4C,0x00,0x7A,0x81,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,
0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x80,0x82,0x00,0x02,0x91,0x40,0x92,
0x4C,0x90,0x37,0x00,0x7A,0x81,0x82,0x80,0x00,0x02,0x91,0x40,0x92,0x4C,0x90,0x37,
0x00,0x7A,0x81,0x82,0x80,0x00,0x02,0x91,0x40,0x92,0x4C,0x90,0x37,0x00,0x7A,0x81,
0x82,0x80,0x00,0x02,0x91,0x40,0x92,0x4C,0x90,0x37,0x00,0x7A,0x81,0x82,0x80,0x00,
0x02,0x90,0x3E,0x91,0x3F,0x92,0x4B,0x00,0x7A,0x81,0x82,0x00,0x02,0x91,0x3F,0x92,
0x4B,0x00,0x7A,0x81,0x82,0x00,0x02,0x91,0x3F,0x92,0x4B,0x00,0x7A,0x81,0x82,0x00,
0x02,0x92,0x4F,0x00,0x7A,0x80,0x82,0x00,0x02,0x90,0x3C,0x92,0x4F,0x00,0x7A,0x82,
0x00,0x02,0x92,0x4A,0x00,0x3C,0x80,0x00,0x3E,0x82,0x00,0x02,0x91,0x3F,0x92,0x4B,
0x90,0x37,0x00,0x7A,0x81,0x82,0x80,0x00,0x02,0x90,0x3A,0x91,0x3F,0x92,0x4B,0x00,
0x3C,0x80,0x00,0x3E,0x81,0x82,0x00,0x02,0x90,0x3A,0x91,0x3A,0x92,0x46,0x00,0x7A,
0x00,0x02,0x00,0x7A,0x81,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,
0x4F,0x00,0x7A,0x80,0x82,0x00,0x02,0x90,0x39,0x91,0x3C,0x92,0x48,0x00,0x7A,0x00,
0x02,0x00,0x7A,0x81,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,
0x00,0x7A,0x80,0x82,0x00,0x02,0x90,0x37,0x91,0x3E,0x92,0x4A,0x00,0x7A,0x81,0x82,
0x00,0x02,0x91,0x3E,0x92,0x4A,0x00,0x7A,0x81,0x82,0x00,0x02,0x91,0x3E,0x92,0x4A,
0x00,0x7A,0x81,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,
0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x91,0x3E,0x92,0x4A,0x00,
0x7A,0x81,0x82,0x00,0x02,0x91,0x3E,0x92,0x4A,0x00,0x7A,0x81,0x82,0x00,0x02,0x91,
0x3E,0x92,0x4A,0x00,0x7A,0x81,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,
0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x91,0x3E,
0x92,0x4A,0x00,0x7A,0x81,0x82,0x00,0x02,0x91,0x3E,0x92,0x4A,0x00,0x7A,0x80,0x81,
0x82,0x00,0x02,0x91,0x3E,0x92,0x4A,0x90,0x37,0x00,0x7A,0x81,0x82,0x80,0x00,0x02,
0x91,0x3E,0x92,0x4A,0x90,0x37,0x00,0x7A,0x81,0x82,0x80,0x00,0x02,0x90,0x3E,0x91,
0x3F,0x92,0x4B,0x00,0x7A,0x81,0x82,0x00,0x02,0x91,0x3F,0x92,0x4B,0x00,0x7A,0x81,
0x82,0x00,0x02,0x91,0x3F,0x92,0x4B,0x00,0x7A,0x81,0x82,0x00,0x02,0x92,0x4F,0x00,
0x7A,0x80,0x82,0x00,0x02,0x90,0x3C,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,
0x00,0x3C,0x80,0x00,0x3E,0x82,0x00,0x02,0x91,0x3F,0x92,0x4B,0x90,0x37,0x00,0x7A,
0x81,0x82,0x80,0x00,0x02,0x90,0x3A,0x91,0x3F,0x92,0x4B,0x00,0x3C,0x80,0x00,0x3E,
0x81,0x82,0x00,0x02,0x90,0x3A,0x91,0x40,0x92,0x4C,0x00,0x7A,0x81,0x82,0x00,0x02,
0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,
0x00,0x7A,0x80,0x82,0x00,0x02,0x90,0x3C,0x91,0x40,0x92,0x4C,0x00,0x7A,0x81,0x82,
0x00,0x02,0x91,0x40,0x92,0x4C,0x00,0x7A,0x81,0x82,0x00,0x02,0x91,0x40,0x92,0x4C,
0x00,0x7A,0x80,0x81,0x82,0x00,0x02,0x90,0x3E,0x91,0x40,0x92,0x4C,0x00,0x7A,0x80,
0x81,0x82,0x00,0x02,0x90,0x3E,0x91,0x41,0x92,0x4D,0x00,0x7A,0x81,0x82,0x00,0x02,
0x91,0x41,0x92,0x4D,0x00,0x7A,0x81,0x82,0x00,0x02,0x91,0x41,0x92,0x4D,0x00,0x7A,
0x81,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x80,0x82,0x00,0x02,0x90,0x37,0x92,0x4F,
0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x91,0x41,0x92,0x4D,
0x00,0x7A,0x81,0x82,0x00,0x02,0x91,0x41,0x92,0x4D,0x00,0x7A,0x81,0x82,0x00,0x02,
0x91,0x40,0x92,0x4C,0x00,0x7A,0x81,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,
0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x80,0x82,0x00,0x02,
0x91,0x40,0x92,0x4C,0x90,0x37,0x00,0x7A,0x81,0x82,0x80,0x00,0x02,0x91,0x40,0x92,
0x4C,0x90,0x37,0x00,0x7A,0x81,0x82,0x80,0x00,0x02,0x91,0x40,0x92,0x4C,0x90,0x37,
0x00,0x7A,0x81,0x82,0x80,0x00,0x02,0x91,0x40,0x92,0x4C,0x90,0x37,0x00,0x7A,0x81,
0x82,0x80,0x00,0x02,0x90,0x3E,0x91,0x3F,0x92,0x4B,0x00,0x7A,0x81,0x82,0x00,0x02,
0x91,0x3F,0x92,0x4B,0x00,0x7A,0x81,0x82,0x00,0x02,0x91,0x3F,0x92,0x4B,0x00,0x7A,
0x81,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x80,0x82,0x00,0x02,0x90,0x3C,0x92,0x4F,
0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x3C,0x80,0x00,0x3E,0x82,0x00,0x02,0x91,
0x3F,0x92,0x4B,0x90,0x37,0x00,0x7A,0x81,0x82,0x80,0x00,0x02,0x90,0x3A,0x91,0x3F,
0x92,0x4B,0x00,0x3C,0x80,0x00,0x3E,0x81,0x82,0x00,0x02,0x90,0x3A,0x91,0x3A,0x92,
0x46,0x00,0x7A,0x00,0x02,0x00,0x7A,0x81,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,
0x00,0x02,0x92,0x4F,0x00,0x7A,0x80,0x82,0x00,0x02,0x90,0x39,0x91,0x3C,0x92,0x48,
0x00,0x7A,0x00,0x02,0x00,0x7A,0x81,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,
0x02,0x92,0x4F,0x00,0x7A,0x80,0x82,0x00,0x02,0x90,0x4F,0x91,0x37,0x92,0x3E,0x00,
0x7A,0x82,0x00,0x02,0x92,0x3E,0x00,0x7A,0x82,0x00,0x02,0x92,0x3E,0x00,0x7A,0x82,
0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,
0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x3E,0x00,0x7A,0x82,0x00,0x02,0x92,0x3E,
0x00,0x7A,0x82,0x00,0x02,0x92,0x3E,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,
0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,
0x02,0x92,0x3E,0x00,0x7A,0x82,0x00,0x02,0x92,0x3E,0x00,0x7A,0x80,0x81,0x82,0x00,
0x02,0x92,0x3E,0x90,0x4A,0x91,0x37,0x00,0x7A,0x82,0x80,0x81,0x00,0x02,0x92,0x3E,
0x90,0x4A,0x91,0x37,0x00,0x7A,0x82,0x80,0x81,0x00,0x02,0x90,0x56,0x91,0x3E,0x92,
0x3F,0x00,0x7A,0x82,0x00,0x02,0x92,0x3F,0x00,0x7A,0x82,0x00,0x02,0x92,0x3F,0x00,
0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x80,0x81,0x82,0x00,0x02,0x90,0x54,0x91,
0x3C,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x3C,0x80,0x81,0x00,0x3E,
0x82,0x00,0x02,0x92,0x3F,0x90,0x4B,0x91,0x37,0x00,0x7A,0x82,0x80,0x81,0x00,0x02,
0x90,0x52,0x91,0x3A,0x92,0x3F,0x00,0x3C,0x80,0x81,0x00,0x3E,0x82,0x00,0x02,0x90,
0x52,0x91,0x3A,0x92,0x40,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,
0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x80,0x81,0x82,0x00,
0x02,0x90,0x54,0x91,0x3C,0x92,0x40,0x00,0x7A,0x82,0x00,0x02,0x92,0x40,0x00,0x7A,
0x82,0x00,0x02,0x92,0x40,0x00,0x7A,0x80,0x81,0x82,0x00,0x02,0x90,0x56,0x91,0x3E,
0x92,0x40,0x00,0x7A,0x80,0x81,0x82,0x00,0x02,0x90,0x56,0x91,0x3E,0x92,0x41,0x00,
0x7A,0x82,0x00,0x02,0x92,0x41,0x00,0x7A,0x82,0x00,0x02,0x92,0x41,0x00,0x7A,0x82,
0x00,0x02,0x92,0x4F,0x00,0x7A,0x80,0x81,0x82,0x00,0x02,0x90,0x4F,0x91,0x37,0x92,
0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x41,0x00,
0x7A,0x82,0x00,0x02,0x92,0x41,0x00,0x7A,0x82,0x00,0x02,0x92,0x40,0x00,0x7A,0x82,
0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,
0x92,0x4F,0x00,0x7A,0x80,0x81,0x82,0x00,0x02,0x92,0x40,0x90,0x4C,0x91,0x37,0x00,
0x7A,0x82,0x80,0x81,0x00,0x02,0x92,0x40,0x90,0x4C,0x91,0x37,0x00,0x7A,0x82,0x80,
0x81,0x00,0x02,0x92,0x40,0x90,0x4C,0x91,0x37,0x00,0x7A,0x82,0x80,0x81,0x00,0x02,
0x92,0x40,0x90,0x4C,0x91,0x37,0x00,0x7A,0x82,0x80,0x81,0x00,0x02,0x90,0x56,0x91,
0x3E,0x92,0x3F,0x00,0x7A,0x82,0x00,0x02,0x92,0x3F,0x00,0x7A,0x82,0x00,0x02,0x92,
0x3F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x80,0x81,0x82,0x00,0x02,0x90,
0x54,0x91,0x3C,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x3C,0x80,0x81,
0x00,0x3E,0x82,0x00,0x02,0x92,0x3F,0x90,0x4B,0x91,0x37,0x00,0x7A,0x82,0x80,0x81,
0x00,0x02,0x90,0x52,0x91,0x3A,0x92,0x3F,0x00,0x3C,0x80,0x81,0x00,0x3E,0x82,0x00,
0x02,0x90,0x52,0x91,0x3A,0x92,0x3A,0x00,0x7A,0x00,0x02,0x00,0x7A,0x82,0x00,0x02,
0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x80,0x81,0x82,0x00,0x02,
0x90,0x51,0x91,0x39,0x92,0x3C,0x00,0x7A,0x00,0x02,0x00,0x7A,0x82,0x00,0x02,0x92,
0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x80,0x81,0x82,0x00,0x02,0x90,
0x4F,0x91,0x37,0x92,0x3E,0x00,0x7A,0x82,0x00,0x02,0x92,0x3E,0x00,0x7A,0x82,0x00,
0x02,0x92,0x3E,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,
0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x3E,0x00,
0x7A,0x82,0x00,0x02,0x92,0x3E,0x00,0x7A,0x82,0x00,0x02,0x92,0x3E,0x00,0x7A,0x82,
0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,
0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x3E,0x00,0x7A,0x82,0x00,0x02,0x92,0x3E,
0x00,0x7A,0x80,0x81,0x82,0x00,0x02,0x92,0x3E,0x90,0x4A,0x91,0x37,0x00,0x7A,0x82,
0x80,0x81,0x00,0x02,0x92,0x3E,0x90,0x4A,0x91,0x37,0x00,0x7A,0x82,0x80,0x81,0x00,
0x02,0x90,0x56,0x91,0x3E,0x92,0x3F,0x00,0x7A,0x82,0x00,0x02,0x92,0x3F,0x00,0x7A,
0x82,0x00,0x02,0x92,0x3F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x80,0x81,
0x82,0x00,0x02,0x90,0x54,0x91,0x3C,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,
0x00,0x3C,0x80,0x81,0x00,0x3E,0x82,0x00,0x02,0x92,0x3F,0x90,0x4B,0x91,0x37,0x00,
0x7A,0x82,0x80,0x81,0x00,0x02,0x90,0x52,0x91,0x3A,0x92,0x3F,0x00,0x3C,0x80,0x81,
0x00,0x3E,0x82,0x00,0x02,0x90,0x52,0x91,0x3A,0x92,0x40,0x00,0x7A,0x82,0x00,0x02,
0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,
0x00,0x7A,0x80,0x81,0x82,0x00,0x02,0x90,0x54,0x91,0x3C,0x92,0x40,0x00,0x7A,0x82,
0x00,0x02,0x92,0x40,0x00,0x7A,0x82,0x00,0x02,0x92,0x40,0x00,0x7A,0x80,0x81,0x82,
0x00,0x02,0x90,0x56,0x91,0x3E,0x92,0x40,0x00,0x7A,0x80,0x81,0x82,0x00,0x02,0x90,
0x56,0x91,0x3E,0x92,0x41,0x00,0x7A,0x82,0x00,0x02,0x92,0x41,0x00,0x7A,0x82,0x00,
0x02,0x92,0x41,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x80,0x81,0x82,0x00,
0x02,0x90,0x4F,0x91,0x37,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,
0x82,0x00,0x02,0x92,0x41,0x00,0x7A,0x82,0x00,0x02,0x92,0x41,0x00,0x7A,0x82,0x00,
0x02,0x92,0x40,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,
0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x80,0x81,0x82,0x00,0x02,0x92,
0x40,0x90,0x4C,0x91,0x37,0x00,0x7A,0x82,0x80,0x81,0x00,0x02,0x92,0x40,0x90,0x4C,
0x91,0x37,0x00,0x7A,0x82,0x80,0x81,0x00,0x02,0x92,0x40,0x90,0x4C,0x91,0x37,0x00,
0x7A,0x82,0x80,0x81,0x00,0x02,0x92,0x40,0x90,0x4C,0x91,0x37,0x00,0x7A,0x82,0x80,
0x81,0x00,0x02,0x90,0x56,0x91,0x3E,0x92,0x3F,0x00,0x7A,0x82,0x00,0x02,0x92,0x3F,
0x00,0x7A,0x82,0x00,0x02,0x92,0x3F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,
0x80,0x81,0x82,0x00,0x02,0x90,0x54,0x91,0x3C,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,
0x92,0x4A,0x00,0x3C,0x80,0x81,0x00,0x3E,0x82,0x00,0x02,0x92,0x3F,0x90,0x4B,0x91,
0x37,0x00,0x7A,0x82,0x80,0x81,0x00,0x02,0x90,0x52,0x91,0x3A,0x92,0x3F,0x00,0x3C,
0x80,0x81,0x00,0x3E,0x82,0x00,0x02,0x90,0x52,0x91,0x3A,0x92,0x3A,0x00,0x7A,0x00,
0x02,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,
0x7A,0x80,0x81,0x82,0x00,0x02,0x90,0x51,0x91,0x39,0x92,0x3C,0x00,0x7A,0x00,0x02,
0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,
0x80,0x81,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,
0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,
0x02,0x91,0x37,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,
0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x81,0x82,0x00,0x02,
0x91,0x43,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,
0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,
0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,
0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x81,0x82,0x00,0x02,0x90,0x37,0x92,0x4F,0x00,
0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,
0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,
0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,
0x00,0x7A,0x82,0x00,0x02,0x91,0x3C,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,
0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,
0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,
0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x80,0x81,0x82,0x00,
0x02,0x91,0x3E,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x81,0x82,
0x00,0x02,0x91,0x3A,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x81,
0x82,0x00,0x02,0x91,0x37,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,
0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x81,0x82,
0x00,0x02,0x91,0x43,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,
0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,
0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,
0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x81,0x82,0x00,0x02,0x90,0x3F,0x91,
0x3C,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,
0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,
0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,
0x00,0x02,0x92,0x4F,0x00,0x7A,0x80,0x81,0x82,0x00,0x02,0x90,0x3E,0x91,0x3B,0x92,
0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,
0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,
0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,0x92,0x4A,0x00,0x7A,0x82,0x00,0x02,
0x92,0x4F,0x00,0x7A,0x80,0x81,0x82,0x00,0x02,0x90,0x48,0x91,0x4C,0x92,0x54,0x00,
0x7A,0x81,0x00,0x02,0x91,0x48,0x00,0x7A,0x81,0x00,0x02,0x91,0x48,0x00,0x7A,0x81,
0x00,0x02,0x91,0x4C,0x00,0x7A,0x81,0x00,0x02,0x91,0x3C,0x00,0x7A,0x00,0x02,0x00,
0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x81,0x00,0x02,0x91,0x48,0x00,0x7A,
0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,
0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x81,0x00,0x02,0x91,
0x4C,0x00,0x7A,0x81,0x00,0x02,0x91,0x48,0x00,0x7A,0x81,0x00,0x02,0x91,0x48,0x00,
0x7A,0x81,0x00,0x02,0x91,0x4C,0x00,0x7A,0x81,0x00,0x02,0x91,0x41,0x00,0x7A,0x00,
0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x81,0x00,0x02,0x91,0x4D,
0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,
0x00,0x7A,0x00,0x02,0x00,0x7A,0x81,0x00,0x02,0x91,0x48,0x00,0x7A,0x00,0x02,0x00,
0x7A,0x82,0x80,0x81,0x00,0x02,0x91,0x3E,0x92,0x56,0x90,0x4A,0x01,0xF4,0x01,0xF1,
0x00,0x02,0x03,0xE5,0x80,0x81,0x82,0x00,0x02,0x90,0x4C,0x92,0x4C,0x91,0x3B,0x01,
0xF4,0x01,0xF1,0x80,0x82,0x81,0x00,0x02,0x90,0x4B,0x92,0x4B,0x91,0x3B,0x03,0xE5,
0x80,0x82,0x81,0x00,0x02,0x92,0x43,0x91,0x43,0x90,0x40,0x00,0x7A,0x82,0x81,0x00,
0x02,0x92,0x43,0x91,0x43,0x00,0x7A,0x82,0x81,0x00,0x02,0x92,0x40,0x91,0x40,0x00,
0x7A,0x82,0x81,0x00,0x02,0x92,0x40,0x91,0x40,0x00,0x7A,0x82,0x81,0x00,0x02,0x92,
0x40,0x91,0x40,0x00,0x7A,0x82,0x81,0x00,0x02,0x92,0x40,0x91,0x40,0x00,0x7A,0x82,
0x81,0x00,0x02,0x92,0x43,0x91,0x43,0x00,0x7A,0x82,0x81,0x00,0x02,0x92,0x43,0x91,
0x43,0x00,0x7A,0x82,0x81,0x00,0x02,0x92,0x43,0x91,0x43,0x00,0x7A,0x82,0x81,0x00,
0x02,0x92,0x43,0x91,0x43,0x00,0x7A,0x82,0x81,0x00,0x02,0x92,0x40,0x91,0x40,0x00,
0x7A,0x82,0x81,0x00,0x02,0x92,0x40,0x91,0x40,0x00,0x7A,0x82,0x81,0x00,0x02,0x92,
0x40,0x91,0x40,0x00,0x7A,0x82,0x81,0x00,0x02,0x92,0x40,0x91,0x40,0x00,0x7A,0x82,
0x81,0x00,0x02,0x92,0x43,0x91,0x43,0x00,0x7A,0x82,0x81,0x00,0x02,0x92,0x43,0x91,
0x43,0x00,0x7A,0x80,0x82,0x81,0x00,0x02,0x90,0x3C,0x91,0x40,0x92,0x43,0x00,0x7A,
0x82,0x00,0x02,0x92,0x43,0x00,0x7A,0x82,0x00,0x02,0x92,0x40,0x00,0x7A,0x82,0x00,
0x02,0x92,0x40,0x00,0x7A,0x82,0x00,0x02,0x92,0x40,0x00,0x7A,0x00,0x02,0x00,0x7A,
0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x82,0x00,0x02,0x92,0x40,0x00,0x7A,0x00,
0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,
0x02,0x00,0x7A,0x82,0x00,0x02,0x92,0x47,0x00,0x7A,0x00,0x02,0x00,0x7A,0x80,0x81,
0x82,0x00,0x02,0x90,0x3D,0x91,0x40,0x92,0x45,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,
0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,
0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,
0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,
0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x80,0x81,0x82,0x00,0x02,0x90,0x3C,0x91,0x40,
0x92,0x43,0x00,0x7A,0x82,0x00,0x02,0x92,0x43,0x00,0x7A,0x82,0x00,0x02,0x92,0x40,
0x00,0x7A,0x82,0x00,0x02,0x92,0x40,0x00,0x7A,0x82,0x00,0x02,0x92,0x40,0x00,0x7A,
0x82,0x00,0x02,0x92,0x40,0x00,0x7A,0x82,0x00,0x02,0x92,0x43,0x00,0x7A,0x82,0x00,
0x02,0x92,0x43,0x00,0x7A,0x80,0x81,0x82,0x00,0x02,0x90,0x3E,0x91,0x42,0x92,0x43,
0x00,0x7A,0x82,0x00,0x02,0x92,0x43,0x00,0x7A,0x82,0x00,0x02,0x92,0x40,0x00,0x7A,
0x82,0x00,0x02,0x92,0x40,0x00,0x7A,0x82,0x00,0x02,0x92,0x40,0x00,0x7A,0x82,0x00,
0x02,0x92,0x40,0x00,0x7A,0x82,0x00,0x02,0x92,0x43,0x00,0x7A,0x82,0x00,0x02,0x92,
0x43,0x00,0x7A,0x80,0x81,0x82,0x00,0x02,0x90,0x40,0x91,0x43,0x92,0x4F,0x00,0x7A,
0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,0x82,0x00,
0x02,0x92,0x4C,0x00,0x7A,0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,0x82,0x00,0x02,0x92,
0x4C,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,
0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,
0x00,0x02,0x92,0x4C,0x00,0x7A,0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,0x82,0x00,0x02,
0x92,0x4C,0x00,0x7A,0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,
0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x80,0x81,0x82,0x00,0x02,0x90,0x3C,
0x91,0x40,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,
0x92,0x4C,0x00,0x7A,0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,0x82,0x00,0x02,0x92,0x4C,
0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x82,0x00,
0x02,0x92,0x4C,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,
0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x82,0x00,0x02,0x92,0x53,0x00,0x7A,
0x00,0x02,0x00,0x7A,0x80,0x81,0x82,0x00,0x02,0x90,0x3D,0x91,0x40,0x92,0x51,0x00,
0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,
0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,
0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,
0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x80,0x81,0x82,
0x00,0x02,0x90,0x3C,0x91,0x40,0x92,0x53,0x00,0x7A,0x82,0x00,0x02,0x92,0x53,0x00,
0x7A,0x82,0x00,0x02,0x92,0x4E,0x00,0x7A,0x82,0x00,0x02,0x92,0x4E,0x00,0x7A,0x82,
0x00,0x02,0x92,0x4E,0x00,0x7A,0x82,0x00,0x02,0x92,0x4E,0x00,0x7A,0x82,0x00,0x02,
0x92,0x53,0x00,0x7A,0x82,0x00,0x02,0x92,0x53,0x00,0x7A,0x80,0x81,0x82,0x00,0x02,
0x90,0x3B,0x91,0x3F,0x92,0x53,0x00,0x7A,0x82,0x00,0x02,0x92,0x53,0x00,0x7A,0x82,
0x00,0x02,0x92,0x4E,0x00,0x7A,0x82,0x00,0x02,0x92,0x4E,0x00,0x7A,0x82,0x00,0x02,
0x92,0x4E,0x00,0x7A,0x82,0x00,0x02,0x92,0x4E,0x00,0x7A,0x82,0x00,0x02,0x92,0x53,
0x00,0x7A,0x82,0x00,0x02,0x92,0x53,0x00,0x7A,0x80,0x81,0x82,0x00,0x02,0x90,0x3C,
0x91,0x43,0x92,0x40,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,
0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x81,0x82,0x00,0x02,0x91,0x42,
0x92,0x3E,0x00,0x7A,0x81,0x82,0x00,0x02,0x91,0x43,0x92,0x40,0x00,0x7A,0x80,0x81,
0x82,0x00,0x02,0x90,0x3E,0x91,0x45,0x92,0x42,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,
0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,
0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x80,0x81,0x82,0x00,0x02,0x90,0x3C,0x91,0x56,
0x92,0x3E,0x00,0x7A,0x80,0x81,0x82,0x00,0x7F,0x90,0x3C,0x91,0x58,0x92,0x40,0x00,
0x7A,0x80,0x81,0x82,0x02,0x73,0x90,0x3C,0x91,0x58,0x92,0x40,0x00,0x7A,0x80,0x81,
0x82,0x00,0x7F,0x90,0x3E,0x91,0x5A,0x92,0x42,0x00,0x7A,0x80,0x81,0x82,0x02,0x73,
0x90,0x40,0x91,0x40,0x92,0x40,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,
0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,
0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,
0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,
0x00,0x02,0x00,0x7A,0x80,0x00,0x02,0x90,0x3C,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,
0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x81,0x82,0x00,0x02,0x91,0x40,0x92,0x4C,0x00,
0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x81,0x82,0x00,
0x02,0x91,0x40,0x92,0x4C,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,
0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x81,0x82,0x00,0x02,0x91,
0x47,0x92,0x53,0x00,0x7A,0x00,0x02,0x00,0x7A,0x80,0x81,0x82,0x00,0x02,0x90,0x3D,
0x91,0x45,0x92,0x51,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,
0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,
0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,
0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,
0x00,0x7A,0x80,0x81,0x82,0x00,0x02,0x90,0x3C,0x91,0x43,0x92,0x4F,0x00,0x7A,0x00,
0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,
0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x80,0x81,0x82,0x00,0x02,
0x90,0x3E,0x91,0x42,0x92,0x4E,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,
0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,
0x00,0x02,0x00,0x7A,0x80,0x81,0x82,0x00,0x02,0x90,0x40,0x91,0x40,0x92,0x4C,0x00,
0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,
0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,
0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,
0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x80,0x00,0x02,
0x90,0x3C,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,
0x81,0x82,0x00,0x02,0x91,0x40,0x92,0x4C,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,
0x00,0x7A,0x00,0x02,0x00,0x7A,0x81,0x82,0x00,0x02,0x91,0x40,0x92,0x4C,0x00,0x7A,
0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,
0x00,0x02,0x00,0x7A,0x81,0x82,0x00,0x02,0x91,0x47,0x92,0x53,0x00,0x7A,0x00,0x02,
0x00,0x7A,0x80,0x81,0x82,0x00,0x02,0x90,0x3D,0x91,0x49,0x92,0x55,0x00,0x7A,0x00,
0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,
0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x81,0x82,0x00,0x02,0x91,
0x45,0x92,0x51,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,
0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,
0x7A,0x80,0x81,0x82,0x00,0x02,0x90,0x3C,0x91,0x47,0x92,0x53,0x00,0x7A,0x00,0x02,
0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,
0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x80,0x00,0x02,0x90,0x3B,0x00,
0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,
0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x00,0x02,0x00,0x7A,0x80,0x81,0x82,
0x00,0x02,0x90,0x43,0x92,0x4F,0x91,0x58,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,
0x7A,0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,0x82,
0x00,0x02,0x92,0x4C,0x00,0x7A,0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,0x82,0x00,0x02,
0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,
0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,
0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,0x82,0x00,
0x02,0x92,0x4C,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,
0x4F,0x00,0x7A,0x80,0x82,0x00,0x02,0x90,0x40,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,
0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,0x82,0x00,0x02,0x92,0x4C,
0x00,0x7A,0x82,0x81,0x00,0x02,0x92,0x4C,0x91,0x58,0x00,0x7A,0x82,0x00,0x02,0x92,
0x4C,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,
0x7A,0x82,0x81,0x00,0x02,0x92,0x4F,0x91,0x58,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,
0x00,0x7A,0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,
0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,0x82,0x81,
0x00,0x02,0x92,0x4F,0x91,0x5F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x80,
0x82,0x81,0x00,0x02,0x90,0x40,0x92,0x4F,0x91,0x5D,0x00,0x7A,0x82,0x00,0x02,0x92,
0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,0x82,0x00,0x02,0x92,0x4C,0x00,
0x7A,0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,0x82,
0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,
0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4C,
0x00,0x7A,0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,
0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,
0x02,0x92,0x4F,0x00,0x7A,0x80,0x82,0x81,0x00,0x02,0x90,0x40,0x92,0x4F,0x91,0x5B,
0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,
0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,0x82,0x00,
0x02,0x92,0x4C,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,
0x4F,0x00,0x7A,0x80,0x82,0x81,0x00,0x02,0x90,0x42,0x92,0x4F,0x91,0x5A,0x00,0x7A,
0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,0x82,0x00,
0x02,0x92,0x4C,0x00,0x7A,0x82,0x00,0x02,0x92,0x4C,0x00,0x7A,0x82,0x00,0x02,0x92,
0x4C,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,0x7A,0x82,0x00,0x02,0x92,0x4F,0x00,
0x7A,0x80,0x82,0x81,0x00,0x02,0x90,0x3E,0x92,0x4F,0x91,0x58,0x00,0x7A,0x82,0x80,
0x00,0x02,0x92,0x4F,0x90,0x43,0x00,0x7A,0x82,0x80,0x00,0x02,0x90,0x40,0x92,0x4C,
0x00,0x7A,0x82,0x80,0x00,0x02,0x92,0x4C,0x90,0x40,0x00,0x7A,0x82,0x80,0x00,0x02,
0x92,0x4C,0x90,0x40,0x00,0x7A,0x82,0x80,0x00,0x02,0x92,0x4C,0x90,0x40,0x00,0x7A,
0x82,0x80,0x00,0x02,0x92,0x4F,0x90,0x43,0x00,0x7A,0x82,0x80,0x00,0x02,0x92,0x4F,
0x90,0x43,0x00,0x7A,0x82,0x80,0x00,0x02,0x90,0x40,0x92,0x4F,0x00,0x7A,0x82,0x80,
0x00,0x02,0x92,0x4F,0x90,0x43,0x00,0x7A,0x82,0x80,0x00,0x02,0x90,0x42,0x92,0x4C,
0x00,0x7A,0x82,0x80,0x00,0x02,0x92,0x4C,0x90,0x40,0x00,0x7A,0x82,0x80,0x00,0x02,
0x92,0x4C,0x90,0x40,0x00,0x7A,0x82,0x80,0x00,0x02,0x92,0x4C,0x90,0x40,0x00,0x7A,
0x82,0x80,0x00,0x02,0x92,0x4F,0x90,0x43,0x00,0x7A,0x82,0x80,0x00,0x02,0x92,0x4F,
0x90,0x43,0x00,0x7A,0x82,0x81,0x80,0x00,0x02,0x90,0x3E,0x92,0x4A,0x91,0x56,0x00,
0x7A,0x82,0x81,0x80,0x00,0x7F,0x92,0x4C,0x91,0x58,0x90,0x4C,0x00,0x7A,0x82,0x81,
0x80,0x02,0x73,0x92,0x4C,0x91,0x58,0x90,0x4C,0x00,0x7A,0x82,0x81,0x80,0x00,0x7F,
0x92,0x4E,0x91,0x5A,0x90,0x4E,0x00,0x7A,0x82,0x81,0x80,0x02,0x73,0x92,0x4C,0x91,
0x58,0x90,0x4C,0x0F,0x9D,0x82,0x81,0x80,0xF0};

Playtune p;

void setup()
{

  strip.setBrightness(BRIGHTNESS);
  strip.begin(); // 네오픽셀 제어 시작
  strip.show();  // 네오픽셀 점등

  pinMode(APDS9960_INT, INPUT);

  // Initialize Serial port
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("-------------------------------------"));
  Serial.println(F("SparkFun APDS-9960 - Light Interrupts"));
  Serial.println(F("-------------------------------------"));

  // Initialize interrupt service routine
  attachInterrupt(0, interruptRoutine, FALLING);

  // Initialize APDS-9960 (configure I2C and initial values)
  if (apds.init())
  {
    Serial.println(F("APDS-9960 initialization complete"));
  }
  else
  {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }

  // Set high and low interrupt thresholds
  if (!apds.setLightIntLowThreshold(LIGHT_INT_LOW))
  {
    Serial.println(F("Error writing low threshold"));
  }
  if (!apds.setLightIntHighThreshold(LIGHT_INT_HIGH))
  {
    Serial.println(F("Error writing high threshold"));
  }

  // Start running the APDS-9960 light sensor (no interrupts)
  if (apds.enableLightSensor(false))
  {
    Serial.println(F("Light sensor is now running"));
  }
  else
  {
    Serial.println(F("Something went wrong during light sensor init!"));
  }

  // Read high and low interrupt thresholds
  if (!apds.getLightIntLowThreshold(threshold))
  {
    Serial.println(F("Error reading low threshold"));
  }
  else
  {
    Serial.print(F("Low Threshold: "));
    Serial.println(threshold);
  }
  if (!apds.getLightIntHighThreshold(threshold))
  {
    Serial.println(F("Error reading high threshold"));
  }
  else
  {
    Serial.print(F("High Threshold: "));
    Serial.println(threshold);
  }

  // Enable interrupts
  if (!apds.setAmbientLightIntEnable(1))
  {
    Serial.println(F("Error enabling interrupts"));
  }

  // Wait for initialization and calibration to finish
  delay(500);

  p.tune_initchan(10);
  p.tune_initchan(11);
  p.tune_initchan(12);
}

void loop()
{
  // If interrupt occurs, print out the light levels
  if (isr_flag == 1)
  {

    // Read the light levels (ambient, red, green, blue) and print
    if (!apds.readAmbientLight(ambient_light) ||
        !apds.readRedLight(red_light) ||
        !apds.readGreenLight(green_light) ||
        !apds.readBlueLight(blue_light))
    {
      Serial.println("Error reading light values");
    }
    else
    {
      if (ambient_light < 10)
      {
        strip.begin();
        // strip.setPixelColor(0, 255, 0, 0, 0); // 첫 번째 숫자: 0번째(1번) 픽셀 소자, 0=1번째 픽셀, 1=2번째 픽셀 ...
        //strip.setPixelColor(1, 0, 255, 0, 0); // 둘째, 셋제, 넷째는 R(RED), G(GREEN), B(BLUE), W(White)값, 0~255사이 표현
        //strip.setPixelColor(2, 0, 0, 255, 0); // 3번째 픽셀에 B(Blue)소자를 255로 켜기
        for(i = 0; i < 24; i++){
          strip.setPixelColor(i, 0, 0, 0, 255); // 4번째 픽셀에 W(White)소자를 255로 켜기
          strip.show();
          delay(150);
        }

      }
      else
      {
        for(i = 0; i < 24; i++){
          strip.setPixelColor(i, 0, 0, 0, 0); // 4번째 픽셀에 W(White)소자를 255로 켜기
          strip.show();
          delay(50);
        }
      }

      Serial.print("Interrupt! Ambient: ");
      Serial.print(ambient_light);
      Serial.print(" R: ");
      Serial.print(red_light);
      Serial.print(" G: ");
      Serial.print(green_light);
      Serial.print(" B: ");
      Serial.println(blue_light);
    }

    // Reset flag and clear APDS-9960 interrupt (IMPORTANT!)
    isr_flag = 0;
    if (!apds.clearAmbientLightInt())
    {
      Serial.println("Error clearing interrupt");
    }
  }
  p.tune_playscore(avengers); // start
  while (p.tune_playing);     // wait while playing
  p.tune_delay(3000);         // do not use Arduino delay
}