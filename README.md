# BBC Master ROM Dumper
An Arduino Mega shield to read and verify the BBC Master 1MBit ROM and compares the ROM images to known CRC-32 values for various ROMs.

## Introduction
Modern EPROM programmers like the TL866 are unable to read the 28 pin 1Mbit ROM used in the BBC Master.  If you are troubleshooting BBC Masters and you don't have a known good BBC Master to test the ROM in, this makes it difficult to troubleshoot. 


I built a shield for the Arduino Mega 2560 which reads the ROM and compares the CRC-32 hash values to know good ROMs found on the internet with a option of displaying the contents of the ROM via the Arduino IDE serial monitor. 

![Shield](https://github.com/MyRetroStore/BBC-Master-ROM-Tester/blob/main/hardware/BBC%20Master%20ROM%20Tester%20Shield.jpg?raw=true)


## Usage

This Arduino sketch was built and tested using Arduino IDE 1.8.19 but should work with later versions.

Before uploading the .ino to the Mega2560, review the config.h file and change the paramaters as required:

- To display ROM contents set ReadOnly to true

Once the code has been uploaded insert the chip (Remove power before inserting the ROM) and open the serial monitor set to 115200 baud.

Program will run and compare the CRC-32 hash of the ROM under test, or display the contents of the ROM.

![Serial Output](https://github.com/MyRetroStore/BBC-Master-ROM-Tester/blob/main/software/serial-output.png?raw=true)

The Arduino code comes pre-configured with a set of CRC-32 hashes from the common ROM's, but additional hashes can be added using the supplied python [script](https://github.com/MyRetroStore/BBC-Master-ROM-Tester/tree/main/software)


##  Shield

The gerber files for the shield can be found under hardware/gerbers

![Retro ROM Tester](https://github.com/MyRetroStore/RetroRomTester/blob/main/hardware/Retro%20ROM%20Tester.png?raw=true)

## BOM
| Reference | Value |
| ------ | ------ |
| Pass | Green LED 0805 |
| Fail | Red LED 0805 |
| U1 | 40 Pin ZIF Socket |
| SW1 | Push Button Switch 6x6x6 |
| | 2.54mm SIL Male Headers

## Purchasing

If you don't want build the shield yourself, assembled shields can be purchased from:
- [MyRetroStore](https://myretrostore.co.uk/)
- [eBay](https://www.ebay.co.uk/str/myretrostoreuk)

## License
CC BY-NC-SA 4.0


[![ko-fi](https://ko-fi.com/img/githubbutton_sm.svg)](https://ko-fi.com/H2H8RDX9W)
