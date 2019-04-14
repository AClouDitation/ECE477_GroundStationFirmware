/**
 *  Keil project for NRF24L01+ transceiver
 *
 *  Transmitter code
 *
 *  Before you start, select your target, on the right of the "Load" button
 *
 *  @author     Tilen Majerle
 *  @email      tilen@majerle.eu
 *  @website    http://stm32f4-discovery.net
 *  @ide        Keil uVision 5
 *  @packs      STM32F4xx Keil packs version 2.2.0 or greater required
 *  @stdperiph  STM32F4xx Standard peripheral drivers version 1.4.0 or greater required
 */
/* Include core modules */
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_nrf24l01.h"
//#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_usart.h"
#include <stdio.h>

/* My address */
uint8_t MyAddress[] = {
    0xE7,
    0xE7,
    0xE7,
    0xE7,
    0xE7
};
/* Receiver address */
uint8_t TxAddress[] = {
    0x7E,
    0x7E,
    0x7E,
    0x7E,
    0x7E
};

uint8_t dataOut[10];

int main(void) {
    TM_NRF24L01_Transmit_Status_t transmissionStatus;

    /* Initialize system and Delay functions */
    TM_DELAY_Init();

    /* Initialize USART, TX: PB6, RX: PB7 */
    TM_USART_Init(USART1, TM_USART_PinsPack_1, 115200);

    /* Initialize NRF24L01+ on channel 15 and 32bytes of payload */
    /* By default 2Mbps data rate and 0dBm output power */
    /* NRF24L01 goes to RX mode by default */
    TM_NRF24L01_Init(15, 32);

    /* Set 2MBps data rate and -18dBm output power */
    TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_2M, TM_NRF24L01_OutputPower_M18dBm);

    /* Set my address, 5 bytes */
    TM_NRF24L01_SetMyAddress(MyAddress);
    /* Set TX address, 5 bytes */
    TM_NRF24L01_SetTxAddress(TxAddress);

    /* Reset counter */
    TM_DELAY_SetTime(2001);
    //sprintf((char *)dataOut, "100$100$150$100$150$150$100$100");
    dataOut[0] = 1;     dataOut[1] = 100; dataOut[2] = 100; dataOut[3] = 150;
    dataOut[4] = 100;   dataOut[5] = 150; dataOut[6] = 150; dataOut[7] = 100;
    dataOut[8] = 100;
    while (1) {
        /* Every 5ms */
        //if (TM_DELAY_Time() > 5) {
            //TM_DELAY_SetTime(0);
            /* Fill data with something */
            /* source select $ gimbal x $ gimbal y $ ch1 $ ch2 $ ch3 $ ch4 $ ch5 $ ch6 $ */

            while(!TM_USART_Gets(USART1, dataOut, 10));
            TM_NRF24L01_Transmit(dataOut);

            /* Wait for data to be sent */
            do {
                transmissionStatus = TM_NRF24L01_GetTransmissionStatus();
            } while (transmissionStatus == TM_NRF24L01_Transmit_Status_Sending);


        //}
    }
}
