#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/multicore.h"
#include "hardware/gpio.h"
#include "hardware/spi.h"
#include "hardware/timer.h"
#include "core1.h"
#include "define.h"

int LastABLen = 1;
int NowABLen = 1;

int main()
{
    #ifdef DEBUG
    stdio_init_all();
    #endif
    
    #ifndef DEBUG
    set_sys_clock_khz(30000, true); 
    #endif

    gpio_init(LED_PIN); 
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_init(DATA); 
    gpio_set_dir(DATA, GPIO_OUT);
    gpio_init(CLOCK); 
    gpio_set_dir(CLOCK, GPIO_OUT);
    gpio_init(CI522_RST); 
    gpio_set_dir(CI522_RST, GPIO_OUT);
    gpio_init(ABLEN); 
    //gpio_pull_up(ABLEN); 
    
    gpio_put(DATA, 1);
    gpio_put(CLOCK, 1);
    
    gpio_init(CI522_NSS);
    gpio_set_dir(CI522_NSS, GPIO_OUT);
    gpio_put(CI522_NSS, 1);

    spi_init(spi1, 1000000);

    spi_set_format(spi1, 8, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST);

    gpio_set_function(CI522_SCK, GPIO_FUNC_SPI);
    gpio_set_function(CI522_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(CI522_MISO, GPIO_FUNC_SPI);

    #ifdef DEBUG
    printf("[core0] OOO custom nfc driver is loaded...\n");
    #endif

    while(1)
    {
        #ifdef DEBUG
        //printf("[core0] Alive..\n");
        #endif

        NowABLen = gpio_get(ABLEN);
        if(NowABLen != LastABLen && NowABLen == 1)
        {
            //gpio_put(LED_PIN, 0);
            #ifdef DEBUG
            printf("[core0] Not flashing...\n");
            #endif

            multicore_reset_core1();
            busy_wait_us(500);
            gpio_put(DATA, 1);
            gpio_put(CLOCK, 1);
            //spi_deinit(spi1);
            gpio_put(CI522_RST, 0);
        }
        if(NowABLen != LastABLen && NowABLen == 0)
        {
            gpio_put(CI522_RST, 1);
            gpio_put(DATA, 1);
            gpio_put(CLOCK, 1);
            multicore_reset_core1();
            busy_wait_us(500);

            #ifdef DEBUG
            printf("[core0] Flashing...\n");
            #endif

            multicore_launch_core1(core1_entry); 
        }
        LastABLen = NowABLen;
        busy_wait_us(500);
    }
    
    return 0;
}