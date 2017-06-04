/***********************************************************************************
 * INCLUDES
 */

#include "hal_types.h"
#include "hal_lcd.h"
#include "hal_mcu.h"
#include "hal_board.h"
#include <string.h>

#include <util.h>


/***********************************************************************************
 * CONSTANTS AND DEFINES
 */

#define BIT0              0x01
#define BIT1              0x02
#define BIT2              0x04
#define BIT3              0x08
#define BIT4              0x10
#define BIT5              0x20
#define BIT6              0x40
#define BIT7              0x80

/* The ASCII fonts library, the matrix size is w*h = 8*12 */
__code const uint8 ASCIITAB[]=
{
/* first line of the char */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,    /*" "=00H*/ 
0x00,0x70,0xf8,0xf8,0x70,0x00,0x00,0x00,    /*"!"=01H*/ 
0x00,0x38,0x78,0x00,0x00,0x78,0x38,0x00,    /*"""=02h*/ 
0x20,0xf8,0xf8,0x20,0xf8,0xf8,0x20,0x00,    /*"#"=03h*/ 
0x60,0xf0,0x9c,0x9c,0x90,0x10,0x00,0x00,    /*"$"=04h*/ 
0x60,0x60,0x00,0x80,0xc0,0x60,0x00,0x00,    /*"%"=05h*/ 
0xb0,0xf8,0xc8,0xf8,0xb0,0x00,0x80,0x00,    /*"&"=06h*/ 
0x00,0x40,0x78,0x38,0x00,0x00,0x00,0x00,    /*"'"=07h*/ 
0x00,0xc0,0xe0,0x30,0x18,0x08,0x00,0x00,    /*"("=08h*/ 
0x00,0x08,0x18,0x30,0xe0,0xc0,0x00,0x00,    /*")"=09h*/ 
0x80,0xa0,0xe0,0xc0,0xc0,0xe0,0xa0,0x80,    /*"*"=0Ah*/ 
0x00,0x80,0x80,0xe0,0xe0,0x80,0x80,0x00,    /*"+"=0Bh*/ 
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,    /*";"=0Ch*/ 
0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,    /*"-"=0Dh*/ 
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,    /*"."=0Eh*/ 
0x00,0x00,0x00,0x80,0xc0,0x60,0x30,0x00,    /*"/"=0Fh*/ 
0xf0,0xf8,0x08,0xc8,0x68,0xf8,0xf0,0x00,    /*"0"=10h*/ 
0x20,0x20,0xf0,0xf8,0x00,0x00,0x00,0x00,    /*"1"=11h*/ 
0x30,0x38,0x08,0x88,0xf8,0x70,0x00,0x00,    /*"2"=12h*/ 
0x10,0x18,0x88,0x88,0xf8,0x70,0x00,0x00,    /*"3"=13h*/ 
0x80,0xc0,0x60,0x30,0xf8,0xf8,0x00,0x00,    /*"4"=14h*/ 
0xf8,0xf8,0x88,0x88,0x88,0x08,0x00,0x00,    /*"5"=15h*/ 
0xe0,0xf0,0x98,0x88,0x88,0x00,0x00,0x00,    /*"6"=16h*/ 
0x38,0x38,0x08,0x08,0x88,0xf8,0x78,0x00,    /*"7"=17h*/ 
0x70,0xf8,0xc8,0x88,0xf8,0x70,0x00,0x00,    /*"8"=18h*/ 
0x70,0xf8,0x88,0x88,0xf8,0xf0,0x00,0x00,    /*"9"=19h*/ 
0x00,0x00,0x60,0x60,0x60,0x00,0x00,0x00,    /*":"=1Ah*/ 
0x00,0x00,0x60,0x60,0x60,0x00,0x00,0x00,    /*";"=1Bh*/ 
0x80,0xc0,0x60,0x30,0x18,0x08,0x00,0x00,    /*"<"=1Ch*/ 
0x00,0x40,0x40,0x40,0x40,0x40,0x40,0x00,    /*"="=1Dh*/ 
0x00,0x08,0x18,0x30,0x60,0xc0,0x80,0x00,    /*">"=1Eh*/ 
0x10,0x18,0x88,0xc8,0x78,0x30,0x00,0x00,    /*"?"=1Fh*/ 
0xf0,0xf8,0x08,0xc8,0xc8,0xf8,0xf0,0x00,    /*"@"=20h*/ 
0xe0,0xf0,0x18,0x18,0xf0,0xe0,0x00,0x00,    /*"A"=21h*/ 
0x08,0xf8,0xf8,0x88,0x88,0xf8,0x70,0x00,    /*"B"=22h*/ 
0xe0,0xf0,0x18,0x08,0x08,0x38,0x30,0x00,    /*"C"=23h*/ 
0x08,0xf8,0xf8,0x08,0x18,0xf0,0xe0,0x00,    /*"D"=24h*/ 
0x08,0xf8,0xf8,0x88,0x88,0xc8,0x18,0x00,    /*"E"=25h*/ 
0x08,0xf8,0xf8,0x88,0x88,0xd8,0x38,0x00,    /*"F"=26h*/ 
0xe0,0xf0,0x18,0x08,0x08,0x38,0x30,0x00,    /*"G"=27h*/ 
0xf8,0xf8,0x80,0x80,0xf8,0xf8,0x00,0x00,    /*"h"=28h*/ 
0x00,0x08,0xf8,0xf8,0x08,0x00,0x00,0x00,    /*"I"=29h*/ 
0x00,0x00,0x00,0x08,0xf8,0xf8,0x08,0x00,    /*"J"=2Ah*/ 
0x08,0xf8,0xf8,0x80,0xe0,0x78,0x18,0x00,    /*"K"=2Bh*/ 
0x08,0xf8,0xf8,0x08,0x00,0x00,0x00,0x00,    /*"L"=2Ch*/ 
0xf8,0xf8,0x70,0xe0,0x70,0xf8,0xf8,0x00,    /*"M"=2Dh*/ 
0xf8,0xf8,0xe0,0xc0,0x80,0xf8,0xf8,0x00,    /*"N"=2Eh*/ 
0xe0,0xf0,0x18,0x08,0x18,0xf0,0xe0,0x00,    /*"O"=2Fh*/ 
0x08,0xf8,0xf8,0x88,0x88,0xf8,0x70,0x00,    /*"P"=30h*/ 
0xe0,0xf0,0x18,0x08,0x18,0xf0,0xe0,0x00,    /*"Q"=31h*/ 
0x08,0xf8,0xf8,0x88,0x88,0xf8,0x70,0x00,    /*"R"=32h*/ 
0x70,0xf8,0x88,0x88,0x38,0x30,0x00,0x00,    /*"S"=33h*/ 
0x18,0x08,0xf8,0xf8,0x08,0x18,0x00,0x00,    /*"T"=34h*/ 
0xf8,0xf8,0x00,0x00,0xf8,0xf8,0x00,0x00,    /*"U"=35h*/ 
0xf8,0xf8,0x00,0x00,0xf8,0xf8,0x00,0x00,    /*"V"=36h*/ 
0xf8,0xf8,0x00,0x80,0x00,0xf8,0xf8,0x00,    /*"W"=37h*/ 
0x38,0x78,0xc0,0xc0,0x78,0x38,0x00,0x00,    /*"X"=38h*/ 
0x78,0xf8,0x80,0x80,0xf8,0x78,0x00,0x00,    /*"Y"=39h*/ 
0x38,0x18,0x88,0xe8,0x78,0x18,0x18,0x00,    /*"Z"=3Ah*/ 
0x00,0x00,0xf8,0xf8,0x08,0x08,0x00,0x00,    /*"["=3Bh*/ 
0x30,0x60,0xc0,0x80,0x00,0x00,0x00,0x00,    /*"\"=3Ch*/ 
0x00,0x00,0x08,0x08,0xf8,0xf8,0x00,0x00,    /*"]"=3Dh*/ 
0x20,0x30,0x18,0x0c,0x18,0x30,0x20,0x00,    /*"^"=3Eh*/ 
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,    /*"_"=3Fh*/ 
0x00,0x00,0x0c,0x1c,0x10,0x00,0x00,0x00,    /*"`"=40h*/ 
0x00,0x40,0x40,0x40,0xc0,0x80,0x00,0x00,    /*"a"=41h*/ 
0x08,0xf8,0xf8,0x40,0x40,0xc0,0x80,0x00,    /*"b"=42h*/ 
0x80,0xc0,0x40,0x40,0xc0,0x80,0x00,0x00,    /*"c"=43h*/ 
0x80,0xc0,0x40,0x48,0xf8,0xf8,0x00,0x00,    /*"d"=44h*/ 
0x80,0xc0,0x40,0x40,0xc0,0x80,0x00,0x00,    /*"e"=45h*/ 
0x80,0xf0,0xf8,0x88,0x98,0x10,0x00,0x00,    /*"f"=46h*/ 
0x80,0xc0,0x40,0x40,0x80,0xc0,0x40,0x00,    /*"g"=47h*/ 
0x08,0xf8,0xf8,0x80,0x40,0xc0,0x80,0x00,    /*"h"=48h*/ 
0x00,0x40,0x40,0xd8,0xd8,0x00,0x00,0x00,    /*"i"=49h*/ 
0x00,0x00,0x40,0x40,0xd8,0xd8,0x00,0x00,    /*"j"=4Ah*/ 
0x08,0xf8,0xf8,0x00,0x80,0xc0,0x40,0x00,    /*"k"=4Bh*/ 
0x00,0x08,0x08,0xf8,0xf8,0x00,0x00,0x00,    /*"l"=4Ch*/ 
0xc0,0xc0,0x40,0xc0,0x40,0xc0,0x80,0x00,    /*"m"=4Dh*/ 
0xc0,0xc0,0x40,0x40,0xc0,0x80,0x00,0x00,    /*"n"=4Eh*/ 
0x80,0xc0,0x40,0x40,0xc0,0x80,0x00,0x00,    /*"o'=4Fh*/ 
0x40,0xc0,0x80,0x40,0x40,0xc0,0x80,0x00,    /*"p"=50h*/ 
0x80,0xc0,0x40,0x40,0x80,0xc0,0x40,0x00,    /*"q"=51h*/ 
0x40,0xc0,0xc0,0x00,0xc0,0xc0,0x80,0x00,    /*"r"=52h*/ 
0x80,0xc0,0x40,0x40,0xc0,0x80,0x00,0x00,    /*"s"=53h*/ 
0x40,0xe0,0xf0,0x40,0x40,0x40,0x00,0x00,    /*"t"=54h*/ 
0xc0,0xc0,0x00,0x00,0xc0,0xc0,0x00,0x00,    /*"u"=55h*/ 
0xc0,0xc0,0x00,0x00,0xc0,0xc0,0x00,0x00,    /*"v"=56h*/ 
0xc0,0xc0,0x00,0x00,0x00,0xc0,0xc0,0x00,    /*"w"=57h*/ 
0x40,0xc0,0x80,0x00,0x80,0xc0,0x40,0x00,    /*"x"=58h*/ 
0x00,0xc0,0xc0,0x00,0x00,0xc0,0xc0,0x00,    /*"y"=59h*/ 
0xc0,0x40,0x40,0x40,0xc0,0xc0,0x00,0x00,    /*"z"=5Ah*/ 
0x80,0xc0,0x70,0x38,0x08,0x08,0x00,0x00,    /*"{"=5Bh*/ 
0x00,0x00,0x00,0x78,0x78,0x00,0x00,0x00,    /*"|"=5Ch*/ 
0x08,0x08,0x38,0x70,0xc0,0x80,0x00,0x00,    /*"}"=5Dh*/ 
0x30,0x38,0x08,0x18,0x30,0x20,0x38,0x08,    /*"~"=5Fh*/ 
0x00,0x80,0xc0,0x60,0xc0,0x80,0x00,0x00,    /*" "=0x60*/


/* second line of the char */
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     /*" "=00H*/ 
0x00,0x00,0x0d,0x0d,0x00,0x00,0x00,0x00,     /*"!"=01H*/ 
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     /*"""=02h*/ 
0x02,0x0f,0x0f,0x02,0x0f,0x0f,0x02,0x00,     /*"#"=03h*/ 
0x04,0x04,0x1c,0x1c,0x07,0x03,0x00,0x00,     /*"$"=04h*/ 
0x0c,0x06,0x03,0x01,0x0c,0x0c,0x00,0x00,     /*"%"=05h*/ 
0x07,0x0f,0x08,0x0d,0x07,0x0f,0x09,0x00,     /*"&"=06h*/ 
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     /*"'"=07h*/ 
0x00,0x01,0x03,0x06,0x0c,0x08,0x00,0x00,     /*"("=08h*/ 
0x00,0x08,0x0c,0x06,0x03,0x01,0x00,0x00,     /*")"=09h*/ 
0x00,0x02,0x03,0x01,0x01,0x03,0x02,0x00,     /*"*"=0Ah*/ 
0x00,0x00,0x00,0x03,0x03,0x00,0x00,0x00,     /*"+"=0Bh*/ 
0x00,0x10,0x1c,0x0c,0x0c,0x00,0x00,0x00,     /*";"=0Ch*/ 
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     /*"-"=0Dh*/ 
0x00,0x00,0x0c,0x0c,0x0c,0x00,0x00,0x00,     /*"."=0Eh*/ 
0x0c,0x06,0x03,0x01,0x00,0x00,0x00,0x00,     /*"/"=0Fh*/ 
0x07,0x0f,0x0b,0x09,0x08,0x0f,0x07,0x00,     /*"0"=10h*/ 
0x08,0x08,0x0f,0x0f,0x08,0x08,0x00,0x00,     /*"1"=11h*/ 
0x0c,0x0e,0x0b,0x09,0x0c,0x0c,0x00,0x00,     /*"2"=12h*/ 
0x04,0x0c,0x08,0x08,0x0f,0x07,0x00,0x00,     /*"3"=13h*/ 
0x01,0x01,0x01,0x09,0x0f,0x0f,0x09,0x00,     /*"4"=14h*/ 
0x04,0x0c,0x08,0x08,0x0f,0x07,0x00,0x00,     /*"5"=15h*/ 
0x07,0x0f,0x08,0x08,0x0f,0x07,0x00,0x00,     /*"6"=16h*/ 
0x00,0x00,0x0e,0x0f,0x01,0x00,0x00,0x00,     /*"7"=17h*/ 
0x07,0x0f,0x08,0x09,0x0f,0x07,0x00,0x00,     /*"8"=18h*/ 
0x00,0x08,0x0c,0x0f,0x03,0x00,0x00,0x00,     /*"9"=19h*/ 
0x00,0x00,0x06,0x06,0x06,0x00,0x00,0x00,     /*":"=1Ah*/ 
0x00,0x00,0x16,0x1e,0x0e,0x00,0x00,0x00,     /*";"=1Bh*/ 
0x00,0x01,0x03,0x06,0x0c,0x08,0x00,0x00,     /*"<"=1Ch*/ 
0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x00,     /*"="=1Dh*/ 
0x00,0x08,0x0c,0x06,0x03,0x01,0x00,0x00,     /*">"=1Eh*/ 
0x00,0x00,0x0d,0x0d,0x00,0x00,0x00,0x00,     /*"?"=1Fh*/ 
0x07,0x0f,0x08,0x09,0x09,0x09,0x01,0x00,     /*"@"=20h*/ 
0x0f,0x0f,0x01,0x01,0x0f,0x0f,0x00,0x00,     /*"A"=21h*/ 
0x08,0x0f,0x0f,0x08,0x08,0x0f,0x07,0x00,     /*"B"=22h*/ 
0x03,0x07,0x0c,0x08,0x08,0x0e,0x06,0x00,     /*"C"=23h*/ 
0x08,0x0f,0x0f,0x08,0x0c,0x07,0x03,0x00,     /*"D"=24h*/ 
0x08,0x0f,0x0f,0x08,0x08,0x09,0x0c,0x00,     /*"E"=25h*/ 
0x08,0x0f,0x0f,0x08,0x00,0x01,0x00,0x00,     /*"F"=26h*/ 
0x03,0x07,0x0c,0x08,0x09,0x0f,0x0f,0x00,     /*"G"=27h*/ 
0x0f,0x0f,0x00,0x00,0x0f,0x0f,0x00,0x00,     /*"h"=28h*/ 
0x00,0x08,0x0f,0x0f,0x08,0x00,0x00,0x00,     /*"I"=29h*/ 
0x07,0x0f,0x08,0x08,0x0f,0x07,0x00,0x00,     /*"J"=2Ah*/ 
0x08,0x0f,0x0f,0x00,0x03,0x0f,0x0c,0x00,     /*"K"=2Bh*/ 
0x08,0x0f,0x0f,0x08,0x08,0x0e,0x0f,0x00,     /*"L"=2Ch*/ 
0x0f,0x0f,0x00,0x00,0x00,0x0f,0x0f,0x00,     /*"M"=2Dh*/ 
0x0f,0x0f,0x00,0x01,0x03,0x0f,0x0f,0x00,     /*"N"=2Eh*/ 
0x03,0x07,0x0c,0x08,0x0c,0x07,0x03,0x00,     /*"O"=2Fh*/ 
0x08,0x0f,0x0f,0x08,0x00,0x00,0x00,0x00,     /*"P"=30h*/ 
0x03,0x07,0x04,0x16,0x1f,0x1f,0x13,0x00,     /*"Q"=31h*/ 
0x08,0x0f,0x0f,0x00,0x01,0x0f,0x0e,0x00,     /*"R"=32h*/ 
0x06,0x0e,0x08,0x09,0x0f,0x06,0x00,0x00,     /*"S"=33h*/ 
0x00,0x08,0x0f,0x0f,0x08,0x00,0x00,0x00,     /*"T"=34h*/ 
0x07,0x0f,0x08,0x08,0x0f,0x07,0x00,0x00,     /*"U"=35h*/ 
0x03,0x07,0x0c,0x0c,0x07,0x03,0x00,0x00,     /*"V"=36h*/ 
0x01,0x0f,0x0e,0x01,0x0e,0x0f,0x01,0x00,     /*"W"=37h*/ 
0x0e,0x0f,0x01,0x01,0x0f,0x0e,0x00,0x00,     /*"X"=38h*/ 
0x00,0x08,0x0f,0x0f,0x08,0x00,0x00,0x00,     /*"Y"=39h*/ 
0x0c,0x0f,0x0b,0x08,0x08,0x0c,0x0e,0x00,     /*"Z"=3Ah*/ 
0x00,0x00,0x0f,0x0f,0x08,0x08,0x00,0x00,     /*"["=3Bh*/ 
0x00,0x00,0x00,0x01,0x03,0x06,0x0c,0x00,     /*"\"=3Ch*/ 
0x00,0x00,0x08,0x08,0x0f,0x0f,0x00,0x00,     /*"]"=3Dh*/ 
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     /*"^"=3Eh*/ 
0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,     /*"_"=3Fh*/ 
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     /*"`"=40h*/ 
0x06,0x0f,0x09,0x09,0x07,0x0f,0x08,0x00,     /*"a"=41h*/ 
0x08,0x0f,0x07,0x08,0x08,0x0f,0x07,0x00,     /*"b"=42h*/ 
0x07,0x0f,0x08,0x08,0x0c,0x04,0x00,0x00,     /*"c"=43h*/ 
0x07,0x0f,0x08,0x08,0x07,0x0f,0x08,0x00,     /*"d"=44h*/ 
0x07,0x0f,0x09,0x09,0x0d,0x05,0x00,0x00,     /*"e"=45h*/ 
0x08,0x0f,0x0f,0x08,0x00,0x00,0x00,0x00,     /*"f"=46h*/ 
0x13,0x37,0x24,0x24,0x3f,0x1f,0x00,0x00,     /*"g"=47h*/ 
0x08,0x0f,0x0f,0x00,0x00,0x0f,0x0f,0x00,     /*"h"=48h*/ 
0x00,0x08,0x08,0x0f,0x0f,0x08,0x08,0x00,     /*"i"=49h*/ 
0x18,0x38,0x20,0x20,0x3f,0x1f,0x00,0x00,     /*"j"=4Ah*/ 
0x08,0x0f,0x0f,0x01,0x03,0x0e,0x0c,0x00,     /*"k"=4Bh*/ 
0x00,0x08,0x08,0x0f,0x0f,0x08,0x08,0x00,     /*"l"=4Ch*/ 
0x0f,0x0f,0x00,0x07,0x00,0x0f,0x0f,0x00,     /*"m"=4Dh*/ 
0x0f,0x0f,0x00,0x00,0x0f,0x0f,0x00,0x00,     /*"n"=4Eh*/ 
0x07,0x0f,0x08,0x08,0x0f,0x07,0x00,0x00,     /*"o'=4Fh*/ 
0x20,0x3f,0x3f,0x28,0x08,0x0f,0x07,0x00,     /*"p"=50h*/ 
0x07,0x0f,0x08,0x28,0x3f,0x3f,0x80,0x00,     /*"q"=51h*/ 
0x08,0x0f,0x0f,0x09,0x00,0x01,0x01,0x00,     /*"r"=52h*/ 
0x04,0x0d,0x09,0x0a,0x0e,0x04,0x00,0x00,     /*"s"=53h*/ 
0x00,0x07,0x0f,0x08,0x0c,0x04,0x00,0x00,     /*"t"=54h*/ 
0x07,0x0f,0x08,0x08,0x07,0x0f,0x08,0x00,     /*"u"=55h*/ 
0x03,0x07,0x0c,0x0c,0x07,0x03,0x00,0x00,     /*"v"=56h*/ 
0x03,0x0f,0x0c,0x03,0x0c,0x0f,0x03,0x00,     /*"w"=57h*/ 
0x08,0x0c,0x07,0x03,0x07,0x0c,0x08,0x00,     /*"x"=58h*/ 
0x20,0x23,0x27,0x34,0x1c,0x0f,0x03,0x00,     /*"y"=59h*/ 
0x0c,0x0e,0x0a,0x09,0x09,0x0c,0x00,0x00,     /*"z"=5Ah*/ 
0x00,0x01,0x07,0x0e,0x08,0x08,0x00,0x00,     /*"{"=5Bh*/ 
0x00,0x00,0x00,0x0f,0x0f,0x00,0x00,0x00,     /*"|"=5Ch*/ 
0x08,0x08,0x0e,0x07,0x01,0x00,0x00,0x00,     /*"}"=5Dh*/ 
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,     /*"~"=5Fh*/ 
0x07,0x07,0x04,0x04,0x04,0x07,0x07,0x00,     /*" "=0x60*/
};


