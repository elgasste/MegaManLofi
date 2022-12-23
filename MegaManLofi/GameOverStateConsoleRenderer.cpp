#include "GameOverStateConsoleRenderer.h"
#include "IConsoleBuffer.h"
#include "ConsoleRenderConfig.h"

using namespace std;
using namespace MegaManLofi;

GameOverStateConsoleRenderer::GameOverStateConsoleRenderer( const shared_ptr<IConsoleBuffer> consoleBuffer,
                                                            const shared_ptr<ConsoleRenderConfig> renderConfig ) :
   _consoleBuffer( consoleBuffer ),
   _renderConfig( renderConfig )
{
}

void GameOverStateConsoleRenderer::Render()
{
   _consoleBuffer->SetDefaultBackgroundColor( _renderConfig->GameOverBackgroundColor );

   auto left = ( _renderConfig->ConsoleWidthChars / 2 ) - ( _renderConfig->GameOverImage.Width / 2 );
   auto top = ( _renderConfig->ConsoleHeightChars / 2 ) - ( _renderConfig->GameOverImage.Height / 2 );

   _consoleBuffer->Draw( left, top, _renderConfig->GameOverImage );
}
