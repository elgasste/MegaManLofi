#include <string>
#include <format>

#include "TitleStateConsoleRenderer.h"
#include "IConsoleBuffer.h"
#include "IRandom.h"
#include "IFrameRateProvider.h"
#include "IGameEventAggregator.h"
#include "ConsoleRenderConfig.h"
#include "KeyboardInputConfig.h"
#include "ConsoleColor.h"
#include "GameEvent.h"

using namespace std;
using namespace MegaManLofi;

TitleStateConsoleRenderer::TitleStateConsoleRenderer( const shared_ptr<IConsoleBuffer> consoleBuffer,
                                                      const shared_ptr<IRandom> random,
                                                      const shared_ptr<IFrameRateProvider> frameRateProvider,
                                                      const shared_ptr<IGameEventAggregator> eventAggregator,
                                                      const shared_ptr<ConsoleRenderConfig> renderConfig,
                                                      const shared_ptr<KeyboardInputConfig> inputConfig ) :
   _consoleBuffer( consoleBuffer ),
   _random( random ),
   _frameRateProvider( frameRateProvider ),
   _eventAggregator( eventAggregator ),
   _renderConfig( renderConfig ),
   _inputConfig( inputConfig ),
   _isAnimatingPlayerThwipOutTransition( false ),
   _isAnimatingPlayerThwipOut( false ),
   _isAnimatingPostThwipDelay( false ),
   _playerThwipBottomUnits( 0 ),
   _preThwipElapsedSeconds( 0 ),
   _postThwipElapsedSeconds( 0 )
{
   for ( int i = 0; i < renderConfig->TitleStarCount; i++ )
   {
      _starCoordinates.push_back( { random->GetUnsignedInt( 0, (unsigned int)( ( renderConfig->ConsoleWidthChars - 1 ) * renderConfig->ArenaCharWidth ) ),
                                    random->GetUnsignedInt( 0, (unsigned int)( ( renderConfig->ConsoleHeightChars - 1 ) * renderConfig->ArenaCharHeight ) ) } );
      _starVelocities.push_back( random->GetUnsignedInt( (unsigned int)renderConfig->MinTitleStarVelocity,
                                                         (unsigned int)renderConfig->MaxTitleStarVelocity ) );
   }

   _eventAggregator->RegisterEventHandler( GameEvent::GameStarted, std::bind( &TitleStateConsoleRenderer::HandleGameStartedEvent, this ) );
}

void TitleStateConsoleRenderer::HandleGameStartedEvent()
{
   _isAnimatingPlayerThwipOutTransition = true;
   _preThwipElapsedSeconds = 0;
   _playerThwipBottomUnits = ( (long long)_renderConfig->TitlePlayerTopChars + (long long)_renderConfig->TitlePlayerImage.Height ) * _renderConfig->ArenaCharHeight;
}

void TitleStateConsoleRenderer::Render()
{
   _consoleBuffer->SetDefaultForegroundColor( _renderConfig->TitleScreenForegroundColor );
   _consoleBuffer->SetDefaultBackgroundColor( _renderConfig->TitleScreenBackgroundColor );

   DrawStars();

   _consoleBuffer->Draw( _renderConfig->TitleTextLeftChars, _renderConfig->TitleTextTopChars, _renderConfig->TitleTextImage );
   _consoleBuffer->Draw( _renderConfig->TitleSubTextLeftChars, _renderConfig->TitleSubTextTopChars, _renderConfig->TitleSubTextImage );
   _consoleBuffer->Draw( _renderConfig->TitleBuildingLeftChars, _renderConfig->TitleBuildingTopChars, _renderConfig->TitleBuildingImage );
   _consoleBuffer->Draw( _renderConfig->TitleStartMessageLeftChars, _renderConfig->TitleStartMessageTopChars, _renderConfig->TitleStartMessageImage );

   if ( _isAnimatingPlayerThwipOutTransition )
   {
      DrawPlayerThwipOutTransitionAnimation();
   }
   else if ( _isAnimatingPlayerThwipOut )
   {
      DrawPlayerThwipOutAnimation();
   }
   else if ( _isAnimatingPostThwipDelay )
   {
      DrawPostThwipDelayAnimation();
   }
   else
   {
      _consoleBuffer->Draw( _renderConfig->TitlePlayerLeftChars, _renderConfig->TitlePlayerTopChars, _renderConfig->TitlePlayerImage );
   }

   DrawKeyBindings();
}

