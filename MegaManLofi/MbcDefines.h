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
#define MBCCMD_INCF        0x04     // increment the value of a float register slot
#define MBCCMD_INCI        0x05     // increment the value of an int register slot

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
#define MBCBR_NEQF         0x31     // check inequality of two floats
#define MBCBR_LTF          0x32     // check if left float is less than right float
#define MBCBR_GTF          0x33     // check if left float is greater than right float
#define MBCBR_LTEF         0x34     // check if left float is less than or equal to right float
#define MBCBR_GTEF         0x35     // check if left float is greater than or equal to right float
#define MBCBR_EQI          0x36     // check equality of two ints; arg0 and arg1 are register indexes, arg2 is where the "else" block starts, arg3 is the line after the whole branch
#define MBCBR_NEQI         0x37     // check inequality of two ints
#define MBCBR_LTI          0x38     // check if left int is less than right int
#define MBCBR_GTI          0x39     // check if left int is greater than right int
#define MBCBR_LTEI         0x3A     // check if left int is less than or equal to right int
#define MBCBR_GTEI         0x3B     // check if left int is greater than or equal to right int
#define MBCBR_TRUEF        0x3C     // check if float is greater than zero ("true"); arg0 is a register index, arg1 is where the "else" block starts, arg2 is the line after the whole branch
#define MBCBR_FALSEF       0x3D     // check if float is less than or equal to zero ("false")
#define MBCBR_TRUEI        0x3E     // check if int is greater than zero ("true")
#define MBCBR_FALSEI       0x3F     // check if int is less than or equal to zero ("false")

// getters
#define MBCGET_FRAMESECONDS                  0x40  // get the last frame's elapsed seconds

#define MBCGET_PLAYERPOSITIONLEFT            0x41  // get the player's left position in the arena
#define MBCGET_PLAYERPOSITIONTOP             0x42  // get the player's top position in the arena
#define MBCGET_PLAYERVELOCITYX               0x43  // get the player's X velocity
#define MBCGET_PLAYERVELOCITYY               0x44  // get the player's Y velocity
#define MBCGET_PLAYERDIRECTION               0x45  // get the player's direction
#define MBCGET_PLAYERHEALTH                  0x46  // get the player's health
#define MBCGET_PLAYERMAXHEALTH               0x47  // get the player's max health
#define MBCGET_PLAYERMOVEMENTTYPE            0x48  // get the player's movement type
#define MBCGET_PLAYERDAMAGESECONDS           0x49  // get the number of seconds the player is invulnerable after taking damage
#define MBCGET_PLAYERISINVULNERABLE          0x4A  // get the player's invulnerability status
#define MBCGET_PLAYERISKNOCKEDBACK           0x4B  // get the player's knocked-back status
#define MBCGET_PLAYERKNOCKBACKSECONDS        0x4C  // get the number of seconds the player gets knocked back after taking damage
#define MBCGET_PLAYERKNOCKBACKVELOCITY       0x4D  // get the player's knock-back velocity

#define MBCGET_POSITIONLEFT                  0x4E  // get the entity's left position in the arena
#define MBCGET_POSITIONTOP                   0x4F  // get the entity's top position in the arena
#define MBCGET_VELOCITYX                     0x50  // get the entity's X velocity
#define MBCGET_VELOCITYY                     0x51  // get the entity's Y velocity
#define MBCGET_DIRECTION                     0x52  // get the entity's direction
#define MBCGET_HEALTH                        0x53  // get the entity's health
#define MBCGET_MAXHEALTH                     0x54  // get the entity's max health
#define MBCGET_MOVEMENTTYPE                  0x55  // get the entity's movement type
#define MBCGET_DAMAGESECONDS                 0x56  // get the number of seconds the entity is invulnerable after taking damage
#define MBCGET_ISINVULNERABLE                0x57  // get the entity's invulnerability status
#define MBCGET_ISKNOCKEDBACK                 0x58  // get the entity's knocked-back status
#define MBCGET_KNOCKBACKSECONDS              0x59  // get the number of seconds the entity gets knocked back after taking damage
#define MBCGET_KNOCKBACKVELOCITY             0x5A  // get the entity's knock-back velocity

#define MBCGET_ARENAENTITYCOUNT              0x5B  // get the number of entities in the current arena
#define MBCGET_ARENAENTITYTYPE               0x5C  // get the arena entity's type; arg0 is the entity's index, arg1 is a register index
#define MBCGET_ARENAENTITYPOSITIONLEFT       0x5D  // get the left position of an arena entity; arg0 is the entity's index, arg1 is a register index
#define MBCGET_ARENAENTITYPOSITIONTOP        0x5E  // get the top position of an arena entity; arg0 is the entity's index, arg1 is a register index
#define MBCGET_ARENAENTITYVELOCITYX          0x5F  // get the X velocity of an arena entity; arg0 is the entity's index, arg1 is a register index
#define MBCGET_ARENAENTITYVELOCITYY          0x60  // get the Y velocity of an arena entity; arg0 is the entity's index, arg1 is a register index
#define MBCGET_ARENAENTITYDIRECTION          0x61  // get the direction of an arena entity; arg0 is the entity's index, arg1 is a register index
#define MBCGET_ARENAENTITYHEALTH             0x62  // get the health of an arena entity; arg0 is the entity's index, arg1 is a register index
#define MBCGET_ARENAENTITYMAXHEALTH          0x63  // get the max health of an arena entity; arg0 is the entity's index, arg1 is a register index
#define MBCGET_ARENAENTITYMOVEMENTTYPE       0x64  // get the movement type of an arena entity; arg0 is the entity's index, arg1 is a register index
#define MBCGET_ARENAENTITYDAMAGESECONDS      0x65  // get the damage seconds of an arena entity; arg0 is the entity's index, arg1 is a register index
#define MBCGET_ARENAENTITYISINVULNERABLE     0x66  // get the invulnerability status of an arena entity; arg0 is the entity's index, arg1 is a register index
#define MBCGET_ARENAENTITYISKNOCKEDBACK      0x67  // get the knocked-back status of an arena entity; arg0 is the entity's index, arg1 is a register index
#define MBCGET_ARENAENTITYKNOCKBACKSECONDS   0x68  // get the knock-back seconds of an arena entity; arg0 is the entity's index, arg1 is a register index
#define MBCGET_ARENAENTITYKNOCKBACKVELOCITY  0x69  // get the knock-back velocity of an arena entity; arg0 is the entity's index, arg1 is a register index

#define MBCGET_RANDOM         0x79     // get a random unsigned int; arg0/arg1 are the min/max number indices, arg1 is the index of the result

// setters
#define MBCSET_VELOCITYX      0x80     // set the entity's X velocity
#define MBCSET_VELOCITYY      0x81     // set the entity's Y velocity
#define MBCSET_DIRECTION      0x82     // set the entity's direction

// do-ers
#define MBCDO_SHOOT           0xA0     // shoot in the entity's direction
#define MBCDO_SHOOTTARGET     0xA1     // shoot at a specific target's position; arg0 and arg1 are the target's Left/Top position, respectively
