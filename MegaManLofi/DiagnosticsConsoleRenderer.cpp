#include <format>

#include "DiagnosticsConsoleRenderer.h"
#include "ConsoleBuffer.h"
#include "IFrameRateProvider.h"
#include "ConsoleRenderDefs.h"
#include "IArenaInfoProvider.h"
#include "EntityConsoleSpriteRepository.h"
#include "ReadOnlyArena.h"
#include "ConsoleColor.h"

#define DIAGNOSTICS_WIDTH 35

using namespace std;
using namespace MegaManLofi;

DiagnosticsConsoleRenderer::DiagnosticsConsoleRenderer( const shared_ptr<ConsoleBuffer> consoleBuffer,
                                                        const shared_ptr<IFrameRateProvider> frameRateProvider,
                                                        const shared_ptr<ConsoleRenderDefs> renderDefs,
                                                        const shared_ptr<IArenaInfoProvider> arenaInfoProvider,
                                                        const shared_ptr<EntityConsoleSpriteRepository> spriteRepository ) :
   _consoleBuffer( consoleBuffer ),
   _frameRateProvider( frameRateProvider ),
   _renderDefs( renderDefs ),
   _arenaInfoProvider( arenaInfoProvider ),
   _spriteRepository( spriteRepository )
{
}

void DiagnosticsConsoleRenderer::Render()
{
   auto left = _renderDefs->ConsoleWidthChars - DIAGNOSTICS_WIDTH;

   auto elapsedSecondsString = format( " Elapsed seconds:    {0} ", _frameRateProvider->GetElapsedNanoseconds() / 1'000'000'000 );
   auto minFrameRateString = format( " Min frame rate:     {0} ", _frameRateProvider->GetMinimumFrameRate() );
   auto totalFramesString = format( " Total frames:       {0} ", _frameRateProvider->GetCurrentFrame() );
   auto framesPerSecondString = format( " Average frame rate: {0} ", _frameRateProvider->GetAverageFrameRate() );
   auto lagFramesString = format( " Lag frames:         {0} ", _frameRateProvider->GetLagFrameCount() );
   auto arenaEntitiesString = format( " Arena entities:     {0} ", _arenaInfoProvider->GetArena()->GetEntityCount() );
   auto activeSpritesString = format( " Active sprites:     {0} ", _spriteRepository->GetSpriteCount() );

   while ( elapsedSecondsString.length() < DIAGNOSTICS_WIDTH ) { elapsedSecondsString += ' '; }
   while ( minFrameRateString.length() < DIAGNOSTICS_WIDTH ) { minFrameRateString += ' '; }
   while ( totalFramesString.length() < DIAGNOSTICS_WIDTH ) { totalFramesString += ' '; }
   while ( framesPerSecondString.length() < DIAGNOSTICS_WIDTH ) { framesPerSecondString += ' '; }
   while ( lagFramesString.length() < DIAGNOSTICS_WIDTH ) { lagFramesString += ' '; }
   while ( arenaEntitiesString.length() < DIAGNOSTICS_WIDTH ) { arenaEntitiesString += ' '; }
   while ( activeSpritesString.length() < DIAGNOSTICS_WIDTH ) { activeSpritesString += ' '; }

   short top = 0;

   _consoleBuffer->Draw( left, top++, elapsedSecondsString, ConsoleColor::DarkGrey, ConsoleColor::Black );
   if ( _frameRateProvider->HasMinimumFrameRate() )
   {
      _consoleBuffer->Draw( left, top++, minFrameRateString, ConsoleColor::DarkGrey, ConsoleColor::Black );
   }
   _consoleBuffer->Draw( left, top++, totalFramesString, ConsoleColor::DarkGrey, ConsoleColor::Black );
   _consoleBuffer->Draw( left, top++, framesPerSecondString, ConsoleColor::DarkGrey, ConsoleColor::Black );
   if ( _frameRateProvider->HasMinimumFrameRate() )
   {
      _consoleBuffer->Draw( left, top++, lagFramesString, ConsoleColor::DarkGrey, ConsoleColor::Black );
   }
   _consoleBuffer->Draw( left, top++, arenaEntitiesString, ConsoleColor::DarkGrey, ConsoleColor::Black );
   _consoleBuffer->Draw( left, top++, activeSpritesString, ConsoleColor::DarkGrey, ConsoleColor::Black );
}
