#include <string>

#include "ConsoleSpriteDefsGenerator.h"
#include "ConsoleSpriteDefs.h"
#include "ConsoleSprite.h"
#include "EntityConsoleSprite.h"
#include "GameDefGeneratorDefines.h"

using namespace std;
using namespace MegaManLofi;

shared_ptr<ConsoleSpriteDefs> ConsoleSpriteDefsGenerator::GenerateConsoleSpriteDefs( const shared_ptr<IFrameRateProvider> frameRateProvider )
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

shared_ptr<ConsoleSprite> ConsoleSpriteDefsGenerator::GeneratePlayerStandingSprite( Direction direction, const shared_ptr<IFrameRateProvider> frameRateProvider )
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
   for ( int i = 0; i < (int)chars.size(); i++ ) { image.Pixels.push_back( { chars[i], true, ConsoleColor::Blue, ConsoleColor::Black } ); }
   sprite->AddImage( image );

   return sprite;
}

shared_ptr<ConsoleSprite> ConsoleSpriteDefsGenerator::GeneratePlayerWalkingSprite( Direction direction, const shared_ptr<IFrameRateProvider> frameRateProvider )
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

   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, .15f ) );
   ConsoleImage image1 = { 4, 3 };
   ConsoleImage image2 = { 4, 3 };
   for ( int i = 0; i < (int)chars1.size(); i++ ) { image1.Pixels.push_back( { chars1[i], true, ConsoleColor::Blue, ConsoleColor::Black } ); }
   for ( int i = 0; i < (int)chars2.size(); i++ ) { image2.Pixels.push_back( { chars2[i], true, ConsoleColor::Blue, ConsoleColor::Black } ); }
   sprite->AddImage( image1 );
   sprite->AddImage( image2 );

   return sprite;
}

shared_ptr<ConsoleSprite> ConsoleSpriteDefsGenerator::GeneratePlayerAirborneSprite( Direction direction, const shared_ptr<IFrameRateProvider> frameRateProvider )
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
   for ( int i = 0; i < (int)chars.size(); i++ ) { image.Pixels.push_back( { chars[i], true, ConsoleColor::Blue, ConsoleColor::Black } ); }
   sprite->AddImage( image );

   return sprite;
}

shared_ptr<ConsoleSprite> ConsoleSpriteDefsGenerator::GeneratePlayerKnockBackSprite( Direction direction, const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   string chars1 = "";
   string chars2 = "";
   ConsoleImage image1 = { 4, 3 };
   ConsoleImage image2 = { 4, 3 };

   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, 0.1f ) );

   switch ( direction )
   {
      case Direction::UpRight:
      case Direction::Right:
      case Direction::DownRight:
         chars1 =
            "0*o " \
            "|\\`*" \
            "*/*\\";
         chars2 =
            "0 o " \
            "|\\` " \
            " / \\";
         for ( int i = 0; i < (int)chars1.size(); i++ ) { image1.Pixels.push_back( { chars1[i], true, ConsoleColor::DarkBlue, ConsoleColor::Black } ); }
         image1.Pixels[1].ForegroundColor = ConsoleColor::Red;
         image1.Pixels[7].ForegroundColor = ConsoleColor::Red;
         image1.Pixels[8].ForegroundColor = ConsoleColor::Red;
         image1.Pixels[10].ForegroundColor = ConsoleColor::Red;
         for ( int i = 0; i < (int)chars2.size(); i++ ) { image2.Pixels.push_back( { chars2[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); }
         break;
      default:
         chars1 =
            " o*0" \
            "*`/|" \
            "/*\\*";
         chars2 =
            " o 0" \
            " `/|" \
            "/ \\ ";
         for ( int i = 0; i < (int)chars1.size(); i++ ) { image1.Pixels.push_back( { chars1[i], true, ConsoleColor::DarkBlue, ConsoleColor::Black } ); }
         image1.Pixels[2].ForegroundColor = ConsoleColor::Red;
         image1.Pixels[4].ForegroundColor = ConsoleColor::Red;
         image1.Pixels[9].ForegroundColor = ConsoleColor::Red;
         image1.Pixels[11].ForegroundColor = ConsoleColor::Red;
         for ( int i = 0; i < (int)chars2.size(); i++ ) { image2.Pixels.push_back( { chars2[i], true, ConsoleColor::Cyan, ConsoleColor::Black } ); }
         break;
   }

   sprite->AddImage( image1 );
   sprite->AddImage( image2 );

   return sprite;
}

shared_ptr<ConsoleSprite> ConsoleSpriteDefsGenerator::GenerateBadDudeStandingSprite( Direction direction, const shared_ptr<IFrameRateProvider> frameRateProvider )
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
   for ( int i = 0; i < (int)chars.size(); i++ ) { image.Pixels.push_back( { chars[i], true, ConsoleColor::Magenta, ConsoleColor::Black } ); }
   sprite->AddImage( image );

   return sprite;
}

