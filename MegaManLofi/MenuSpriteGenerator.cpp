#include "MenuSpriteGenerator.h"
#include "ConsoleSprite.h"

using namespace std;
using namespace MegaManLofi;

shared_ptr<IConsoleSprite> MenuSpriteGenerator::GenerateMenuCaratSprite( const shared_ptr<IFrameRateProvider> frameRateProvider )
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
