# Creating CRC-32 hashes

The python script was developed in Linux, but should run on any OS with python 3.x

## Introduction

The python script **generate_crc.py** is used to generate CRC-32 hashes from ROM files. This is useful if the ROM you are testing does not have a CRC-32 hash pre-configured in the Arduino code. 

As the Arduino Mega 2560 has a limited amount of RAM, when reading the ROM the contents, it's read in 4k chunks. A CRC-32 is calculated for each 4Kb bank and then once all 4x4KKb banks are completed a final CRC-32 value is calculated. 
 
 The hashes are stored in the file [crc.h](https://github.com/MyRetroStore/BBC-Master-ROM-Tester/blob/main/software/Arduino/crc.h)


## Usage


Run:
```
./generate_crc.py -i filename
```
This calculates the CRC-32 hash for the ROM binary (filename) and updates the crc.h file. 

 Example:
 ```
 ./generate_crc.py -i ROMs/basic.bin
 ```

Once completed copy ```crc.h``` back to the Arduino folder and compile and upload the sketch to the Mega2560.



 
 ## To Do
 - While basic error checking is done in the python script, there is a lot more error checking that cold be done so not all errors will be catered for (Eg if you had to remove the marker text from crc.h then the script will probably throw a weird error)
 - I'm not a programmer so while this script seems to work, it's not the most efficient and there are probably a lot better ways to do things. 
 


## License
CC BY-NC-SA 4.0