// LCD lines
#define LCD_LINE_COUNT                  4
#define LCD_LINE_LENGTH                 16

#define HAL_CONFIG_IO_OUTPUT(port, pin, val)      HAL_CONFIG_IO_OUTPUT_PREP(port, pin, val)
#define HAL_CONFIG_IO_OUTPUT_PREP(port, pin, val) st( P##port##SEL &= ~BV(pin); \
                                                      P##port##_##pin## = val; \
                                                      P##port##DIR |= BV(pin); )


/***********************************************************************************
 * LOCAL VARIABLES
 */
static char XDATA pLcdLineBuffer[LCD_LINE_LENGTH];

/***********************************************************************************
 * LOCAL FUNCTIONS
 */
static void HalLcd_HW_Control(uint8 command);
static void HalLcd_HW_Write(uint8 ch);
static void lcdWriteLine(uint8 line, const char *pText);
static void halLcd_ConfigIO(void);
static void SET_DDRAM_ADDR(uint8 x, uint8 y);
static void halLcdWriteChar_first(uint8 line, uint8 col, char text);
static void halLcdWriteChar_second(uint8 line, uint8 col, char text);
//static uint8 HalLcd_Convert_DZK(uint8 index, uint16 addr);

/***********************************************************************************
 * @fn          HalLcd_HW_Control
 *
 * @brief       Send command to display
 *
 * @param       uint8 command
 *
 * @return      none
 */
