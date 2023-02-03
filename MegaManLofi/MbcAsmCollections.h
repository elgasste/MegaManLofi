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
      MBCCOMP_ENDWHILE
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
      { MBCCOMP_ABSI, MBCCMD_ABSI }
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
      { MBCCOMP_ENDWHILE, 0 }
   };
}
