#include <format>
#include <algorithm>

#include "MbcAsmCompiler.h"
#include "MbcAsmCollections.h"
#include "StringUtilities.h"

using namespace std;
using namespace MbcAsm;
using namespace MegaManLofi;

MbcAsmCompiler::MbcAsmCompiler() :
   _mainStartIndex( 0 )
{
}

vector<mbc_instruction> MbcAsmCompiler::Compile( const string& code )
{
   BuildTokenLines( code );
   VerifyTokenLines();
   InsertNoopTokenLines();
   BuildFuncIndicesMap();
   BuildIfBlocksMap();
   BuildLoopMaps();

   _instructions = vector<mbc_instruction>();

   for ( int i = 0; i < (int)_tokenLines.size(); i++ )
   {
      if ( i == 0 && _mainStartIndex != 0 )
      {
         _instructions.push_back( CreateGotoInstruction( _mainStartIndex ) );
      }
      else
      {
         CompileTokenLine( i );
      }
   }

   return _instructions;
}

void MbcAsmCompiler::BuildTokenLines( const string& code )
{
   _tokenLines = vector<vector<string>>();
   _sourceLinesMap = map<int, int>();
   auto stringLines = StringUtilities::Explode( code, '\n' );

   for ( int i = 0; i < (int)stringLines.size(); i++ )
   {
      auto tokens = StringUtilities::ExplodeByWhitespace( stringLines[i] );

      // skip empty lines and comment-only lines
      if ( tokens.size() > 0 && tokens[0][0] != '#' )
      {
         // ensure upper-case tokens
         for ( int j = 0; j < (int)tokens.size(); j++ )
         {
            tokens[j] = StringUtilities::ToUpper( tokens[j] );
         }

         _tokenLines.push_back( tokens );
         _sourceLinesMap[(int)_tokenLines.size() - 1] = i + 1;
      }
   }

   if ( _tokenLines.size() == 0 )
   {
      throw exception( "no code found" );
   }
}

void MbcAsmCompiler::VerifyTokenLines() const
{
   for ( int i = 0; i < (int)_tokenLines.size(); i++ )
   {
      auto tokenLine = _tokenLines[i];

      if ( find( AllTokens.begin(), AllTokens.end(), tokenLine[0] ) == AllTokens.end() )
      {
         Error( format( "invalid command: {0}", tokenLine[0] ), i );
      }

      auto argCount = (int)tokenLine.size() - 1;
      auto expectedArgCount = TokenArgCountMap.at( tokenLine[0] );

      if ( argCount < expectedArgCount )
      {
         Error( format( "missing arg(s) for command: {0}", tokenLine[0] ), i );
      }

      for ( int j = 1; j <= expectedArgCount; j++ )
      {
         if ( tokenLine[j][0] == '#' )
         {
            Error( format( "missing arg(s) for command: {0}", tokenLine[0] ), i );
         }
      }

      if ( argCount > expectedArgCount && _tokenLines[i][expectedArgCount + 1][0] != '#' )
      {
         Error( format( "too many args for command: {0}", tokenLine[0] ), i );
      }
   }
}

void MbcAsmCompiler::InsertNoopTokenLines()
{
   // REGF/REGI instructions are always followed by a "data" instruction, so to keep our MBC
   // assembly code one-to-one with bytecode output, just add a NOOP after each REGF/REGI
   for ( int i = 0; i < (int)_tokenLines.size(); i++ )
   {
      if ( _tokenLines[i][0] == MBCCOMP_REGI || _tokenLines[i][0] == MBCCOMP_REGF )
      {
         i++;
         _tokenLines.insert( _tokenLines.begin() + i, vector<string> { MBCCOMP_NOOP } );
         AdjustSourceLinesMapForInsertion( i );
      }
   }
}