static void HalLcd_HW_Control(uint8 command)
{
    LCD_SPI_BEGIN();
    LCD_DO_CONTROL();
    LCD_SPI_TX(command);
    LCD_SPI_WAIT_RXRDY();
    LCD_SPI_END();
}

/***********************************************************************************
 * @fn          HalLcd_HW_Write
 *
 * @brief       Write character to display
 *
 * @param       uint8 ch - character to display
 *
 * @return      none
 */
static void HalLcd_HW_Write(uint8 ch)
{
    LCD_SPI_BEGIN();
    LCD_DO_WRITE();
    LCD_SPI_TX(ch);
    LCD_SPI_WAIT_RXRDY();
    LCD_SPI_END();
}


/***********************************************************************************
 * @fn          lcdWriteLine
 *
 * @brief       Write line on display
 *
 * @param       uint8 line - display line
 *              char *pText - text buffer to write
 *
 * @return      none
 */

static void lcdWriteLine(uint8 line, const char XDATA *pText)
{
  uint8 count;
  uint8 j;
  uint16 addr;
  uint8 totalLength = (uint8)strlen( (char *)pText );
  const char XDATA *pTextcpy;

  pTextcpy = pText;

  /* Write the content first */
  
  //write first line       
  SET_DDRAM_ADDR(0, (line - 1) * 2);
  for (count=0; count<totalLength; count++)
  {
        addr = ((*(pText++)) - 0x20) << 3; //*8

        for (j = 0; j < 8; j++)
        {
        	HalLcd_HW_Write(ASCIITAB[addr + j]);
        }

    }

  //write second line       
  SET_DDRAM_ADDR(0, (line - 1) * 2 + 1);
  for (count=0; count<totalLength; count++)
  {
        addr = ((*(pTextcpy++)) + 0x40) << 3; //*8

        for (j = 0; j < 8; j++)
        {
        	HalLcd_HW_Write(ASCIITAB[addr + j]);
        }

    }


}


