#include <format>

#include "DiagnosticsConsoleRenderer.h"
#include "IConsoleBuffer.h"
#include "IGameClock.h"
#include "ConsoleRenderDefs.h"
#include "ConsoleColor.h"

#define DIAGNOSTICS_WIDTH 35

using namespace std;
using namespace MegaManLofi;

DiagnosticsConsoleRenderer::DiagnosticsConsoleRenderer( const shared_ptr<IConsoleBuffer> consoleBuffer,
                                                        const shared_ptr<IGameClock> clock,
                                                        const shared_ptr<ConsoleRenderDefs> renderDefs ) :
   _consoleBuffer( consoleBuffer ),
   _clock( clock ),
   _renderDefs( renderDefs )
{
}

void DiagnosticsConsoleRenderer::Render()
{
   auto left = _renderDefs->ConsoleWidthChars - DIAGNOSTICS_WIDTH;

   auto elapsedSecondsString = format( " Elapsed Seconds:    {0} ", _clock->GetElapsedNanoseconds() / 1'000'000'000 );
   auto totalFramesString = format( " Total frames:       {0} ", _clock->GetCurrentFrame() );
   auto framesPerSecondString = format( " Average frame rate: {0} ", _clock->GetAverageFrameRate() );

   while ( elapsedSecondsString.length() < DIAGNOSTICS_WIDTH ) { elapsedSecondsString += ' '; }
   while ( totalFramesString.length() < DIAGNOSTICS_WIDTH ) { totalFramesString += ' '; }
   while ( framesPerSecondString.length() < DIAGNOSTICS_WIDTH ) { framesPerSecondString += ' '; }

   _consoleBuffer->Draw( left, 0, elapsedSecondsString, ConsoleColor::DarkGrey, ConsoleColor::Black );
   _consoleBuffer->Draw( left, 1, totalFramesString, ConsoleColor::DarkGrey, ConsoleColor::Black );
   _consoleBuffer->Draw( left, 2, framesPerSecondString, ConsoleColor::DarkGrey, ConsoleColor::Black );
}
