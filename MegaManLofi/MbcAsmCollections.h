#pragma once

#include <string>
#include <vector>
#include <map>

#include "MbcDefines.h"
#include "MbcAsmDefines.h"

namespace MbcAsm
{
   const std::vector<std::string> AllTokens =
   {
      MBCCOMP_REGF,
      MBCCOMP_REGI,
      MBCCOMP_CLEARFREG,
      MBCCOMP_CLEARIREG,
      MBCCOMP_ADDF,
      MBCCOMP_SUBF,
      MBCCOMP_MULF,
      MBCCOMP_DIVF,
      MBCCOMP_ABSF,
      MBCCOMP_ADDI,
      MBCCOMP_SUBI,
      MBCCOMP_MULI,
      MBCCOMP_DIVI,
      MBCCOMP_ABSI,
      MBCCOMP_FUNC,
      MBCCOMP_CALL,
      MBCCOMP_RET,
      MBCCOMP_STOP,
      MBCCOMP_IF_EQF,
      MBCCOMP_IF_LTF,
      MBCCOMP_IF_GTF,
      MBCCOMP_IF_LTEF,
      MBCCOMP_IF_GTEF,
      MBCCOMP_IF_EQI,
      MBCCOMP_IF_LTI,
      MBCCOMP_IF_GTI,
      MBCCOMP_IF_LTEI,
      MBCCOMP_IF_GTEI,
      MBCCOMP_ELSE,
      MBCCOMP_ENDIF,
      MBCCOMP_WHILE_EQF,
      MBCCOMP_WHILE_LTF,
      MBCCOMP_WHILE_GTF,
      MBCCOMP_WHILE_LTEF,
      MBCCOMP_WHILE_GTEF,
      MBCCOMP_WHILE_EQI,
      MBCCOMP_WHILE_LTI,
      MBCCOMP_WHILE_GTI,
      MBCCOMP_WHILE_LTEI,
      MBCCOMP_WHILE_GTEI,
      MBCCOMP_BREAK,
      MBCCOMP_ENDWHILE,
      MBCCOMP_GET_FRAMESECONDS,
      MBCCOMP_GET_PLAYERPOSITIONLEFT,
      MBCCOMP_GET_PLAYERPOSITIONTOP,
      MBCCOMP_GET_PLAYERVELOCITYX,
      MBCCOMP_GET_PLAYERVELOCITYY,
      MBCCOMP_GET_PLAYERDIRECTION,
      MBCCOMP_GET_PLAYERHEALTH,
      MBCCOMP_GET_PLAYERMAXHEALTH,
      MBCCOMP_GET_PLAYERMOVEMENTTYPE,
      MBCCOMP_GET_PLAYERISINVULNERABLE,
      MBCCOMP_GET_POSITIONLEFT,
      MBCCOMP_GET_POSITIONTOP,
      MBCCOMP_GET_VELOCITYX,
      MBCCOMP_GET_VELOCITYY,
      MBCCOMP_GET_DIRECTION,
      MBCCOMP_GET_HEALTH,
      MBCCOMP_GET_MAXHEALTH,
      MBCCOMP_GET_MOVEMENTTYPE,
      MBCCOMP_GET_ISINVULNERABLE,
      MBCCOMP_GET_ARENAENTITYCOUNT,
      MBCCOMP_GET_ARENAENTITYPOSITIONLEFT,
      MBCCOMP_GET_ARENAENTITYPOSITIONTOP,
      MBCCOMP_GET_ARENAENTITYVELOCITYX,
      MBCCOMP_GET_ARENAENTITYVELOCITYY,
      MBCCOMP_GET_ARENAENTITYDIRECTION,
      MBCCOMP_GET_ARENAENTITYHEALTH,
      MBCCOMP_GET_ARENAENTITYMAXHEALTH,
      MBCCOMP_GET_ARENAENTITYMOVEMENTTYPE,
      MBCCOMP_GET_ARENAENTITYDAMAGESECONDS,
      MBCCOMP_GET_ARENAENTITYISINVULNERABLE,
      MBCCOMP_GET_ARENAENTITYTYPE,
      MBCCOMP_SET_VELOCITYX,
      MBCCOMP_SET_VELOCITYY,
      MBCCOMP_SET_DIRECTION,
      MBCCOMP_DO_SHOOT,
      MBCCOMP_DO_SHOOTTARGET
   };