/**************************************************************************************************
 * @fn      halLcd_ConfigIO
 *
 * @brief   Configure IO lines needed for LCD control.
 *
 * @param   None
 *
 * @return  None
 **************************************************************************************************/
static void halLcd_ConfigIO(void)
{
  /* GPIO configuration */
  HAL_CONFIG_IO_OUTPUT(HAL_LCD_MODE_PORT,  HAL_LCD_MODE_PIN,  1);
  HAL_CONFIG_IO_OUTPUT(HAL_LCD_CS_PORT,    HAL_LCD_CS_PIN,    1);
  HAL_CONFIG_IO_OUTPUT(HAL_LCD_BK_PORT,    HAL_LCD_BK_PIN,    0);

}

static void SET_DDRAM_ADDR(uint8 x, uint8 y)
{
	HalLcd_HW_Control(y|0xb0);
	HalLcd_HW_Control((x >> 4)|0x10);
	HalLcd_HW_Control(x&0x0f);
}

/***********************************************************************************
 * GLOBAL FUNCTIONS
 */

 /***********************************************************************************
 * @fn          halLcdInit
 *
 * @brief       Initalise LCD
 *
 * @param       none
 *
 * @return      none
 */
void halLcdInit(void)
{
    // Initialize I/O
    halLcd_ConfigIO();

        HalLcd_HW_Control(0xa2);
        HalLcd_HW_Control(0xa0);
        HalLcd_HW_Control(0xc8);
        HalLcd_HW_Control(0x40);
        HalLcd_HW_Control(0x2c);
        HalLcd_HW_Control(0x2e);
       HalLcd_HW_Control(0x2f);
        HalLcd_HW_Control(0x26);
        HalLcd_HW_Control(0x81);
    //    SendCmd(ContrastLevel);
        HalLcd_HW_Control(0x1a);
        HalLcd_HW_Control(0xaf);
    /* set contrast */
    halLcdSetContrast(0x88);

    // Clear the display
    halLcdClear();
}

