#include <string>

#include "PlayerSpriteGenerator.h"
#include "Direction.h"

using namespace std;
using namespace MegaManLofi;

map<Direction, ConsoleImage> PlayerSpriteGenerator::GenerateStaticSpriteMap()
{
   auto spriteMap = map<Direction, ConsoleImage>();

   // facing left without firing
   spriteMap[Direction::Left].Width = 4;
   spriteMap[Direction::Left].Height = 3;
   for ( auto line : { "  O ",
                       "o-|\\",
                       " / \\" } ) { for ( int i = 0; i < spriteMap[Direction::Left].Width; i++ ) { spriteMap[Direction::Left].Pixels.push_back( { line[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); } }

   // facing up-left without firing
   spriteMap[Direction::UpLeft].Width = 4;
   spriteMap[Direction::UpLeft].Height = 3;
   for ( auto line : { "o O ",
                       " `|\\",
                       " / \\" } ) { for ( int i = 0; i < spriteMap[Direction::UpLeft].Width; i++ ) { spriteMap[Direction::UpLeft].Pixels.push_back( { line[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); } }

   // facing down-left without firing
   spriteMap[Direction::DownLeft].Width = 4;
   spriteMap[Direction::DownLeft].Height = 3;
   for ( auto line : { "  O ",
                       " ,|\\",
                       "o/ \\" } ) { for ( int i = 0; i < spriteMap[Direction::DownLeft].Width; i++ ) { spriteMap[Direction::DownLeft].Pixels.push_back( { line[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); } }

   // facing right without firing
   spriteMap[Direction::Right].Width = 4;
   spriteMap[Direction::Right].Height = 3;
   for ( auto line : { " O  ",
                       "/|-o",
                       "/ \\ " } ) { for ( int i = 0; i < spriteMap[Direction::Right].Width; i++ ) { spriteMap[Direction::Right].Pixels.push_back( { line[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); } }

   // facing up-right without firing
   spriteMap[Direction::UpRight].Width = 4;
   spriteMap[Direction::UpRight].Height = 3;
   for ( auto line : { " O o",
                       "/|' ",
                       "/ \\ " } ) { for ( int i = 0; i < spriteMap[Direction::UpRight].Width; i++ ) { spriteMap[Direction::UpRight].Pixels.push_back( { line[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); } }

   // facing down-right without firing
   spriteMap[Direction::DownRight].Width = 4;
   spriteMap[Direction::DownRight].Height = 3;
   for ( auto line : { " O  ",
                       "/|. ",
                       "/ \\o" } ) { for ( int i = 0; i < spriteMap[Direction::DownRight].Width; i++ ) { spriteMap[Direction::DownRight].Pixels.push_back( { line[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); } }

   // facing up without firing
   spriteMap[Direction::Up].Width = 4;
   spriteMap[Direction::Up].Height = 3;
   for ( auto line : { " Oo ",
                       "/|' ",
                       "/ \\ " } ) { for ( int i = 0; i < spriteMap[Direction::Up].Width; i++ ) { spriteMap[Direction::Up].Pixels.push_back( { line[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); } }

   // facing down without firing
   spriteMap[Direction::Down].Width = 4;
   spriteMap[Direction::Down].Height = 3;
   for ( auto line : { " O  ",
                       "/|. ",
                       "/`o " } ) { for ( int i = 0; i < spriteMap[Direction::Down].Width; i++ ) { spriteMap[Direction::Down].Pixels.push_back( { line[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); } }

   return spriteMap;
}

map<Direction, ConsoleImage> PlayerSpriteGenerator::GenerateMovingSpriteMap()
{
   // TODO: figure out how to do sprite swapping based on frame count
   return GenerateStaticSpriteMap();
}

shared_ptr<ConsoleSprite> PlayerSpriteGenerator::GeneratePlayerThwipSprite()
{
   auto thwipSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( .05 ) );

   ConsoleImage image0 = { 1, 3 };
   image0.Pixels.push_back( { '|', true, ConsoleColor::Blue, ConsoleColor::Black } );
   image0.Pixels.push_back( { '|', true, ConsoleColor::DarkBlue, ConsoleColor::Black } );
   image0.Pixels.push_back( { '|', true, ConsoleColor::Blue, ConsoleColor::Black } );
   thwipSprite->AddImage( image0 );

   ConsoleImage image1 = { 1, 3 };
   image1.Pixels.push_back( { '|', true, ConsoleColor::DarkBlue, ConsoleColor::Black } );
   image1.Pixels.push_back( { '|', true, ConsoleColor::Blue, ConsoleColor::Black } );
   image1.Pixels.push_back( { '|', true, ConsoleColor::DarkBlue, ConsoleColor::Black } );
   thwipSprite->AddImage( image1 );

   return thwipSprite;
}

shared_ptr<ConsoleSprite> PlayerSpriteGenerator::GenerateExplosionParticleSprite()
{
   auto particleSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( .25 ) );

   ConsoleImage image0 = { 1, 1 };
   image0.Pixels.push_back( { 'O', true, ConsoleColor::Blue, ConsoleColor::Black } );
   particleSprite->AddImage( image0 );

   ConsoleImage image1 = { 1, 1 };
   image1.Pixels.push_back( { 'o', true, ConsoleColor::DarkBlue, ConsoleColor::Black } );
   particleSprite->AddImage( image1 );

   return particleSprite;
}