   const std::vector<std::string> IfTokens =
   {
      MBCCOMP_IF_EQI,
      MBCCOMP_IF_LTI,
      MBCCOMP_IF_GTI,
      MBCCOMP_IF_LTEI,
      MBCCOMP_IF_GTEI,
      MBCCOMP_IF_EQF,
      MBCCOMP_IF_LTF,
      MBCCOMP_IF_GTF,
      MBCCOMP_IF_LTEF,
      MBCCOMP_IF_GTEF
   };

   const std::vector<std::string> LoopTokens =
   {
      MBCCOMP_WHILE_EQF,
      MBCCOMP_WHILE_LTF,
      MBCCOMP_WHILE_GTF,
      MBCCOMP_WHILE_LTEF,
      MBCCOMP_WHILE_GTEF,
      MBCCOMP_WHILE_EQI,
      MBCCOMP_WHILE_LTI,
      MBCCOMP_WHILE_GTI,
      MBCCOMP_WHILE_LTEI,
      MBCCOMP_WHILE_GTEI
   };

   const std::vector<std::string> RegTokens =
   {
      MBCCOMP_REGF,
      MBCCOMP_REGI
   };

   const std::vector<std::string> MathTokens =
   {
      MBCCOMP_ADDF,
      MBCCOMP_SUBF,
      MBCCOMP_MULF,
      MBCCOMP_DIVF,
      MBCCOMP_ADDI,
      MBCCOMP_SUBI,
      MBCCOMP_MULI,
      MBCCOMP_DIVI
   };

   const std::map<std::string, mbc_command> IfTokenMap =
   {
      { MBCCOMP_IF_EQF, MBCBR_EQF },
      { MBCCOMP_IF_LTF, MBCBR_LTF },
      { MBCCOMP_IF_GTF, MBCBR_GTF },
      { MBCCOMP_IF_LTEF, MBCBR_LTEF },
      { MBCCOMP_IF_GTEF, MBCBR_GTEF },
      { MBCCOMP_IF_EQI, MBCBR_EQI },
      { MBCCOMP_IF_LTI, MBCBR_LTI },
      { MBCCOMP_IF_GTI, MBCBR_GTI },
      { MBCCOMP_IF_LTEI, MBCBR_LTEI },
      { MBCCOMP_IF_GTEI, MBCBR_GTEI }
   };

   const std::map<std::string, mbc_command> BranchTokenMap =
   {
      { MBCCOMP_WHILE_EQF, MBCBR_EQF },
      { MBCCOMP_WHILE_LTF, MBCBR_LTF },
      { MBCCOMP_WHILE_GTF, MBCBR_GTF },
      { MBCCOMP_WHILE_LTEF, MBCBR_LTEF },
      { MBCCOMP_WHILE_GTEF, MBCBR_GTEF },
      { MBCCOMP_WHILE_EQI, MBCBR_EQI },
      { MBCCOMP_WHILE_LTI, MBCBR_LTI },
      { MBCCOMP_WHILE_GTI, MBCBR_GTI },
      { MBCCOMP_WHILE_LTEI, MBCBR_LTEI },
      { MBCCOMP_WHILE_GTEI, MBCBR_GTEI }
   };

   const std::map<std::string, mbc_command> RegTokenMap =
   {
      { MBCCOMP_REGF, MBCCMD_REGF },
      { MBCCOMP_REGI, MBCCMD_REGI }
   };

   const std::map<std::string, mbc_command> MathTokenMap =
   {
      { MBCCOMP_ADDF, MBCCMD_ADDF },
      { MBCCOMP_SUBF, MBCCMD_SUBF },
      { MBCCOMP_MULF, MBCCMD_MULF },
      { MBCCOMP_DIVF, MBCCMD_DIVF },
      { MBCCOMP_ADDI, MBCCMD_ADDI },
      { MBCCOMP_SUBI, MBCCMD_SUBI },
      { MBCCOMP_MULI, MBCCMD_MULI },
      { MBCCOMP_DIVI, MBCCMD_DIVI }
   };