/***********************************************************************************
 * @fn          halLcdClear
 *
 * @brief       Clear all lines on display
 *
 * @param       none
 *
 * @return      none
 */
void halLcdClear(void)
{
	uint8 i,j;
	for (i = 0; i < 8; i++)
	{
		for (j = 0; j < 132; j++)
		{
			SET_DDRAM_ADDR(j,i);
			HalLcd_HW_Write(0);
		}
	}
}

/***********************************************************************************
 * @fn          halLcdSetContrast
 *
 * @brief       Set display contrast
 *
 * @param       uint8 value - contrast value
 *
 * @return      none
 */
void halLcdSetContrast(uint8 value)
{
    HalLcd_HW_Control(0x81);
    HalLcd_HW_Control(value);
}

/***********************************************************************************
 * @fn          halLcdGetLineLength
 *
 * @brief       Get max number of characters on each line
 *
 * @param       nono
 *
 * @return      uint8 - number of characters on a line
 */
uint8 halLcdGetLineLength(void)
{
    return LCD_LINE_LENGTH;
}

/***********************************************************************************
 * @fn          halLcdGetNumLines
 *
 * @brief       Get number of lines on display
 *
 * @param       nono
 *
 * @return      uint8 - number of characters on a line
 */
uint8 halLcdGetNumLines(void)
{
  return LCD_LINE_COUNT;
}

