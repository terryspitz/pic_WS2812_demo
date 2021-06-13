/* 
 * File:   main.c
 * Author: terryspitz
 * 
 * LED strip demo for WS2812B ring, e.g. https://coolcomponents.co.uk/products/12-led-ring-sk6812-5050-rgb-led-with-integrated-drivers-adafruit-neopixel-compatible
 * 
 * MCC configured using https://blog.kubovy.eu/2019/02/17/ws281x-using-pic/ 
 * 
 * Connect WS2812B Data In on pin RC1 or reconfigure this in MCC Pin Manager: Grid View (in MPLAB X IDE)
 * 
 *  */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "mcc_generated_files/mcc.h"

#define LED_COUNT 12
uint8_t WS281x_led[LED_COUNT][3];

inline void WS281x_show() {
    //the magic happens here
    SPI1_WriteBlock(WS281x_led, LED_COUNT*3);
    __delay_us(50);
}

inline void WS281x_all(uint8_t r, uint8_t g, uint8_t b) {
    for (uint8_t led = 0; led < LED_COUNT; led++) {
        WS281x_led[led][0] = g;
        WS281x_led[led][1] = r;
        WS281x_led[led][2] = b;
    }
    WS281x_show();
}

inline void leds_off() {
    WS281x_all(0x00, 0x00, 0x00);
    WS281x_show();
}

void main(void) {
    SYSTEM_Initialize();
    SPI1_Open(SPI1_DEFAULT);

    //startup sequence
//    IO_RC0_SetHigh();
    WS281x_all(255, 0, 0);
    __delay_ms(500);
    WS281x_all(0, 255, 0);
    __delay_ms(500);
    WS281x_all(0, 0, 255);
    __delay_ms(500);
            
    while(1) {
        //flash an LED
        LED_RC5_SetHigh();
        __delay_ms(50);
        LED_RC5_SetLow();
        __delay_ms(50);
        
        leds_off();

        // scanner on LED strip
        for(uint8_t repeat=0; repeat<15; ++repeat) {
            for(uint8_t chase=0; chase<LED_COUNT; ++chase) {
                for(uint8_t led=0; led<LED_COUNT; ++led) {
                    uint8_t level = (led+LED_COUNT-chase)%LED_COUNT;
                    level = level > 8 ? 8 : level;
                    WS281x_led[led][2] = level*level;
                }
                WS281x_show();
                __delay_ms(50);
            }
        }        
        
    } //while 1

}
