/*
  Copyright: MyRetroStore (myretrostore.co.uk)
  Date: 22 August 2024
  Version 1.0
*/
  
#include <DIO2.h> // include the fast I/O functions
#include "config.h"
#include "MD5.h"
#include "crc.h"
#include <stdint.h>

//----------------------------------------------------------------------
// Global variables
//----------------------------------------------------------------------
const uint32_t POLY = 0xEDB88320;

#define CE 6 
#define LED_Green A7
#define LED_Red A6

boolean hashmatch = false;
uint8_t addressLines[] = {36, 33, 32, 29, 28, 27, 26, A2, 11, 10, 7, 9, A1, 12, 13, A0, 8};
uint8_t dataLines[] = {37, 38, 39, 24, 23, 22, 4, 5};

size_t index = 0;

uint8_t ROM[4100] = {};  // Buffer to hold 4k of ROM
uint16_t ROM_counter = 0;
uint8_t ROM_Byte;

int ledState = HIGH;     // the current state of LED
long addressStart = 0, addressEnd = 0;
long addressCurrent;
uint8_t error_count=0;
uint32_t crc1;
uint32_t crc2;
uint32_t crc3;
uint32_t crc4;


//----------------------------------------------------------------------
// Calculate CRC-32 value
//----------------------------------------------------------------------
uint32_t calculate_crc32(uint32_t crc, uint32_t value) {
  crc ^= value;
  for (uint8_t i = 0; i < 32; i++) {
    if (crc & 1) {
      crc = (crc >> 1) ^ POLY;
    } else {
      crc >>= 1;
    }
  }
  return crc;
}

//----------------------------------------------------------------------
// Toggle LED for feedback during checking
//----------------------------------------------------------------------
void toggleLED()
{
  digitalWrite2(LED_Red, ledState);
  digitalWrite2(LED_Green, !ledState);
  ledState = !ledState;
}

//----------------------------------------------------------------------
// Check if CRC-32 value read is valid
//----------------------------------------------------------------------
void check_crc(uint32_t crc)
{
  hashmatch = false;

  const size_t crcCnt_16k = sizeof crc_16k / sizeof * crc_16k;

  t_crc_16k data;
  ROM_counter = 0;

  char file[32];
  char hash[33];

  for (index = 0; index < crcCnt_16k; index++) {
    readArray(index, data);

    uint32_t hashnumber = strtoul(data.hash, NULL, 16);

    if (crc == hashnumber) {
      hashmatch = true;
      Serial.print("Match found for ");
      Serial.println(data.fileName);
      // toggleLED();
      break;
    }
  }

  if (hashmatch == false) {
    Serial.print("CRC-32 match not found for ");
    Serial.println(crc, HEX);
    error_count++;
  }
}

//----------------------------------------------------------------------
// Read data from data pins
//----------------------------------------------------------------------
uint8_t getData(void)
{
  uint8_t readback;
  char data = 0;
  for (uint8_t i = 0; i < sizeof(dataLines); i++)
  {
    readback = digitalRead2(dataLines[i]);
    data |= readback ? (1 << i) : 0;
  }
  return data;
}

//----------------------------------------------------------------------
// Set Address
//----------------------------------------------------------------------
void setAddr(long address)
{
  for (uint8_t i = 0; i < sizeof(addressLines); i++)
  {
    digitalWrite2(addressLines[i], ((address >> i) & 1));
  }
}

//----------------------------------------------------------------------
// Init I/O Pins
//----------------------------------------------------------------------
void InitialisePins()
{
  pinMode2(LED_Green, OUTPUT);
  pinMode2(LED_Red, OUTPUT);

  digitalWrite2(LED_Green, LOW);
  digitalWrite2(LED_Red, LOW);

  pinMode2(CE, OUTPUT);
  digitalWrite2(CE, HIGH);

  for (int i = 0; i < sizeof(addressLines); i++)
  {
    pinMode2(addressLines[i], OUTPUT);
    digitalWrite2(addressLines[i], LOW);
  }

  for (int i = 0; i < sizeof(dataLines); i++)
  {
    pinMode2(dataLines[i], INPUT);
  }
}

//----------------------------------------------------------------------
// Setup
//----------------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  Serial.println("\n============================");
  Serial.println("   BBC Master ROM Tester");
  Serial.println("    myretrostore.co.uk");
  Serial.print("        "); Serial.println(VERSION);
  Serial.println("============================");
}

//----------------------------------------------------------------------
// Function to read array from PROGMEM
//----------------------------------------------------------------------
void readArray(int index, t_crc_16k &data) {
  memcpy_P(&data, &crc_16k[index], sizeof(t_crc_16k));
}