shared_ptr<ConsoleSprite> ConsoleSpriteDefsGenerator::GenerateBadDudeWalkingSprite( Direction direction, const shared_ptr<IFrameRateProvider> frameRateProvider )
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

   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, .15f ) );
   ConsoleImage image1 = { 4, 3 };
   ConsoleImage image2 = { 4, 3 };
   for ( int i = 0; i < (int)chars1.size(); i++ ) { image1.Pixels.push_back( { chars1[i], true, ConsoleColor::Magenta, ConsoleColor::Black } ); }
   for ( int i = 0; i < (int)chars2.size(); i++ ) { image2.Pixels.push_back( { chars2[i], true, ConsoleColor::Magenta, ConsoleColor::Black } ); }
   sprite->AddImage( image1 );
   sprite->AddImage( image2 );

   return sprite;
}

shared_ptr<ConsoleSprite> ConsoleSpriteDefsGenerator::GenerateBadDudeAirborneSprite( Direction direction, const shared_ptr<IFrameRateProvider> frameRateProvider )
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
   for ( int i = 0; i < (int)chars.size(); i++ ) { image.Pixels.push_back( { chars[i], true, ConsoleColor::Magenta, ConsoleColor::Black } ); }
   sprite->AddImage( image );

   return sprite;
}

shared_ptr<ConsoleSprite> ConsoleSpriteDefsGenerator::GenerateBulletSprite( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, 0 ) );
   
   ConsoleImage bulletImage;

   bulletImage.Width = 1;
   bulletImage.Height = 1;
   bulletImage.Pixels.push_back( { 'o', true, ConsoleColor::Yellow, ConsoleColor::Black } );

   sprite->AddImage( bulletImage );

   return sprite;
}

shared_ptr<ConsoleSprite> ConsoleSpriteDefsGenerator::GenerateSmallHealthDropSprite( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, 0.25f ) );

   ConsoleImage image0;
   ConsoleImage image1;

   image0.Width = 1;
   image0.Height = 1;
   image0.Pixels.push_back( { '.', true, ConsoleColor::White, ConsoleColor::Black } );

   image1.Width = 1;
   image1.Height = 1;
   image1.Pixels.push_back( { '.', true, ConsoleColor::Grey, ConsoleColor::Black } );

   sprite->AddImage( image0 );
   sprite->AddImage( image1 );

   return sprite;
}

shared_ptr<ConsoleSprite> ConsoleSpriteDefsGenerator::GenerateLargeHealthDropSprite( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, 0.25f ) );

   ConsoleImage image0;
   ConsoleImage image1;

   image0.Width = 1;
   image0.Height = 1;
   image0.Pixels.push_back( { 'O', true, ConsoleColor::White, ConsoleColor::Black } );

   image1.Width = 1;
   image1.Height = 1;
   image1.Pixels.push_back( { 'O', true, ConsoleColor::Grey, ConsoleColor::Black } );

   sprite->AddImage( image0 );
   sprite->AddImage( image1 );

   return sprite;
}

shared_ptr<ConsoleSprite> ConsoleSpriteDefsGenerator::GenerateExtraLifeSprite( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, 0.25f ) );

   ConsoleImage image0;
   ConsoleImage image1;

   image0.Width = 1;
   image0.Height = 1;
   image0.Pixels.push_back( { '@', true, ConsoleColor::Blue, ConsoleColor::Black } );

   image1.Width = 1;
   image1.Height = 1;
   image1.Pixels.push_back( { '@', true, ConsoleColor::DarkBlue, ConsoleColor::Black } );

   sprite->AddImage( image0 );
   sprite->AddImage( image1 );

   return sprite;
}

