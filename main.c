/* 
 * File:   main.c
 * Author: terryspitz
 */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "mcc_generated_files/mcc.h"


#define LED_COUNT 12
uint8_t WS281x_led[LED_COUNT][3];

inline void WS281x_show() {
    SPI1_WriteBlock(WS281x_led, LED_COUNT*3);
    __delay_us(50);
}

inline void WS281x_all(uint8_t r, uint8_t g, uint8_t b) {
    for (uint8_t led = 0; led < LED_COUNT; led++) {
        WS281x_led[led][0] = g;
        WS281x_led[led][1] = r;
        WS281x_led[led][2] = b;
    }
}

inline void leds_off() {
    WS281x_all(0x00, 0x00, 0x00);
    WS281x_show();
}

void main(void) {
    SYSTEM_Initialize();
    SPI1_Open(SPI1_DEFAULT);

//    IO_RC0_SetHigh();
    
            
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
