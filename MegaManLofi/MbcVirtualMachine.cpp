#include <math.h>

#include "MbcVirtualMachine.h"

using namespace std;
using namespace MegaManLofi;

MbcVirtualMachine::MbcVirtualMachine()
{
   Reset();
}

void MbcVirtualMachine::ClearFloatRegisters()
{
   for ( int i = 0; i < MBC_REGISTERS; i++ )
   {
      _floatRegisters[i] = 0.0f;
   }
}

void MbcVirtualMachine::ClearIntRegisters()
{
   for ( int i = 0; i < MBC_REGISTERS; i++ )
   {
      _intRegisters[i] = 0;
   }
}

void MbcVirtualMachine::Reset()
{
   ClearReturnStack();
   _currentInstruction = 0;
   _currentLine = 0;
   _isTicking = false;
}

void MbcVirtualMachine::ClearReturnStack()
{
   if ( _returnStack.size() > 0 )
   {
      _returnStack = stack<mbc_instruction>();
   }
}

void MbcVirtualMachine::SetInstructions( vector<mbc_instruction> instructions )
{
   _instructions = instructions;
   ClearFloatRegisters();
   ClearIntRegisters();
   Reset();
}

void MbcVirtualMachine::Tick()
{
   ClearReturnStack();
   _isTicking = true;

   for ( _currentLine = 0; _currentLine < (int)_instructions.size(); _currentLine++ )
   {
      _currentInstruction = _instructions[_currentLine];
      HandleCommand( MBC_PARSE_CMD( _currentInstruction ) );

      if ( !_isTicking )
      {
         break;
      }
   }
}

bool MbcVirtualMachine::HandleCommand( mbc_command command )
{
   switch ( command )
   {
      case MBCCMD_REGF:
         RegisterFloat();
         return true;
      case MBCCMD_REGI:
         RegisterInt();
         return true;
      case MBCCMD_CLEARFREG:
         ClearFloatRegisters();
         return true;
      case MBCCMD_CLEARIREG:
         ClearIntRegisters();
         return true;
      case MBCCMD_INCF:
         _floatRegisters[MBC_PARSE_ARG0( _currentInstruction )]++;
         return true;
      case MBCCMD_INCI:
         _intRegisters[MBC_PARSE_ARG0( _currentInstruction )]++;
         return true;

      case MBCCMD_ADDF:
         DoFloatMath( MathOp::Add );
         return true;
      case MBCCMD_SUBF:
         DoFloatMath( MathOp::Subtract );
         return true;
      case MBCCMD_MULF:
         DoFloatMath( MathOp::Multiply );
         return true;
      case MBCCMD_DIVF:
         DoFloatMath( MathOp::Divide );
         return true;
      case MBCCMD_ABSF:
         DoFloatAbs();
         return true;
      case MBCCMD_ADDI:
         DoIntMath( MathOp::Add );
         return true;
      case MBCCMD_SUBI:
         DoIntMath( MathOp::Subtract );
         return true;
      case MBCCMD_MULI:
         DoIntMath( MathOp::Multiply );
         return true;
      case MBCCMD_DIVI:
         DoIntMath( MathOp::Divide );
         return true;
      case MBCCMD_ABSI:
         DoIntAbs();
         return true;

      case MBCCMD_GOTO:
         Goto();
         return true;
      case MBCCMD_SUBR:
         Subroutine();
         return true;
      case MBCCMD_RET:
         Return();
         return true;
      case MBCCMD_STOP:
         Reset();
         return true;

      case MBCBR_EQF:
         DoFloatCondition( ConditionOp::Equals );
         return true;
      case MBCBR_NEQF:
         DoFloatCondition( ConditionOp::DoesNotEqual );
         return true;
      case MBCBR_LTF:
         DoFloatCondition( ConditionOp::LessThan );
         return true;
      case MBCBR_LTEF:
         DoFloatCondition( ConditionOp::LessThanOrEqual );
         return true;
      case MBCBR_GTF:
         DoFloatCondition( ConditionOp::GreaterThan );
         return true;
      case MBCBR_GTEF:
         DoFloatCondition( ConditionOp::GreaterThanOrEqual );
         return true;
      case MBCBR_EQI:
         DoIntCondition( ConditionOp::Equals );
         return true;
      case MBCBR_NEQI:
         DoIntCondition( ConditionOp::DoesNotEqual );
         return true;
      case MBCBR_LTI:
         DoIntCondition( ConditionOp::LessThan );
         return true;
      case MBCBR_LTEI:
         DoIntCondition( ConditionOp::LessThanOrEqual );
         return true;
      case MBCBR_GTI:
         DoIntCondition( ConditionOp::GreaterThan );
         return true;
      case MBCBR_GTEI:
         DoIntCondition( ConditionOp::GreaterThanOrEqual );
         return true;
      case MBCBR_TRUEF:
         DoBoolFloatCondition( true );
         return true;
      case MBCBR_FALSEF:
         DoBoolFloatCondition( false );
         return true;
      case MBCBR_TRUEI:
         DoBoolIntCondition( true );
         return true;
      case MBCBR_FALSEI:
         DoBoolIntCondition( false );
         return true;
      default:
         return false;
   }
}

