#include <SPI.h>

// Definitions
#define DEBUG true
// Global Variables
int MSG_WIDTH = 65; //This is the length of the Status message; Could probably be more elegant and dynamic, EX: sizeof(Status)
// Variables to be Changed by SPI Handler
volatile int SPI_Index;
volatile boolean DATA_ACTIVE;
volatile char* Status = "FF01 0112 0211 0310 0409 0508 0607 0706 0805 0904 1003 1102 1201>";

// SPI Interrupt Handler
ISR (SPI_STC_vect) {
  // Receive Byte
  char c = SPDR;
  // Check if Byte is SYN
  if (c == '<') {
    SPDR = Status[0];
    if(DEBUG) { 
      Serial.print(0);
      Serial.print(" : ");
      Serial.print(c);
      Serial.print(" : ");
      Serial.println(Status[0]);
    }
    SPI_Index = 1;
    DATA_ACTIVE = true;
  }
  // Check if Byte is DATA
  else if (c == '?' && DATA_ACTIVE) {
    SPDR = Status[SPI_Index]; //Respond with coresponding DATA Byte in sequence
    if(DEBUG) { 
      Serial.print(SPI_Index);
      Serial.print(" : ");
      Serial.print(c);
      Serial.print(" : ");
      Serial.println(Status[SPI_Index]);
    }
    if (SPI_Index >= MSG_WIDTH) {
      SPDR = '<';
      SPI_Index = 1;
      DATA_ACTIVE = false;
      if(DEBUG) { Serial.println("Index at Bounds (End of Message)"); }
    }
    else {
      SPI_Index++;
    }
  }
  // End of Sequence; Reset
  if (SPI_Index == MSG_WIDTH) {
    SPI_Index = 0;
    DATA_ACTIVE = false;
  }
  // Check if Index is out of Bounds
  if (SPI_Index > MSG_WIDTH) {
    // This should really never happen
    SPI_Index = 0;
    DATA_ACTIVE = false;
    if(DEBUG) { Serial.println("Index out of Bounds"); }
  }
}

void setup() {
  // Setup Serial and Pins
  Serial.begin(115200);
  if(DEBUG) { Serial.println("BOOT: Peripheral"); }
  pinMode(MISO, OUTPUT);
  // SPI Setup
  SPCR |= _BV(SPE); //Enable SPI Interrupt
  SPI.attachInterrupt(); //Turn on Interrupt
  SPI.begin();
  // Configure SPI Transaction Protocol
  SPI_Index = 1;
  DATA_ACTIVE = false;
  SPDR = '<'; //Prep Initial Response (ACK)
}

void loop() {
  // Q: What do we want? 
  // A: An Interrupt!
  // Q: When do we want it?
  // A: Whenever it happ...
  delay(2); //Just do something, anything
}
