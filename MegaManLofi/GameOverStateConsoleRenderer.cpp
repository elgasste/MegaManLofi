#include "GameOverStateConsoleRenderer.h"
#include "IConsoleBuffer.h"
#include "ConsoleRenderConfig.h"
#include "KeyboardInputConfig.h"
#include "ConsoleColor.h"

using namespace std;
using namespace MegaManLofi;

GameOverStateConsoleRenderer::GameOverStateConsoleRenderer( const shared_ptr<IConsoleBuffer> consoleBuffer,
                                                            const shared_ptr<ConsoleRenderConfig> renderConfig,
                                                            const shared_ptr<KeyboardInputConfig> inputConfig ) :
   _consoleBuffer( consoleBuffer ),
   _renderConfig( renderConfig ),
   _inputConfig( inputConfig )
{
}

void GameOverStateConsoleRenderer::Render()
{
   _consoleBuffer->SetDefaultBackgroundColor( _renderConfig->GameOverBackgroundColor );

   auto left = ( _renderConfig->ConsoleWidthChars / 2 ) - ( _renderConfig->GameOverImage.Width / 2 );
   auto top = ( _renderConfig->ConsoleHeightChars / 2 ) - ( _renderConfig->GameOverImage.Height / 2 );

   _consoleBuffer->Draw( left, top, _renderConfig->GameOverImage );
}