void MbcVirtualMachine::RegisterFloat()
{
   auto regIndex = MBC_PARSE_ARG0( _currentInstruction );
   float val;
   memcpy( &val, &( _instructions[++_currentLine] ), 4 );
   _floatRegisters[regIndex] = val;
}

void MbcVirtualMachine::RegisterInt()
{
   auto regIndex = MBC_PARSE_ARG0( _currentInstruction );
   _intRegisters[regIndex] = (int)_instructions[++_currentLine];
}

void MbcVirtualMachine::DoFloatMath( MathOp op )
{
   auto leftRegIndex = MBC_PARSE_ARG0( _currentInstruction );
   auto rightRegIndex = MBC_PARSE_ARG1( _currentInstruction );
   auto resultRegIndex = MBC_PARSE_ARG2( _currentInstruction );

   switch ( op )
   {
      case MathOp::Add:
         _floatRegisters[resultRegIndex] = _floatRegisters[leftRegIndex] + _floatRegisters[rightRegIndex];
         break;
      case MathOp::Subtract:
         _floatRegisters[resultRegIndex] = _floatRegisters[leftRegIndex] - _floatRegisters[rightRegIndex];
         break;
      case MathOp::Multiply:
         _floatRegisters[resultRegIndex] = _floatRegisters[leftRegIndex] * _floatRegisters[rightRegIndex];
         break;
      case MathOp::Divide:
         _floatRegisters[resultRegIndex] = _floatRegisters[leftRegIndex] / _floatRegisters[rightRegIndex];
         break;
   }
}

void MbcVirtualMachine::DoFloatAbs()
{
   auto regIndex = MBC_PARSE_ARG0( _currentInstruction );
   auto resultRegIndex = MBC_PARSE_ARG1( _currentInstruction );

   _floatRegisters[resultRegIndex] = abs( _floatRegisters[regIndex] );
}

void MbcVirtualMachine::DoIntMath( MathOp op )
{
   auto leftRegIndex = MBC_PARSE_ARG0( _currentInstruction );
   auto rightRegIndex = MBC_PARSE_ARG1( _currentInstruction );
   auto resultRegIndex = MBC_PARSE_ARG2( _currentInstruction );

   switch ( op )
   {
      case MathOp::Add:
         _intRegisters[resultRegIndex] = _intRegisters[leftRegIndex] + _intRegisters[rightRegIndex];
         break;
      case MathOp::Subtract:
         _intRegisters[resultRegIndex] = _intRegisters[leftRegIndex] - _intRegisters[rightRegIndex];
         break;
      case MathOp::Multiply:
         _intRegisters[resultRegIndex] = _intRegisters[leftRegIndex] * _intRegisters[rightRegIndex];
         break;
      case MathOp::Divide:
         _intRegisters[resultRegIndex] = _intRegisters[leftRegIndex] / _intRegisters[rightRegIndex];
         break;
   }
}

void MbcVirtualMachine::DoIntAbs()
{
   auto regIndex = MBC_PARSE_ARG0( _currentInstruction );
   auto resultRegIndex = MBC_PARSE_ARG1( _currentInstruction );

   _intRegisters[resultRegIndex] = abs( _intRegisters[regIndex] );
}

void MbcVirtualMachine::Goto()
{
   _currentLine = _instructions[_currentLine + 1] - 1;
}

