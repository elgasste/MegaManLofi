#include <string>
#include <format>

#include "TitleStateConsoleRenderer.h"
#include "IConsoleBuffer.h"
#include "IRandom.h"
#include "IFrameRateProvider.h"
#include "ConsoleRenderConfig.h"
#include "KeyboardInputConfig.h"
#include "ConsoleColor.h"

using namespace std;
using namespace MegaManLofi;

TitleStateConsoleRenderer::TitleStateConsoleRenderer( const shared_ptr<IConsoleBuffer> consoleBuffer,
                                                      const shared_ptr<IRandom> random,
                                                      const shared_ptr<IFrameRateProvider> frameRateProvider,
                                                      const shared_ptr<ConsoleRenderConfig> renderConfig,
                                                      const shared_ptr<KeyboardInputConfig> inputConfig ) :
   _consoleBuffer( consoleBuffer ),
   _random( random ),
   _frameRateProvider( frameRateProvider ),
   _renderConfig( renderConfig ),
   _inputConfig( inputConfig )
{
   for ( int i = 0; i < renderConfig->TitleStarCount; i++ )
   {
      _starCoordinates.push_back( { random->GetUnsignedInt( 0, (unsigned int)( ( renderConfig->ConsoleWidth - 1 ) * renderConfig->ArenaCharWidth ) ),
                                    random->GetUnsignedInt( 0, (unsigned int)( ( renderConfig->ConsoleHeight - 1 ) * renderConfig->ArenaCharHeight ) ) } );
      _starVelocities.push_back( random->GetUnsignedInt( (unsigned int)renderConfig->MinTitleStarVelocity,
                                                         (unsigned int)renderConfig->MaxTitleStarVelocity ) );
   }
}

void TitleStateConsoleRenderer::Render()
{
   _consoleBuffer->SetDefaultForegroundColor( _renderConfig->TitleScreenForegroundColor );
   _consoleBuffer->SetDefaultBackgroundColor( _renderConfig->TitleScreenBackgroundColor );

   DrawStars();

   _consoleBuffer->Draw( _renderConfig->TitleTextX, _renderConfig->TitleTextY, _renderConfig->TitleTextSprite );
   _consoleBuffer->Draw( _renderConfig->TitleSubTextX, _renderConfig->TitleSubTextY, _renderConfig->TitleSubTextSprite );
   _consoleBuffer->Draw( _renderConfig->TitlePlayerX, _renderConfig->TitlePlayerY, _renderConfig->TitlePlayerSprite );
   _consoleBuffer->Draw( _renderConfig->TitleBuildingX, _renderConfig->TitleBuildingY, _renderConfig->TitleBuildingSprite );
   _consoleBuffer->Draw( _renderConfig->TitleStartMessageX, _renderConfig->TitleStartMessageY, _renderConfig->TitleStartMessageSprite );

   DrawKeyBindings();
}

void TitleStateConsoleRenderer::DrawStars()
{
   for ( int i = 0; i < (int)_starCoordinates.size(); i++ )
   {
      auto left = (short)( _starCoordinates[i].X / _renderConfig->ArenaCharWidth );
      auto top = (short)( _starCoordinates[i].Y / _renderConfig->ArenaCharHeight );
      _consoleBuffer->Draw( left, top, _renderConfig->TitleStarSprite );

      _starCoordinates[i].X += ( _starVelocities[i] / _frameRateProvider->GetFramesPerSecond() );

      // if it's flown off the screen, generate a new star
      if ( _starCoordinates[i].X >= ( _renderConfig->ArenaCharWidth * _renderConfig->ConsoleWidth ) )
      {
         _starCoordinates[i] = { 0, _random->GetUnsignedInt( 0, (unsigned int)( ( _renderConfig->ConsoleHeight - 1 ) * _renderConfig->ArenaCharHeight ) ) };
         _starVelocities[i] = _random->GetUnsignedInt( (unsigned int)_renderConfig->MinTitleStarVelocity, (unsigned int)_renderConfig->MaxTitleStarVelocity );
      }
   }
}

void TitleStateConsoleRenderer::DrawKeyBindings() const
{
   auto leftOfMiddleX = _renderConfig->TitleKeyBindingsMiddleX - 2;
   auto top = _renderConfig->TitleKeyBindingsY;

   for ( auto const& [keyCode, mappedButton] : _inputConfig->KeyMap )
   {
      string keyString( format( "{0} Key", _inputConfig->KeyNames.at(keyCode) ) );
      string buttonString( format( "{0} Button", _inputConfig->ButtonNames.at(mappedButton) ) );

      _consoleBuffer->Draw( leftOfMiddleX - (int)keyString.length() - 2, top, format( "{0} -> {1}", keyString, buttonString ), _renderConfig->TitleKeyBindingsForegroundColor );

      top++;
   }
}