/***********************************************************************************
 * @fn          halLcdWriteChar
 *
 * @brief       Write single character
 *
 * @param       uint8 line - display line
 *              uint8 col - column
 *              char text - character to display
 *
 * @return      none
 */
void halLcdWriteChar(uint8 line, uint8 col, char text)
{
    halLcdWriteChar_first(line,col,text);
    halLcdWriteChar_second(line,col,text);
}


static void halLcdWriteChar_first(uint8 line, uint8 col, char text)
{
    uint8 j;
    uint16 addr;

    if (col < LCD_LINE_LENGTH && line <= HAL_LCD_LINE_4)
    {
            
        //write first line 
        SET_DDRAM_ADDR(col * 8, (line - 1) * 2);
        addr = (text - 0x20) << 3; //*8

        for (j = 0; j < 8; j++)
        {
        	HalLcd_HW_Write(ASCIITAB[addr + j]);
        }

    }
    else
    {
        return;
    }
}

static void halLcdWriteChar_second(uint8 line, uint8 col, char text)
{
    uint8 j;
    uint16 addr;

    if (col < LCD_LINE_LENGTH && line <= HAL_LCD_LINE_4)
    {
            
        //write second line 
        SET_DDRAM_ADDR(col * 8, (line - 1) * 2 + 1);
        addr = (text + 0x40) << 3; //(text - 0x20 + 0x60)*8 

        for (j = 0; j < 8; j++)
        {
        	HalLcd_HW_Write(ASCIITAB[addr + j]);
        }


    }
    else
    {
        return;
    }
}
/***********************************************************************************
 * @fn          halLcdWriteLine
 *
 * @brief       Write line on display
 *
 * @param       uint8 line - display line
 *              char *pLine2 - pointer to text buffer to write
 *
 * @return      none
 */