shared_ptr<ConsoleSprite> ConsoleSpriteDefsGenerator::GenerateStationaryTurretSprite( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, 0.50f ) );

   ConsoleImage image0;
   ConsoleImage image1;

   image0.Width = 1;
   image0.Height = 4;
   image0.Pixels.push_back( { '%', true, ConsoleColor::DarkMagenta, ConsoleColor::Black } );
   image0.Pixels.push_back( { '|', true, ConsoleColor::DarkGrey, ConsoleColor::Black } );
   image0.Pixels.push_back( { '|', true, ConsoleColor::DarkGrey, ConsoleColor::Black } );
   image0.Pixels.push_back( { 'X', true, ConsoleColor::DarkGrey, ConsoleColor::Black } );

   image1.Width = 1;
   image1.Height = 4;
   image1.Pixels.push_back( { '%', true, ConsoleColor::Red, ConsoleColor::Black } );
   image1.Pixels.push_back( { '|', true, ConsoleColor::DarkGrey, ConsoleColor::Black } );
   image1.Pixels.push_back( { '|', true, ConsoleColor::DarkGrey, ConsoleColor::Black } );
   image1.Pixels.push_back( { 'X', true, ConsoleColor::DarkGrey, ConsoleColor::Black } );

   sprite->AddImage( image0 );
   sprite->AddImage( image1 );

   return sprite;
}

shared_ptr<ConsoleSprite> ConsoleSpriteDefsGenerator::GenerateSpinningTurretSprite( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, 0.50f ) );

   ConsoleImage image0;
   ConsoleImage image1;

   image0.Width = 1;
   image0.Height = 1;
   image0.Pixels.push_back( { '@', true, ConsoleColor::DarkMagenta, ConsoleColor::Black } );

   image1.Width = 1;
   image1.Height = 1;
   image1.Pixels.push_back( { '@', true, ConsoleColor::Red, ConsoleColor::Black } );

   sprite->AddImage( image0 );
   sprite->AddImage( image1 );

   return sprite;
}

shared_ptr<ConsoleSprite> ConsoleSpriteDefsGenerator::GeneratePlayerThwipSprite( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto thwipSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, .05f ) );

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

shared_ptr<ConsoleSprite> ConsoleSpriteDefsGenerator::GeneratePlayerThwipInTransitionSprite( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, .05f ) );

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

shared_ptr<ConsoleSprite> ConsoleSpriteDefsGenerator::GeneratePlayerThwipOutTransitionSprite( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, .05f ) );

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

shared_ptr<ConsoleSprite> ConsoleSpriteDefsGenerator::GeneratePlayerExplosionParticleSprite( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto particleSprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, .25f ) );

   ConsoleImage image1 = { 1, 1 };
   image1.Pixels.push_back( { 'O', true, ConsoleColor::Blue, ConsoleColor::Black } );
   particleSprite->AddImage( image1 );

   ConsoleImage image2 = { 1, 1 };
   image2.Pixels.push_back( { 'o', true, ConsoleColor::DarkBlue, ConsoleColor::Black } );
   particleSprite->AddImage( image2 );

   return particleSprite;
}

shared_ptr<ConsoleSprite> ConsoleSpriteDefsGenerator::GenerateStageGetReadySprite( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, .25f ) );

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

shared_ptr<ConsoleSprite> ConsoleSpriteDefsGenerator::GenerateMenuCaratSprite( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto sprite = shared_ptr<ConsoleSprite>( new ConsoleSprite( frameRateProvider, .25f ) );

   ConsoleImage image1 = { 1, 1 };
   image1.Pixels.push_back( { '>', true, ConsoleColor::White, ConsoleColor::Black } );
   sprite->AddImage( image1 );

   ConsoleImage image2 = { 1, 1 };
   image2.Pixels.push_back( { '>', true, ConsoleColor::DarkGrey, ConsoleColor::Black } );
   sprite->AddImage( image2 );

   return sprite;
}

