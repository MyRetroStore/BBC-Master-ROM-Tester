/*
  Author: MyRetroStore (myretrostore.co.uk)
  Date: 22 August 2024
  Version 1.0

  Code to read the 28 pin BBC Master 128Kb (1Mbit) ROM and compare it against known ROM dumps
  using a Arduino Mega 2560 Shield.

  The BBC Master ROM is a strange 28 pin ROM that cannot be read with a standard EPROM programmer like
  the TL866 which is by I designed this shield. 

  As the Arduino Mega 2560 only has a limited amount of RAM the whole 128K cannnot be read in
  one go, so it's read in 4K chunks and compared to CRC-32 hashes of the original ROM dump.

  This will check if your ROM is valid or corrupted, and can be also used to dump it's contents. 

  Tested against BBC Master ROM Chip 231024-1031 and RetroClinic MultiOS ROM

  You can optionally set the ReadOnly flag to only read the ROM and not do any checks.
  (See config.h)

  To generate the CRC-32 values see the python script on the github page:
  https://github.com/MyRetroStore/BBC-Master-ROM-Tester

  Dependencies:
  DIO2 Library (https://github.com/Locoduino/DIO2)

  Arduino code was built and compiled on Arduino 1.8.19

*/