void halLcdWriteLine(uint8 line, const char XDATA *pLine)
{
    uint8 n;
    if (pLine) {
        for (n = 0; n < LCD_LINE_LENGTH; n++) {
            if (*pLine) {
                pLcdLineBuffer[n] = *(pLine++);
            } else {
                pLcdLineBuffer[n] = ' ';
            }
        }
        lcdWriteLine(line, pLcdLineBuffer);
    }
}

/***********************************************************************************
 * @fn          halLcdWriteLines
 *
 * @brief       Write lines on display
 *
 * @param       char *pLine1 - pointer to text buffer for line 1
 *              char *pLine2 - pointer to text buffer for line 2
 *              char *pLine3 - pointer to text buffer for line 3
 *
 * @return      none
 */
void halLcdWriteLines(const char XDATA *pLine1, const char XDATA *pLine2,
                      const char XDATA *pLine3)
{
    if (pLine1) halLcdWriteLine(1, pLine1);
    if (pLine2) halLcdWriteLine(2, pLine2);
    if (pLine3) halLcdWriteLine(3, pLine3);
}

/***********************************************************************************
 * @fn          halLcdDisplayValue
 *
 * @brief       Display value on display with optional text on right and left side
 *
 * @param       uint8 line
 *              char *pLeft
 *              int32 value
 *              char *pRight
 *
 * @return      none
 */
