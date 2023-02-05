#pragma once

typedef unsigned int mbc_instruction;
typedef unsigned char mbc_command;

#define MBC_REGISTERS      32

// command/arg masks
#define MBC_CMD_MASK       0xFF000000  // 8 bits for the command
#define MBC_ARG0_MASK      0xFC0000    // 6 bits for each arg
#define MBC_ARG1_MASK      0x3F000
#define MBC_ARG2_MASK      0xFC0
#define MBC_ARG3_MASK      0x3F

// command/arg bitshifts
#define MBC_CMD_SHIFT      0x18
#define MBC_ARG0_SHIFT     0x12
#define MBC_ARG1_SHIFT     0xC
#define MBC_ARG2_SHIFT     0x6

// command/arg parsers
#define MBC_PARSE_CMD( x ) (mbc_command)( ( x & MBC_CMD_MASK ) >> MBC_CMD_SHIFT )
#define MBC_PARSE_ARG0( x ) (int)( ( x & MBC_ARG0_MASK ) >> MBC_ARG0_SHIFT )
#define MBC_PARSE_ARG1( x ) (int)( ( x & MBC_ARG1_MASK ) >> MBC_ARG1_SHIFT )
#define MBC_PARSE_ARG2( x ) (int)( ( x & MBC_ARG2_MASK ) >> MBC_ARG2_SHIFT )
#define MBC_PARSE_ARG3( x ) (int)( x & MBC_ARG3_MASK )

// commands
#define MBCCMD_REGF        0x00     // put a float in a register; next whole instruction should be the float
#define MBCCMD_REGI        0x01     // put an int in a register; next whole instruction should be the int
#define MBCCMD_CLEARFREG   0x02     // clear the float registers
#define MBCCMD_CLEARIREG   0x03     // clear the int registers

#define MBCCMD_ADDF        0x10     // add two floats; arg0 and arg1 are register indexes, result is stored in arg2 register index
#define MBCCMD_SUBF        0x11     // subtracts
#define MBCCMD_MULF        0x12     // multiplies
#define MBCCMD_DIVF        0x13     // divides
#define MBCCMD_ABSF        0x14     // absolute value; arg0 is register index of target number, result is stored in arg1 register index
#define MBCCMD_ADDI        0x15     // add two ints; arg0 and arg1 are register indexes, result is stored in arg2 register index
#define MBCCMD_SUBI        0x16     // subtracts
#define MBCCMD_MULI        0x17     // multiplies
#define MBCCMD_DIVI        0x18     // divides
#define MBCCMD_ABSI        0x19     // absolute value; arg0 is register index of target number, result is stored in arg1 register index

#define MBCCMD_GOTO        0x20     // jump to line; arg0 has the line number
#define MBCCMD_SUBR        0x21     // jump to subroutine; arg0 has the subroutine's line number
#define MBCCMD_RET         0x22     // return from subroutine
#define MBCCMD_STOP        0x23     // stop executing bitcode

// branch operations
#define MBCBR_EQF          0x30     // check equality of two floats; arg0 and arg1 are register indexes, arg2 is where the "else" block starts, arg3 is the line after the whole branch
#define MBCBR_LTF          0x31     // check if left float is less than right float
#define MBCBR_GTF          0x32     // check if left float is greater than right float
#define MBCBR_LTEF         0x33     // check if left float is less than or equal to right float
#define MBCBR_GTEF         0x34     // check if left float is greater than or equal to right float
#define MBCBR_EQI          0x35     // check equality of two ints; arg0 and arg1 are register indexes, arg2 is where the "else" block starts, arg3 is the line after the whole branch
#define MBCBR_LTI          0x36     // check if left int is less than right int
#define MBCBR_GTI          0x37     // check if left int is greater than right int
#define MBCBR_LTEI         0x38     // check if left int is less than or equal to right int
#define MBCBR_GTEI         0x39     // check if left int is greater than or equal to right int

// getters
#define MBCGET_FRAMESECS   0x50     // get the last frame's elapsed seconds
#define MBCGET_PLRPOSL     0x51     // get the player's left position in the arena
#define MBCGET_PLRPOST     0x52     // get the player's top position in the arena
#define MBCGET_PLRVELX     0x53     // get the player's X velocity
#define MBCGET_PLRVELY     0x54     // get the player's Y velocity
#define MBCGET_PLRDIR      0x55     // get the player's direction
#define MBCGET_PLRHTH      0x56     // get the player's health
#define MBCGET_PLRMXHTH    0x57     // get the player's max health
#define MBCGET_PLRMVM      0x58     // get the player's movement type
#define MBCGET_ENTPOSL     0x59     // get the entity's left position in the arena
#define MBCGET_ENTPOST     0x5A     // get the entity's top position in the arena
#define MBCGET_ENTVELX     0x5B     // get the entity's X velocity
#define MBCGET_ENTVELY     0x5C     // get the entity's Y velocity
#define MBCGET_ENTDIR      0x5D     // get the entity's direction
#define MBCGET_ENTHTH      0x5E     // get the entity's health
#define MBCGET_ENTMXHTH    0x5F     // get the entity's max health
#define MBCGET_ENTMVM      0x60     // get the entity's movement type

// setters
#define MBCSET_ENTVELX     0x80     // set the entity's X velocity
#define MBCSET_ENTVELY     0x81     // set the entity's Y velocity
