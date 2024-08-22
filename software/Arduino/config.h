
#define VERSION "Version 1.0"

// Set to true or false
#define ReadOnly false //Set to true to only read ROM and don't compare to BIN file

//These are used with the above flag to read the whole chip, or portions of it.
long DebugStartAddress = 0x04000;
long DebugEndAddress =   0x040FF;
