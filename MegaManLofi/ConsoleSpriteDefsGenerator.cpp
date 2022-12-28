#include <string>

#include "ConsoleSpriteDefsGenerator.h"
#include "ConsoleSpriteDefs.h"
#include "ConsoleSprite.h"
#include "EntityConsoleSprite.h"

using namespace std;
using namespace MegaManLofi;

shared_ptr<ConsoleSpriteDefs> ConsoleSpriteDefsGenerator::GenerateConsoleSpriteDefs( const std::shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto spriteDefs = make_shared<ConsoleSpriteDefs>();

   spriteDefs->PlayerThwipSprite = GeneratePlayerThwipSprite( frameRateProvider );
   spriteDefs->PlayerThwipInTransitionSprite = GeneratePlayerThwipInTransitionSprite( frameRateProvider );
   spriteDefs->PlayerThwipOutTransitionSprite = GeneratePlayerThwipOutTransitionSprite( frameRateProvider );
   spriteDefs->PlayerExplosionParticleSprite = GeneratePlayerExplosionParticleSprite( frameRateProvider );

   spriteDefs->StageGetReadySprite = GenerateStageGetReadySprite( frameRateProvider );

   spriteDefs->MenuCaratSprite = GenerateMenuCaratSprite( frameRateProvider );

   spriteDefs->EntitySpriteMap = GenerateEntitySpriteMap( frameRateProvider );

   return spriteDefs;
}