   const std::map<std::string, mbc_command> GeneralTokenMap =
   {
      { MBCCOMP_CLEARFREG, MBCCMD_CLEARFREG },
      { MBCCOMP_CLEARIREG, MBCCMD_CLEARIREG },
      { MBCCOMP_RET, MBCCMD_RET },
      { MBCCOMP_STOP, MBCCMD_STOP },
      { MBCCOMP_ABSF, MBCCMD_ABSF },
      { MBCCOMP_ABSI, MBCCMD_ABSI },
      { MBCCOMP_GET_FRAMESECONDS, MBCGET_FRAMESECONDS },
      { MBCCOMP_GET_PLAYERPOSITIONLEFT, MBCGET_PLAYERPOSITIONLEFT },
      { MBCCOMP_GET_PLAYERPOSITIONTOP, MBCGET_PLAYERPOSITIONTOP },
      { MBCCOMP_GET_PLAYERVELOCITYX, MBCGET_PLAYERVELOCITYX },
      { MBCCOMP_GET_PLAYERVELOCITYY, MBCGET_PLAYERVELOCITYY },
      { MBCCOMP_GET_PLAYERDIRECTION, MBCGET_PLAYERDIRECTION },
      { MBCCOMP_GET_PLAYERHEALTH, MBCGET_PLAYERHEALTH },
      { MBCCOMP_GET_PLAYERMAXHEALTH, MBCGET_PLAYERMAXHEALTH },
      { MBCCOMP_GET_PLAYERMOVEMENTTYPE, MBCGET_PLAYERMOVEMENTTYPE },
      { MBCCOMP_GET_PLAYERISINVULNERABLE, MBCGET_PLAYERISINVULNERABLE },
      { MBCCOMP_GET_POSITIONLEFT, MBCGET_POSITIONLEFT },
      { MBCCOMP_GET_POSITIONTOP, MBCGET_POSITIONTOP },
      { MBCCOMP_GET_VELOCITYX, MBCGET_VELOCITYX },
      { MBCCOMP_GET_VELOCITYY, MBCGET_VELOCITYY },
      { MBCCOMP_GET_DIRECTION, MBCGET_DIRECTION },
      { MBCCOMP_GET_HEALTH, MBCGET_HEALTH },
      { MBCCOMP_GET_MAXHEALTH, MBCGET_MAXHEALTH },
      { MBCCOMP_GET_MOVEMENTTYPE, MBCGET_MOVEMENTTYPE },
      { MBCCOMP_GET_ISINVULNERABLE, MBCGET_ISINVULNERABLE },
      { MBCCOMP_GET_ARENAENTITYCOUNT, MBCGET_ARENAENTITYCOUNT },
      { MBCCOMP_GET_ARENAENTITYPOSITIONLEFT, MBCGET_ARENAENTITYPOSITIONLEFT },
      { MBCCOMP_GET_ARENAENTITYPOSITIONTOP, MBCGET_ARENAENTITYPOSITIONTOP },
      { MBCCOMP_GET_ARENAENTITYVELOCITYX, MBCGET_ARENAENTITYVELOCITYX },
      { MBCCOMP_GET_ARENAENTITYVELOCITYY, MBCGET_ARENAENTITYVELOCITYY },
      { MBCCOMP_GET_ARENAENTITYDIRECTION, MBCGET_ARENAENTITYDIRECTION },
      { MBCCOMP_GET_ARENAENTITYHEALTH, MBCGET_ARENAENTITYHEALTH },
      { MBCCOMP_GET_ARENAENTITYMAXHEALTH, MBCGET_ARENAENTITYMAXHEALTH },
      { MBCCOMP_GET_ARENAENTITYMOVEMENTTYPE, MBCGET_ARENAENTITYMOVEMENTTYPE },
      { MBCCOMP_GET_ARENAENTITYDAMAGESECONDS, MBCGET_ARENAENTITYDAMAGESECONDS },
      { MBCCOMP_GET_ARENAENTITYISINVULNERABLE, MBCGET_ARENAENTITYISINVULNERABLE },
      { MBCCOMP_GET_ARENAENTITYTYPE, MBCGET_ARENAENTITYTYPE },
      { MBCCOMP_SET_VELOCITYX, MBCSET_VELOCITYX },
      { MBCCOMP_SET_VELOCITYY, MBCSET_VELOCITYY },
      { MBCCOMP_SET_DIRECTION, MBCSET_DIRECTION },
      { MBCCOMP_DO_SHOOT, MBCDO_SHOOT },
      { MBCCOMP_DO_SHOOTTARGET, MBCDO_SHOOTTARGET }
   };

