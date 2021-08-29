#include <SPI.h>

#define DEBUG true

int MSG_WIDTH = 110; //Used to create a buffer large enough to hold the unfiltered response
int HOLDINT = 1000; //Above 1000 to prevent issues

char* StatusRequest() {
    // Set SS to LOW (Peripheral Selected/Active)
    digitalWrite(SS, LOW);
    // Define SPI Transaction Protocol Variables
    char ACK1;
    char* MSG;
    int i = 0;
    boolean looping = true;
    char Status[MSG_WIDTH];
    // Request ACK Byte
    SPI.transfer('!');
    ACK1 = SPI.transfer('<'); //Send SYN Byte
    delayMicroseconds(HOLDINT); //Delay long enough for Peripheral to set its SPDR pointer to its response. HOLDINT isn't long enough if the Controller Receives '<' or '?'
    // Verify ACK
    if (ACK1 == '!') {
        // Good ACK
        if(DEBUG) { Serial.println("Good ACK1 from Peripheral"); }
        // Loop Until '>' is Received
        while(looping) {
            Status[i] = SPI.transfer('?'); //Send '?' to appease the Code Wizards and receive data
            delayMicroseconds(HOLDINT); //Delay long enough for Peripheral to set its SPDR pointer to its response. HOLDINT isn't long enough if the Controller Receives '<' or '?'
            // Check for End of Message
            if (Status[i] == '>') {
                // Set Final Message to Substring Before '>' and Break Loop
                MSG = strtok(Status, ">");
                looping = false;
                if(DEBUG) { Serial.println("Good ACK from Peripheral, Connection Closed"); }
                break;
            }
            if (DEBUG) {
              Serial.print(i);
              Serial.print(" : ");
              Serial.println(Status[i]);
            }
            // Move to Next Character
            i++;
        }
        return MSG;
    }
    else {
        // I'm having a Heart-ACK-ACK-ACK, You oughta know by now
        if(DEBUG) { 
          Serial.print("Failed ACK from Peripheral : ");
          Serial.println(ACK1); 
        }
        return "N"; //You really only get this if the transmissions get desynchronized or while connecting to serial
    }
    // Set SS to HIGH (Peripheral Deselected/Inactive)
    digitalWrite(SS, HIGH);
}

void setup() {
    // Setup Serial and Pins
    Serial.begin(115200);
    if(DEBUG) { Serial.println("BOOT: Controller"); }
    pinMode(SS, OUTPUT);
    // SPI Setup
    SPI.setClockDivider(SPI_CLOCK_DIV4); //Arduino Nano Clock Speed: 16MHz / 4 = 4MHz SPI; Only set by Controller
    SPI.begin();
}

void loop() {
    if (DEBUG) { Serial.println("LOOP"); }
    Serial.println(StatusRequest());
    delay(500); //Delay 5 Seconds between message requests
}
