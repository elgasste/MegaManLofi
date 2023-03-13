#pragma once

#include <string>
#include <vector>
#include <map>

#include "MbcDefines.h"
#include "MbcAsmDefines.h"
#include "MathOp.h"
#include "ConditionOp.h"

namespace MbcAsm
{
   class MbcAsmCompiler
   {
   public:
      MbcAsmCompiler();

      std::vector<mbc_instruction> Compile( const std::string& code );

   private:
      void BuildTokenLines( const std::string& code );
      void VerifyTokenLines() const;
      void InsertNoopTokenLines();
      void BuildFuncIndicesMap();
      void BuildIfBlocksMap();
      void AddIfBlockRecursive( int& index );
      void BuildLoopMaps();
      void AddLoopBlockRecursive( int& index );

      void CompileTokenLine( int index );
      void AddBranchInstruction( const std::vector<std::string>& tokenLine, mbc_command command, int falseIndex );
      void AddRegInstruction( const std::vector<std::string>& tokenLine );
      void AddMathInstruction( const std::vector<std::string>& tokenLine );
      void AddGeneralInstruction( const std::vector<std::string>& tokenLine );
      void AddGotoInstruction( int gotoIndex );
      void AddSubrInstruction( const std::vector<std::string>& tokenLine );
      void AddFloatConditionInstruction( const std::vector<std::string>& tokenLine, MegaManLofi::ConditionOp op, int falseIndex );
      void AddIntConditionInstruction( const std::vector<std::string>& tokenLine, MegaManLofi::ConditionOp op, int falseIndex );
      void AddFloatBoolInstruction( const std::vector<std::string>& tokenLine, bool op, int falseIndex );
      void AddIntBoolInstruction( const std::vector<std::string>& tokenLine, bool op, int falseIndex );

      mbc_instruction CreateFloatMathInstruction( const std::vector<std::string>& tokenLine, MegaManLofi::MathOp op ) const;
      mbc_instruction CreateIntMathInstruction( const std::vector<std::string>& tokenLine, MegaManLofi::MathOp op ) const;

      mbc_instruction FloatAsInstruction( float val ) const;

      void AdjustFuncIndicesForRemoval( int removalIndex );
      void AdjustSourceLinesMapForRemoval( int removalIndex );
      void AdjustGotoIndicesForRemoval( int removalIndex );
      void AdjustSourceLinesMapForInsertion( int insertionIndex );

      void Error( const std::string& message, int index ) const;

#if MBCCOMP_ENABLELOGGER == 1
      void LogAsm() const;
      void LogMbc() const;
#endif

   private:
      std::vector<std::vector<std::string>> _tokenLines;
      std::vector<mbc_instruction> _instructions;

      std::map<int, int> _sourceLinesMap;          // maps token line indices to source lines
      std::map<std::string, int> _funcIndicesMap;  // maps function names to token line indices
      std::map<int, int> _ifBlocksMap;             // maps "if" indices to "endif" indices
      std::map<int, int> _loopBlocksMap;           // maps loop start indices to end indices
      std::map<int, int> _loopBreaksMap;           // maps loop break indices to loop start indices
      int _mainStartIndex;                         // token line index of the "main" function
   };
}
