/******************************************************************************
DS3234_RTC_SRAM_Demo.ino
Andreas Grommek
original creation date: November 2, 2021
https://github.com/sparkfun/SparkFun_DS3234_RTC_Arduino_Library

Demonstration of storing data to and retrieving data from DS3234 SRAM.

Resources:
SPI.h - Arduino SPI Library

Development environment specifics:
Arduino 1.8.16
Any Arduino
SparkFun Real Time Clock Module (v14)
*****************************************************************************/
#include <SPI.h>
#include <SparkFunDS3234RTC.h>

//////////////////////////////////
// Configurable Pin Definitions //
//////////////////////////////////

#define DS13074_CS_PIN 10 // DeadOn RTC Chip-select pin

void setup()  {
  // Use the serial monitor to view output
  Serial.begin(9600);
  while (!Serial); // needed for those Arduinos with Serial-over-USB
  Serial.println(F("\n### Starting DS3242_RTC_SRAM_Demo! ###\n"));

  // Call rtc.begin([cs]) to initialize the library
  // The chip-select pin should be sent as the only parameter
  rtc.begin(DS13074_CS_PIN);

  // We don't set time or do other clock-related stuff here.
  // This is a demo about SRAM functionality, after all. ;-)

  /* Write and read *single* bytes to/from SRAM */

  // The DS3234 has 256 bytes of SRAM
  // --> the address is a uint8_t ranging from 0 to 255
  // (it does not matter if we write it as decimal or hexadecimal value)
  uint8_t sram_address = 161; // == 0xA1

  // chose a some data to store in SRAM
  uint8_t data_byte = 42;

  Serial.print(F("Writing a byte with value "));
  Serial.print(data_byte, DEC);
  Serial.print(F(" to memory address 0x"));
  Serial.print(sram_address, HEX);
  Serial.print(F("."));
  Serial.println();

  // Do the writing here...
  // Note: If you have battery support on your RTC module, this value
  // will survive reboots and even extended peroiods without external power. 
  rtc.writeToSRAM(sram_address, data_byte);

  // now we read the value back from SRAM
  uint8_t read_back_byte = rtc.readFromSRAM(sram_address);
  
  Serial.print(F("We have read back a byte with value "));
  Serial.print(read_back_byte, DEC);
  Serial.print(F(" from memory address 0x"));
  Serial.print(sram_address, HEX);
  Serial.print(F("."));
  Serial.println();

  /* Write and read *several* bytes to/from an array */

  // Put some data into an array of bytes.
  // This is the data we will write to SRAM in a single operation. 
  uint8_t write_buffer[] = {101, 102, 103, 104, 199, 255};

  // Get the length of the array to write (and later to read back).
  //
  // Note: Don't be confused about the data type "size_t" here, it is not really important.
  // size_t is just the data type used by C++ whenever sizes are used as function paramters
  // or returned by functions.
  // size_t is always an unsigned integer. On Arduinos based on AVR 328p (Arduino Uno & Co)
  // size_t is the same as uint16_t. On SAMD21 (Arduino Zero and friends), size_t is 
  // equivalent to uint32_t. 
  const size_t len = sizeof(write_buffer);

  Serial.print(F("Writing "));
  Serial.print(len, DEC);
  Serial.print(F(" byte(s) to SRAM, starting at address 0x"));
  Serial.print(sram_address, HEX);
  Serial.println();
  Serial.print(F("Values to write: "));
  for (size_t i=0; i<len; ++i) {
    Serial.print(write_buffer[i], DEC);
    Serial.print(F(" "));
  }
  Serial.println();

  // Now we do the writing.
  // Note: We use write_buffer without the square brackets here,
  // i.e. "write_buffer" instead of "write_buffer[]". The name
  // of an array "decays" to a pointer when used in this way to a
  // pointer to the memory location of the first element in the array.
  rtc.writeToSRAM(sram_address, write_buffer, len);
  //rtc.writeToSRAM(sram_address, &write_buffer[0], len); // this a 100% equivalent to the line above!

  // create an array to hold the data we read back from SRAM
  uint8_t read_buffer[len] = {0}; // initialize array with zeros

  // Now read back the stored data into read_buffer.
  // Note: The function does not return a value. The bytes read back from SRAM are 
  // stored in the array read_buffer which is given as a parameter!
  // Warning: You have to take care yourself, that read_buffer is big enough to hold 
  // the data you want to read back from SRAM. Otherwise you will have an "out-of-bounds"
  // write of your array and "bad things" will happen (i.e. your program will probably crash).
  rtc.readFromSRAM(sram_address, read_buffer, len);

  // Now show what we have read from SRAM.
  Serial.print(F("Data we have read back starting from SRAM address 0x"));
  Serial.print(sram_address, HEX);
  Serial.print(F(": "));
  for (size_t i=0; i<len; ++i) {
    Serial.print(read_buffer[i], DEC);
    Serial.print(F(" "));
  }
  Serial.println();

  Serial.println(F("\n### DS3242_RTC_SRAM_Demo finished! ###\n"));

} // end of setup(), end of demo

void loop()  {
    // do nothing in loop()
}