shared_ptr<IConsoleSprite> ConsoleSpriteDefsGenerator::GeneratePlayerStandingSprite( Direction direction, const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   string chars = "";

   switch ( direction )
   {
      case Direction::DownLeft:
      case Direction::Left:
      case Direction::UpLeft:
         chars =
            "  O " \
            "o-|\\" \
            " / \\";
         break;
      case Direction::UpRight:
      case Direction::Right:
      case Direction::DownRight:
         chars =
            " O  " \
            "/|-o" \
            "/ \\ ";
         break;
      case Direction::Up:
      case Direction::Down:
         chars =
            " O  " \
            "/|\\ " \
            "/ \\ ";
         break;
   }

   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, 0 ) );
   ConsoleImage image = { 4, 3 };
   for ( int i = 0; i < (int)chars.size(); i++ ) { image.Pixels.push_back( { chars[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); }
   sprite->AddImage( image );

   return sprite;
}

shared_ptr<IConsoleSprite> ConsoleSpriteDefsGenerator::GeneratePlayerWalkingSprite( Direction direction, const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   string chars1 = "";
   string chars2 = "";

   switch ( direction )
   {
      case Direction::DownLeft:
      case Direction::Left:
      case Direction::UpLeft:
         chars1 =
            "  O " \
            "o-|\\" \
            " / <";
         chars2 =
            "  O " \
            "o-|\\" \
            "  |\\";
         break;
      case Direction::UpRight:
      case Direction::Right:
      case Direction::DownRight:
         chars1 =
            " O  " \
            "/|-o" \
            "> \\ ";
         chars2 =
            " O  " \
            "/|-o" \
            "/|  ";
         break;
      case Direction::Up:
      case Direction::Down:
         chars1 =
            " O  " \
            "/|\\ " \
            "/ \\ ";
         chars2 =
            " O  " \
            "/|\\ " \
            "/ \\ ";
         break;
   }

   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, .15 ) );
   ConsoleImage image1 = { 4, 3 };
   ConsoleImage image2 = { 4, 3 };
   for ( int i = 0; i < (int)chars1.size(); i++ ) { image1.Pixels.push_back( { chars1[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); }
   for ( int i = 0; i < (int)chars2.size(); i++ ) { image2.Pixels.push_back( { chars2[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); }
   sprite->AddImage( image1 );
   sprite->AddImage( image2 );

   return sprite;
}

shared_ptr<IConsoleSprite> ConsoleSpriteDefsGenerator::GeneratePlayerAirborneSprite( Direction direction, const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   string chars = "";

   switch ( direction )
   {
      case Direction::DownLeft:
      case Direction::Left:
      case Direction::UpLeft:
         chars =
            "  O/" \
            "o-| " \
            " / \\";
         break;
      case Direction::UpRight:
      case Direction::Right:
      case Direction::DownRight:
         chars =
            "\\O  " \
            " |-o" \
            "/ \\ ";
         break;
      case Direction::Up:
      case Direction::Down:
         chars =
            "\\O/ " \
            " |  " \
            "/ \\ ";
         break;
   }

   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, 0 ) );
   ConsoleImage image = { 4, 3 };
   for ( int i = 0; i < (int)chars.size(); i++ ) { image.Pixels.push_back( { chars[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); }
   sprite->AddImage( image );

   return sprite;
}

std::shared_ptr<IConsoleSprite> ConsoleSpriteDefsGenerator::GenerateBulletSprite( const std::shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, 0 ) );
   
   ConsoleImage bulletImage;

   bulletImage.Width = 1;
   bulletImage.Height = 1;
   bulletImage.Pixels.push_back( { 'o', true, ConsoleColor::White, ConsoleColor::Black } );

   sprite->AddImage( bulletImage );

   return sprite;
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

   string chars1 =
      "    " \
      "    " \
      " ## ";
   string chars2 =
      "    " \
      "*  *" \
      " ** ";
   ConsoleImage image1 = { 4, 3 };
   ConsoleImage image2 = { 4, 3 };
   for ( int i = 0; i < (int)chars1.size(); i++ ) { image1.Pixels.push_back( { chars1[i], true, ConsoleColor::Blue, ConsoleColor::Black } ); }
   for ( int i = 0; i < (int)chars2.size(); i++ ) { image2.Pixels.push_back( { chars2[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); }
   sprite->AddImage( image1 );
   sprite->AddImage( image2 );

   return sprite;
}

shared_ptr<IConsoleSprite> ConsoleSpriteDefsGenerator::GeneratePlayerThwipOutTransitionSprite( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, .05 ) );

   string chars1 =
      "    " \
      "*  *" \
      " ** ";
   string chars2 =
      "    " \
      "    " \
      " ## ";
   ConsoleImage image1 = { 4, 3 };
   ConsoleImage image2 = { 4, 3 };
   for ( int i = 0; i < (int)chars1.size(); i++ ) { image1.Pixels.push_back( { chars1[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); }
   for ( int i = 0; i < (int)chars2.size(); i++ ) { image2.Pixels.push_back( { chars2[i], true, ConsoleColor::Blue, ConsoleColor::Black } ); }
   sprite->AddImage( image1 );
   sprite->AddImage( image2 );

   return sprite;
}

shared_ptr<IConsoleSprite> ConsoleSpriteDefsGenerator::GeneratePlayerExplosionParticleSprite( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto particleSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, .25 ) );

   ConsoleImage image1 = { 1, 1 };
   image1.Pixels.push_back( { 'O', true, ConsoleColor::Blue, ConsoleColor::Black } );
   particleSprite->AddImage( image1 );

   ConsoleImage image2 = { 1, 1 };
   image2.Pixels.push_back( { 'o', true, ConsoleColor::DarkBlue, ConsoleColor::Black } );
   particleSprite->AddImage( image2 );

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

   ConsoleImage image1 = { 1, 1 };
   image1.Pixels.push_back( { '>', true, ConsoleColor::White, ConsoleColor::Black } );
   sprite->AddImage( image1 );

   ConsoleImage image2 = { 1, 1 };
   image2.Pixels.push_back( { '>', true, ConsoleColor::DarkGrey, ConsoleColor::Black } );
   sprite->AddImage( image2 );

   return sprite;
}

map<int, shared_ptr<IEntityConsoleSprite>> ConsoleSpriteDefsGenerator::GenerateEntitySpriteMap( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto entitySpriteMap = map<int, shared_ptr<IEntityConsoleSprite>>();

   auto playerEntitySprite = make_shared<EntityConsoleSprite>();

   playerEntitySprite->AddSprite( MovementType::Standing, Direction::Left, GeneratePlayerStandingSprite( Direction::Left, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::Standing, Direction::UpLeft, GeneratePlayerStandingSprite( Direction::UpLeft, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::Standing, Direction::Up, GeneratePlayerStandingSprite( Direction::Up, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::Standing, Direction::UpRight, GeneratePlayerStandingSprite( Direction::UpRight, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::Standing, Direction::Right, GeneratePlayerStandingSprite( Direction::Right, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::Standing, Direction::DownRight, GeneratePlayerStandingSprite( Direction::DownRight, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::Standing, Direction::Down, GeneratePlayerStandingSprite( Direction::Down, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::Standing, Direction::DownLeft, GeneratePlayerStandingSprite( Direction::DownLeft, frameRateProvider ) );

   playerEntitySprite->AddSprite( MovementType::Walking, Direction::Left, GeneratePlayerWalkingSprite( Direction::Left, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::Walking, Direction::UpLeft, GeneratePlayerWalkingSprite( Direction::UpLeft, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::Walking, Direction::Up, GeneratePlayerWalkingSprite( Direction::Up, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::Walking, Direction::UpRight, GeneratePlayerWalkingSprite( Direction::UpRight, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::Walking, Direction::Right, GeneratePlayerWalkingSprite( Direction::Right, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::Walking, Direction::DownRight, GeneratePlayerWalkingSprite( Direction::DownRight, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::Walking, Direction::Down, GeneratePlayerWalkingSprite( Direction::Down, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::Walking, Direction::DownLeft, GeneratePlayerWalkingSprite( Direction::DownLeft, frameRateProvider ) );

   playerEntitySprite->AddSprite( MovementType::Airborne, Direction::Left, GeneratePlayerAirborneSprite( Direction::Left, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::Airborne, Direction::UpLeft, GeneratePlayerAirborneSprite( Direction::UpLeft, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::Airborne, Direction::Up, GeneratePlayerAirborneSprite( Direction::Up, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::Airborne, Direction::UpRight, GeneratePlayerAirborneSprite( Direction::UpRight, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::Airborne, Direction::Right, GeneratePlayerAirborneSprite( Direction::Right, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::Airborne, Direction::DownRight, GeneratePlayerAirborneSprite( Direction::DownRight, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::Airborne, Direction::Down, GeneratePlayerAirborneSprite( Direction::Down, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::Airborne, Direction::DownLeft, GeneratePlayerAirborneSprite( Direction::DownLeft, frameRateProvider ) );

   entitySpriteMap[0] = playerEntitySprite;

   auto bulletSprite = GenerateBulletSprite( frameRateProvider );
   auto bulletEntitySprite = make_shared<EntityConsoleSprite>();

   bulletEntitySprite->AddSprite( MovementType::Standing, Direction::Left, bulletSprite );
   bulletEntitySprite->AddSprite( MovementType::Standing, Direction::UpLeft, bulletSprite );
   bulletEntitySprite->AddSprite( MovementType::Standing, Direction::Up, bulletSprite );
   bulletEntitySprite->AddSprite( MovementType::Standing, Direction::UpRight, bulletSprite );
   bulletEntitySprite->AddSprite( MovementType::Standing, Direction::Right, bulletSprite );
   bulletEntitySprite->AddSprite( MovementType::Standing, Direction::DownRight, bulletSprite );
   bulletEntitySprite->AddSprite( MovementType::Standing, Direction::Down, bulletSprite );
   bulletEntitySprite->AddSprite( MovementType::Standing, Direction::DownLeft, bulletSprite );

   bulletEntitySprite->AddSprite( MovementType::Walking, Direction::Left, bulletSprite );
   bulletEntitySprite->AddSprite( MovementType::Walking, Direction::UpLeft, bulletSprite );
   bulletEntitySprite->AddSprite( MovementType::Walking, Direction::Up, bulletSprite );
   bulletEntitySprite->AddSprite( MovementType::Walking, Direction::UpRight, bulletSprite );
   bulletEntitySprite->AddSprite( MovementType::Walking, Direction::Right, bulletSprite );
   bulletEntitySprite->AddSprite( MovementType::Walking, Direction::DownRight, bulletSprite );
   bulletEntitySprite->AddSprite( MovementType::Walking, Direction::Down, bulletSprite );
   bulletEntitySprite->AddSprite( MovementType::Walking, Direction::DownLeft, bulletSprite );

   bulletEntitySprite->AddSprite( MovementType::Airborne, Direction::Left, bulletSprite );
   bulletEntitySprite->AddSprite( MovementType::Airborne, Direction::UpLeft, bulletSprite );
   bulletEntitySprite->AddSprite( MovementType::Airborne, Direction::Up, bulletSprite );
   bulletEntitySprite->AddSprite( MovementType::Airborne, Direction::UpRight, bulletSprite );
   bulletEntitySprite->AddSprite( MovementType::Airborne, Direction::Right, bulletSprite );
   bulletEntitySprite->AddSprite( MovementType::Airborne, Direction::DownRight, bulletSprite );
   bulletEntitySprite->AddSprite( MovementType::Airborne, Direction::Down, bulletSprite );
   bulletEntitySprite->AddSprite( MovementType::Airborne, Direction::DownLeft, bulletSprite );

   entitySpriteMap[1] = bulletEntitySprite;

   return entitySpriteMap;
}
