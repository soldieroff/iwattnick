// Command
#define SetColumnAddressLSB 0x00 // address low
#define SetColumnAddressMSB 0x10 // address high
#define SetPowerControl 0x28 // Power Control
#define SetScrollLine 0x40 // rolling line
#define SetPageAddress 0xB0 // page address
#define SetLCDContrast 0x20 // contrast

#define SetContrastRefBase 0x81 // contrast reference base address, and below the double-byte instruction constitutes
#define SetContrastRef 0x00 // contrast variable values, and above constitute a double-byte instruction

#define SetAllPielOn 0xA4
#define SetInverseDisplay 0xA6 // discolor Show
#define SetDisplayEnable 0xAE // Enable display
#define SetXDirection 0xA0 // horizontally
#define SetYDirection 0xC0 // vertical
#define SetReset 0xE2 // software reset
#define Nop 0xE3 // null operation
#define SetLcdBiasRatio 0xA2 // set the bias
#define SetCursorUpdateMode 0xE0 // set cursor update mode
#define ResetCursorUpdateMode 0xEE // reset cursor update mode
#define SetIndicatorOff 0xAC // Close Pointer
#define SetIndicatorOn 0xAD00 // Open pointer
#define SetBoosterRatio 0xF800 // Booster rate
#define AdvanceProgramControl0 0xFA10 // Advanced Control

enum Direction
{
LeftToRight = SetXDirection | 0, // left to right display
RightToLeft = SetXDirection | 0x01, // right-to-left display
UpToDown = SetYDirection | 0x08, // on to the next display
DownToUp = SetYDirection | 0, // next to the display
};

enum InverseDisplay
{
EnableInverse = SetInverseDisplay | 1, // enable discolor Show
DisableInverse = SetInverseDisplay | 0 // Close discolor Show
};

enum DisplayEnable
{
EnableDisplay = SetDisplayEnable | 1, // Open Display
DisableDisplay = SetDisplayEnable | 0 // Close Show
};
