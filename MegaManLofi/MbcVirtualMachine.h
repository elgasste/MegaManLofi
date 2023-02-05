#pragma once

#include <vector>
#include <stack>

#include "MbcDefines.h"
#include "MathOp.h"
#include "ConditionOp.h"

namespace MegaManLofi
{
   class MbcVirtualMachine
   {
   public:
      MbcVirtualMachine();

      virtual void SetInstructions( std::vector<mbc_instruction> instructions );
      void Tick();

   protected:
      virtual void Reset();
      virtual void ClearFloatRegisters();
      virtual void ClearIntRegisters();
      virtual bool HandleCommand( mbc_command command );

   private:
      void RegisterFloat();
      void RegisterInt();
      void DoFloatMath( MathOp op );
      void DoFloatAbs();
      void DoIntMath( MathOp op );
      void DoIntAbs();
      void Goto();
      void Subroutine();
      void Return();
      void DoFloatCondition( ConditionOp op );
      void DoIntCondition( ConditionOp op );

   protected:
      std::vector<mbc_instruction> _instructions;
      bool _isTicking;

      float _floatRegisters[MBC_REGISTERS];
      int _intRegisters[MBC_REGISTERS];
      std::stack<mbc_instruction> _returnStack;

      mbc_instruction _currentInstruction;
      int _currentLine;
   };
}