   const std::map<std::string, int> TokenArgCountMap =
   {
      { MBCCOMP_REGF, 2 },
      { MBCCOMP_REGI, 2 },
      { MBCCOMP_CLEARFREG, 0 },
      { MBCCOMP_CLEARIREG, 0 },
      { MBCCOMP_ADDF, 3 },
      { MBCCOMP_SUBF, 3 },
      { MBCCOMP_MULF, 3 },
      { MBCCOMP_DIVF, 3 },
      { MBCCOMP_ABSF, 2 },
      { MBCCOMP_ADDI, 3 },
      { MBCCOMP_SUBI, 3 },
      { MBCCOMP_MULI, 3 },
      { MBCCOMP_DIVI, 3 },
      { MBCCOMP_ABSI, 2 },
      { MBCCOMP_FUNC, 1 },
      { MBCCOMP_CALL, 1 },
      { MBCCOMP_RET, 0 },
      { MBCCOMP_STOP, 0 },
      { MBCCOMP_IF_EQF, 2 },
      { MBCCOMP_IF_LTF, 2 },
      { MBCCOMP_IF_GTF, 2 },
      { MBCCOMP_IF_LTEF, 2 },
      { MBCCOMP_IF_GTEF, 2 },
      { MBCCOMP_IF_EQI, 2 },
      { MBCCOMP_IF_LTI, 2 },
      { MBCCOMP_IF_GTI, 2 },
      { MBCCOMP_IF_LTEI, 2 },
      { MBCCOMP_IF_GTEI, 2 },
      { MBCCOMP_ELSE, 0 },
      { MBCCOMP_ENDIF, 0 },
      { MBCCOMP_WHILE_EQF, 2 },
      { MBCCOMP_WHILE_LTF, 2 },
      { MBCCOMP_WHILE_GTF, 2 },
      { MBCCOMP_WHILE_LTEF, 2 },
      { MBCCOMP_WHILE_GTEF, 2 },
      { MBCCOMP_WHILE_EQI, 2 },
      { MBCCOMP_WHILE_LTI, 2 },
      { MBCCOMP_WHILE_GTI, 2 },
      { MBCCOMP_WHILE_LTEI, 2 },
      { MBCCOMP_WHILE_GTEI, 2 },
      { MBCCOMP_BREAK, 0 },
      { MBCCOMP_ENDWHILE, 0 },
      { MBCCOMP_GET_FRAMESECONDS, 1 },
      { MBCCOMP_GET_PLAYERPOSITIONLEFT, 1 },
      { MBCCOMP_GET_PLAYERPOSITIONTOP, 1 },
      { MBCCOMP_GET_PLAYERVELOCITYX, 1 },
      { MBCCOMP_GET_PLAYERVELOCITYY, 1 },
      { MBCCOMP_GET_PLAYERDIRECTION, 1 },
      { MBCCOMP_GET_PLAYERHEALTH, 1 },
      { MBCCOMP_GET_PLAYERMAXHEALTH, 1 },
      { MBCCOMP_GET_PLAYERMOVEMENTTYPE, 1 },
      { MBCCOMP_GET_PLAYERISINVULNERABLE, 1 },
      { MBCCOMP_GET_POSITIONLEFT, 1 },
      { MBCCOMP_GET_POSITIONTOP, 1 },
      { MBCCOMP_GET_VELOCITYX, 1 },
      { MBCCOMP_GET_VELOCITYY, 1 },
      { MBCCOMP_GET_DIRECTION, 1 },
      { MBCCOMP_GET_HEALTH, 1 },
      { MBCCOMP_GET_MAXHEALTH, 1 },
      { MBCCOMP_GET_MOVEMENTTYPE, 1 },
      { MBCCOMP_GET_ISINVULNERABLE, 1 },
      { MBCCOMP_GET_ARENAENTITYCOUNT, 1 },
      { MBCCOMP_GET_ARENAENTITYPOSITIONLEFT, 2 },
      { MBCCOMP_GET_ARENAENTITYPOSITIONTOP, 2 },
      { MBCCOMP_GET_ARENAENTITYVELOCITYX, 2 },
      { MBCCOMP_GET_ARENAENTITYVELOCITYY, 2 },
      { MBCCOMP_GET_ARENAENTITYDIRECTION, 2 },
      { MBCCOMP_GET_ARENAENTITYHEALTH, 2 },
      { MBCCOMP_GET_ARENAENTITYMAXHEALTH, 2 },
      { MBCCOMP_GET_ARENAENTITYMOVEMENTTYPE, 2 },
      { MBCCOMP_GET_ARENAENTITYDAMAGESECONDS, 2 },
      { MBCCOMP_GET_ARENAENTITYISINVULNERABLE, 2 },
      { MBCCOMP_GET_ARENAENTITYTYPE, 2 },
      { MBCCOMP_SET_VELOCITYX, 1 },
      { MBCCOMP_SET_VELOCITYY, 1 },
      { MBCCOMP_SET_DIRECTION, 1 },
      { MBCCOMP_DO_SHOOT, 0 },
      { MBCCOMP_DO_SHOOTTARGET, 2 }
   };
}
