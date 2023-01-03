#include <string>
#include <format>

#include "TitleStateConsoleRenderer.h"
#include "ConsoleBuffer.h"
#include "IRandom.h"
#include "IFrameRateProvider.h"
#include "IGameEventAggregator.h"
#include "ConsoleRenderDefs.h"
#include "KeyboardInputDefs.h"
#include "IConsoleAnimationProvider.h"
#include "IConsoleAnimation.h"
#include "IConsoleSprite.h"
#include "ConsoleColor.h"

using namespace std;
using namespace MegaManLofi;

TitleStateConsoleRenderer::TitleStateConsoleRenderer( const shared_ptr<ConsoleBuffer> consoleBuffer,
                                                      const shared_ptr<IRandom> random,
                                                      const shared_ptr<IFrameRateProvider> frameRateProvider,
                                                      const shared_ptr<IGameEventAggregator> eventAggregator,
                                                      const shared_ptr<ConsoleRenderDefs> renderDefs,
                                                      const shared_ptr<KeyboardInputDefs> inputDefs,
                                                      const shared_ptr<IConsoleAnimationProvider> animationProvider ) :
   _consoleBuffer( consoleBuffer ),
   _random( random ),
   _frameRateProvider( frameRateProvider ),
   _eventAggregator( eventAggregator ),
   _renderDefs( renderDefs ),
   _inputDefs( inputDefs ),
   _animationProvider( animationProvider ),
   _thwipOutAnimation( animationProvider->GetAnimation( ConsoleAnimationType::PlayerThwipOut ) )
{
   for ( int i = 0; i < renderDefs->TitleStarCount; i++ )
   {
      _starCoordinates.push_back( { (float)random->GetUnsignedInt( 0, (unsigned int)( ( renderDefs->ConsoleWidthChars - 1 ) * renderDefs->ArenaCharWidth ) ),
                                    (float)random->GetUnsignedInt( 0, (unsigned int)( ( renderDefs->ConsoleHeightChars - 1 ) * renderDefs->ArenaCharHeight ) ) } );
      _starVelocities.push_back( (float)random->GetUnsignedInt( (unsigned int)renderDefs->MinTitleStarVelocity,
                                                                (unsigned int)renderDefs->MaxTitleStarVelocity ) );
   }

   _eventAggregator->RegisterEventHandler( GameEvent::GameStarted, std::bind( &TitleStateConsoleRenderer::HandleGameStartedEvent, this ) );
}

void TitleStateConsoleRenderer::HandleGameStartedEvent()
{
   Coordinate<short> startPosition = { _renderDefs->TitlePlayerLeftChars, _renderDefs->TitlePlayerTopChars };
   Coordinate<short> endPosition = { _renderDefs->TitlePlayerLeftChars, -( _renderDefs->TitlePlayerImage.Height ) };
   _animationProvider->GetAnimation( ConsoleAnimationType::PlayerThwipOut )->Start( startPosition, endPosition );
}

void TitleStateConsoleRenderer::Render()
{
   _consoleBuffer->SetDefaultForegroundColor( _renderDefs->TitleScreenForegroundColor );
   _consoleBuffer->SetDefaultBackgroundColor( _renderDefs->TitleScreenBackgroundColor );

   DrawStars();

   _consoleBuffer->Draw( _renderDefs->TitleTextLeftChars, _renderDefs->TitleTextTopChars, _renderDefs->TitleTextImage );
   _consoleBuffer->Draw( _renderDefs->TitleSubTextLeftChars, _renderDefs->TitleSubTextTopChars, _renderDefs->TitleSubTextImage );
   _consoleBuffer->Draw( _renderDefs->TitleBuildingLeftChars, _renderDefs->TitleBuildingTopChars, _renderDefs->TitleBuildingImage );
   _consoleBuffer->Draw( _renderDefs->TitleStartMessageLeftChars, _renderDefs->TitleStartMessageTopChars, _renderDefs->TitleStartMessageImage );

   if ( _thwipOutAnimation->IsRunning() )
   {
      _thwipOutAnimation->Draw();
      _thwipOutAnimation->Tick();
   }
   else
   {
      _consoleBuffer->Draw( _renderDefs->TitlePlayerLeftChars, _renderDefs->TitlePlayerTopChars, _renderDefs->TitlePlayerImage );
   }

   DrawKeyBindings();
}

bool TitleStateConsoleRenderer::HasFocus() const
{
   return _thwipOutAnimation->IsRunning();
}

void TitleStateConsoleRenderer::DrawStars()
{
   for ( int i = 0; i < (int)_starCoordinates.size(); i++ )
   {
      auto left = (short)( _starCoordinates[i].Left / _renderDefs->ArenaCharWidth );
      auto top = (short)( _starCoordinates[i].Top / _renderDefs->ArenaCharHeight );
      _consoleBuffer->Draw( left, top, _renderDefs->TitleStarImage );

      _starCoordinates[i].Left += ( _starVelocities[i] * _frameRateProvider->GetFrameSeconds() );

      // if it's flown off the screen, generate a new star
      if ( _starCoordinates[i].Left >= ( _renderDefs->ArenaCharWidth * _renderDefs->ConsoleWidthChars ) )
      {
         _starCoordinates[i] = { 0, (float)_random->GetUnsignedInt( 0, (unsigned int)( ( _renderDefs->ConsoleHeightChars - 1 ) * _renderDefs->ArenaCharHeight ) ) };
         _starVelocities[i] = (float)_random->GetUnsignedInt( (unsigned int)_renderDefs->MinTitleStarVelocity, (unsigned int)_renderDefs->MaxTitleStarVelocity );
      }
   }
}

void TitleStateConsoleRenderer::DrawKeyBindings() const
{
   auto leftOfMiddleX = _renderDefs->TitleKeyBindingsMiddleXChars - 2;
   auto top = _renderDefs->TitleKeyBindingsTopChars;

   for ( auto const& [keyCode, mappedButton] : _inputDefs->KeyMap )
   {
      string keyString( format( "{0} Key", _inputDefs->KeyNames.at( keyCode ) ) );
      string buttonString( format( "{0} Button", _inputDefs->ButtonNames.at(mappedButton) ) );

      _consoleBuffer->Draw( leftOfMiddleX - (int)keyString.length() - 2, top, format( "{0} -> {1}", keyString, buttonString ), _renderDefs->TitleKeyBindingsForegroundColor );

      top++;
   }
}