bool TitleStateConsoleRenderer::HasFocus() const
{
   return _isAnimatingPlayerThwipOutTransition || _isAnimatingPlayerThwipOut || _isAnimatingPostThwipDelay;
}

void TitleStateConsoleRenderer::DrawStars()
{
   for ( int i = 0; i < (int)_starCoordinates.size(); i++ )
   {
      auto left = (short)( _starCoordinates[i].Left / _renderConfig->ArenaCharWidth );
      auto top = (short)( _starCoordinates[i].Top / _renderConfig->ArenaCharHeight );
      _consoleBuffer->Draw( left, top, _renderConfig->TitleStarImage );

      _starCoordinates[i].Left += ( _starVelocities[i] / _frameRateProvider->GetFramesPerSecond() );

      // if it's flown off the screen, generate a new star
      if ( _starCoordinates[i].Left >= ( _renderConfig->ArenaCharWidth * _renderConfig->ConsoleWidthChars ) )
      {
         _starCoordinates[i] = { 0, _random->GetUnsignedInt( 0, (unsigned int)( ( _renderConfig->ConsoleHeightChars - 1 ) * _renderConfig->ArenaCharHeight ) ) };
         _starVelocities[i] = _random->GetUnsignedInt( (unsigned int)_renderConfig->MinTitleStarVelocity, (unsigned int)_renderConfig->MaxTitleStarVelocity );
      }
   }
}

void TitleStateConsoleRenderer::DrawKeyBindings() const
{
   auto leftOfMiddleX = _renderConfig->TitleKeyBindingsMiddleXChars - 2;
   auto top = _renderConfig->TitleKeyBindingsTopChars;

   for ( auto const& [keyCode, mappedButton] : _inputConfig->KeyMap )
   {
      string keyString( format( "{0} Key", _inputConfig->KeyNames.at(keyCode) ) );
      string buttonString( format( "{0} Button", _inputConfig->ButtonNames.at(mappedButton) ) );

      _consoleBuffer->Draw( leftOfMiddleX - (int)keyString.length() - 2, top, format( "{0} -> {1}", keyString, buttonString ), _renderConfig->TitleKeyBindingsForegroundColor );

      top++;
   }
}

void TitleStateConsoleRenderer::DrawPlayerThwipOutTransitionAnimation()
{
   _preThwipElapsedSeconds += ( 1 / (double)_frameRateProvider->GetFramesPerSecond() );

   auto sprite = _renderConfig->PlayerThwipOutTransitionSprite;
   _consoleBuffer->Draw( _renderConfig->TitlePlayerLeftChars, _renderConfig->TitlePlayerTopChars, sprite );

   if ( _preThwipElapsedSeconds >= sprite->GetTotalTraversalSeconds() )
   {
      _isAnimatingPlayerThwipOutTransition = false;
      _isAnimatingPlayerThwipOut = true;
   }
   else
   {
      sprite->Tick( _frameRateProvider->GetFramesPerSecond() );
   }
}

void TitleStateConsoleRenderer::DrawPlayerThwipOutAnimation()
{
   auto thwipDeltaUnits = ( _renderConfig->PlayerThwipVelocity / _frameRateProvider->GetFramesPerSecond() );
   _playerThwipBottomUnits -= thwipDeltaUnits;

   auto playerSprite = _renderConfig->TitlePlayerImage;
   auto thwipSpriteLeftOffsetChars = (short)( ( playerSprite.Width - _renderConfig->PlayerThwipSprite->GetWidth() ) / 2 );
   auto playerThwipBottomChars = (short)( _playerThwipBottomUnits / _renderConfig->ArenaCharHeight );

   if ( playerThwipBottomChars <= 0 )
   {
      _isAnimatingPlayerThwipOut = false;

      _postThwipElapsedSeconds = 0;
      _isAnimatingPostThwipDelay = true;

      return;
   }

   _consoleBuffer->Draw( _renderConfig->TitlePlayerLeftChars + thwipSpriteLeftOffsetChars,
                         playerThwipBottomChars - _renderConfig->PlayerThwipSprite->GetHeight(),
                         _renderConfig->PlayerThwipSprite );
   _renderConfig->PlayerThwipSprite->Tick( _frameRateProvider->GetFramesPerSecond() );
}

void TitleStateConsoleRenderer::DrawPostThwipDelayAnimation()
{
   _postThwipElapsedSeconds += ( 1 / (double)_frameRateProvider->GetFramesPerSecond() );

   if ( _postThwipElapsedSeconds >= _renderConfig->TitlePostThwipDelaySeconds )
   {
      _isAnimatingPostThwipDelay = false;
   }
}