void MbcVirtualMachine::Subroutine()
{
   auto subroutineLine = _instructions[_currentLine + 1];
   _returnStack.push( _currentLine + 1 );
   _currentLine = subroutineLine - 1;
}

void MbcVirtualMachine::Return()
{
   _currentLine = _returnStack.top();
   _returnStack.pop();
}

void MbcVirtualMachine::DoFloatCondition( ConditionOp op )
{
   auto leftRegIndex = MBC_PARSE_ARG0( _currentInstruction );
   auto rightRegIndex = MBC_PARSE_ARG1( _currentInstruction );
   auto falseBlockLine = (int)_instructions[++_currentLine];

   switch ( op )
   {
      case ConditionOp::Equals:              if ( _floatRegisters[leftRegIndex] != _floatRegisters[rightRegIndex] ) _currentLine = falseBlockLine - 1; break;
      case ConditionOp::DoesNotEqual:        if ( _floatRegisters[leftRegIndex] == _floatRegisters[rightRegIndex] ) _currentLine = falseBlockLine - 1; break;
      case ConditionOp::LessThan:            if ( _floatRegisters[leftRegIndex] >= _floatRegisters[rightRegIndex] ) _currentLine = falseBlockLine - 1; break;
      case ConditionOp::LessThanOrEqual:     if ( _floatRegisters[leftRegIndex] >  _floatRegisters[rightRegIndex] ) _currentLine = falseBlockLine - 1; break;
      case ConditionOp::GreaterThan:         if ( _floatRegisters[leftRegIndex] <= _floatRegisters[rightRegIndex] ) _currentLine = falseBlockLine - 1; break;
      case ConditionOp::GreaterThanOrEqual:  if ( _floatRegisters[leftRegIndex] <  _floatRegisters[rightRegIndex] ) _currentLine = falseBlockLine - 1; break;
   }
}

void MbcVirtualMachine::DoIntCondition( ConditionOp op )
{
   auto leftRegIndex = MBC_PARSE_ARG0( _currentInstruction );
   auto rightRegIndex = MBC_PARSE_ARG1( _currentInstruction );
   auto falseBlockLine = (int)_instructions[++_currentLine];

   switch ( op )
   {
      case ConditionOp::Equals:              if ( _intRegisters[leftRegIndex] != _intRegisters[rightRegIndex] ) _currentLine = falseBlockLine - 1; break;
      case ConditionOp::DoesNotEqual:        if ( _intRegisters[leftRegIndex] != _intRegisters[rightRegIndex] ) _currentLine = falseBlockLine - 1; break;
      case ConditionOp::LessThan:            if ( _intRegisters[leftRegIndex] >= _intRegisters[rightRegIndex] ) _currentLine = falseBlockLine - 1; break;
      case ConditionOp::LessThanOrEqual:     if ( _intRegisters[leftRegIndex] >  _intRegisters[rightRegIndex] ) _currentLine = falseBlockLine - 1; break;
      case ConditionOp::GreaterThan:         if ( _intRegisters[leftRegIndex] <= _intRegisters[rightRegIndex] ) _currentLine = falseBlockLine - 1; break;
      case ConditionOp::GreaterThanOrEqual:  if ( _intRegisters[leftRegIndex] <  _intRegisters[rightRegIndex] ) _currentLine = falseBlockLine - 1; break;
   }
}

void MbcVirtualMachine::DoBoolFloatCondition( bool op )
{
   auto regIndex = MBC_PARSE_ARG0( _currentInstruction );
   auto falseBlockLine = (int)_instructions[++_currentLine];

   if ( ( op && _floatRegisters[regIndex] <= 0.0f ) || ( !op && _floatRegisters[regIndex] > 0.0f ) )
   {
      _currentLine = falseBlockLine - 1;
   }
}

void MbcVirtualMachine::DoBoolIntCondition( bool op )
{
   auto regIndex = MBC_PARSE_ARG0( _currentInstruction );
   auto falseBlockLine = (int)_instructions[++_currentLine];

   if ( ( op && _intRegisters[regIndex] <= 0 ) || ( !op && _intRegisters[regIndex] > 0 ) )
   {
      _currentLine = falseBlockLine - 1;
   }
}