void MbcAsmCompiler::BuildFuncIndicesMap()
{
   _funcIndicesMap = map<string, int>();

   if ( _tokenLines[0][0] != MBCCOMP_FUNC )
   {
      throw exception( "code must begin with a function" );
   }

   bool searchingForFunc = true;
   bool mainFuncFound = false;
   bool inMainFunc = false;

   for ( int i = 0; i < (int)_tokenLines.size(); i++ )
   {
      auto commandToken = _tokenLines[i][0];

      if ( searchingForFunc )
      {
         if ( commandToken == MBCCOMP_FUNC )
         {
            auto name = StringUtilities::ToUpper( _tokenLines[i][1] );

            // if the first function is not the main function, insert a NOOP, which
            // will be replaced with a GOTO during compilation
            if ( i == 0 && name != MBCCOMP_MAINFUNC )
            {
               _tokenLines.insert( _tokenLines.begin(), vector<string> { MBCCOMP_NOOP } );
               AdjustSourceLinesMapForInsertion( i );
               continue;
            }

            if ( _funcIndicesMap.find( name ) != _funcIndicesMap.end() )
            {
               Error( format( "function \"{0}\" defined more than once", name ), i );
            }

            _funcIndicesMap[name] = i;
            _tokenLines.erase( _tokenLines.begin() + i );
            AdjustSourceLinesMapForRemoval( i );
            searchingForFunc = false;

            if ( name == MBCCOMP_MAINFUNC )
            {
               mainFuncFound = true;
               inMainFunc = true;
               _mainStartIndex = i;
            }

            i--;
         }
      }
      else
      {
         if ( commandToken == MBCCOMP_RET )
         {
            searchingForFunc = true;

            if ( inMainFunc )
            {
               inMainFunc = false;
               _tokenLines[i][0] = MBCCOMP_STOP;
            }
         }
         else if ( commandToken == MBCCOMP_FUNC )
         {
            Error( "nested functions are not allowed", i );
         }
      }
   }

   if ( !searchingForFunc )
   {
      throw exception( "last function is missing a return" );
   }

   if ( !mainFuncFound )
   {
      throw exception( "'main' function was not found" );
   }
}

// MUFFINS: there's an issue here where if you have a nested IF, when it ends it goes straight into the
// outer IF block's ELSE, instead of the ENDIF.
void MbcAsmCompiler::BuildIfBlocksMap()
{
   _ifBlocksMap = map<int, ConditionalInfo>();

   for ( int i = 0; i < (int)_tokenLines.size(); i++ )
   {
      auto commandToken = _tokenLines[i][0];

      if ( commandToken == MBCCOMP_ELSE || commandToken == MBCCOMP_ENDIF )
      {
         Error( format( "invalid conditional token: {0}", commandToken ), i );
      }
      else if ( find( IfTokens.begin(), IfTokens.end(), commandToken ) != IfTokens.end() )
      {
         AddIfBlockRecursive( i );
      }
   }
}

void MbcAsmCompiler::AddIfBlockRecursive( int& index )
{
   int startIndex = index;
   int elseGotoIndex = -1;
   int loopCounter = 0;
   index++;

   for ( ; index < (int)_tokenLines.size(); index++ )
   {
      for ( const auto& [name, funcIndex] : _funcIndicesMap )
      {
         if ( funcIndex == index )
         {
            Error( "condition must end inside function", startIndex );
         }
      }

      auto commandToken = _tokenLines[index][0];

      if ( find( LoopTokens.begin(), LoopTokens.end(), commandToken ) != LoopTokens.end() )
      {
         loopCounter++;
      }
      else if ( commandToken == MBCCOMP_ENDWHILE )
      {
         loopCounter--;

         if ( loopCounter < 0 )
         {
            Error( format( "invalid loop token: {0}", commandToken ), startIndex );
         }
      }
      else if ( commandToken == MBCCOMP_ENDIF )
      {
         if ( loopCounter > 0 )
         {
            Error( "loop must end before ENDIF", index );
         }

         _ifBlocksMap[startIndex].EndLine = index;

         if ( _ifBlocksMap[startIndex].ElseLine < 0 )
         {
            _ifBlocksMap[startIndex].ElseLine = index;
         }

         // MUFFINS: if an ELSE was found earlier, point its GOTO at this index
         if ( elseGotoIndex >= 0 )
         {
            _tokenLines[elseGotoIndex][1] = format( "{0}", index );
         }

         _tokenLines.erase( _tokenLines.begin() + index );
         AdjustFuncIndicesForRemoval( index );
         AdjustSourceLinesMapForRemoval( index );
         AdjustGotoIndicesForRemoval( index );
         index--;

         return;
      }
      else if ( commandToken == MBCCOMP_ELSE )
      {
         if ( loopCounter > 0 )
         {
            Error( "loop must end before ELSE", index );
         }

         if ( _ifBlocksMap[startIndex].ElseLine >= 0 )
         {
            Error( "multiple ELSE statements are not allowed", startIndex );
         }

         // MUFFINS: turn the ELSE into a GOTO, and point it at the ENDIF line when we get there
         elseGotoIndex = index;
         _ifBlocksMap[startIndex].ElseLine = index + 1;
         _tokenLines[index][0] = MBCCOMP_GOTO;
         _tokenLines[index].insert( _tokenLines[index].begin() + 1, "-1" );
      }
      else if ( find( IfTokens.begin(), IfTokens.end(), commandToken ) != IfTokens.end() )
      {
         AddIfBlockRecursive( index );
      }
   }

   // I'm not sure it's possible to get here, but we should account for it
   Error( "condition must have a matching ENDIF", startIndex );
}

