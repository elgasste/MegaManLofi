#include "PlayingMenuStateConsoleRenderer.h"
#include "IConsoleBuffer.h"
#include "ConsoleRenderConfig.h"

using namespace std;
using namespace MegaManLofi;

PlayingMenuStateConsoleRenderer::PlayingMenuStateConsoleRenderer( const shared_ptr<IConsoleBuffer> consoleBuffer,
                                                                  const shared_ptr<ConsoleRenderConfig> renderConfig ) :
   _consoleBuffer( consoleBuffer ),
   _renderConfig( renderConfig )
{
}

void PlayingMenuStateConsoleRenderer::Render()
{
   auto horizontalMidpointChars = _renderConfig->ConsoleWidthChars / 2;
   auto verticalMidpointChars = _renderConfig->ConsoleHeightChars / 2;

   _consoleBuffer->Draw( horizontalMidpointChars - 14, verticalMidpointChars - 3, "Welcome to the in-game menu!" );
   _consoleBuffer->Draw( horizontalMidpointChars - 26, verticalMidpointChars - 1, "There'll be a bunch of rad stuff in here eventually," );
   _consoleBuffer->Draw( horizontalMidpointChars - 29, verticalMidpointChars, "like switching weapons and seeing your stats and all that," );
   _consoleBuffer->Draw( horizontalMidpointChars - 26, verticalMidpointChars + 1, "but for now press Start to go back, or Select to quit." );
}
