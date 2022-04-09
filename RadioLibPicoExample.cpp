/*
   A copy of the "RadioLib SX127x Receive with Interrupts Example" 
   modified to build for the Raspberry Pico without Arduino

   ****

   This example listens for LoRa transmissions and tries to
   receive them. Once a packet is received, an interrupt is
   triggered. To successfully receive data, the following
   settings have to be the same on both transmitter
   and receiver:
    - carrier frequency
    - bandwidth
    - spreading factor
    - coding rate
    - sync word

   Other modules from SX127x/RFM9x family can also be used.

   For default module settings, see the wiki page
   https://github.com/jgromes/RadioLib/wiki/Default-configuration#sx127xrfm9x---lora-modem

   For full API reference, see the GitHub Pages
   https://jgromes.github.io/RadioLib/
*/


#include <stdio.h>
#include "pico/stdlib.h"

#include "config.h"
#include <RadioLib.h>

volatile bool receivedFlag = false;
volatile bool enableInterrupt = true;

void setFlag()
{
    if(!enableInterrupt) 
        return;

    // we got a packet, set the flag
    receivedFlag = true;
}

int main()
{
    stdio_init_all();

    // Pins set in config.h
    SX1276 radio = new Module(PIN_CS, PIN_DI0, PIN_RST, PIN_DI1);

    printf("[SX1276] Initializing ... ");
    int state = radio.begin(868, 125.0, 9, 7, 0x12, 10, 8, 0);
    
    if (state == RADIOLIB_ERR_NONE) 
    {
        printf("success!\n");
    }
    else 
    {
        printf("failed, code: %d\n", state);
        while (true);
    }

    radio.setDio0Action(setFlag);
    printf("[SX1276] Starting to listen ...");
    state = radio.startReceive();
    if (state == RADIOLIB_ERR_NONE) 
    {
        printf("success!\n");
    }
    else 
    {
        printf("failed, code %d\n", state);
        while (true);
    }

    while (1)
    {
        // check if the flag is set
        if(receivedFlag) 
        {
            // disable the interrupt service routine while
            // processing the data
            enableInterrupt = false;

            // reset flag
            receivedFlag = false;

            // you can read received data as an Arduino String
            std::string str;
            int state = radio.readData(str);

            // you can also read received data as byte array
            /*
            byte byteArr[8];
            int state = radio.readData(byteArr, 8);
            */

            if (state == RADIOLIB_ERR_NONE)
            {
                // packet was successfully received
                printf("[SX1276] Received packet!\n");

                // print data of the packet
                printf("[SX1276] Data:\t\t %s\n", str.c_str());

                // print RSSI (Received Signal Strength Indicator)
                printf("[SX1276] RSSI:\t\t %f dBm\n", radio.getRSSI());

                // print SNR (Signal-to-Noise Ratio)
                printf("[SX1276] SNR:\t\t %f dB\n", radio.getSNR());

                // print frequency error
                printf("[SX1276] Frequency error:\t %f\n", radio.getFrequencyError());
            } 
            else if (state == RADIOLIB_ERR_CRC_MISMATCH) 
            {
                // packet was received, but is malformed
                printf("[SX1276] CRC error!");
            }
            else 
            {
                // some other error occurred
                printf("[SX1276] Failed, code %d\n", state);
            }

            // put module back to listen mode
            radio.startReceive();

            // we're ready to receive more packets,
            // enable interrupt service routine
            enableInterrupt = true;
        }
    }
    return 0;
}