void MbcAsmCompiler::BuildLoopMaps()
{
   _loopBlocksMap = map<int, int>();
   _loopBreaksMap = map<int, int>();

   for ( int i = 0; i < (int)_tokenLines.size(); i++ )
   {
      auto commandToken = _tokenLines[i][0];

      if ( commandToken == MBCCOMP_BREAK || commandToken == MBCCOMP_ENDWHILE )
      {
         Error( format( "invalid loop command: {0}", commandToken ), i );
      }
      else if ( find( LoopTokens.begin(), LoopTokens.end(), commandToken ) != LoopTokens.end() )
      {
         AddLoopBlockRecursive( i );
      }
   }
}

void MbcAsmCompiler::AddLoopBlockRecursive( int& index )
{
   int startIndex = index;
   index++;

   for ( ; index < (int)_tokenLines.size(); index++ )
   {
      for ( const auto& [name, funcIndex] : _funcIndicesMap )
      {
         if ( funcIndex == index )
         {
            Error( "loop must end inside function", startIndex );
         }
      }

      auto commandToken = _tokenLines[index][0];

      if ( commandToken == MBCCOMP_ENDWHILE )
      {
         _loopBlocksMap[startIndex] = index;
         return;
      }
      else if ( commandToken == MBCCOMP_BREAK )
      {
         _loopBreaksMap[index] = startIndex;
      }
      else if ( find( LoopTokens.begin(), LoopTokens.end(), commandToken ) != LoopTokens.end() )
      {
         AddLoopBlockRecursive( index );
      }
   }

   Error( "loop must have a matching ENDWHILE", startIndex );
}

void MbcAsmCompiler::CompileTokenLine( int index )
{
   auto tokenLine = _tokenLines[index];
   auto commandToken = tokenLine[0];

   if ( commandToken == MBCCOMP_NOOP )
   {
      return;
   }
   else if ( commandToken == MBCCOMP_CALL )
   {
      _instructions.push_back( CreateSubrInstruction( _funcIndicesMap.at( StringUtilities::ToUpper( tokenLine[1] ) ) ) );
   }
   else if ( commandToken == MBCCOMP_GOTO )
   {
      // MUFFINS: test this out
      _instructions.push_back( CreateGotoInstruction( stoi( _tokenLines[index][1] ) ) );
   }
   else if ( commandToken == MBCCOMP_BREAK )
   {
      _instructions.push_back( CreateGotoInstruction( _loopBlocksMap.at( _loopBreaksMap.at( index ) ) + 1 ) );
   }
   else if ( commandToken == MBCCOMP_ENDWHILE )
   {
      bool blockFound = false;
      for ( const auto& [loopStartLine, loopEndLine] : _loopBlocksMap )
      {
         if ( loopEndLine == index )
         {
            blockFound = true;
            _instructions.push_back( CreateGotoInstruction( loopStartLine ) );
            break;
         }
      }

      // we should never get here, but it has to be accounted for
      if ( !blockFound )
      {
         Error( "COMPILER ERROR!! loop end has no start, Steve messed up!", index );
      }
   }
   else if ( find( IfTokens.begin(), IfTokens.end(), commandToken ) != IfTokens.end() )
   {
      AddIfInstruction( tokenLine, index );
   }
   else if ( find( LoopTokens.begin(), LoopTokens.end(), commandToken ) != LoopTokens.end() )
   {
      AddLoopInstruction( tokenLine, index );
   }
   else if ( find( RegTokens.begin(), RegTokens.end(), commandToken ) != RegTokens.end() )
   {
      AddRegInstruction( tokenLine );
   }
   else if ( find( MathTokens.begin(), MathTokens.end(), commandToken ) != MathTokens.end() )
   {
      AddMathInstruction( tokenLine );
   }
   else
   {
      AddGeneralInstruction( tokenLine );
   }
}

void MbcAsmCompiler::AddIfInstruction( const vector<string>& tokenLine, int index )
{
   auto arg2 = _ifBlocksMap.at( index ).ElseLine;
   auto arg3 = _ifBlocksMap.at( index ).EndLine;
   AddBranchInstruction( tokenLine, IfTokenMap.at( tokenLine[0] ), arg2, arg3 );
}