map<int, shared_ptr<EntityConsoleSprite>> ConsoleSpriteDefsGenerator::GenerateEntitySpriteMap( const shared_ptr<IFrameRateProvider> frameRateProvider )
{
   auto entitySpriteMap = map<int, shared_ptr<EntityConsoleSprite>>();

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

   playerEntitySprite->AddSprite( MovementType::KnockBack, Direction::Left, GeneratePlayerKnockBackSprite( Direction::Left, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::KnockBack, Direction::UpLeft, GeneratePlayerKnockBackSprite( Direction::UpLeft, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::KnockBack, Direction::Up, GeneratePlayerKnockBackSprite( Direction::Up, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::KnockBack, Direction::UpRight, GeneratePlayerKnockBackSprite( Direction::UpRight, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::KnockBack, Direction::Right, GeneratePlayerKnockBackSprite( Direction::Right, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::KnockBack, Direction::DownRight, GeneratePlayerKnockBackSprite( Direction::DownRight, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::KnockBack, Direction::Down, GeneratePlayerKnockBackSprite( Direction::Down, frameRateProvider ) );
   playerEntitySprite->AddSprite( MovementType::KnockBack, Direction::DownLeft, GeneratePlayerKnockBackSprite( Direction::DownLeft, frameRateProvider ) );

   entitySpriteMap[METAID_PLAYER] = playerEntitySprite;

   // bullet
   auto bulletSprite = GenerateBulletSprite( frameRateProvider );
   auto bulletEntitySprite = make_shared<EntityConsoleSprite>();
   for ( int i = 0; i < (int)MovementType::MovementTypeCount; i++ )
   {
      for ( int j = 0; j < (int)Direction::DirectionCount; j++ )
      {
         bulletEntitySprite->AddSprite( (MovementType)i, (Direction)j, bulletSprite );
      }
   }
   entitySpriteMap[METAID_PROJECTILE_GOODBULLET] = bulletEntitySprite;
   entitySpriteMap[METAID_PROJECTILE_BADBULLET] = bulletEntitySprite;

   // small health drop
   auto smallHealthDropSprite = GenerateSmallHealthDropSprite( frameRateProvider );
   auto smallHealthDropEntitySprite = make_shared<EntityConsoleSprite>();
   for ( int i = 0; i < (int)MovementType::MovementTypeCount; i++ )
   {
      for ( int j = 0; j < (int)Direction::DirectionCount; j++ )
      {
         smallHealthDropEntitySprite->AddSprite( (MovementType)i, (Direction)j, smallHealthDropSprite );
      }
   }
   entitySpriteMap[METAID_ITEM_SMALLHEALTHDROP] = smallHealthDropEntitySprite;

   // large health drop
   auto largeHealthDropSprite = GenerateLargeHealthDropSprite( frameRateProvider );
   auto largeHealthDropEntitySprite = make_shared<EntityConsoleSprite>();
   for ( int i = 0; i < (int)MovementType::MovementTypeCount; i++ )
   {
      for ( int j = 0; j < (int)Direction::DirectionCount; j++ )
      {
         largeHealthDropEntitySprite->AddSprite( (MovementType)i, (Direction)j, largeHealthDropSprite );
      }
   }
   entitySpriteMap[METAID_ITEM_LARGEHEALTHDROP] = largeHealthDropEntitySprite;

   // extra life
   auto extraLifeSprite = GenerateExtraLifeSprite( frameRateProvider );
   auto extraLifeEntitySprite = make_shared<EntityConsoleSprite>();
   for ( int i = 0; i < (int)MovementType::MovementTypeCount; i++ )
   {
      for ( int j = 0; j < (int)Direction::DirectionCount; j++ )
      {
         extraLifeEntitySprite->AddSprite( (MovementType)i, (Direction)j, extraLifeSprite );
      }
   }
   entitySpriteMap[METAID_ITEM_EXTRALIFE] = extraLifeEntitySprite;

   // stationary turret
   auto stationaryTurretSprite = GenerateStationaryTurretSprite( frameRateProvider );
   auto stationaryTurretEntitySprite = make_shared<EntityConsoleSprite>();
   for ( int i = 0; i < (int)MovementType::MovementTypeCount; i++ )
   {
      for ( int j = 0; j < (int)Direction::DirectionCount; j++ )
      {
         stationaryTurretEntitySprite->AddSprite( (MovementType)i, (Direction)j, stationaryTurretSprite );
      }
   }
   entitySpriteMap[METAID_ENEMY_STATIONARYTURRET] = stationaryTurretEntitySprite;

   // spinning turret
   auto spinningTurretSprite = GenerateSpinningTurretSprite( frameRateProvider );
   auto spinningTurretEntitySprite = make_shared<EntityConsoleSprite>();
   for ( int i = 0; i < (int)MovementType::MovementTypeCount; i++ )
   {
      for ( int j = 0; j < (int)Direction::DirectionCount; j++ )
      {
         spinningTurretEntitySprite->AddSprite( (MovementType)i, (Direction)j, spinningTurretSprite );
      }
   }
   entitySpriteMap[METAID_ENEMY_SPINNINGTURRET] = spinningTurretEntitySprite;

   // targeting turret
   entitySpriteMap[METAID_ENEMY_TARGETINGTURRET] = spinningTurretEntitySprite;

   // bad dude
   auto badDudeEntitySprite = make_shared<EntityConsoleSprite>();

   badDudeEntitySprite->AddSprite( MovementType::Standing, Direction::Left, GenerateBadDudeStandingSprite( Direction::Left, frameRateProvider ) );
   badDudeEntitySprite->AddSprite( MovementType::Standing, Direction::UpLeft, GenerateBadDudeStandingSprite( Direction::UpLeft, frameRateProvider ) );
   badDudeEntitySprite->AddSprite( MovementType::Standing, Direction::Up, GenerateBadDudeStandingSprite( Direction::Up, frameRateProvider ) );
   badDudeEntitySprite->AddSprite( MovementType::Standing, Direction::UpRight, GenerateBadDudeStandingSprite( Direction::UpRight, frameRateProvider ) );
   badDudeEntitySprite->AddSprite( MovementType::Standing, Direction::Right, GenerateBadDudeStandingSprite( Direction::Right, frameRateProvider ) );
   badDudeEntitySprite->AddSprite( MovementType::Standing, Direction::DownRight, GenerateBadDudeStandingSprite( Direction::DownRight, frameRateProvider ) );
   badDudeEntitySprite->AddSprite( MovementType::Standing, Direction::Down, GenerateBadDudeStandingSprite( Direction::Down, frameRateProvider ) );
   badDudeEntitySprite->AddSprite( MovementType::Standing, Direction::DownLeft, GenerateBadDudeStandingSprite( Direction::DownLeft, frameRateProvider ) );

   badDudeEntitySprite->AddSprite( MovementType::Walking, Direction::Left, GenerateBadDudeWalkingSprite( Direction::Left, frameRateProvider ) );
   badDudeEntitySprite->AddSprite( MovementType::Walking, Direction::UpLeft, GenerateBadDudeWalkingSprite( Direction::UpLeft, frameRateProvider ) );
   badDudeEntitySprite->AddSprite( MovementType::Walking, Direction::Up, GenerateBadDudeWalkingSprite( Direction::Up, frameRateProvider ) );
   badDudeEntitySprite->AddSprite( MovementType::Walking, Direction::UpRight, GenerateBadDudeWalkingSprite( Direction::UpRight, frameRateProvider ) );
   badDudeEntitySprite->AddSprite( MovementType::Walking, Direction::Right, GenerateBadDudeWalkingSprite( Direction::Right, frameRateProvider ) );
   badDudeEntitySprite->AddSprite( MovementType::Walking, Direction::DownRight, GenerateBadDudeWalkingSprite( Direction::DownRight, frameRateProvider ) );
   badDudeEntitySprite->AddSprite( MovementType::Walking, Direction::Down, GenerateBadDudeWalkingSprite( Direction::Down, frameRateProvider ) );
   badDudeEntitySprite->AddSprite( MovementType::Walking, Direction::DownLeft, GenerateBadDudeWalkingSprite( Direction::DownLeft, frameRateProvider ) );

   badDudeEntitySprite->AddSprite( MovementType::Airborne, Direction::Left, GenerateBadDudeAirborneSprite( Direction::Left, frameRateProvider ) );
   badDudeEntitySprite->AddSprite( MovementType::Airborne, Direction::UpLeft, GenerateBadDudeAirborneSprite( Direction::UpLeft, frameRateProvider ) );
   badDudeEntitySprite->AddSprite( MovementType::Airborne, Direction::Up, GenerateBadDudeAirborneSprite( Direction::Up, frameRateProvider ) );
   badDudeEntitySprite->AddSprite( MovementType::Airborne, Direction::UpRight, GenerateBadDudeAirborneSprite( Direction::UpRight, frameRateProvider ) );
   badDudeEntitySprite->AddSprite( MovementType::Airborne, Direction::Right, GenerateBadDudeAirborneSprite( Direction::Right, frameRateProvider ) );
   badDudeEntitySprite->AddSprite( MovementType::Airborne, Direction::DownRight, GenerateBadDudeAirborneSprite( Direction::DownRight, frameRateProvider ) );
   badDudeEntitySprite->AddSprite( MovementType::Airborne, Direction::Down, GenerateBadDudeAirborneSprite( Direction::Down, frameRateProvider ) );
   badDudeEntitySprite->AddSprite( MovementType::Airborne, Direction::DownLeft, GenerateBadDudeAirborneSprite( Direction::DownLeft, frameRateProvider ) );

   entitySpriteMap[METAID_ENEMY_BADDUDE] = badDudeEntitySprite;

   return entitySpriteMap;
}
