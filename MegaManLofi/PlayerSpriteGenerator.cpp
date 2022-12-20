#include <string>

#include "PlayerSpriteGenerator.h"
#include "ConsoleSprite.h"
#include "Direction.h"

using namespace std;
using namespace MegaManLofi;

shared_ptr<IConsoleSprite> PlayerSpriteGenerator::GenerateThwipSprite( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto thwipSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, .05 ) );

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

shared_ptr<IConsoleSprite> PlayerSpriteGenerator::GenerateThwipInTransitionSprite( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, .05 ) );

   string chars0 =
      "    " \
      "    " \
      " ## ";
   string chars1 =
      "    " \
      "*  *" \
      " ** ";
   ConsoleImage image0 = { 4, 3 };
   ConsoleImage image1 = { 4, 3 };
   for ( int i = 0; i < (int)chars0.size(); i++ ) { image0.Pixels.push_back( { chars0[i], true, ConsoleColor::Blue, ConsoleColor::Black } ); }
   for ( int i = 0; i < (int)chars1.size(); i++ ) { image1.Pixels.push_back( { chars1[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); }
   sprite->AddImage( image0 );
   sprite->AddImage( image1 );

   return sprite;
}

shared_ptr<IConsoleSprite> PlayerSpriteGenerator::GenerateThwipOutTransitionSprite( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, .05 ) );

   string chars0 =
      "    " \
      "*  *" \
      " ** ";
   string chars1 =
      "    " \
      "    " \
      " ## ";
   ConsoleImage image0 = { 4, 3 };
   ConsoleImage image1 = { 4, 3 };
   for ( int i = 0; i < (int)chars0.size(); i++ ) { image0.Pixels.push_back( { chars0[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); }
   for ( int i = 0; i < (int)chars1.size(); i++ ) { image1.Pixels.push_back( { chars1[i], true, ConsoleColor::Blue, ConsoleColor::Black } ); }
   sprite->AddImage( image0 );
   sprite->AddImage( image1 );

   return sprite;
}

shared_ptr<IConsoleSprite> PlayerSpriteGenerator::GenerateExplosionParticleSprite( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto particleSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, .25 ) );

   ConsoleImage image0 = { 1, 1 };
   image0.Pixels.push_back( { 'O', true, ConsoleColor::Blue, ConsoleColor::Black } );
   particleSprite->AddImage( image0 );

   ConsoleImage image1 = { 1, 1 };
   image1.Pixels.push_back( { 'o', true, ConsoleColor::DarkBlue, ConsoleColor::Black } );
   particleSprite->AddImage( image1 );

   return particleSprite;
}

map<Direction, shared_ptr<IConsoleSprite>> PlayerSpriteGenerator::GenerateStandingSpriteMap( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto spriteMap = map<Direction, shared_ptr<IConsoleSprite>>();

   // facing left
   auto leftSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, 0 ) );
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
   auto rightSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, 0 ) );
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
   auto verticalSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, 0 ) );
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

map<Direction, shared_ptr<IConsoleSprite>> PlayerSpriteGenerator::GenerateWalkingSpriteMap( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto spriteMap = map<Direction, shared_ptr<IConsoleSprite>>();

   // facing left
   auto leftSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, .15 ) );
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
   auto rightSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, .15 ) );
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
   auto verticalSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, 0 ) );
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

map<Direction, shared_ptr<IConsoleSprite>> PlayerSpriteGenerator::GenerateFallingSpriteMap( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto spriteMap = map<Direction, shared_ptr<IConsoleSprite>>();

   // facing left
   auto leftSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, 0 ) );
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
   auto rightSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, 0 ) );
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
   auto verticalSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, 0 ) );
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