//----------------------------------------------------------------------
// Loop through 4 x 4k address locations and check CRC hashes
//----------------------------------------------------------------------
void read_data()
{
  char hexString[10]; 

  if (addressEnd != 0 ) // This is to ensure to only increase the address in the 2nd loop onwards
  {
    addressStart = addressStart + 0x4000;
  }

  addressEnd = addressStart + 0x4000;

  Serial.print("\nReading 0x");
  sprintf(hexString, "%05lX", addressStart);      // Pad value to 5 places
  Serial.print(hexString);
  Serial.print(" - 0x");
  sprintf(hexString, "%05lX", addressEnd - 1);
  Serial.println(hexString);

  for (long address = addressStart; address < addressEnd; address++)
  {
    setAddr(address);   //SetAddress(address); // tells the ROM the byte we want ...
    delayMicroseconds(1);
    digitalWrite2(CE, LOW);
    ROM_Byte = getData();
    digitalWrite2(CE, HIGH); // Disable CE

    ROM[ROM_counter] = ROM_Byte;
    ROM_counter++;

    //-----------------------------------------------------------------------
    // Calc first CRC
    //-----------------------------------------------------------------------
    if  (address == (addressStart + 0x0FFF ))
    {
      uint32_t crc = 0xFFFFFFFF;
      for (size_t i = 0; i < 4096; i++) {
        crc = calculate_crc32(crc, ROM[i]);
      }
      crc ^= 0xFFFFFFFF;
      crc1 = crc;
      //  Serial.println(crc1, HEX);
      toggleLED();

      ROM_counter = 0;
    }

    //-----------------------------------------------------------------------
    // Check 2nd CRC
    //-----------------------------------------------------------------------
    if  (address == (addressStart + 0x1FFF ))
    {
      uint32_t crc = 0xFFFFFFFF;
      for (size_t i = 0; i < 4096; i++) {
        crc = calculate_crc32(crc, ROM[i]);
      }
      crc ^= 0xFFFFFFFF;
      crc2 = crc;
      //   Serial.println(crc2, HEX);
      toggleLED();
      ROM_counter = 0;
    }

    //-----------------------------------------------------------------------
    // Check 3rd CRC
    //-----------------------------------------------------------------------
    if  (address == (addressStart + 0x2FFF ))
    {
      uint32_t crc = 0xFFFFFFFF;
      for (size_t i = 0; i < 4096; i++) {
        crc = calculate_crc32(crc, ROM[i]);
      }
      crc ^= 0xFFFFFFFF;
      crc3 = crc;
      //  Serial.println(crc3, HEX);
      toggleLED();
      ROM_counter = 0;
    }

    //-----------------------------------------------------------------------
    // Check 4th CRC
    //-----------------------------------------------------------------------
    if  (address == (addressStart + 0x3FFF ))
    {
      uint32_t crc = 0xFFFFFFFF;
      for (size_t i = 0; i < 4096; i++) {
        crc = calculate_crc32(crc, ROM[i]);
      }
      crc ^= 0xFFFFFFFF;
      crc4 = crc;
      //  Serial.println(crc4, HEX);
      toggleLED();
      ROM_counter = 0;
    }
  }

  uint32_t crc = 0xFFFFFFFF;

  // Combine CRC values
  crc = calculate_crc32(crc, crc1);
  crc = calculate_crc32(crc, crc2);
  crc = calculate_crc32(crc, crc3);
  crc = calculate_crc32(crc, crc4);

  // Finalize the CRC calculation
  crc ^= 0xFFFFFFFF;

  check_crc(crc); // Check if CRC matches
}

//----------------------------------------------------------------------
// Print HEX to screen for ReadOnly mode
//----------------------------------------------------------------------

void debug()
{
  char str[40];

  for (long address = DebugStartAddress; address < DebugEndAddress + 1; address++)
  {
    setAddr(address);   // tells the ROM the byte we want ...
    delayMicroseconds(1);
    digitalWrite2(CE, LOW);
    ROM_Byte = getData();
    digitalWrite2(CE, HIGH); // Disable CE

    if (address % 16 == 0)
    {
      ///  sprintf(str, "\r\n0x%05lX", address);
      ///  Serial.print(str);
      Serial.println("");
    }

    sprintf(str, " %02X", ROM_Byte);
    Serial.print(str);
  }

  Serial.print("\r\n\nDump complete from 0x");
  Serial.print(DebugStartAddress, HEX);
  Serial.print(" - 0x");
  Serial.println(DebugEndAddress, HEX);
}

//----------------------------------------------------------------------
// Main Loop
//----------------------------------------------------------------------
void loop() {

  const size_t crcCnt_16k = sizeof crc_16k / sizeof * crc_16k;
  t_crc_16k data;

  InitialisePins();

  if (ReadOnly == false) {
    // Routing for reading ROM, calculating CRC and checking
    for (int i = 0; i < 8; i++) { // 8 ROMs of 128k each
      read_data();
    }
  } else {
    debug();    //Only dump ROM due to config setting
  }

  Serial.println("\nROM Dump complete");

  if (error_count == 0) {
    digitalWrite(LED_Red, LOW);
    digitalWrite(LED_Green, HIGH);
    Serial.println("All ROM's matched");
  } else {
    Serial.print(error_count); Serial.println(" CRC-32 value not found");
    digitalWrite(LED_Red, HIGH);
    digitalWrite(LED_Green, LOW);
  }

  while (1);
}