void MbcAsmCompiler::AddLoopInstruction( const vector<string>& tokenLine, int index )
{
   auto arg2 = _loopBlocksMap.at( index ) + 1;
   AddBranchInstruction( tokenLine, BranchTokenMap.at( tokenLine[0] ), arg2, arg2 );
}

void MbcAsmCompiler::AddBranchInstruction( const vector<string>& tokenLine, mbc_command command, int arg2, int arg3 )
{
   auto instruction = (mbc_instruction)( command << MBC_CMD_SHIFT );

   switch ( command )
   {
      case MBCBR_EQF:
         _instructions.push_back( instruction | CreateFloatConditionInstruction( tokenLine, ConditionOp::Equals, arg2, arg3 ) );
         break;
      case MBCBR_LTF:
         _instructions.push_back( instruction | CreateFloatConditionInstruction( tokenLine, ConditionOp::LessThan, arg2, arg3 ) );
         break;
      case MBCBR_GTF:
         _instructions.push_back( instruction | CreateFloatConditionInstruction( tokenLine, ConditionOp::GreaterThan, arg2, arg3 ) );
         break;
      case MBCBR_LTEF:
         _instructions.push_back( instruction | CreateFloatConditionInstruction( tokenLine, ConditionOp::LessThanOrEqual, arg2, arg3 ) );
         break;
      case MBCBR_GTEF:
         _instructions.push_back( instruction | CreateFloatConditionInstruction( tokenLine, ConditionOp::GreaterThanOrEqual, arg2, arg3 ) );
         break;
      case MBCBR_EQI:
         _instructions.push_back( instruction | CreateIntConditionInstruction( tokenLine, ConditionOp::Equals, arg2, arg3 ) );
         break;
      case MBCBR_LTI:
         _instructions.push_back( instruction | CreateIntConditionInstruction( tokenLine, ConditionOp::LessThan, arg2, arg3 ) );
         break;
      case MBCBR_GTI:
         _instructions.push_back( instruction | CreateIntConditionInstruction( tokenLine, ConditionOp::GreaterThan, arg2, arg3 ) );
         break;
      case MBCBR_LTEI:
         _instructions.push_back( instruction | CreateIntConditionInstruction( tokenLine, ConditionOp::LessThanOrEqual, arg2, arg3 ) );
         break;
      case MBCBR_GTEI:
         _instructions.push_back( instruction | CreateIntConditionInstruction( tokenLine, ConditionOp::GreaterThanOrEqual, arg2, arg3 ) );
         break;
   }
}

void MbcAsmCompiler::AddRegInstruction( const vector<string>& tokenLine )
{
   auto command = RegTokenMap.at( tokenLine[0] );
   auto instruction = (mbc_instruction)( command << MBC_CMD_SHIFT );

   switch ( command )
   {
      case MBCCMD_REGF:
         _instructions.push_back( instruction | ( (mbc_instruction)stoi( tokenLine[1] ) << MBC_ARG0_SHIFT ) );
         _instructions.push_back( FloatAsInstruction( stof( tokenLine[2] ) ) );
         break;
      case MBCCMD_REGI:
         _instructions.push_back( instruction | ( (mbc_instruction)stoi( tokenLine[1] ) << MBC_ARG0_SHIFT ) );
         _instructions.push_back( stoi( tokenLine[2] ) );
         break;
   }
}

void MbcAsmCompiler::AddMathInstruction( const vector<string>& tokenLine )
{
   auto command = MathTokenMap.at( tokenLine[0] );
   auto instruction = (mbc_instruction)( command << MBC_CMD_SHIFT );

   switch ( command )
   {
      case MBCCMD_ADDF:
         _instructions.push_back( instruction | CreateFloatMathInstruction( tokenLine, MathOp::Add ) );
         break;
      case MBCCMD_SUBF:
         _instructions.push_back( instruction | CreateFloatMathInstruction( tokenLine, MathOp::Subtract ) );
         break;
      case MBCCMD_MULF:
         _instructions.push_back( instruction | CreateFloatMathInstruction( tokenLine, MathOp::Multiply ) );
         break;
      case MBCCMD_DIVF:
         _instructions.push_back( instruction | CreateFloatMathInstruction( tokenLine, MathOp::Divide ) );
         break;
      case MBCCMD_ADDI:
         _instructions.push_back( instruction | CreateIntMathInstruction( tokenLine, MathOp::Add ) );
         break;
      case MBCCMD_SUBI:
         _instructions.push_back( instruction | CreateFloatMathInstruction( tokenLine, MathOp::Subtract ) );
         break;
      case MBCCMD_MULI:
         _instructions.push_back( instruction | CreateFloatMathInstruction( tokenLine, MathOp::Multiply ) );
         break;
      case MBCCMD_DIVI:
         _instructions.push_back( instruction | CreateFloatMathInstruction( tokenLine, MathOp::Divide ) );
         break;
   }
}

