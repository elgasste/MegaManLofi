#include <string>

#include "ConsoleSpriteDefsGenerator.h"
#include "ConsoleSpriteDefs.h"
#include "ConsoleSprite.h"

using namespace std;
using namespace MegaManLofi;

shared_ptr<ConsoleSpriteDefs> ConsoleSpriteDefsGenerator::GenerateConsoleSpriteDefs( const std::shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto spriteDefs = make_shared<ConsoleSpriteDefs>();

   spriteDefs->PlayerStandingSpriteMap = GeneratePlayerStandingSpriteMap( frameRateProvider );
   spriteDefs->PlayerWalkingSpriteMap = GeneratePlayerWalkingSpriteMap( frameRateProvider );
   spriteDefs->PlayerAirborneSpriteMap = GeneratePlayerAirborneSpriteMap( frameRateProvider );
   spriteDefs->PlayerThwipSprite = GeneratePlayerThwipSprite( frameRateProvider );
   spriteDefs->PlayerThwipInTransitionSprite = GeneratePlayerThwipInTransitionSprite( frameRateProvider );
   spriteDefs->PlayerThwipOutTransitionSprite = GeneratePlayerThwipOutTransitionSprite( frameRateProvider );
   spriteDefs->PlayerExplosionParticleSprite = GeneratePlayerExplosionParticleSprite( frameRateProvider );

   spriteDefs->StageGetReadySprite = GenerateStageGetReadySprite( frameRateProvider );

   spriteDefs->MenuCaratSprite = GenerateMenuCaratSprite( frameRateProvider );

   spriteDefs->EntitySpriteMap = GenerateEntitySpriteMap( frameRateProvider );

   return spriteDefs;
}

map<Direction, shared_ptr<IConsoleSprite>> ConsoleSpriteDefsGenerator::GeneratePlayerStandingSpriteMap( const shared_ptr<IFrameRateProvider> frameRateProvider )
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

map<Direction, shared_ptr<IConsoleSprite>> ConsoleSpriteDefsGenerator::GeneratePlayerWalkingSpriteMap( const shared_ptr<IFrameRateProvider> frameRateProvider )
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

map<Direction, shared_ptr<IConsoleSprite>> ConsoleSpriteDefsGenerator::GeneratePlayerAirborneSpriteMap( const shared_ptr<IFrameRateProvider> frameRateProvider )
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

shared_ptr<IConsoleSprite> ConsoleSpriteDefsGenerator::GeneratePlayerThwipSprite( const shared_ptr<IFrameRateProvider> frameRateProvider )
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

shared_ptr<IConsoleSprite> ConsoleSpriteDefsGenerator::GeneratePlayerThwipInTransitionSprite( const shared_ptr<IFrameRateProvider> frameRateProvider )
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

shared_ptr<IConsoleSprite> ConsoleSpriteDefsGenerator::GeneratePlayerThwipOutTransitionSprite( const shared_ptr<IFrameRateProvider> frameRateProvider )
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

shared_ptr<IConsoleSprite> ConsoleSpriteDefsGenerator::GeneratePlayerExplosionParticleSprite( const shared_ptr<IFrameRateProvider> frameRateProvider )
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

shared_ptr<IConsoleSprite> ConsoleSpriteDefsGenerator::GenerateStageGetReadySprite( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, .25 ) );

   ConsoleImage getReadyImage = { 10, 1 };
   string message = "GET READY!";
   for ( int i = 0; i < (int)message.size(); i++ )
   {
      getReadyImage.Pixels.push_back( { message[i], false, ConsoleColor::Cyan, ConsoleColor::Black } );
   }

   sprite->AddImage( getReadyImage );
   sprite->AddImage( { 0, 0 } );

   return sprite;
}

shared_ptr<IConsoleSprite> ConsoleSpriteDefsGenerator::GenerateMenuCaratSprite( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, .25 ) );

   ConsoleImage image0 = { 1, 1 };
   image0.Pixels.push_back( { '>', true, ConsoleColor::White, ConsoleColor::Black } );
   sprite->AddImage( image0 );

   ConsoleImage image1 = { 1, 1 };
   image1.Pixels.push_back( { '>', true, ConsoleColor::DarkGrey, ConsoleColor::Black } );
   sprite->AddImage( image1 );

   return sprite;
}

map<int, shared_ptr<IConsoleSprite>> ConsoleSpriteDefsGenerator::GenerateEntitySpriteMap( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto bulletSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, 0 ) );

   ConsoleImage bulletImage;

   bulletImage.Width = 1;
   bulletImage.Height = 1;
   bulletImage.Pixels.push_back( { 'o', true, ConsoleColor::White, ConsoleColor::Black } );

   bulletSprite->AddImage( bulletImage );

   return
   {
      { 1, bulletSprite}
   };
}
