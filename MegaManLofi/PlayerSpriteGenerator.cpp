#include <string>

#include "PlayerSpriteGenerator.h"
#include "ConsoleSprite.h"
#include "Direction.h"
#include "ConsoleColor.h"

using namespace std;
using namespace MegaManLofi;

map<Direction, ConsoleSprite> PlayerSpriteGenerator::GenerateStaticSpriteMap()
{
   auto spriteMap = map<Direction, ConsoleSprite>();

   // facing left without firing
   spriteMap[Direction::Left].Width = 4;
   spriteMap[Direction::Left].Height = 3;
   for ( auto line : { "  O ",
                       "o-|\\",
                       " / \\" } ) { for ( int i = 0; i < spriteMap[Direction::Left].Width; i++ ) { spriteMap[Direction::Left].Pixels.push_back( { line[i], ConsoleColor::Cyan } ); } }

   // facing up-left without firing
   spriteMap[Direction::UpLeft].Width = 4;
   spriteMap[Direction::UpLeft].Height = 3;
   for ( auto line : { "o O ",
                       " `|\\",
                       " / \\" } ) { for ( int i = 0; i < spriteMap[Direction::UpLeft].Width; i++ ) { spriteMap[Direction::UpLeft].Pixels.push_back( { line[i], ConsoleColor::Cyan } ); } }

   // facing down-left without firing
   spriteMap[Direction::DownLeft].Width = 4;
   spriteMap[Direction::DownLeft].Height = 3;
   for ( auto line : { "  O ",
                       " ,|\\",
                       "o/ \\" } ) { for ( int i = 0; i < spriteMap[Direction::DownLeft].Width; i++ ) { spriteMap[Direction::DownLeft].Pixels.push_back( { line[i], ConsoleColor::Cyan } ); } }

   // facing right without firing
   spriteMap[Direction::Right].Width = 4;
   spriteMap[Direction::Right].Height = 3;
   for ( auto line : { " O  ",
                       "/|-o",
                       "/ \\ " } ) { for ( int i = 0; i < spriteMap[Direction::Right].Width; i++ ) { spriteMap[Direction::Right].Pixels.push_back( { line[i], ConsoleColor::Cyan } ); } }

   // facing up-right without firing
   spriteMap[Direction::UpRight].Width = 4;
   spriteMap[Direction::UpRight].Height = 3;
   for ( auto line : { " O o",
                       "/|' ",
                       "/ \\ " } ) { for ( int i = 0; i < spriteMap[Direction::UpRight].Width; i++ ) { spriteMap[Direction::UpRight].Pixels.push_back( { line[i], ConsoleColor::Cyan } ); } }

   // facing down-right without firing
   spriteMap[Direction::DownRight].Width = 4;
   spriteMap[Direction::DownRight].Height = 3;
   for ( auto line : { " O  ",
                       "/|. ",
                       "/ \\o" } ) { for ( int i = 0; i < spriteMap[Direction::DownRight].Width; i++ ) { spriteMap[Direction::DownRight].Pixels.push_back( { line[i], ConsoleColor::Cyan } ); } }

   // facing up without firing
   spriteMap[Direction::Up].Width = 4;
   spriteMap[Direction::Up].Height = 3;
   for ( auto line : { " Oo ",
                       "/|' ",
                       "/ \\ " } ) { for ( int i = 0; i < spriteMap[Direction::Up].Width; i++ ) { spriteMap[Direction::Up].Pixels.push_back( { line[i], ConsoleColor::Cyan } ); } }

   // facing down without firing
   spriteMap[Direction::Down].Width = 4;
   spriteMap[Direction::Down].Height = 3;
   for ( auto line : { " O  ",
                       "/|. ",
                       "/`o " } ) { for ( int i = 0; i < spriteMap[Direction::Down].Width; i++ ) { spriteMap[Direction::Down].Pixels.push_back( { line[i], ConsoleColor::Cyan } ); } }

   return spriteMap;
}

map<Direction, ConsoleSprite> PlayerSpriteGenerator::GenerateMovingSpriteMap()
{
   // TODO: figure out how to do sprite swapping based on frame count
   return GenerateStaticSpriteMap();
}