void MbcAsmCompiler::AddGeneralInstruction( const vector<string>& tokenLine )
{
   auto command = GeneralTokenMap.at( tokenLine[0] );
   auto instruction = (mbc_instruction)( command << MBC_CMD_SHIFT );
   auto argShift = MBC_ARG0_SHIFT;

   for ( int i = 0; i < TokenArgCountMap.at( tokenLine[0] ); i++ )
   {
      instruction |= ( (mbc_instruction)stoi( tokenLine[i + 1] ) << argShift );
      argShift -= 6;
   }

   _instructions.push_back( instruction );
}

mbc_instruction MbcAsmCompiler::CreateGotoInstruction( int gotoIndex ) const
{
   return (mbc_instruction)( ( MBCCMD_GOTO << MBC_CMD_SHIFT ) | ( gotoIndex << MBC_ARG0_SHIFT ) );
}

mbc_instruction MbcAsmCompiler::CreateSubrInstruction( int subrIndex ) const
{
   return (mbc_instruction)( ( MBCCMD_SUBR << MBC_CMD_SHIFT ) | ( subrIndex << MBC_ARG0_SHIFT ) );
}

mbc_instruction MbcAsmCompiler::CreateFloatMathInstruction( const vector<string>& tokenLine, MathOp op ) const
{
   auto instruction =
      ( (mbc_instruction)stoi( tokenLine[1] ) << MBC_ARG0_SHIFT ) |
      ( (mbc_instruction)stoi( tokenLine[2] ) << MBC_ARG1_SHIFT ) |
      ( (mbc_instruction)stoi( tokenLine[3] ) << MBC_ARG2_SHIFT );

   switch ( op )
   {
      case MathOp::Add:
         instruction |= (mbc_instruction)( MBCCMD_ADDF << MBC_CMD_SHIFT );
         break;
      case MathOp::Subtract:
         instruction |= (mbc_instruction)( MBCCMD_SUBF << MBC_CMD_SHIFT );
         break;
      case MathOp::Multiply:
         instruction |= (mbc_instruction)( MBCCMD_MULF << MBC_CMD_SHIFT );
         break;
      case MathOp::Divide:
         instruction |= (mbc_instruction)( MBCCMD_DIVF << MBC_CMD_SHIFT );
         break;
   }

   return instruction;
}

mbc_instruction MbcAsmCompiler::CreateIntMathInstruction( const vector<string>& tokenLine, MathOp op ) const
{
   auto instruction =
      ( (mbc_instruction)stoi( tokenLine[1] ) << MBC_ARG0_SHIFT ) |
      ( (mbc_instruction)stoi( tokenLine[2] ) << MBC_ARG1_SHIFT ) |
      ( (mbc_instruction)stoi( tokenLine[3] ) << MBC_ARG2_SHIFT );

   switch ( op )
   {
      case MathOp::Add:
         instruction |= (mbc_instruction)( MBCCMD_ADDI << MBC_CMD_SHIFT );
         break;
      case MathOp::Subtract:
         instruction |= (mbc_instruction)( MBCCMD_SUBI << MBC_CMD_SHIFT );
         break;
      case MathOp::Multiply:
         instruction |= (mbc_instruction)( MBCCMD_MULI << MBC_CMD_SHIFT );
         break;
      case MathOp::Divide:
         instruction |= (mbc_instruction)( MBCCMD_DIVI << MBC_CMD_SHIFT );
         break;
   }

   return instruction;
}