void halLcdDisplayValue(uint8 line, char XDATA *pLeft, int32 value,
                        char XDATA *pRight)
{
    uint8 n;
    char *pValue = convInt32ToText(value);
    for (n = 0; n < LCD_LINE_LENGTH; n++) {
        if (pLeft && *pLeft) {
            pLcdLineBuffer[n] = *(pLeft++);
        } else if (*pValue) {
            pLcdLineBuffer[n] = *(pValue++);
        } else if (pRight && *pRight) {
            pLcdLineBuffer[n] = *(pRight++);
        } else {
            pLcdLineBuffer[n] = ' ';
        }
    }
    lcdWriteLine(line, pLcdLineBuffer);
}

/***********************************************************************************
 * @fn          halLcdDisplayCounters
 *
 * @brief       Display counter values with text on display
 *
 * @param       uint8 line - display line
 *              int32 lValue - left value
 *              char lChar - left text
 *              int32 rValue - right value
 *              char rChar - right text
 *
 * @return      none
 */
void halLcdDisplayCounters(uint8 line, int32 lValue, char lChar,
                           int32 rValue, char rChar)
{
    uint8 n;
    char *pCounter;

    // Left part
    pLcdLineBuffer[0] = lChar;
    pLcdLineBuffer[1] = '=';
    pCounter = convInt32ToText(lValue);
    for (n = 2; n < 8; n++) {
        if (*pCounter) {
            pLcdLineBuffer[n] = *(pCounter++);
        } else {
            pLcdLineBuffer[n] = ' ';
        }
    }
    pLcdLineBuffer[8] = rChar;
    pLcdLineBuffer[9] = '=';
    pCounter = convInt32ToText(rValue);
    for (n = 10; n < 16; n++) {
        if (*pCounter) {
            pLcdLineBuffer[n] = *(pCounter++);
        } else {
            pLcdLineBuffer[n] = ' ';
        }
    }
    lcdWriteLine(line, pLcdLineBuffer);
}

static const char hex[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
/***********************************************************************************
 * @fn          halLcdDisplayUint16
 *
 * @brief       Write a 16 bit value on the specified line on the LCD
 *
 * @param       uint16 value - value to display
 *              uint8 radix - must be HAL_LCD_RADIX_DEC
 *              uint8 line - line on display
 *
 * @return      none
 */
void halLcdDisplayUint16(uint8 line, uint8 radix, uint16 value)
{

    if (radix == HAL_LCD_RADIX_DEC) {
        uint8 n = 0;
        char *pValue = convInt32ToText((uint32)value);
        while (*pValue)
            halLcdWriteChar(line, n++, *(pValue++));
    }
    else if (radix == HAL_LCD_RADIX_HEX) {
        halLcdWriteChar(line, 0, '0');
        halLcdWriteChar(line, 1, 'x');
        halLcdWriteChar(line, 2, hex[(value & 0xF000) >> 12]);
        halLcdWriteChar(line, 3, hex[(value & 0x0F00) >>  8]);
        halLcdWriteChar(line, 4, hex[(value & 0x00F0) >>  4]);
        halLcdWriteChar(line, 5, hex[(value & 0x000F)]);
    }
}


/***********************************************************************************
 * @fn          halLcdClearAllSpecChars
 *
 * @brief       Clear special characters
 *
 * @param       none
 *
 * @return      none
 */
void halLcdClearAllSpecChars(void)
{

}

/***********************************************************************************
 * @fn          halLcdCreateSpecChar
 *
 * @brief       Create special character
 *
 * @param       uint8 index
 *              const char *p5x8Spec
 *
 * @return      none
 */
void halLcdCreateSpecChar(uint8 index, const char XDATA *p5x8Spec)
{

}

/***********************************************************************************
 * @fn          halLcdWriteSpecChar
 *
 * @brief       Write special character
 *
 * @param       uint8 line  - display line
 *              uint8 col   - column
 *              uint8 index - index of spec character
 *
 * @return      none
 */
void halLcdWriteSpecChar(uint8 line, uint8 col, uint8 index)
{

}
