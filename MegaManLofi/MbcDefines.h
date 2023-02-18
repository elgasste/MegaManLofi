#pragma once

typedef int mbc_instruction;
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
#define MBCGET_FRAMESECONDS         0x50     // get the last frame's elapsed seconds

#define MBCGET_PLAYERPOSITIONLEFT   0x51     // get the player's left position in the arena
#define MBCGET_PLAYERPOSITIONTOP    0x52     // get the player's top position in the arena
#define MBCGET_PLAYERVELOCITYX      0x53     // get the player's X velocity
#define MBCGET_PLAYERVELOCITYY      0x54     // get the player's Y velocity
#define MBCGET_PLAYERDIRECTION      0x55     // get the player's direction
#define MBCGET_PLAYERHEALTH         0x56     // get the player's health
#define MBCGET_PLAYERMAXHEALTH      0x57     // get the player's max health
#define MBCGET_PLAYERMOVEMENTTYPE   0x58     // get the player's movement type
#define MBCGET_PLAYERDAMAGESECONDS  0x59     // get the number of seconds the player is invulnerable after taking damage
#define MBCGET_PLAYERISINVULNERABLE 0x5A     // get the player's invulnerability status
#define MBCGET_PLAYERISKNOCKEDBACK  0x5B     // get the player's knocked-back status

#define MBCGET_POSITIONLEFT         0x5C     // get the entity's left position in the arena
#define MBCGET_POSITIONTOP          0x5D     // get the entity's top position in the arena
#define MBCGET_VELOCITYX            0x5E     // get the entity's X velocity
#define MBCGET_VELOCITYY            0x5F     // get the entity's Y velocity
#define MBCGET_DIRECTION            0x60     // get the entity's direction
#define MBCGET_HEALTH               0x61     // get the entity's health
#define MBCGET_MAXHEALTH            0x62     // get the entity's max health
#define MBCGET_MOVEMENTTYPE         0x63     // get the entity's movement type
#define MBCGET_DAMAGESECONDS        0x64     // get the number of seconds the entity is invulnerable after taking damage
#define MBCGET_ISINVULNERABLE       0x65     // get the entity's invulnerability status
#define MBCGET_ISKNOCKEDBACK        0x66     // get the entity's knocked-back status

#define MBCGET_ARENAENTITYCOUNT           0x67  // get the number of entities in the current arena
#define MBCGET_ARENAENTITYPOSITIONLEFT    0x68  // get the left position of an arena entity; arg0 is the entity's index, arg1 is a register index
#define MBCGET_ARENAENTITYPOSITIONTOP     0x69  // get the top position of an arena entity; arg0 is the entity's index, arg1 is a register index
#define MBCGET_ARENAENTITYVELOCITYX       0x6A  // get the X velocity of an arena entity; arg0 is the entity's index, arg1 is a register index
#define MBCGET_ARENAENTITYVELOCITYY       0x6B  // get the Y velocity of an arena entity; arg0 is the entity's index, arg1 is a register index
#define MBCGET_ARENAENTITYDIRECTION       0x6C  // get the direction of an arena entity; arg0 is the entity's index, arg1 is a register index
#define MBCGET_ARENAENTITYHEALTH          0x6E  // get the health of an arena entity; arg0 is the entity's index, arg1 is a register index
#define MBCGET_ARENAENTITYMAXHEALTH       0x6F  // get the max health of an arena entity; arg0 is the entity's index, arg1 is a register index
#define MBCGET_ARENAENTITYMOVEMENTTYPE    0x70  // get the movement type of an arena entity; arg0 is the entity's index, arg1 is a register index
#define MBCGET_ARENAENTITYDAMAGESECONDS   0x71  // get the damage seconds of an arena entity; arg0 is the entity's index, arg1 is a register index
#define MBCGET_ARENAENTITYISINVULNERABLE  0x72  // get the invulnerability status of an arena entity; arg0 is the entity's index, arg1 is a register index
#define MBCGET_ARENAENTITYTYPE            0x73  // get the arena entity's type; arg0 is the entity's index, arg1 is a register index
#define MBCGET_ARENAENTITYISKNOCKEDBACK   0x74  // get the knocked-back status of an arena entity; arg0 is the entity's index, arg1 is a register index

// setters
#define MBCSET_VELOCITYX            0x80     // set the entity's X velocity
#define MBCSET_VELOCITYY            0x81     // set the entity's Y velocity
#define MBCSET_DIRECTION            0x82     // set the entity's direction

// do-ers
#define MBCDO_SHOOT                 0xA0     // shoot in the entity's direction
#define MBCDO_SHOOTTARGET           0xA1     // shoot at a specific target's position; arg0 and arg1 are the target's Left/Top position, respectively