mbc_instruction MbcAsmCompiler::CreateFloatConditionInstruction( const vector<string>& tokenLine, ConditionOp op, int arg2, int arg3 ) const
{
   auto instruction =
      ( (mbc_instruction)stoi( tokenLine[1] ) << MBC_ARG0_SHIFT ) |
      ( (mbc_instruction)stoi( tokenLine[2] ) << MBC_ARG1_SHIFT ) |
      ( (mbc_instruction)arg2 << MBC_ARG2_SHIFT ) |
      ( (mbc_instruction)arg3 );

   switch ( op )
   {
      case ConditionOp::Equals:
         instruction |= (mbc_instruction)( MBCBR_EQF << MBC_CMD_SHIFT );
         break;
      case ConditionOp::LessThan:
         instruction |= (mbc_instruction)( MBCBR_LTF << MBC_CMD_SHIFT );
         break;
      case ConditionOp::LessThanOrEqual:
         instruction |= (mbc_instruction)( MBCBR_LTEF << MBC_CMD_SHIFT );
         break;
      case ConditionOp::GreaterThan:
         instruction |= (mbc_instruction)( MBCBR_GTF << MBC_CMD_SHIFT );
         break;
      case ConditionOp::GreaterThanOrEqual:
         instruction |= (mbc_instruction)( MBCBR_GTEF << MBC_CMD_SHIFT );
         break;
   }

   return instruction;
}

mbc_instruction MbcAsmCompiler::CreateIntConditionInstruction( const vector<string>& tokenLine, ConditionOp op, int arg2, int arg3 ) const
{
   auto instruction =
      ( (mbc_instruction)stoi( tokenLine[1] ) << MBC_ARG0_SHIFT ) |
      ( (mbc_instruction)stoi( tokenLine[2] ) << MBC_ARG1_SHIFT ) |
      ( (mbc_instruction)arg2 << MBC_ARG2_SHIFT ) |
      ( (mbc_instruction)arg3 );

   switch ( op )
   {
      case ConditionOp::Equals:
         instruction |= (mbc_instruction)( MBCBR_EQI << MBC_CMD_SHIFT );
         break;
      case ConditionOp::LessThan:
         instruction |= (mbc_instruction)( MBCBR_LTI << MBC_CMD_SHIFT );
         break;
      case ConditionOp::LessThanOrEqual:
         instruction |= (mbc_instruction)( MBCBR_LTEI << MBC_CMD_SHIFT );
         break;
      case ConditionOp::GreaterThan:
         instruction |= (mbc_instruction)( MBCBR_GTI << MBC_CMD_SHIFT );
         break;
      case ConditionOp::GreaterThanOrEqual:
         instruction |= (mbc_instruction)( MBCBR_GTEI << MBC_CMD_SHIFT );
         break;
   }

   return instruction;
}

mbc_instruction MbcAsmCompiler::FloatAsInstruction( float val ) const
{
   mbc_instruction instruction;
   memcpy( &instruction, &val, 4 );
   return instruction;
}

void MbcAsmCompiler::AdjustFuncIndicesForRemoval( int removalIndex )
{
   for ( const auto& [name, funcIndex] : _funcIndicesMap )
   {
      if ( funcIndex > removalIndex )
      {
         _funcIndicesMap[name]--;
      }

      if ( _mainStartIndex > removalIndex )
      {
         _mainStartIndex--;
      }
   }
}

void MbcAsmCompiler::AdjustSourceLinesMapForRemoval( int removalIndex )
{
   for ( int i = removalIndex; i < (int)_tokenLines.size(); i++ )
   {
      if ( _sourceLinesMap.find( i + 1 ) != _sourceLinesMap.end() )
      {
         _sourceLinesMap[i] = _sourceLinesMap[i + 1];
         _sourceLinesMap.erase( i + 1 );
      }
   }
}

// MUFFINS
void MbcAsmCompiler::AdjustGotoIndicesForRemoval( int removalIndex )
{
   for ( int i = 0; i < (int)_tokenLines.size(); i++ )
   {
      if ( _tokenLines[i][0] == MBCCOMP_GOTO )
      {
         auto gotoIndex = stoi( _tokenLines[i][1] );

         if ( gotoIndex > removalIndex )
         {
            _tokenLines[i][1] = format( "{0}", gotoIndex - 1 );
         }
      }
   }
}

void MbcAsmCompiler::AdjustSourceLinesMapForInsertion( int insertionIndex )
{
   for ( int i = (int)_tokenLines.size() - 2; i >= insertionIndex; i-- )
   {
      if ( _sourceLinesMap.find( i ) != _sourceLinesMap.end() )
      {
         _sourceLinesMap[i + 1] = _sourceLinesMap[i];
         _sourceLinesMap.erase( i );
      }
   }
}

void MbcAsmCompiler::Error( const std::string& message, int index ) const
{
   throw exception( format( "line {0}: {1}", _sourceLinesMap.at( index ), message ).c_str() );
}
