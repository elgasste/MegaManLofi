#include "GameOverStateConsoleRenderer.h"
#include "ConsoleBuffer.h"
#include "ConsoleRenderDefs.h"

using namespace std;
using namespace MegaManLofi;

GameOverStateConsoleRenderer::GameOverStateConsoleRenderer( const shared_ptr<ConsoleBuffer> consoleBuffer,
                                                            const shared_ptr<ConsoleRenderDefs> renderDefs ) :
   _consoleBuffer( consoleBuffer ),
   _renderDefs( renderDefs )
{
}

void GameOverStateConsoleRenderer::Render()
{
   _consoleBuffer->SetDefaultBackgroundColor( _renderDefs->GameOverBackgroundColor );

   auto left = ( _renderDefs->ConsoleWidthChars / 2 ) - ( _renderDefs->GameOverImage.Width / 2 );
   auto top = ( _renderDefs->ConsoleHeightChars / 2 ) - ( _renderDefs->GameOverImage.Height / 2 );

   _consoleBuffer->Draw( left, top, _renderDefs->GameOverImage );
}
