#include <format>

#include "DiagnosticsConsoleRenderer.h"
#include "IConsoleBuffer.h"
#include "IGameClock.h"
#include "ConsoleRenderDefs.h"
#include "IArenaInfoProvider.h"
#include "IEntityConsoleSpriteRepository.h"
#include "IReadOnlyArena.h"
#include "ConsoleColor.h"

#define DIAGNOSTICS_WIDTH 35

using namespace std;
using namespace MegaManLofi;

DiagnosticsConsoleRenderer::DiagnosticsConsoleRenderer( const shared_ptr<IConsoleBuffer> consoleBuffer,
                                                        const shared_ptr<IGameClock> clock,
                                                        const shared_ptr<ConsoleRenderDefs> renderDefs,
                                                        const shared_ptr<IArenaInfoProvider> arenaInfoProvider,
                                                        const shared_ptr<IEntityConsoleSpriteRepository> spriteRepository ) :
   _consoleBuffer( consoleBuffer ),
   _clock( clock ),
   _renderDefs( renderDefs ),
   _arenaInfoProvider( arenaInfoProvider ),
   _spriteRepository( spriteRepository )
{
}

void DiagnosticsConsoleRenderer::Render()
{
   auto left = _renderDefs->ConsoleWidthChars - DIAGNOSTICS_WIDTH;

   auto elapsedSecondsString = format( " Elapsed Seconds:    {0} ", _clock->GetElapsedNanoseconds() / 1'000'000'000 );
   auto totalFramesString = format( " Total frames:       {0} ", _clock->GetCurrentFrame() );
   auto framesPerSecondString = format( " Average frame rate: {0} ", _clock->GetAverageFrameRate() );
   auto arenaEntitiesString = format( " Arena entities:     {0} ", _arenaInfoProvider->GetArena()->GetEntityCount() );
   auto activeSpritesString = format( " Active sprites:     {0} ", _spriteRepository->GetSpriteCount() );

   while ( elapsedSecondsString.length() < DIAGNOSTICS_WIDTH ) { elapsedSecondsString += ' '; }
   while ( totalFramesString.length() < DIAGNOSTICS_WIDTH ) { totalFramesString += ' '; }
   while ( framesPerSecondString.length() < DIAGNOSTICS_WIDTH ) { framesPerSecondString += ' '; }
   while ( arenaEntitiesString.length() < DIAGNOSTICS_WIDTH ) { arenaEntitiesString += ' '; }
   while ( activeSpritesString.length() < DIAGNOSTICS_WIDTH ) { activeSpritesString += ' '; }

   _consoleBuffer->Draw( left, 0, elapsedSecondsString, ConsoleColor::DarkGrey, ConsoleColor::Black );
   _consoleBuffer->Draw( left, 1, totalFramesString, ConsoleColor::DarkGrey, ConsoleColor::Black );
   _consoleBuffer->Draw( left, 2, framesPerSecondString, ConsoleColor::DarkGrey, ConsoleColor::Black );
   _consoleBuffer->Draw( left, 3, arenaEntitiesString, ConsoleColor::DarkGrey, ConsoleColor::Black );
   _consoleBuffer->Draw( left, 4, activeSpritesString, ConsoleColor::DarkGrey, ConsoleColor::Black );
}
