#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "define.h"
#include "core1.h"
#include <rfid_rc522/rc522.h> 


uint8_t Count;
uint8_t buffer[18];
StatusCode status;

void core1_entry()
{
    Count = sizeof(buffer);

    #ifdef DEBUG
    printf("[core1] Initing CI522!\n");
    #endif

    MFRC522Ptr_t mfrc = MFRC522_Init(CI522_NSS);
    PCD_Init(mfrc, spi1);
    PCD_SetAntennaGain(mfrc, RxGain_48dB);
    busy_wait_us(500);

    #ifdef DEBUG
    printf("[core1] Inited CI522!\n");
    PCD_DumpVersionToSerial(mfrc);
    #endif
    
    while(1)
    {     
            
        gpio_put(LED_PIN, 0);
        //printf("[core1] Waiting for card\n");
        while(!PICC_IsNewCardPresent(mfrc));

        #ifdef DEBUG
        printf("[core1] Selecting card\n");
        #endif
        PICC_ReadCardSerial(mfrc);

        if(((mfrc->uid).sak &= 0x7F) != 0x00)
        {
            #ifdef DEBUG
            printf("[core1] Not NTAG213 card\n");
            #endif
            continue;
        }
        
        #ifdef DEBUG
        printf("[core1] Reading card\n");
        #endif

        status = MIFARE_Read(mfrc, 7, buffer, &Count);
        if (status != STATUS_OK) 
        {
            #ifdef DEBUG
            printf("[core1] MIFARE_Read() failed: %s \n", GetStatusCodeName(status));
            #endif
            continue;
        }

        NewProcess(buffer);
        busy_wait_us(200);

    }
}

void NewProcess(uint8_t *record)
{
    for (uint8_t index = 2; index < 10; index++) 
    {
        if((int)record[index] != 48 && (int)record[index] != 49)
        {
            return;
        }
    }

    gpio_put(LED_PIN, 1);

    //000000001xxxxxxxx011
    Start();
    Data0();
    Data0();
    Data0();
    Data0();
    Data0();
    Data0();
    Data0();
    Data1();

    for (uint8_t index = 2; index < 10; index++) 
    {
        if((int)record[index] == 48)
        {
            Data0();
        }
        else
        {
            Data1();
        }
    }
    Data0();
    Data1();
    Data1();

    busy_wait_us(1200);
    busy_wait_us(500);
}

void Start()
{
    gpio_put(DATA, 0);
    gpio_put(CLOCK, 0);
    busy_wait_us(600);
    gpio_put(CLOCK, 0);
    busy_wait_us(600);
    gpio_put(CLOCK, 0);
    busy_wait_us(600);
}

void Data0()
{
    gpio_put(DATA, 0);
    gpio_put(CLOCK, 1);
    busy_wait_us(600);
    gpio_put(CLOCK, 0);
    busy_wait_us(600);
    gpio_put(CLOCK, 1);
    busy_wait_us(600);
}

void Data1()
{
    gpio_put(DATA, 1);
    gpio_put(CLOCK, 1);
    busy_wait_us(600);
    gpio_put(CLOCK, 0);
    busy_wait_us(600);
    gpio_put(CLOCK, 1);
    busy_wait_us(600);
}