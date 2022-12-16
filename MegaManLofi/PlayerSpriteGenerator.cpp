#include <string>

#include "PlayerSpriteGenerator.h"
#include "Direction.h"

using namespace std;
using namespace MegaManLofi;

shared_ptr<ConsoleSprite> PlayerSpriteGenerator::GenerateThwipSprite()
{
   auto thwipSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( .05 ) );

   ConsoleImage image0 = { 1, 3 };
   image0.Pixels.push_back( { '#', true, ConsoleColor::Blue, ConsoleColor::Black } );
   image0.Pixels.push_back( { '#', true, ConsoleColor::DarkBlue, ConsoleColor::Black } );
   image0.Pixels.push_back( { '#', true, ConsoleColor::Blue, ConsoleColor::Black } );
   thwipSprite->AddImage( image0 );

   ConsoleImage image1 = { 1, 3 };
   image1.Pixels.push_back( { '#', true, ConsoleColor::DarkBlue, ConsoleColor::Black } );
   image1.Pixels.push_back( { '#', true, ConsoleColor::Blue, ConsoleColor::Black } );
   image1.Pixels.push_back( { '#', true, ConsoleColor::DarkBlue, ConsoleColor::Black } );
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

map<Direction, shared_ptr<ConsoleSprite>> PlayerSpriteGenerator::GenerateStandingSpriteMap()
{
   auto spriteMap = map<Direction, shared_ptr<ConsoleSprite>>();

   // facing left
   auto leftSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( 0 ) );
   string leftChars =
      "  O " \
      "o-|\\" \
      " / \\";
   ConsoleImage leftImage = { 4, 3 };
   for ( int i = 0; i < (int)leftChars.size(); i++ ) { leftImage.Pixels.push_back( { leftChars[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); }
   leftSprite->AddImage( leftImage );
   spriteMap[Direction::UpLeft] = leftSprite;
   spriteMap[Direction::Left] = leftSprite;
   spriteMap[Direction::DownLeft] = leftSprite;

   // facing right
   auto rightSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( 0 ) );
   string rightChars =
      " O  " \
      "/|-o" \
      "/ \\ ";
   ConsoleImage rightImage = { 4, 3 };
   for ( int i = 0; i < (int)rightChars.size(); i++ ) { rightImage.Pixels.push_back( { rightChars[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); }
   rightSprite->AddImage( rightImage );
   spriteMap[Direction::UpRight] = rightSprite;
   spriteMap[Direction::Right] = rightSprite;
   spriteMap[Direction::DownRight] = rightSprite;

   // facing up or down
   auto verticalSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( 0 ) );
   string verticalChars =
      " O  " \
      "/|\\ " \
      "/ \\ ";
   ConsoleImage verticalImage = { 4, 3 };
   for ( int i = 0; i < (int)verticalChars.size(); i++ ) { verticalImage.Pixels.push_back( { verticalChars[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); }
   verticalSprite->AddImage( verticalImage );
   spriteMap[Direction::Up] = verticalSprite;
   spriteMap[Direction::Down] = verticalSprite;

   return spriteMap;
}

map<Direction, shared_ptr<ConsoleSprite>> PlayerSpriteGenerator::GenerateWalkingSpriteMap()
{
   auto spriteMap = map<Direction, shared_ptr<ConsoleSprite>>();

   // facing left
   auto leftSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( .15 ) );
   string leftChars0 =
      "  O " \
      "o-|\\" \
      " / <";
   string leftChars1 =
      "  O " \
      "o-|\\" \
      "  |\\";
   ConsoleImage leftImage0 = { 4, 3 };
   ConsoleImage leftImage1 = { 4, 3 };
   for ( int i = 0; i < (int)leftChars0.size(); i++ ) { leftImage0.Pixels.push_back( { leftChars0[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); }
   for ( int i = 0; i < (int)leftChars1.size(); i++ ) { leftImage1.Pixels.push_back( { leftChars1[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); }
   leftSprite->AddImage( leftImage0 );
   leftSprite->AddImage( leftImage1 );
   spriteMap[Direction::UpLeft] = leftSprite;
   spriteMap[Direction::Left] = leftSprite;
   spriteMap[Direction::DownLeft] = leftSprite;

   // facing right
   auto rightSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( .15 ) );
   string rightChars0 =
      " O  " \
      "/|-o" \
      "> \\ ";
   string rightChars1 =
      " O  " \
      "/|-o" \
      "/|  ";
   ConsoleImage rightImage0 = { 4, 3 };
   ConsoleImage rightImage1 = { 4, 3 };
   for ( int i = 0; i < (int)rightChars0.size(); i++ ) { rightImage0.Pixels.push_back( { rightChars0[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); }
   for ( int i = 0; i < (int)rightChars1.size(); i++ ) { rightImage1.Pixels.push_back( { rightChars1[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); }
   rightSprite->AddImage( rightImage0 );
   rightSprite->AddImage( rightImage1 );
   spriteMap[Direction::UpRight] = rightSprite;
   spriteMap[Direction::Right] = rightSprite;
   spriteMap[Direction::DownRight] = rightSprite;

   // facing up or down (this will probably very rarely happen)
   auto verticalSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( 0 ) );
   string verticalChars =
      " O  " \
      "/|\\ " \
      "/ \\ ";
   ConsoleImage verticalImage = { 4, 3 };
   for ( int i = 0; i < (int)verticalChars.size(); i++ ) { verticalImage.Pixels.push_back( { verticalChars[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); }
   verticalSprite->AddImage( verticalImage );
   spriteMap[Direction::Up] = verticalSprite;
   spriteMap[Direction::Down] = verticalSprite;

   return spriteMap;
}

map<Direction, shared_ptr<ConsoleSprite>> PlayerSpriteGenerator::GenerateFallingSpriteMap()
{
   auto spriteMap = map<Direction, shared_ptr<ConsoleSprite>>();

   // facing left
   auto leftSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( 0 ) );
   string leftChars =
      "  O/" \
      "o-| " \
      " / \\";
   ConsoleImage leftImage = { 4, 3 };
   for ( int i = 0; i < (int)leftChars.size(); i++ ) { leftImage.Pixels.push_back( { leftChars[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); }
   leftSprite->AddImage( leftImage );
   spriteMap[Direction::UpLeft] = leftSprite;
   spriteMap[Direction::Left] = leftSprite;
   spriteMap[Direction::DownLeft] = leftSprite;

   // facing right
   auto rightSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( 0 ) );
   string rightChars =
      "\\O  " \
      " |-o" \
      "/ \\ ";
   ConsoleImage rightImage = { 4, 3 };
   for ( int i = 0; i < (int)rightChars.size(); i++ ) { rightImage.Pixels.push_back( { rightChars[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); }
   rightSprite->AddImage( rightImage );
   spriteMap[Direction::UpRight] = rightSprite;
   spriteMap[Direction::Right] = rightSprite;
   spriteMap[Direction::DownRight] = rightSprite;

   // facing up or down
   auto verticalSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( 0 ) );
   string verticalChars =
      "\\O/ " \
      " |  " \
      "/ \\ ";
   ConsoleImage verticalImage = { 4, 3 };
   for ( int i = 0; i < (int)verticalChars.size(); i++ ) { verticalImage.Pixels.push_back( { verticalChars[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); }
   verticalSprite->AddImage( verticalImage );
   spriteMap[Direction::Up] = verticalSprite;
   spriteMap[Direction::Down] = verticalSprite;

   return spriteMap;
}
